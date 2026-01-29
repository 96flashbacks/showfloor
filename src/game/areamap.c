#include <ultra64.h>

#include "sm64.h"
#include "areamap.h"
#include "ingame_menu.h"
#include "segment2.h"
#include "engine/graph_node.h"
#include "memory.h"
#include "game_init.h"
#include "geo_misc.h"
#include "level_update.h"
#include "headers.h"

#include "levels/bbh/header.h"
#include "levels/bitdw/header.h"
#include "levels/bob/header.h"
#include "levels/ccm/header.h"
#include "levels/ddd/header.h"
#include "levels/jrb/header.h"
#include "levels/thi/header.h"
#include "levels/castle_grounds/header.h"
#include "levels/lll/header.h"
#include "levels/ddd/header.h"
#include "levels/wf/header.h"
#include "levels/castle_courtyard/header.h"

/*
 * map pointers
 */
struct AreaMapData *gMapPtrs[] = {
    &areaMap_04_01, /* big boo's haunt  */
    &areaMap_05_01, /* cool cool mountain  */
    &areaMap_12_01, /* jolly roger bay */
    &areaMap_13_01, /* tiny-huge island */
    &areaMap_16_01, /* castle grounds */
    &areaMap_22_01, /* lethal lava land */
    &areaMap_23_01, /* dire dire docks - area 1 */
    &areaMap_23_02, /* dire dire docks - area 2 */
    &areaMap_24_01, /* whomp's fortress */
    &areaMap_26_01, /* courtyard */
    &areaMap_27_01,  /* bob'omb battlefield  */
    &areaMap_28_01  /* bowser in the dark world/yoko scrl  */
};

/*
 * map status definitions
 */
#define MAP_STATUS_OFFSCREEN 0
#define MAP_STATUS_LOWERING 1
#define MAP_STATUS_ONSCREEN 2
#define MAP_STATUS_RISING 3

/*
 * areamap status and Y position
 */
static s8 gMapStatus = MAP_STATUS_OFFSCREEN;
static f32 gMapPosi = 300.0f;

/*
 * base of the areamap
 */
void render_map_base(Gfx **dl, struct AreaMapData *mapPtr) {
    Mtx *translate = alloc_display_list(sizeof(Mtx));
    Vtx *vertex = alloc_display_list(8 * sizeof(Vtx));
    u8 *textTop = segmented_to_virtual(mapPtr->image_upper);
    u8 *textBottom = segmented_to_virtual(mapPtr->image_lower);

    make_vertex(vertex, 0, -32, 0, 0, 0, 1030, 255, 255, 255, 220); /* power meter alpha value */
    make_vertex(vertex, 1, 32, 0, 0, 2048, 1030, 255, 255, 255, 220);
    make_vertex(vertex, 2, 32, 32, 0, 2048, 0, 255, 255, 255, 220);
    make_vertex(vertex, 3, -32, 32, 0, 0, 0, 255, 255, 255, 220);
    make_vertex(vertex, 4, -32, -32, 0, 0, 1030, 255, 255, 255, 220);
    make_vertex(vertex, 5, 32, -32, 0, 2048, 1030, 255, 255, 255, 220);
    make_vertex(vertex, 6, 32, 0, 0, 2048, 0, 255, 255, 255, 220);
    make_vertex(vertex, 7, -32, 0, 0, 0, 0, 255, 255, 255, 220);

    guTranslate(translate, mapPtr->map_x, gMapPosi, 0.0f);
    gSPMatrix((*dl)++, translate, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_PUSH);

    gDPSetTextureFilter((*dl)++, G_TF_POINT); /* power meter texture filter */
    gSPVertex((*dl)++, vertex, 8, 0);

    gLoadBlockTexture((*dl)++, 64, 32, G_IM_FMT_RGBA, textTop);
    gSPDisplayList((*dl)++, dl_draw_quad_verts_0123);

    gLoadBlockTexture((*dl)++, 64, 32, G_IM_FMT_RGBA, textBottom);
    gSPDisplayList((*dl)++, dl_draw_quad_verts_4567);

    gSPPopMatrix((*dl)++, G_MTX_MODELVIEW);
}

/*
 * areamap arrow
 */
void render_map_arrow(Gfx **dl, struct AreaMapData *mapPtr) {
    Mtx *translate = alloc_display_list(sizeof(Mtx));
    Mtx *rotate = alloc_display_list(sizeof(Mtx));
    Vtx *vertex = alloc_display_list(4 * sizeof(Vtx));

    make_vertex(vertex, 0, -4, -4, 0, 0, 256, 255, 0, 0, 255);
    make_vertex(vertex, 1, 4, -4, 0, 256, 256, 255, 0, 0, 255);
    make_vertex(vertex, 2, 4, 4, 0, 256, 0, 255, 0, 0, 255);
    make_vertex(vertex, 3, -4, 4, 0, 0, 0, 255, 0, 0, 255);

    guTranslate(translate,
                (mapPtr->map_x + (gMarioState->pos[0] / 256) * mapPtr->side_g + mapPtr->xo_g),
                (gMapPosi - (gMarioState->pos[2] / 256) * mapPtr->side_g + mapPtr->zo_g), 0.0f);
    guRotate(rotate, gMarioState->faceAngle[1] / 180 + 180, 0.0f, 0.0f, 1.0f);
    gSPMatrix((*dl)++, translate, G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_PUSH);
    gSPMatrix((*dl)++, rotate, G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);

    gDPSetTextureFilter((*dl)++, G_TF_POINT); /* power meter texture filter */
    gSPVertex((*dl)++, vertex, 4, 0);

    gSPDisplayList((*dl)++, dl_draw_quad_verts_0123);

    gSPPopMatrix((*dl)++, G_MTX_MODELVIEW);
}

/*
 * main areamap function
 */
Gfx *AreaMap(s32 callContext, struct GraphNode *node, UNUSED Mat4 *c) {
    struct GraphNodeGenerated *asGenerated = (struct GraphNodeGenerated *) node;
    Gfx *dl = NULL;
    Gfx *dlIter = NULL;
    Mtx *matrix;
    struct AreaMapData *mapPtr;

    if (callContext == GEO_CONTEXT_AREA_INIT) {
        gMapStatus = MAP_STATUS_OFFSCREEN;
        gMapPosi = 300.0f;
    } else if (callContext == GEO_CONTEXT_RENDER) {
        asGenerated->fnNode.node.flags =
            (asGenerated->fnNode.node.flags & 0xFF) | (LAYER_TRANSPARENT << 8);
        dl = alloc_display_list(32 * sizeof(Gfx)); /* TO-DO: make this smaller */
        dlIter = dl;
        mapPtr = segmented_to_virtual(gMapPtrs[asGenerated->parameter]);

        gSPDisplayList(dlIter++, dl_ia8_up_arrow_begin);

        matrix = alloc_display_list(sizeof(Mtx));

        if (matrix == NULL) {
            return NULL;
        }

        /* copied from create_dl_ortho_matrix but modified to use our own pointer */
        guOrtho(matrix, 0.0f, SCREEN_WIDTH, 0.0f, SCREEN_HEIGHT, -10.0f, 10.0f, 1.0f);
        gSPMatrix(dlIter++, VIRTUAL_TO_PHYSICAL(matrix), G_MTX_PROJECTION | G_MTX_MUL | G_MTX_NOPUSH);

        gSPDisplayList(dlIter++, dl_rgba16_unused);
        render_map_base(&dlIter, mapPtr);

        gSPDisplayList(dlIter++, dl_ia8_up_arrow_load_texture_block);
        render_map_arrow(&dlIter, mapPtr);

        gSPDisplayList(dlIter++, dl_ia8_up_arrow_end);

        gSPEndDisplayList(dlIter);

        if (gPlayer3Controller->buttonPressed & R_TRIG) {
            if (gMapStatus == MAP_STATUS_OFFSCREEN) {
                gMapStatus = MAP_STATUS_LOWERING;
            } else if (gMapStatus == MAP_STATUS_ONSCREEN) {
                gMapStatus = MAP_STATUS_RISING;
            }
        }

        if (gMapStatus == MAP_STATUS_LOWERING) {
            gMapPosi -= 15.0f;
            if (gMapPosi < 190.0f) {
                gMapPosi = 190.0f;
                gMapStatus = MAP_STATUS_ONSCREEN;
            }
        } else if (gMapStatus == MAP_STATUS_RISING) {
            gMapPosi += 15.0f;
            if (gMapPosi > 300.0f) {
                gMapPosi = 300.0f;
                gMapStatus = MAP_STATUS_OFFSCREEN;
            }
        }
    }
    return dl;
}
