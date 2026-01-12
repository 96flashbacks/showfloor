#include <ultra64.h>

#include "sm64.h"
#include "moving_texture.h"
#include "area.h"
#include "camera.h"
#include "rendering_graph_node.h"
#include "engine/math_util.h"
#include "memory.h"
#include "save_file.h"
#include "segment2.h"
#include "engine/surface_collision.h"
#include "geo_misc.h"
#include "rendering_graph_node.h"
#include "object_list_processor.h"

/**
 * This file contains functions for generating display lists with moving textures
 * (abbreviated movtex). This is used for water, sand, haze, mist and treadmills.
 * Each mesh using this system has the animated vertices stored as an array of shorts.
 * The first entry is the texture movement speed. After that the vertices are stored
 * in one of two layouts: one without per-vertex color attributes and one with.
 * [speed, v0(x,y,z, s,t)      , v1(x,y,z, s,t)      , ...]
 * [speed, v0(x,y,z, r,g,b s,t), v1(x,y,z, r,g,b s,t), ...]
 * x, y, z = vertex position as integers
 * s, t = texture coordinates as 6.10 fixed point number. That means coordinates in
 * range [0, 1024] are a unique part of the image, after that it repeats the image.
 *
 * The first vertex 'v0' is special because all subsequent vertices inherit its
 * texture offset. So to animate e.g. a treadmill, the speed component arr[0] is
 * simply added to the s component arr[7] every frame and the texture scrolls
 * horizontally over the entire mesh without changing the rest of the array.
 * Note that while the system allows many kinds of vertex animations, in
 * practice the only animation used is horizontally scrolling textures.
 *
 * After updating the base mesh, the vertices are converted to the format the RSP
 * understands and a display list is generated. The RSP can buffer 16 vertices at
 * a time, and this code assumes everything fits in one buffer, so every moving
 * texture mesh must have at most 16 vertices. As a result some meshes are split
 * up into multiple parts, like the sand pathway inside the pyramid which has 3
 * parts. The water stream in the Cavern of the Metal Cap fits in one mesh.
 *
 * Apart from this general system, there is also a simpler system for flat
 * quads with a rotating texture. This is often used for water, but also
 * for mist, toxic haze and lava inside the volcano. One quad is described
 * by the struct MovtexQuad, and multiple MovtexQuads form a MovtexQuadCollection.
 * A geo node has an id that corresponds to the id of a certain MovtexQuadCollection,
 * which will then be matched with the id of entries in gEnvironmentRegions to get the
 * y-position. The x and z coordinates are stored in the MovtexQuads themself,
 * so the water rectangle is separate from the actually drawn rectangle.
 */

// First entry in array is texture movement speed for both layouts
#define MOVTEX_ATTR_SPEED 0

// Different layouts for vertices
#define MOVTEX_LAYOUT_NOCOLOR 0
#define MOVTEX_LAYOUT_COLORED 1

// Attributes for movtex vertices
#define MOVTEX_ATTR_X 1
#define MOVTEX_ATTR_Y 2
#define MOVTEX_ATTR_Z 3

// For MOVTEX_LAYOUT_NOCOLOR only
#define MOVTEX_ATTR_NOCOLOR_S 4
#define MOVTEX_ATTR_NOCOLOR_T 5

// For MOVTEX_LAYOUT_COLORED only
#define MOVTEX_ATTR_COLORED_R 4
#define MOVTEX_ATTR_COLORED_G 5
#define MOVTEX_ATTR_COLORED_B 6
#define MOVTEX_ATTR_COLORED_S 7
#define MOVTEX_ATTR_COLORED_T 8

/**
 * An object containing all info for a mesh with moving textures.
 * Contains the vertices that are animated, but also the display list which
 * determines the connectivity, as well as the texture, texture blend color
 * and drawing layer.
 */
struct MovtexObject {
    /// number that geo nodes have as parameter to refer to this mesh
    u32 geoId;
    /// which texture to use for this mesh, index into gMovtexIdToTexture
    s32 textureId;
    /// amount of moving vertices
    s32 vtx_count;
    /// segmented address to movtex mesh with vertices
    void *movtexVerts;
    /// display list inserted before moving triangles
    Gfx *beginDl;
    /// display list inserted after moving triangles
    Gfx *endDl;
    /// display list with the actual moving texture triangles.
    /// Assumes the animated vertices are buffered and correct texture is set
    Gfx *triDl;
    // if the list does not have per-vertex colors, all vertices have these colors
    u8 r;      /// red
    u8 g;      /// green
    u8 b;      /// blue
    u8 a;      /// alpha
    s32 layer; /// the drawing layer for this mesh
};

/// Counters to make textures move iff the game is not paused.
s16 gMovtexCounter = 1;
s16 gMovtexCounterPrev = 0;

// Vertex colors for rectangles. Used to give mist a tint
#define MOVTEX_VTX_COLOR_DEFAULT 0 // no tint (white vertex colors)
#define MOVTEX_VTX_COLOR_YELLOW 1  // used for Hazy Maze Cave toxic haze
#define MOVTEX_VTX_COLOR_RED 2     // used for Shifting Sand Land around the Tox box maze

s8 gMovtexVtxColor = MOVTEX_VTX_COLOR_DEFAULT;

/// The height at which Mario entered the last painting. Used for Wet-Dry World only.
float gPaintingMarioYEntry = 0.0f;

/// Variable to ensure the initial Wet-Dry World water level is set only once
s32 gWDWWaterLevelSet = FALSE;

/**
 * An array for converting a movtex texture id to a pointer that can
 * be passed to gDPSetTextureImage.
 */
u8 *gMovtexIdToTexture[] = { texture_waterbox_water, texture_waterbox_mist, texture_waterbox_jrb_water,
                             texture_waterbox_unknown_water, texture_waterbox_lava };

extern Gfx castle_grounds_dl_waterfall[];
extern s16 castle_grounds_movtex_tris_waterfall[];

/**
 * MovtexObjects that have no color attributes per vertex (though the mesh
 * as a whole can have a blend color).
 */
struct MovtexObject gMovtexNonColored[] = {

    // The waterfall outside the castle
    { MOVTEX_CASTLE_WATERFALL, TEXTURE_WATER, 15, castle_grounds_movtex_tris_waterfall,
      dl_waterbox_rgba16_begin, dl_waterbox_end, castle_grounds_dl_waterfall, 0xff, 0xff, 0xff, 0xb4,
      LAYER_TRANSPARENT_INTER },

    { 0x00000000, 0x00000000, 0, NULL, NULL, NULL, NULL, 0x00, 0x00, 0x00, 0x00, 0x00000000 },
};

/**
 * Sets the initial water level in Wet-Dry World based on how high Mario
 * jumped into the painting.
 */
Gfx *geo_wdw_set_initial_water_level(s32 callContext, UNUSED struct GraphNode *node, UNUSED Mat4 mtx) {
    s32 i;
    UNUSED u8 unused[] = { 1, 0, 4, 0, 7, 0, 10, 0 };
    s16 wdwWaterHeight;

    // Why was this global variable needed when they could just check for GEO_CONTEXT_AREA_LOAD?
    if (callContext != GEO_CONTEXT_RENDER) {
        gWDWWaterLevelSet = FALSE;
    } else if (callContext == GEO_CONTEXT_RENDER && gEnvironmentRegions != NULL && !gWDWWaterLevelSet) {
        if (gPaintingMarioYEntry <= 1382.4) {
            wdwWaterHeight = 31;
        } else if (gPaintingMarioYEntry >= 1600.0) {
            wdwWaterHeight = 2816;
        } else {
            wdwWaterHeight = 1024;
        }
        for (i = 0; i < *gEnvironmentRegions; i++) {
            gEnvironmentRegions[i * 6 + 6] = wdwWaterHeight;
        }
        gWDWWaterLevelSet = TRUE;
    }
    return NULL;
}

/**
 * Update moving texture counters that determine when to update the coordinates.
 * Textures update when gMovtexCounterPrev != gMovtexCounter.
 * This ensures water / sand flow stops when the game pauses.
 */
Gfx *geo_movtex_pause_control(s32 callContext, UNUSED struct GraphNode *node, UNUSED Mat4 mtx) {
    if (callContext != GEO_CONTEXT_RENDER) {
        gMovtexCounterPrev = gAreaUpdateCounter - 1;
        gMovtexCounter = gAreaUpdateCounter;
    } else {
        gMovtexCounterPrev = gMovtexCounter;
        gMovtexCounter = gAreaUpdateCounter;
    }
    return NULL;
}

/**
 * Make a vertex that's part of a quad with rotating texture.
 * verts: array of RSP vertices
 * n: index in 'verts' where the vertex is written
 * x, y, z: position
 * rot: base rotation of the texture
 * rotOffset: gets added to base rotation
 * scale: how often the texture repeats, 1 = no repeat
 */
void movtex_make_quad_vertex(Vtx *verts, s32 index, s16 x, s16 y, s16 z, s16 rot, s16 rotOffset,
                             f32 scale, u8 alpha) {
    s16 s = 32.0 * (32.0 * scale - 1.0) * sins(rot + rotOffset);
    s16 t = 32.0 * (32.0 * scale - 1.0) * coss(rot + rotOffset);

    if (gMovtexVtxColor == MOVTEX_VTX_COLOR_YELLOW) {
        make_vertex(verts, index, x, y, z, s, t, 255, 255, 0, alpha);
    } else if (gMovtexVtxColor == MOVTEX_VTX_COLOR_RED) {
        make_vertex(verts, index, x, y, z, s, t, 255, 0, 0, alpha);
    } else {
        make_vertex(verts, index, x, y, z, s, t, 255, 255, 255, alpha);
    }
}

/**
 * Represents a single flat quad with a rotating texture
 * Stores x and z for 4 vertices, though it is often just a rectangle.
 * Does not store the y-position, since that can be dynamic for water levels.
 */
struct MovtexQuad {
    /// the current texture rotation in this quad
    s16 rot;
    /// gets added to rot every frame
    s16 rotspeed;
    /// the amount of times the texture repeats. 1 = no repeat.
    s16 scale;
    /// Coordinates of vertices
    s16 x1;
    s16 z1;
    s16 x2;
    s16 z2;
    s16 x3;
    s16 z3;
    s16 x4;
    s16 z4;
    s16 rotDir;    /// if 1, it rotates counter-clockwise
    s16 alpha;     /// opacity, 255 = fully opaque
    s16 textureId; /// texture id
};

/// Variable for a little optimization: only set the texture when it differs from the previous texture
s16 gMovetexLastTextureId;

/**
 * Generates and returns a display list for a single MovtexQuad at height y.
 */
Gfx *movtex_gen_from_quad(s16 y, struct MovtexQuad *quad) {
    s16 rot;
    s16 rotspeed = quad->rotspeed;
    s16 scale = quad->scale;
    s16 x1 = quad->x1;
    s16 z1 = quad->z1;
    s16 x2 = quad->x2;
    s16 z2 = quad->z2;
    s16 x3 = quad->x3;
    s16 z3 = quad->z3;
    s16 x4 = quad->x4;
    s16 z4 = quad->z4;
    s16 rotDir = quad->rotDir;
    s16 alpha = quad->alpha;
    s16 textureId = quad->textureId;
    Vtx *verts = alloc_display_list(4 * sizeof(*verts));
    Gfx *gfxHead;
    Gfx *gfx;

    if (textureId == gMovetexLastTextureId) {
        gfxHead = alloc_display_list(3 * sizeof(*gfxHead));
    } else {
        gfxHead = alloc_display_list(8 * sizeof(*gfxHead));
    }

    if (gfxHead == NULL || verts == NULL) {
        return NULL;
    }
    gfx = gfxHead;
    if (gMovtexCounter != gMovtexCounterPrev) {
        quad->rot += rotspeed;
    }
    rot = quad->rot;
    if (rotDir == ROTATE_CLOCKWISE) {
        movtex_make_quad_vertex(verts, 0, x1, y, z1, rot, 0, scale, alpha);
        movtex_make_quad_vertex(verts, 1, x2, y, z2, rot, 16384, scale, alpha);
        movtex_make_quad_vertex(verts, 2, x3, y, z3, rot, -32768, scale, alpha);
        movtex_make_quad_vertex(verts, 3, x4, y, z4, rot, -16384, scale, alpha);
    } else { // ROTATE_COUNTER_CLOCKWISE
        movtex_make_quad_vertex(verts, 0, x1, y, z1, rot, 0, scale, alpha);
        movtex_make_quad_vertex(verts, 1, x2, y, z2, rot, -16384, scale, alpha);
        movtex_make_quad_vertex(verts, 2, x3, y, z3, rot, -32768, scale, alpha);
        movtex_make_quad_vertex(verts, 3, x4, y, z4, rot, 16384, scale, alpha);
    }

    // Only add commands to change the texture when necessary
    if (textureId != gMovetexLastTextureId) {
        switch (textureId) {
            case TEXTURE_MIST: // an ia16 texture
                gLoadBlockTexture(gfx++, 32, 32, G_IM_FMT_IA, gMovtexIdToTexture[textureId]);
                break;
            default: // any rgba16 texture
                gLoadBlockTexture(gfx++, 32, 32, G_IM_FMT_RGBA, gMovtexIdToTexture[textureId]);
                break;
        }
        gMovetexLastTextureId = textureId;
    }
    gSPVertex(gfx++, VIRTUAL_TO_PHYSICAL2(verts), 4, 0);
    gSPDisplayList(gfx++, dl_draw_quad_verts_0123);
    gSPEndDisplayList(gfx);
    return gfxHead;
}

/**
 * Generate a display list drawing an array of MoxtexQuad at height 'y'.
 * y: y position of the quads
 * quadArrSegmented: a segmented address to an array of s16. The first number
 * is the number of entries, followed by that number of MovtexQuad structs.
 */
Gfx *movtex_gen_from_quad_array(s16 y, void *quadArrSegmented) {
    s16 *quadArr = segmented_to_virtual(quadArrSegmented);
    s16 numLists = quadArr[0];
    Gfx *gfxHead = alloc_display_list((numLists + 1) * sizeof(*gfxHead));
    Gfx *gfx = gfxHead;
    Gfx *subList;
    s32 i;

    if (gfxHead == NULL) {
        return NULL;
    }
    for (i = 0; i < numLists; i++) {
        // quadArr is an array of s16, so sizeof(MovtexQuad) gets divided by 2
        subList = movtex_gen_from_quad(
            y, (struct MovtexQuad *) (&quadArr[i * (sizeof(struct MovtexQuad) / 2) + 1]));
        if (subList != NULL) {
            gSPDisplayList(gfx++, VIRTUAL_TO_PHYSICAL(subList));
        }
    }
    gSPEndDisplayList(gfx);
    return gfxHead;
}

/**
 * Generate the display list for a list of quads by searching through a collection
 * for a given id.
 * id: id of quad array to generate a list for
 * y: height at which the quads are drawn
 * movetexQuadsSegmented: segmented address to the MovtexQuadCollection array
 * that will be searched.
 */
Gfx *movtex_gen_quads_id(s16 id, s16 y, void *movetexQuadsSegmented) {
    struct MovtexQuadCollection *collection = segmented_to_virtual(movetexQuadsSegmented);
    s32 i = 0;

    while (collection[i].id != -1) {
        if (collection[i].id == id) {
            return movtex_gen_from_quad_array(y, collection[i].quadArraySegmented);
        }
        i++;
    }
    return NULL;
}

extern u8 castle_movtex_water[];
extern u8 jrb_movtex_area1_water[];
extern u8 thi_movtex_area1_water[];
extern u8 castle_grounds_movtex_water[];
extern u8 ddd_movtex_area1_water[];
extern u8 ddd_movtex_area2_water[];
extern u8 wf_movtex_water[];
extern u8 castle_courtyard_movtex_star_statue_water[];

/**
 * Find the quadCollection for a given quad collection id.
 */
void *get_quad_collection_from_id(u32 id) {
    switch (id) {
        case CASTLE_MOVTEX_WATER:
            return castle_movtex_water;
        case JRB_MOVTEX_WATER:
            return jrb_movtex_area1_water;
        case THI_MOVTEX_AREA1_WATER:
            return thi_movtex_area1_water;
        case CASTLE_GROUNDS_MOVTEX_WATER:
            return castle_grounds_movtex_water;
        case DDD_MOVTEX_AREA1_WATER:
            return ddd_movtex_area1_water;
        case DDD_MOVTEX_AREA2_WATER:
            return ddd_movtex_area2_water;
        case WF_MOVTEX_WATER:
            return wf_movtex_water;
        case CASTLE_COURTYARD_MOVTEX_STAR_STATUE_WATER:
            return castle_courtyard_movtex_star_statue_water;
        default:
            return NULL;
    }
}

/**
 * Geo script responsible for drawing quads with a moving texture at the height
 * of the corresponding water region. The node's parameter determines which quad
 * collection is drawn, see moving_texture.h.
 */
Gfx *geo_movtex_draw_water_regions(s32 callContext, struct GraphNode *node, UNUSED Mat4 mtx) {
    Gfx *gfxHead = NULL;
    Gfx *gfx = NULL;
    Gfx *subList;
    void *quadCollection;
    struct GraphNodeGenerated *asGenerated;
    s16 numWaterBoxes;
    s16 waterId;
    s16 waterY;
    s32 i;

    if (callContext == GEO_CONTEXT_RENDER) {
        gMovtexVtxColor = MOVTEX_VTX_COLOR_DEFAULT;
        if (gEnvironmentRegions == NULL) {
            return NULL;
        }
        numWaterBoxes = gEnvironmentRegions[0];
        gfxHead = alloc_display_list((numWaterBoxes + 3) * sizeof(*gfxHead));
        if (gfxHead == NULL) {
            return NULL;
        } else {
            gfx = gfxHead;
        }
        asGenerated = (struct GraphNodeGenerated *) node;
        if (asGenerated->parameter == JRB_MOVTEX_INITIAL_MIST) {
            if (gLakituState.goalPos[1] < 1024.0) { // if camera under water
                return NULL;
            }
            if (save_file_get_star_flags(gCurrSaveFileNum - 1, COURSE_NUM_TO_INDEX(COURSE_JRB))
                & (1 << 0)) { // the "Plunder in the Sunken Ship" star in JRB is collected
                return NULL;
            }
        }
        quadCollection = get_quad_collection_from_id(asGenerated->parameter);
        if (quadCollection == NULL) {
            return NULL;
        }

        asGenerated->fnNode.node.flags =
            (asGenerated->fnNode.node.flags & 0xFF) | (LAYER_TRANSPARENT_INTER << 8);

        gSPDisplayList(gfx++, dl_waterbox_rgba16_begin);
        gMovetexLastTextureId = -1;
        for (i = 0; i < numWaterBoxes; i++) {
            waterId = gEnvironmentRegions[i * 6 + 1];
            waterY = gEnvironmentRegions[i * 6 + 6];
            subList = movtex_gen_quads_id(waterId, waterY, quadCollection);
            if (subList != NULL) {
                gSPDisplayList(gfx++, VIRTUAL_TO_PHYSICAL(subList));
            }
        }
        gSPDisplayList(gfx++, dl_waterbox_end);
        gSPEndDisplayList(gfx);
    }
    return gfxHead;
}

/**
 * Updates a movtex mesh by adding the movtex's speed to the horizontal or
 * vertical texture coordinates depending on 'attr'.
 * movtexVerts: vertices to update
 * attr: which attribute to change
 */
void update_moving_texture_offset(s16 *movtexVerts, s32 attr) {
    s16 movSpeed = movtexVerts[MOVTEX_ATTR_SPEED];
    s16 *curOffset = movtexVerts + attr;

    if (gMovtexCounter != gMovtexCounterPrev) {
        *curOffset += movSpeed;
        // note that texture coordinates are 6.10 fixed point, so this does modulo 1
        if (*curOffset >= 1024) {
            *curOffset -= 1024;
        }
        if (*curOffset <= -1024) {
            *curOffset += 1024;
        }
    }
}

/**
 * Make the first vertex of a moving texture with index 0.
 * This vertex is the base of all vertices with index > 0, which use this
 * vertex's coordinates as base on which to apply offset.
 * The first vertex has offset 0 by definition, simplifying the calculations a bit.
 */
void movtex_write_vertex_first(Vtx *vtx, s16 *movtexVerts, struct MovtexObject *c, s8 attrLayout) {
    s16 x = movtexVerts[MOVTEX_ATTR_X];
    s16 y = movtexVerts[MOVTEX_ATTR_Y];
    s16 z = movtexVerts[MOVTEX_ATTR_Z];
    u8 alpha = c->a;
    u8 r1;
    u8 g1;
    u8 b1;
    s8 r2;
    s8 g2;
    s8 b2;
    s16 s;
    s16 t;

    switch (attrLayout) {
        case MOVTEX_LAYOUT_NOCOLOR:
            r1 = c->r;
            g1 = c->g;
            b1 = c->b;
            s = movtexVerts[MOVTEX_ATTR_NOCOLOR_S];
            t = movtexVerts[MOVTEX_ATTR_NOCOLOR_T];
            make_vertex(vtx, 0, x, y, z, s, t, r1, g1, b1, alpha);
            break;
        case MOVTEX_LAYOUT_COLORED:
            r2 = movtexVerts[MOVTEX_ATTR_COLORED_R];
            g2 = movtexVerts[MOVTEX_ATTR_COLORED_G];
            b2 = movtexVerts[MOVTEX_ATTR_COLORED_B];
            s = movtexVerts[MOVTEX_ATTR_COLORED_S];
            t = movtexVerts[MOVTEX_ATTR_COLORED_T];
            make_vertex(vtx, 0, x, y, z, s, t, r2, g2, b2, alpha);
            break;
    }
}

/**
 * Make a vertex with index > 0. The vertex with index 0 is made in
 * movtex_write_vertex_first and subsequent vertices use vertex 0 as a base
 * for their texture coordinates.
 */
void movtex_write_vertex_index(Vtx *verts, s32 index, s16 *movtexVerts, struct MovtexObject *d,
                               s8 attrLayout) {
    u8 alpha = d->a;
    s16 x;
    s16 y;
    s16 z;
    s16 baseS;
    s16 baseT;
    s16 s;
    s16 t;
    s16 offS;
    s16 offT;
    u8 r1;
    u8 g1;
    u8 b1;
    s8 r2;
    s8 g2;
    s8 b2;

    switch (attrLayout) {
        case MOVTEX_LAYOUT_NOCOLOR:
            x = movtexVerts[index * 5 + MOVTEX_ATTR_X];
            y = movtexVerts[index * 5 + MOVTEX_ATTR_Y];
            z = movtexVerts[index * 5 + MOVTEX_ATTR_Z];
            baseS = movtexVerts[MOVTEX_ATTR_NOCOLOR_S];
            baseT = movtexVerts[MOVTEX_ATTR_NOCOLOR_T];
            offS = movtexVerts[index * 5 + MOVTEX_ATTR_NOCOLOR_S];
            offT = movtexVerts[index * 5 + MOVTEX_ATTR_NOCOLOR_T];
            s = baseS + ((offS * 32) * 32U);
            t = baseT + ((offT * 32) * 32U);
            r1 = d->r;
            g1 = d->g;
            b1 = d->b;
            make_vertex(verts, index, x, y, z, s, t, r1, g1, b1, alpha);
            break;
        case MOVTEX_LAYOUT_COLORED:
            x = movtexVerts[index * 8 + MOVTEX_ATTR_X];
            y = movtexVerts[index * 8 + MOVTEX_ATTR_Y];
            z = movtexVerts[index * 8 + MOVTEX_ATTR_Z];
            baseS = movtexVerts[7];
            baseT = movtexVerts[8];
            offS = movtexVerts[index * 8 + 7];
            offT = movtexVerts[index * 8 + 8];
            s = baseS + ((offS * 32) * 32U);
            t = baseT + ((offT * 32) * 32U);
            r2 = movtexVerts[index * 8 + MOVTEX_ATTR_COLORED_R];
            g2 = movtexVerts[index * 8 + MOVTEX_ATTR_COLORED_G];
            b2 = movtexVerts[index * 8 + MOVTEX_ATTR_COLORED_B];
            make_vertex(verts, index, x, y, z, s, t, r2, g2, b2, alpha);
            break;
    }
}

/**
 * Generate a displaylist for a MovtexObject.
 * 'attrLayout' is one of MOVTEX_LAYOUT_NOCOLOR and MOVTEX_LAYOUT_COLORED.
 */
Gfx *movtex_gen_list(s16 *movtexVerts, struct MovtexObject *movtexList, s8 attrLayout) {
    Vtx *verts = alloc_display_list(movtexList->vtx_count * sizeof(*verts));
    Gfx *gfxHead = alloc_display_list(11 * sizeof(*gfxHead));
    Gfx *gfx = gfxHead;
    s32 i;

    if (verts == NULL || gfxHead == NULL) {
        return NULL;
    }

    movtex_write_vertex_first(verts, movtexVerts, movtexList, attrLayout);
    for (i = 1; i < movtexList->vtx_count; i++) {
        movtex_write_vertex_index(verts, i, movtexVerts, movtexList, attrLayout);
    }

    gSPDisplayList(gfx++, movtexList->beginDl);
    gLoadBlockTexture(gfx++, 32, 32, G_IM_FMT_RGBA, gMovtexIdToTexture[movtexList->textureId]);
    gSPVertex(gfx++, VIRTUAL_TO_PHYSICAL2(verts), movtexList->vtx_count, 0);
    gSPDisplayList(gfx++, movtexList->triDl);
    gSPDisplayList(gfx++, movtexList->endDl);
    gSPEndDisplayList(gfx);
    return gfxHead;
}

/**
 * Function for a geo node that draws a MovtexObject in the gMovtexNonColored list.
 */
Gfx *geo_movtex_draw_nocolor(s32 callContext, struct GraphNode *node, UNUSED Mat4 mtx) {
    s32 i;
    s16 *movtexVerts;
    struct GraphNodeGenerated *asGenerated;
    Gfx *gfx = NULL;

    if (callContext == GEO_CONTEXT_RENDER) {
        i = 0;
        asGenerated = (struct GraphNodeGenerated *) node;
        while (gMovtexNonColored[i].movtexVerts != 0) {
            if (gMovtexNonColored[i].geoId == asGenerated->parameter) {
                asGenerated->fnNode.node.flags =
                    (asGenerated->fnNode.node.flags & 0xFF) | (gMovtexNonColored[i].layer << 8);
                movtexVerts = segmented_to_virtual(gMovtexNonColored[i].movtexVerts);
                update_moving_texture_offset(movtexVerts, MOVTEX_ATTR_NOCOLOR_S);
                gfx = movtex_gen_list(movtexVerts, &gMovtexNonColored[i],
                                      MOVTEX_LAYOUT_NOCOLOR); // no perVertex colors
                break;
            }
            i++;
        }
    }
    return gfx;
}
