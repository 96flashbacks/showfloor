#include <ultra64.h>
#include "sm64.h"

#include "make_const_nonconst.h"

// 0x0A000000 - 0x0A000100
static const Vtx title_screen_bg_vertex_0A000000[] = {
    {{{     0,     60,     -1}, 0, {     0,    608}, {0xff, 0xff, 0xff, 0xff}}},
    {{{    80,     60,     -1}, 0, {  2528,    608}, {0xff, 0xff, 0xff, 0xff}}},
    {{{    80,     80,     -1}, 0, {  2528,      0}, {0xff, 0xff, 0xff, 0xff}}},
    {{{     0,     80,     -1}, 0, {     0,      0}, {0xff, 0xff, 0xff, 0xff}}},
    {{{     0,     40,     -1}, 0, {     0,    608}, {0xff, 0xff, 0xff, 0xff}}},
    {{{    80,     40,     -1}, 0, {  2528,    608}, {0xff, 0xff, 0xff, 0xff}}},
    {{{    80,     60,     -1}, 0, {  2528,      0}, {0xff, 0xff, 0xff, 0xff}}},
    {{{     0,     60,     -1}, 0, {     0,      0}, {0xff, 0xff, 0xff, 0xff}}},
    {{{     0,     20,     -1}, 0, {     0,    608}, {0xff, 0xff, 0xff, 0xff}}},
    {{{    80,     20,     -1}, 0, {  2528,    608}, {0xff, 0xff, 0xff, 0xff}}},
    {{{    80,     40,     -1}, 0, {  2528,      0}, {0xff, 0xff, 0xff, 0xff}}},
    {{{     0,     40,     -1}, 0, {     0,      0}, {0xff, 0xff, 0xff, 0xff}}},
    {{{     0,      0,     -1}, 0, {     0,    608}, {0xff, 0xff, 0xff, 0xff}}},
    {{{    80,      0,     -1}, 0, {  2528,    608}, {0xff, 0xff, 0xff, 0xff}}},
    {{{    80,     20,     -1}, 0, {  2528,      0}, {0xff, 0xff, 0xff, 0xff}}},
    {{{     0,     20,     -1}, 0, {     0,      0}, {0xff, 0xff, 0xff, 0xff}}},
};

// 0x0A000100 - 0x0A000118
const Gfx title_screen_bg_dl_0A000100[] = {
    gsDPSetCombineMode(G_CC_DECALRGB, G_CC_DECALRGB),
    gsDPSetRenderMode(G_RM_AA_OPA_SURF, G_RM_AA_OPA_SURF2),
    gsSPEndDisplayList(),
};

// 0x0A000118 - 0x0A000130
const Gfx title_screen_bg_dl_0A000118[] = {
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPVertex(title_screen_bg_vertex_0A000000, 16, 0),
    gsSPEndDisplayList(),
};

// 0x0A000130 - 0x0A000148
const Gfx title_screen_bg_dl_0A000130[] = {
    gsSP2Triangles( 0,  1,  2, 0x0,  0,  2,  3, 0x0),
    gsSPEndDisplayList(),
};

// 0x0A000148 - 0x0A000160
const Gfx title_screen_bg_dl_0A000148[] = {
    gsSP2Triangles( 4,  5,  6, 0x0,  4,  6,  7, 0x0),
    gsSPEndDisplayList(),
};

// 0x0A000160 - 0x0A000178
const Gfx title_screen_bg_dl_0A000160[] = {
    gsSP2Triangles( 8,  9, 10, 0x0,  8, 10, 11, 0x0),
    gsSPEndDisplayList(),
};

// 0x0A000178 - 0x0A000190
const Gfx title_screen_bg_dl_0A000178[] = {
    gsSP2Triangles(12, 13, 14, 0x0, 12, 14, 15, 0x0),
    gsSPEndDisplayList(),
};

// 0x0A000190 - 0x0A0001C0
const Gfx title_screen_bg_dl_0A000190[] = {
    gsDPPipeSync(),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsSPSetGeometryMode(G_LIGHTING),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
    gsSPEndDisplayList(),
};

// 0x0A0001C0
ALIGNED8 static const Texture title_texture_0A0001C0[] = {
#include "textures/title_screen_bg/title_screen_bg.001C0.rgba16.inc.c"
};

// 0x0A000E40
ALIGNED8 static const Texture title_texture_0A000E40[] = {
#include "textures/title_screen_bg/title_screen_bg.00E40.rgba16.inc.c"
};

// 0x0A001AC0
ALIGNED8 static const Texture title_texture_0A001AC0[] = {
#include "textures/title_screen_bg/title_screen_bg.01AC0.rgba16.inc.c"
};

// 0x0A002740
ALIGNED8 static const Texture title_texture_0A002740[] = {
#include "textures/title_screen_bg/title_screen_bg.02740.rgba16.inc.c"
};

// 0x0A0065C0
const Texture *const mario_title_texture_table[] = {
    title_texture_0A0001C0, title_texture_0A000E40, title_texture_0A001AC0, title_texture_0A002740,
};

UNUSED static const u64 title_screen_bg_unused_0 = 0;
