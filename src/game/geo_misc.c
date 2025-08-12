#include <PR/ultratypes.h>

#include "sm64.h"
#include "geo_misc.h"

#include "area.h"
#include "engine/math_util.h"
#include "level_update.h"
#include "levels/castle_inside/header.h"
#include "mario.h"
#include "mario_actions_cutscene.h"
#include "memory.h"
#include "object_list_processor.h"
#include "rendering_graph_node.h"
#include "save_file.h"
#include "segment2.h"

/**
 * @file geo_misc.c
 * This file contains miscellaneous geo_asm scripts.
 */

/**
 * Create a vertex with the given parameters and insert it into `vtx` at
 * position `n`.
 *
 * Texture coordinates are s10.5 fixed-point, which means you should left-shift the actual coordinates
 * by 5.
 */
#ifndef GBI_FLOATS
void make_vertex(Vtx *vtx, s32 n, s16 x, s16 y, s16 z, s16 tx, s16 ty, u8 r, u8 g, u8 b, u8 a)
#else
void make_vertex(Vtx *vtx, s32 n, f32 x, f32 y, f32 z, s16 tx, s16 ty, u8 r, u8 g, u8 b, u8 a)
#endif
{
    vtx[n].v.ob[0] = x;
    vtx[n].v.ob[1] = y;
    vtx[n].v.ob[2] = z;

    vtx[n].v.flag = 0;

    vtx[n].v.tc[0] = tx;
    vtx[n].v.tc[1] = ty;

    vtx[n].v.cn[0] = r;
    vtx[n].v.cn[1] = g;
    vtx[n].v.cn[2] = b;
    vtx[n].v.cn[3] = a;
}

/**
 * Round `num` to the nearest `s16`.
 */
s16 round_float(f32 num) {
    // Note that double literals are used here, rather than float literals.
    if (num >= 0.0) {
        return num + 0.5;
    } else {
        return num - 0.5;
    }
}
// --- Mario light override ---
#include "PR/gbi.h"
#include "game/game_init.h" // gDisplayListHead

#define LIGHT_X 0
#define LIGHT_Y 0
#define LIGHT_Z 127

/**
 * Forces Mario's lighting to a fixed direction, ignoring stage lighting.
 * Called via GEO_ASM in Mario's geo layout.
 */
Gfx *geo_mario_force_light(s32 callContext, struct GraphNode *node, UNUSED Mat4 *mtx) {
    if (callContext == GEO_CONTEXT_RENDER) {
        static Lights1 sMarioForcedLight = gdSPDefLights1(
            0x3f, 0x3f, 0x3f,   // ambient RGB
            0xff, 0xff, 0xff,   // diffuse RGB
            LIGHT_X, LIGHT_Y, LIGHT_Z // direction (signed bytes)
        );
        gSPSetLights1(gDisplayListHead++, sMarioForcedLight);
    }
    return NULL;
}
