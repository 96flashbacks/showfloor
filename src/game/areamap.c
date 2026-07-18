#include <ultra64.h>

#include "sm64.h"
#include "areamap.h"
#include "segment2.h"
#include "engine/graph_node.h"
#include "memory.h"
#include "game_init.h"
#include "geo_misc.h"
#include "level_update.h"

// These are included since the pointers are defined on each level's header
#include "levels/castle_grounds/header.h"
#include "levels/castle_courtyard/header.h"
#include "levels/wf/header.h"
#include "levels/lll/header.h"
#include "levels/ccm/header.h"
#include "levels/ddd/header.h"

// Areamap status
#define STATUS_HIDDEN 0
#define STATUS_SHOWING 1
#define STATUS_VISIBLE 2
#define STATUS_HIDING 3

// Pointers placed in a similar order to messnum.h
struct AreaMapData *sAreaMapPtrs[] = {
    &castle_grounds_areamap, &courtyard_areamap,         &mountain_areamap,          &fire_bubble_areamap,
    &snow_slider_areamap,    &water_land_area_1_areamap, &water_land_area_2_areamap, NULL,
};

// Initial positions and status (hidden)
static s8 sAreamapStatus = STATUS_HIDDEN;
static f32 sAreamapX = 260.0f;
static f32 sAreamapY = 300.0f;

void *create_areamap_ortho_matrix() {
    Mtx *mtx = alloc_display_list(sizeof(Mtx));

    if (mtx != NULL) {
        guOrtho(mtx, 0.0f, SCREEN_WIDTH, 0.0f, SCREEN_HEIGHT, -10.0f, 10.0f, 1.0f);
    }

    return mtx;
}

/* 
 * Renders the base of the areamap, with the same alpha and filtering as the power meter
 */
void render_areamap_base_dl(Gfx **dlist, struct AreaMapData *areaMap) {
    Mtx *translate = alloc_display_list(sizeof(Mtx));
    Vtx *vertex = alloc_display_list(8 * sizeof(Vtx));
    u8 *textTop = segmented_to_virtual(areaMap->image_upper);
    u8 *textBottom = segmented_to_virtual(areaMap->image_lower);

    // Same alpha value as meter.sou (220)
    make_vertex(vertex, 0, -32, 0, 0, 0, 1030, 255, 255, 255, 220);
    make_vertex(vertex, 1, 32, 0, 0, 2048, 1030, 255, 255, 255, 220);
    make_vertex(vertex, 2, 32, 32, 0, 2048, 0, 255, 255, 255, 220);
    make_vertex(vertex, 3, -32, 32, 0, 0, 0, 255, 255, 255, 220);
    make_vertex(vertex, 4, -32, -32, 0, 0, 1030, 255, 255, 255, 220);
    make_vertex(vertex, 5, 32, -32, 0, 2048, 1030, 255, 255, 255, 220);
    make_vertex(vertex, 6, 32, 0, 0, 2048, 0, 255, 255, 255, 220);
    make_vertex(vertex, 7, -32, 0, 0, 0, 0, 255, 255, 255, 220);

    guTranslate(translate, sAreamapX, sAreamapY, 0.0f);
    gSPMatrix((*dlist)++, translate, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_PUSH);

    gDPSetTextureFilter((*dlist)++, G_TF_POINT); // Same filter as the power meter
    gSPVertex((*dlist)++, vertex, 8, 0);

    gLoadBlockTexture((*dlist)++, 64, 32, G_IM_FMT_RGBA, textTop);
    gSPDisplayList((*dlist)++, dl_draw_quad_verts_0123);

    gLoadBlockTexture((*dlist)++, 64, 32, G_IM_FMT_RGBA, textBottom);
    gSPDisplayList((*dlist)++, dl_draw_quad_verts_4567);

    gSPPopMatrix((*dlist)++, G_MTX_MODELVIEW);
}

/*
 * Renders the arrow, which follows Mario's horizontal positions and facing angle.
 * The variables defined in areamap.h (xo_g, zo_g, and side_g) are used to control
 * how the arrow is offset, which can be used to match the dimensions of the
 * areamap base textures if they aren't centered or at a different scale.
 */
void render_areamap_arrow_dl(Gfx **dlist, struct AreaMapData *areaMap) {
    Mtx *translate = alloc_display_list(sizeof(Mtx));
    Mtx *rotate = alloc_display_list(sizeof(Mtx));
    Vtx *vertex = alloc_display_list(4 * sizeof(Vtx));

    make_vertex(vertex, 0, -4, -4, 0, 0, 256, 255, 0, 0, 255);
    make_vertex(vertex, 1, 4, -4, 0, 256, 256, 255, 0, 0, 255);
    make_vertex(vertex, 2, 4, 4, 0, 256, 0, 255, 0, 0, 255);
    make_vertex(vertex, 3, -4, 4, 0, 0, 0, 255, 0, 0, 255);

    guTranslate(translate,
                (sAreamapX + (gMarioState->pos[0] / 256) * areaMap->side_g + areaMap->xo_g),
                (sAreamapY - (gMarioState->pos[2] / 256) * areaMap->side_g + areaMap->zo_g), 0.0f);
    guRotate(rotate, gMarioState->faceAngle[1] / 180 + 180, 0.0f, 0.0f, 1.0f);
    gSPMatrix((*dlist)++, translate, G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_PUSH);
    gSPMatrix((*dlist)++, rotate, G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);

    gDPSetTextureFilter((*dlist)++, G_TF_BILERP); // Different filter from the base, gives it AA
    gSPVertex((*dlist)++, vertex, 4, 0);

    gSPDisplayList((*dlist)++, dl_draw_quad_verts_0123);

    gSPPopMatrix((*dlist)++, G_MTX_MODELVIEW);
}

/* 
 * Creates the areamap's display lists, nearly identical implementation to init_skybox_display_list
 */
Gfx *init_areamap_dls(struct AreaMapData *areaMap) {
    s32 dlCommandCount = 6 + 16 + 6; // 6 for the start and end, 16 for the base and 6 for the arrow
    void *areamap = alloc_display_list(dlCommandCount * sizeof(Gfx));
    Gfx *dlist = areamap;

    if (areamap == NULL) {
        return NULL;
    } else {
        Mtx *ortho = create_areamap_ortho_matrix();

        gSPDisplayList(dlist++, dl_ia8_up_arrow_begin);
        gSPMatrix(dlist++, VIRTUAL_TO_PHYSICAL(ortho), G_MTX_PROJECTION | G_MTX_MUL | G_MTX_NOPUSH);
        
        gSPDisplayList(dlist++, dl_rgba16_unused);
        render_areamap_base_dl(&dlist, areaMap);
        
        gSPDisplayList(dlist++, dl_ia8_up_arrow_load_texture_block);
        render_areamap_arrow_dl(&dlist, areaMap);

        gSPDisplayList(dlist++, dl_ia8_up_arrow_end);

        gSPEndDisplayList(dlist);
    }
    return areamap;
}

/*
 * Moves the areamap when the R trigger is pressed
 */
void update_areamap() {
    if (gPlayer3Controller->buttonPressed & R_TRIG) {
        switch (sAreamapStatus) {
            case STATUS_HIDDEN:  
                sAreamapStatus = STATUS_SHOWING;
                break;
            case STATUS_VISIBLE:
                sAreamapStatus = STATUS_HIDING;
                break;
        }
    }

    switch (sAreamapStatus) {
        case STATUS_SHOWING:
            sAreamapY -= 10.f;
            if (sAreamapY < 190.f) {
                sAreamapY = 190.f;
                sAreamapStatus = STATUS_VISIBLE;
            }
            break;
        case STATUS_HIDING:
            sAreamapY += 10.f;
            if (sAreamapY > 300.f) {
                sAreamapY = 300.f;
                sAreamapStatus = STATUS_HIDDEN;
            }
            break;
    }
}

/* 
 * The main AreaMap function called by a level's Geolayout
 */
Gfx *AreaMap(s32 callContext, struct GraphNode *node, UNUSED void *data) {
    struct GraphNodeGenerated *asGenerated = (struct GraphNodeGenerated *) node;
    struct AreaMapData *areaMap = segmented_to_virtual(sAreaMapPtrs[asGenerated->parameter]);
    Gfx *dlist = NULL;

    if (callContext == GEO_CONTEXT_AREA_INIT) { // If a new level is being loaded...
        sAreamapStatus = STATUS_HIDDEN; // Set the map to be hidden
        sAreamapY = 300.0f;
    } else if (callContext == GEO_CONTEXT_RENDER) { // Otherwise, render the map as usual
        asGenerated->fnNode.node.flags =
            (asGenerated->fnNode.node.flags & 0xFF) | (LAYER_TRANSPARENT << 8); // Is this node flag necessary?

        // Render before updating
        dlist = init_areamap_dls(areaMap);
            
        // Update the areamap
        update_areamap();
    }

    return dlist;
}
