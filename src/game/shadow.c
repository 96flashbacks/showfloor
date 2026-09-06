#include <PR/ultratypes.h>
#include <PR/gbi.h>
#include <math.h>

#include "engine/math_util.h"
#include "engine/surface_collision.h"
#include "geo_misc.h"
#include "level_table.h"
#include "memory.h"
#include "object_list_processor.h"
#include "rendering_graph_node.h"
#include "segment2.h"
#include "shadow.h"
#include "sm64.h"

/**
 * @file shadow.c
 * This file implements a self-contained subsystem used to draw shadows.
 * The version used here is based on the February 20 1996 backup of 'shade.c'
 * found in the Mario Kart 64 iQue source code's 'kimura.lzh' backup.
 */

// constants from 'shade.c'
#define SHADE_N_GFX_S   5     // Number of DL commands in 'add_shadow_to_display_list'
#define SHADE_H_LIMIT   600.0 // Height limit for rendering shadows
#define SHADE_ALPHA_MIN 80    // Minimum shadow solidity, different value from the final game (120)

#define SHADE_9P        0 // Same as the decomp's 'SHADOW_WITH_9_VERTS'
#define SHADE_4P        1 // Same as the decomp's 'SHADOW_WITH_4_VERTS'

#define SHADE_MARIO     0
#define SHADE_LUIGI     1

/**
 * Encapsulation of information about a shadow.
 */
struct Shadow { // ShadeData
    /* The (x, y, z) position of the object whose shadow this is. */
    f32 parentX;
    f32 parentY;
    f32 parentZ;
    /* The y-position of the floor (or water or lava) underneath the object. */
    f32 floorHeight;
    /* Initial (unmodified) size of the shadow. */
    f32 shadowScale;
    /* (nx, ny, nz) normal vector of the floor underneath the object. */
    f32 floorNormalX;
    f32 floorNormalY;
    f32 floorNormalZ;
    /* "originOffset" of the floor underneath the object. */
    f32 floorOriginOffset;
    /* Angle describing "which way a marble would roll," in degrees. */
    f32 floorDownwardAngle;
    /* Angle describing "how tilted the ground is" in degrees (-90 to 90). */
    f32 floorTilt;
    /* Initial solidity of the shadow, from 0 to 255 (just an alpha value). */
    u8 solidity;
};

/**
 * Constant to indicate that a shadow should not be drawn.
 * This is used to disable shadows during specific frames of Mario's
 * animations.
 */
#define SHADOW_SOLIDITY_NO_SHADOW 0
/**
 * Constant to indicate that a shadow's solidity has been pre-set by a previous
 * function and should not be overwritten.
 */
#define SHADOW_SOILDITY_ALREADY_SET 1
/**
 * Constant to indicate that a shadow's solidity has not yet been set.
 */
#define SHADOW_SOLIDITY_NOT_YET_SET 2

/**
 * Constant to indicate any sort of circular shadow.
 */
#define SHADOW_SHAPE_CIRCLE 10
/**
 * Constant to indicate any sort of rectangular shadow.
 */
#define SHADOW_SHAPE_SQUARE 20

/**
 * Constant to indicate a shadow consists of 9 vertices.
 */
#define SHADOW_WITH_9_VERTS 0
/**
 * Constant to indicate a shadow consists of 4 vertices.
 */
#define SHADOW_WITH_4_VERTS 1

// No shadowRectangle struct or shade_rect_data (hardcoded rectangle shadow s array)

s8 gShadowAboveWaterOrLava; // shade_Wflag

// Missing 3 global variables:
// s8 gMarioOnIceOrCarpet;  // shade_Tflag(?)
// s8 sMarioOnFlyingCarpet; // shade_Aflag(?) (Arajin flag?)
// s16 sSurfaceTypeBelowShadow;

// No ShadeRotateConv ('rotate_rectangle' in the decomp)

/**
 * Return atan2s(x, y) in degrees. Note that the argument order is swapped from
 * the standard atan2.
 */
static f32 atan2_deg(f32 x, f32 y) { // ShadeArcTan
    return ((f32) atan2s(x, y) / 65535.0 * 360.0);
}

/**
 * Shrink a shadow when its parent object is further from the floor, given the
 * initial size of the shadow and the current distance.
 */
static f32 scale_shadow_with_distance(f32 initial, f32 distFromFloor) { // ShadeDecideOneSide
    f32 newScale;

    if (distFromFloor <= 0.0) {
        newScale = initial;
    } else if (distFromFloor >= SHADE_H_LIMIT) {
        newScale = 0.0; // Sets the 'newScale' to 0.0 instead of 'initial * 0.5'
    } else {
        newScale = initial * (1.0 - distFromFloor / SHADE_H_LIMIT); // No 0.5 multiplier on 'distFromFloor'
    }

    return newScale;
}

// No ShadeDecideOneSideZero ('disable_shadow_with_distance' in the decomp), 
// shadows are disabled in the previous function instead

/**
 * Dim a shadow when its parent object is further from the ground.
 */
static u8 dim_shadow_with_distance(u8 solidity, f32 distFromFloor) { // ShadeDecideDensity
    f32 alpha;

    if (solidity <= SHADE_ALPHA_MIN) {
        return solidity;
    } else { // if solidity > SHADE_ALPHA_MIN
        if (distFromFloor <= 0.0) {
            return solidity;
        } else if (distFromFloor >= SHADE_H_LIMIT) {
            return SHADE_ALPHA_MIN;
        } else {
            alpha = ((SHADE_ALPHA_MIN - solidity)) * distFromFloor / SHADE_H_LIMIT + (f32) solidity;
            return alpha;
        }
    }
}

/**
 * Return the water level below a shadow, or 0 if the water level is below
 * -10,000.
 */
static f32 get_water_level_below_shadow(struct Shadow *s) { // ShadeWaterCheck
    f32 waterLevel = find_water_level(s->parentX, s->parentZ);
    if (waterLevel < FLOOR_LOWER_LIMIT_SHADOW) {
        return 0;
    } else if (s->parentY >= waterLevel && waterLevel >= s->floorHeight) {
        gShadowAboveWaterOrLava = TRUE;
        return waterLevel;
    }

    //! @bug Missing return statement. This compiles to return `waterLevel`
    //! incidentally.
}

/**
 * Initialize a shadow. Return 0 on success, 1 on failure.
 *
 * @param xPos,yPos,zPos Position of the parent object (not the shadow)
 * @param shadowScale Diameter of the shadow
 * @param overwriteSolidity Flag for whether the existing shadow solidity should
 *                          be dimmed based on its distance to the floor
 */
static s8 init_shadow(struct Shadow *s, f32 xPos, f32 yPos, f32 zPos, s16 shadowScale, u8 overwriteSolidity) { // ShadeSetStructureData
    f32 waterLevel;
    f32 floorSteepness;
    struct FloorGeometry *floorGeometry;

    s->parentX = xPos;
    s->parentY = yPos;
    s->parentZ = zPos;

    s->floorHeight = find_floor_height_and_data(s->parentX, s->parentY, s->parentZ, &floorGeometry);

    if (gEnvironmentRegions != NULL) {
        waterLevel = get_water_level_below_shadow(s);
    }
    if (gShadowAboveWaterOrLava) {
        //! @bug Use of potentially undefined variable `waterLevel`
        s->floorHeight = waterLevel;

        // Assume that the water is flat.
        s->floorNormalX = 0;
        s->floorNormalY = 1.0;
        s->floorNormalZ = 0;
        s->floorOriginOffset = -waterLevel;
    } else {
        // Don't draw a shadow if the floor is lower than expected possible,
        // or if the y-normal is negative (an unexpected result).
        if (s->floorHeight < FLOOR_LOWER_LIMIT_SHADOW || floorGeometry->normalY <= 0.0) {
            return 1;
        }

        s->floorNormalX = floorGeometry->normalX;
        s->floorNormalY = floorGeometry->normalY;
        s->floorNormalZ = floorGeometry->normalZ;
        s->floorOriginOffset = floorGeometry->originOffset;
    }

    if (overwriteSolidity) {
        s->solidity = dim_shadow_with_distance(overwriteSolidity, yPos - s->floorHeight);
    }

    s->shadowScale = scale_shadow_with_distance(shadowScale, yPos - s->floorHeight);

    s->floorDownwardAngle = atan2_deg(s->floorNormalZ, s->floorNormalX);

    floorSteepness = sqrtf(s->floorNormalX * s->floorNormalX + s->floorNormalZ * s->floorNormalZ);

    // This if-statement avoids dividing by 0.
    if (floorSteepness == 0.0) {
        s->floorTilt = 0;
    } else {
        s->floorTilt = 90.0 - atan2_deg(floorSteepness, s->floorNormalY);
    }
    return 0;
}

/**
 * Given a `vertexNum` from a shadow with nine vertices, update the
 * texture coordinates corresponding to that vertex. That is:
 *      0 = (-15, -15)         1 = (0, -15)         2 = (15, -15)
 *      3 = (-15,   0)         4 = (0,   0)         5 = (15,   0)
 *      6 = (-15,  15)         7 = (0,  15)         8 = (15,  15)
 */
static void get_texture_coords_9_vertices(s8 vertexNum, s16 *textureX, s16 *textureY) { // ShadeCalcTexCoord_9P
    *textureX = vertexNum % 3 * 15 - 15;
    *textureY = vertexNum / 3 * 15 - 15;
}

/**
 * Given a `vertexNum` from a shadow with four vertices, update the
 * texture coordinates corresponding to that vertex. That is:
 *      0 = (-15, -15)         1 = (15, -15)
 *      2 = (-15,  15)         3 = (15,  15)
 */
static void get_texture_coords_4_vertices(s8 vertexNum, s16 *textureX, s16 *textureY) { // ShadeCalcTexCoord_4P
    *textureX = (vertexNum % 2) * 2 * 15 - 15;
    *textureY = (vertexNum / 2) * 2 * 15 - 15;
}

/**
 * Make a shadow's vertex at a position relative to its parent.
 *
 * @param vertices A preallocated display list for vertices
 * @param index Index into `vertices` to insert the vertex
 * @param relX,relY,relZ Vertex position relative to its parent object
 * @param alpha Opacity of the vertex
 * @param shadowVertexType One of SHADOW_WITH_9_VERTS or SHADOW_WITH_4_VERTS
 */
static void make_shadow_vertex_at_xyz(Vtx *vertices, s8 index, f32 relX, f32 relY, f32 relZ, u8 alpha,
                               s8 shadowVertexType) { // ShadeSetEachVtxData
    s16 vtxX = round_float(relX);
    s16 vtxY = round_float(relY);
    s16 vtxZ = round_float(relZ);
    s16 textureX, textureY;

    switch (shadowVertexType) {
        case SHADOW_WITH_9_VERTS:
            get_texture_coords_9_vertices(index, &textureX, &textureY);
            break;
        case SHADOW_WITH_4_VERTS:
            get_texture_coords_4_vertices(index, &textureX, &textureY);
            break;
    }

    // No flying carpet check
    
    make_vertex( // shadows are black
        vertices, index, vtxX, vtxY, vtxZ, textureX << 5, textureY << 5, 255, 255, 255, alpha
    );
}

/**
 * Given an (x, z)-position close to a shadow, extrapolate the y-position
 * according to the floor's normal vector.
 */
static f32 extrapolate_vertex_y_position(struct Shadow s, f32 vtxX, f32 vtxZ) { // ShadeRecalc_y_glb
    return -(s.floorNormalX * vtxX + s.floorNormalZ * vtxZ + s.floorOriginOffset) / s.floorNormalY;
}

/**
 * Given a shadow vertex with the given `index`, return the corresponding texture
 * coordinates ranging in the square with corners at (-1, -1), (1, -1), (-1, 1),
 * and (1, 1) in the x-z plane. See `get_texture_coords_9_vertices()` and
 * `get_texture_coords_4_vertices()`, which have similar functionality, but
 * return 15 times these values.
 */
static void get_vertex_coords(s8 index, s8 shadowVertexType, s8 *xCoord, s8 *zCoord) { // ShadeCalcDevideParam
    *xCoord = index % (3 - shadowVertexType) - 1;
    *zCoord = index / (3 - shadowVertexType) - 1;

    // This just corrects the 4-vertex case to have consistent results with the
    // 9-vertex case.
    // In the 'shade.c' backup it checks if the type is anything other than 0, 
    // rather than if it's specifically equal to 1 (SHADOW_WITH_4_VERTS)
    if (shadowVertexType) {
        if (*xCoord == 0) {
            *xCoord = 1;
        }
        if (*zCoord == 0) {
            *zCoord = 1;
        }
    }
}

/**
 * Populate `xPosVtx`, `yPosVtx`, and `zPosVtx` with the (x, y, z) position of the
 * shadow vertex with the given index. If the shadow is to have 9 vertices,
 * then each of those vertices is clamped down to the floor below it. Otherwise,
 * in the 4 vertex case, the vertex positions are extrapolated from the center
 * of the shadow.
 *
 * In practice, due to the if-statement in `make_shadow_vertex()`, the 9
 * vertex and 4 vertex cases are identical, and the above-described clamping
 * behavior is overwritten.
 */
static void calculate_vertex_xyz(s8 index, struct Shadow s, f32 *xPosVtx, f32 *yPosVtx, f32 *zPosVtx,
                          s8 shadowVertexType) { // ShadeCalcXYZCoord
    f32 tiltedScale = cosf(s.floorTilt * M_PI / 180.0) * s.shadowScale;
    f32 downwardAngle = s.floorDownwardAngle * M_PI / 180.0;
    f32 halfScale;
    f32 halfTiltedScale;
    s8 xCoordUnit;
    s8 zCoordUnit;
    struct FloorGeometry *dummy;

    // This makes xCoordUnit and yCoordUnit each one of -1, 0, or 1.
    get_vertex_coords(index, shadowVertexType, &xCoordUnit, &zCoordUnit);

    halfScale = (xCoordUnit * s.shadowScale) / 2.0;
    halfTiltedScale = (zCoordUnit * tiltedScale) / 2.0;

    *xPosVtx = (halfTiltedScale * sinf(downwardAngle)) + (halfScale * cosf(downwardAngle)) + s.parentX;
    *zPosVtx = (halfTiltedScale * cosf(downwardAngle)) - (halfScale * sinf(downwardAngle)) + s.parentZ;

    if (gShadowAboveWaterOrLava) {
        *yPosVtx = s.floorHeight;
    } else {
        switch (shadowVertexType) {
            /**
             * Note that this dichotomy is later overwritten in
             * make_shadow_vertex().
             */
            case SHADOW_WITH_9_VERTS:
                // Clamp this vertex's y-position to that of the floor directly
                // below it, which may differ from the floor below the center
                // vertex.
                *yPosVtx = find_floor_height_and_data(*xPosVtx, s.parentY, *zPosVtx, &dummy);
                break;
            case SHADOW_WITH_4_VERTS:
                // Do not clamp. Instead, extrapolate the y-position of this
                // vertex based on the directly floor below the parent object.
                *yPosVtx = extrapolate_vertex_y_position(s, *xPosVtx, *zPosVtx);
                break;
        }
    }
}

/**
 * Given a vertex's location, return the dot product of the
 * position of that vertex (relative to the shadow's center) with the floor
 * normal (at the shadow's center).
 *
 * Since it is a dot product, this returns 0 if these two vectors are
 * perpendicular, meaning the ground is locally flat. It returns nonzero
 * in most cases where `vtxY` is on a different floor triangle from the
 * center vertex, as in the case with SHADOW_WITH_9_VERTS, which sets
 * the y-value from `find_floor_height_and_data`. (See the bottom of
 * `calculate_vertex_xyz`.)
 */
static s16 floor_local_tilt(struct Shadow s, f32 vtxX, f32 vtxY, f32 vtxZ) { // ShadeCheckTwoPoint
    f32 relX = vtxX - s.parentX;
    f32 relY = vtxY - s.floorHeight;
    f32 relZ = vtxZ - s.parentZ;

    f32 ret = (relX * s.floorNormalX) + (relY * s.floorNormalY) + (relZ * s.floorNormalZ);
    return ret;
}

/**
 * Make a particular vertex from a shadow, calculating its position and solidity.
 */
static void make_shadow_vertex(Vtx *vertices, s8 index, struct Shadow s, s8 shadowVertexType) { // ShadeSetVtxData
    f32 xPosVtx, yPosVtx, zPosVtx;
    f32 relX, relY, relZ;

    u8 solidity = s.solidity;
    if (gShadowAboveWaterOrLava) {
        solidity = 200;
    }

    calculate_vertex_xyz(index, s, &xPosVtx, &yPosVtx, &zPosVtx, shadowVertexType);

    /**
     * This is the hack that makes "SHADOW_WITH_9_VERTS" act identically to
     * "SHADOW_WITH_4_VERTS" in the game; this same hack is disabled by the
     * GameShark code in this video: https://youtu.be/MSIh4rtNGF0. The code in
     * the video makes `extrapolate_vertex_y_position` return the same value as
     * the last-called function that returns a float; in this case, that's
     * `find_floor_height_and_data`, which this if-statement was designed to
     * overwrite in the first place. Thus, this if-statement is disabled by that
     * code.
     *
     * The last condition here means the y-position calculated previously
     * was probably on a different floor triangle from the center vertex.
     * The gShadowAboveWaterOrLava check is redundant, since `floor_local_tilt`
     * will always be 0 over water or lava (since they are always flat).
     */
    if (shadowVertexType == SHADOW_WITH_9_VERTS && !gShadowAboveWaterOrLava
        && floor_local_tilt(s, xPosVtx, yPosVtx, zPosVtx) != 0) {
        yPosVtx = extrapolate_vertex_y_position(s, xPosVtx, zPosVtx);
        solidity = 0;
    }
    relX = xPosVtx - s.parentX;
    relY = yPosVtx - s.parentY;
    relZ = zPosVtx - s.parentZ;

    make_shadow_vertex_at_xyz(vertices, index, relX, relY, relZ, solidity, shadowVertexType);
}

/**
 * Add a shadow to the given display list.
 */
static void add_shadow_to_display_list(Gfx *displayListHead, Vtx *verts, s8 shadowVertexType) { // ShadeMakeDL
    // No 'shadowShape' variable to switch between square and circle shadows, 
    // since square shadows didn't exist in the demo or the 'shade.c' backup
    gSPDisplayList(displayListHead++, dl_shadow_begin);

    switch (shadowVertexType) {
        case SHADOW_WITH_9_VERTS:
            gSPVertex(displayListHead++, verts, 9, 0);
            gSPDisplayList(displayListHead++, dl_shadow_9_verts);
            break;

        case SHADOW_WITH_4_VERTS:
            gSPVertex(displayListHead++, verts, 4, 0);
            gSPDisplayList(displayListHead++, dl_shadow_4_verts);
            break;
    }

    gSPDisplayList(displayListHead++, dl_shadow_end);
    gSPEndDisplayList(displayListHead);
}

/**
 * Linearly interpolate a shadow's solidity between zero and finalSolidity
 * depending on curr's relation to start and end.
 */
static void linearly_interpolate_solidity_positive(struct Shadow *s, u8 finalSolidity, s16 curr, s16 start,
                                            s16 end) { // ShadePlayerDensUp
    if (curr >= 0 && curr < start) {
        s->solidity = 0;
    } else if (end < curr) {
        s->solidity = finalSolidity;
    } else {
        s->solidity = (f32) finalSolidity * (curr - start) / (end - start);
    }
}

/**
 * Linearly interpolate a shadow's solidity between initialSolidity and zero
 * depending on curr's relation to start and end. Note that if curr < start,
 * the solidity will be zero.
 */
static void linearly_interpolate_solidity_negative(struct Shadow *s, u8 initialSolidity, s16 curr, s16 start,
                                            s16 end) { // ShadePlayerDensDw
    // The curr < start case is not handled. Thus, if start != 0, this function
    // will have the surprising behavior of hiding the shadow until start.
    // This is not necessarily a bug, since this function is only used once,
    // with start == 0.
    if (curr >= start && end >= curr) {
        s->solidity = ((f32) initialSolidity * (1.0 - (f32)(curr - start) / (end - start)));
    } else {
        s->solidity = 0;
    }
}

/**
 * Change a shadow's solidity based on the player's current animation frame.
 */
static s8 correct_shadow_solidity_for_animations(s32 isLuigi, u8 initialSolidity, struct Shadow *shadow) { // ShadeCheckPlayerStatus
    struct Object *player;
    s8 ret;
    s16 animFrame;

    switch (isLuigi) {
        case SHADE_MARIO:
            player = gMarioObject;
            break;
        case SHADE_LUIGI:
            /**
             * This is a remnant of the removed second player, Luigi.
             * This variable lies in memory just after the gMarioObject and
             * has the same type of shadow that Mario does. The `isLuigi`
             * variable is never 1 in the final game.
             */
            player = gLuigiObject;
            break;
    }

    animFrame = player->header.gfx.animInfo.animFrame;
    switch (player->header.gfx.animInfo.animID) {
        case MARIO_ANIM_IDLE_ON_LEDGE:
            ret = SHADOW_SOLIDITY_NO_SHADOW;
            break;
        case MARIO_ANIM_FAST_LEDGE_GRAB:
            linearly_interpolate_solidity_positive(shadow, initialSolidity, animFrame, 5, 14);
            ret = SHADOW_SOILDITY_ALREADY_SET;
            break;
        case MARIO_ANIM_SLOW_LEDGE_GRAB:
            linearly_interpolate_solidity_positive(shadow, initialSolidity, animFrame, 21, 33);
            ret = SHADOW_SOILDITY_ALREADY_SET;
            break;
        case MARIO_ANIM_CLIMB_DOWN_LEDGE:
            linearly_interpolate_solidity_negative(shadow, initialSolidity, animFrame, 0, 5);
            ret = SHADOW_SOILDITY_ALREADY_SET;
            break;
        default:
            ret = SHADOW_SOLIDITY_NOT_YET_SET;
            break;
    }
    return ret;
}

// No Shade_Mario_Patch ('correct_lava_shadow_height' in the decomp),
// meaning shadows don't show up if they are above lava

// Shade_Mario_Luigi ('create_shadow_player' in the decomp) is placed here in the final game

/**
 * Create a circular shadow composed of 9 vertices.
 */
static Gfx *create_shadow_circle_9_verts(f32 xPos, f32 yPos, f32 zPos, s16 shadowScale, u8 solidity) { // Shade_Normal_9Pnt
    Vtx *verts;
    Gfx *displayList;
    struct Shadow shadow;
    s32 i;

    if (init_shadow(&shadow, xPos, yPos, zPos, shadowScale, solidity) != 0) {
        return NULL;
    }

    verts = alloc_display_list(9 * sizeof(Vtx));
    displayList = alloc_display_list(SHADE_N_GFX_S * sizeof(Gfx));

    if (verts == NULL || displayList == NULL) {
        rmonpf(("For debug. Out of memory in ShadeDrawFunc !!\n"));
        return NULL;
    }

    for (i = 0; i < 9; i++) {
        make_shadow_vertex(verts, i, shadow, SHADE_9P);
    }

    add_shadow_to_display_list(displayList, verts, SHADE_9P);

    return displayList;
}

/**
 * Create a circular shadow composed of 4 vertices.
 */
static Gfx *create_shadow_circle_4_verts(f32 xPos, f32 yPos, f32 zPos, s16 shadowScale, u8 solidity) { // Shade_Silent_Bank
    Vtx *verts;
    Gfx *displayList;
    struct Shadow shadow;
    s32 i;

    if (init_shadow(&shadow, xPos, yPos, zPos, shadowScale, solidity) != 0) {
        return NULL;
    }

    verts = alloc_display_list(4 * sizeof(Vtx));
    displayList = alloc_display_list(SHADE_N_GFX_S * sizeof(Gfx));

    if (verts == NULL || displayList == NULL) {
        rmonpf(("For debug. Out of memory in ShadeDrawFunc !!\n"));
        return NULL;
    }

    for (i = 0; i < 4; i++) {
        make_shadow_vertex(verts, i, shadow, SHADE_4P);
    }

    add_shadow_to_display_list(displayList, verts, SHADE_4P);

    return displayList;
}

/**
 * Create a circular shadow composed of 4 vertices and assume that the ground
 * underneath it is totally flat.
 */
static Gfx *create_shadow_circle_assuming_flat_ground(f32 xPos, f32 yPos, f32 zPos, s16 shadowScale,
                                               u8 solidity) { // Shade_Silent_Flat (modified)
    Vtx *verts;
    Gfx *displayList;
    struct FloorGeometry *dummy; // only for calling find_floor_height_and_data
    f32 floorHeight = find_floor_height_and_data(xPos, yPos, zPos, &dummy);
    f32 distBelowFloor = floorHeight - yPos; // Placed after 'floorHeight' and immediately assigned a value
    f32 radius; // Changed to be assigned later so the shadow can scale

    // Missing the (floorHeight < -10000.0) check, 
    // 'distBelowFloor' is assigned here in the final game

    // Changed so the shadow scales and dims, Thwomps appear to use this shadow type in footage
    shadowScale = scale_shadow_with_distance(shadowScale, yPos - floorHeight);
    radius = shadowScale / 2;
    solidity = dim_shadow_with_distance(solidity, yPos - floorHeight);

    verts = alloc_display_list(4 * sizeof(Vtx));
    displayList = alloc_display_list(SHADE_N_GFX_S * sizeof(Gfx));

    if (verts == NULL || displayList == NULL) {
        rmonpf(("For debug. Out of memory in ShadeDrawFunc !!\n"));
        return NULL;
    }

    make_shadow_vertex_at_xyz(verts, 0, -radius, distBelowFloor, -radius, solidity, SHADE_4P);
    make_shadow_vertex_at_xyz(verts, 1,  radius, distBelowFloor, -radius, solidity, SHADE_4P);
    make_shadow_vertex_at_xyz(verts, 2, -radius, distBelowFloor,  radius, solidity, SHADE_4P);
    make_shadow_vertex_at_xyz(verts, 3,  radius, distBelowFloor,  radius, solidity, SHADE_4P);

    add_shadow_to_display_list(displayList, verts, SHADE_4P);

    return displayList;
}

// No Shade_Square_Comn (create_shadow_rectangle), Shade_Square_Wcheck (get_shadow_height_solidity),
// Shade_Square_Flat (create_shadow_square) and Shade_Rectng_Flat (create_shadow_hardcoded_rectangle)

// Placed later than the final game
/**
 * Create a shadow under a player, correcting that shadow's opacity during
 * appropriate animations and other states.
 */
static Gfx *create_shadow_player(f32 xPos, f32 yPos, f32 zPos, s16 shadowScale, u8 solidity, s32 isLuigi) { // Shade_Mario_Luigi
    Vtx *verts;
    Gfx *displayList;
    struct Shadow shadow;
    s8 ret;
    s32 i;

    // No flying carpet checks

    switch (correct_shadow_solidity_for_animations(isLuigi, solidity, &shadow)) {
        case SHADOW_SOLIDITY_NO_SHADOW: 
            return NULL; 
            break;
        case SHADOW_SOILDITY_ALREADY_SET: 
            ret = init_shadow(&shadow, xPos, yPos, zPos, shadowScale, /* overwriteSolidity */ 0); 
            break;
        case SHADOW_SOLIDITY_NOT_YET_SET: 
            ret = init_shadow(&shadow, xPos, yPos, zPos, shadowScale, solidity);
            break;
    }

    if (ret != 0) { 
        return NULL;
    }

    verts = alloc_display_list(9 * sizeof(Vtx));
    displayList = alloc_display_list(SHADE_N_GFX_S * sizeof(Gfx));

    if (verts == NULL || displayList == NULL) {
        rmonpf(("For debug. Out of memory in ShadeDrawFunc !!\n"));
        return NULL;
    }

    // No 'correct_lava_shadow_height'

    for (i = 0; i < 9; i++) {
        make_shadow_vertex(verts, i, shadow, SHADE_9P);
    }

    add_shadow_to_display_list(displayList, verts, SHADE_9P);

    return displayList;
}


/**
 * Create a shadow at the absolute position given, with the given parameters.
 * Return a pointer to the display list representing the shadow.
 */
extern Gfx *create_shadow_below_xyz(f32 xPos, f32 yPos, f32 zPos, s16 shadowScale, u8 shadowSolidity,
                             s8 shadowType) { // ShadeDrawFunc (modified)
    Gfx *displayList; // not initialized as NULL

    // Since only 'gShadowAboveWaterOrLava' exists, it's the only one reset
    gShadowAboveWaterOrLava = FALSE;

    // Missing 'SURFACE_ICE' floor type check

    switch (shadowType) {
        case SHADOW_CIRCLE_9_VERTS:
            displayList = create_shadow_circle_9_verts(xPos, yPos, zPos, shadowScale, shadowSolidity);
            break;
        case SHADOW_CIRCLE_4_VERTS:
            displayList = create_shadow_circle_4_verts(xPos, yPos, zPos, shadowScale, shadowSolidity);
            break;
        case SHADOW_CIRCLE_4_VERTS_FLAT: // Unused in the final game, seemingly used by Thwomps in the demo
            displayList = create_shadow_circle_assuming_flat_ground(xPos, yPos, zPos, shadowScale, shadowSolidity);
            break;
        // Missing cases 10, 11 and 12 (the 3 square shadow types)
        case SHADOW_CIRCLE_MARIO:
            displayList = create_shadow_player(xPos, yPos, zPos, shadowScale, shadowSolidity, SHADE_MARIO);
            break;
        // The 'shade.c' backup has a case for ID 98 commented out, showing it was used to handle Luigi's shadow
        case SHADOW_CIRCLE_LUIGI:
            displayList = create_shadow_player(xPos, yPos, zPos, shadowScale, shadowSolidity, SHADE_LUIGI);
            break;
        // No 'default' case
    }

    return displayList;
}
