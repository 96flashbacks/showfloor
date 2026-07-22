#include <PR/ultratypes.h>

#include "game/memory.h"
#include "game/segment2.h"
#include "game/segment7.h"
#include "intro_geo.h"
#include "sm64.h"
#include "textures.h"
#include "types.h"
#include "buffers/framebuffers.h"
#include "game/game_init.h"

struct GraphNodeMore {
    /*0x00*/ struct GraphNode node;
    /*0x14*/ void *todo;
    /*0x18*/ u32 unk18;
};

/**
 * Geo callback to render the Nintendo logo on the title screen
 */
Gfx *geo_intro_nintendo_logo(s32 state, struct GraphNode *node, UNUSED void *context) {
    struct GraphNode *graphNode = node;
    Gfx *dl = NULL;
    Gfx *dlIter = NULL;
    Mtx *scaleMat;

    if (state == 1) {
        graphNode->flags = (graphNode->flags & 0xFF) | (LAYER_OPAQUE << 8);
        scaleMat = alloc_display_list(sizeof(*scaleMat));
        dl = alloc_display_list(4 * sizeof(*dl));
        dlIter = dl;

        guScale(scaleMat, 2.5f, 2.5f, 0.0f);

        gSPMatrix(dlIter++, scaleMat, G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_PUSH);
        gSPDisplayList(dlIter++, &gfx_nintendo_logo); // draw model
        gSPPopMatrix(dlIter++, G_MTX_MODELVIEW);
        gSPEndDisplayList(dlIter);
    }
    return dl;
}

/**
 * Generates a display list for a single background tile
 *
 * @param index            which tile to render (value from 0 to 11)
 */
static Gfx *intro_backdrop_one_image(s32 index) {
    // intro screen background display lists for each of four 80x20 textures
    static const Gfx *introBackgroundDlRows[] = { title_screen_bg_dl_0A000130,
                                                  title_screen_bg_dl_0A000148,
                                                  title_screen_bg_dl_0A000160,
                                                  title_screen_bg_dl_0A000178 };

    // intro screen background texture X offsets
    static float xCoords[] = {
        0, 80, 160, 240, 0, 80, 160, 240, 0, 80, 160, 240,
    };

    // intro screen background texture Y offsets
    static float yCoords[] = {
        160, 160, 160, 160, 80, 80, 80, 80, 0, 0, 0, 0,
    };

    Mtx *mtx = alloc_display_list(sizeof(*mtx));
    Gfx *displayList = alloc_display_list(36 * sizeof(*displayList));
    Gfx *displayListIter = displayList;
    const u8 *const *vIntroBgTable = segmented_to_virtual(mario_title_texture_table);
    s32 i;

    guTranslate(mtx, xCoords[index], yCoords[index], 0.0f);
    gSPMatrix(displayListIter++, mtx, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_PUSH);
    gSPDisplayList(displayListIter++, &title_screen_bg_dl_0A000118);
    for (i = 0; i < 4; i++) {
        gDPLoadTextureBlock(displayListIter++, vIntroBgTable[i], G_IM_FMT_RGBA, G_IM_SIZ_16b, 80, 20, 0,
                            G_TX_CLAMP, G_TX_CLAMP, 7, 6, G_TX_NOLOD, G_TX_NOLOD)
            gSPDisplayList(displayListIter++, introBackgroundDlRows[i]);
    }
    gSPPopMatrix(displayListIter++, G_MTX_MODELVIEW);
    gSPEndDisplayList(displayListIter);
    return displayList;
}

/**
 * Geo callback to render the intro background tiles
 */
Gfx *geo_intro_regular_backdrop(s32 state, struct GraphNode *node, UNUSED void *context) {
    struct GraphNodeMore *graphNode = (struct GraphNodeMore *) node;
    Gfx *dl = NULL;
    Gfx *dlIter = NULL;
    s32 i;

    if (state == 1) { // draw
        dl = alloc_display_list(16 * sizeof(*dl));
        dlIter = dl;
        graphNode->node.flags = (graphNode->node.flags & 0xFF) | (LAYER_OPAQUE << 8);
        gSPDisplayList(dlIter++, &dl_proj_mtx_fullscreen);
        gSPDisplayList(dlIter++, &title_screen_bg_dl_0A000100);
        for (i = 0; i < 12; i++) {
            gSPDisplayList(dlIter++, intro_backdrop_one_image(i));
        }
        gSPDisplayList(dlIter++, &title_screen_bg_dl_0A000190);
        gSPEndDisplayList(dlIter);
    }
    return dl;
}
