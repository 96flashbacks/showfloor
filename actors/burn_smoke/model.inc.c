// Burn Smoke

// 0x040217C0
static const Vtx burn_smoke_seg4_vertex_040217C0[] = {
    {{{   -50,    -50,      0}, 0, {     0,    992}, {0x14, 0x0a, 0x0a, 0xff}}},
    {{{    50,    -50,      0}, 0, {   992,    992}, {0x14, 0x0a, 0x0a, 0xff}}},
    {{{    50,     50,      0}, 0, {   992,      0}, {0x14, 0x0a, 0x0a, 0xff}}},
    {{{   -50,     50,      0}, 0, {     0,      0}, {0x14, 0x0a, 0x0a, 0xff}}},
};

ALIGNED8 static const u8 burn_smoke_texture_1[] = {
#include "actors/burn_smoke/burn_smoke1.ia16.inc.c"
};
ALIGNED8 static const u8 burn_smoke_texture_2[] = {
#include "actors/burn_smoke/burn_smoke2.ia16.inc.c"
};
ALIGNED8 static const u8 burn_smoke_texture_3[] = {
#include "actors/burn_smoke/burn_smoke3.ia16.inc.c"
};
ALIGNED8 static const u8 burn_smoke_texture_4[] = {
#include "actors/burn_smoke/burn_smoke4.ia16.inc.c"
};
ALIGNED8 static const u8 burn_smoke_texture_5[] = {
#include "actors/burn_smoke/burn_smoke.ia16.inc.c"
};
ALIGNED8 static const u8 burn_smoke_texture_6[] = {
#include "actors/burn_smoke/burn_smoke6.ia16.inc.c"
};
ALIGNED8 static const u8 burn_smoke_texture_7[] = {
#include "actors/burn_smoke/burn_smoke7.ia16.inc.c"
};
ALIGNED8 static const u8 burn_smoke_texture_8[] = {
#include "actors/burn_smoke/burn_smoke8.ia16.inc.c"
};

// 0x04022000 - 0x04022028
const Gfx burn_smoke_seg4_dl_04022000[] = {
    gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_MODULATERGBA, G_CC_MODULATERGBA),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPEndDisplayList(),
};

// 0x04022028 - 0x04022048
const Gfx burn_smoke_seg4_dl_04022028[] = {
    gsSPVertex(burn_smoke_seg4_vertex_040217C0, 4, 0),
    gsSP2Triangles( 0,  1,  2, 0x0,  0,  2,  3, 0x0),
    gsSPEndDisplayList(),
};

// 0x04022048 - 0x04022070
const Gfx burn_smoke_seg4_dl_04022048[] = {
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsSPSetGeometryMode(G_LIGHTING),
    gsSPEndDisplayList(),
};

const Gfx gfx_burn_smoke_v1[] = {
    gsSPDisplayList(burn_smoke_seg4_dl_04022000),
    gsDPLoadTextureBlock(burn_smoke_texture_1, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_CLAMP, G_TX_CLAMP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsSPDisplayList(burn_smoke_seg4_dl_04022028),
    gsSPDisplayList(burn_smoke_seg4_dl_04022048),
    gsSPEndDisplayList(),
};
const Gfx gfx_burn_smoke_v2[] = {
    gsSPDisplayList(burn_smoke_seg4_dl_04022000),
    gsDPLoadTextureBlock(burn_smoke_texture_2, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_CLAMP, G_TX_CLAMP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsSPDisplayList(burn_smoke_seg4_dl_04022028),
    gsSPDisplayList(burn_smoke_seg4_dl_04022048),
    gsSPEndDisplayList(),
};
const Gfx gfx_burn_smoke_v3[] = {
    gsSPDisplayList(burn_smoke_seg4_dl_04022000),
    gsDPLoadTextureBlock(burn_smoke_texture_3, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_CLAMP, G_TX_CLAMP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsSPDisplayList(burn_smoke_seg4_dl_04022028),
    gsSPDisplayList(burn_smoke_seg4_dl_04022048),
    gsSPEndDisplayList(),
};
const Gfx gfx_burn_smoke_v4[] = {
    gsSPDisplayList(burn_smoke_seg4_dl_04022000),
    gsDPLoadTextureBlock(burn_smoke_texture_4, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_CLAMP, G_TX_CLAMP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsSPDisplayList(burn_smoke_seg4_dl_04022028),
    gsSPDisplayList(burn_smoke_seg4_dl_04022048),
    gsSPEndDisplayList(),
};
const Gfx gfx_burn_smoke_v5[] = {
    gsSPDisplayList(burn_smoke_seg4_dl_04022000),
    gsDPLoadTextureBlock(burn_smoke_texture_5, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_CLAMP, G_TX_CLAMP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsSPDisplayList(burn_smoke_seg4_dl_04022028),
    gsSPDisplayList(burn_smoke_seg4_dl_04022048),
    gsSPEndDisplayList(),
};
const Gfx gfx_burn_smoke_v6[] = {
    gsSPDisplayList(burn_smoke_seg4_dl_04022000),
    gsDPLoadTextureBlock(burn_smoke_texture_6, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_CLAMP, G_TX_CLAMP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsSPDisplayList(burn_smoke_seg4_dl_04022028),
    gsSPDisplayList(burn_smoke_seg4_dl_04022048),
    gsSPEndDisplayList(),
};
const Gfx gfx_burn_smoke_v7[] = {
    gsSPDisplayList(burn_smoke_seg4_dl_04022000),
    gsDPLoadTextureBlock(burn_smoke_texture_7, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_CLAMP, G_TX_CLAMP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsSPDisplayList(burn_smoke_seg4_dl_04022028),
    gsSPDisplayList(burn_smoke_seg4_dl_04022048),
    gsSPEndDisplayList(),
};
const Gfx gfx_burn_smoke_v8[] = {
    gsSPDisplayList(burn_smoke_seg4_dl_04022000),
    gsDPLoadTextureBlock(burn_smoke_texture_8, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_CLAMP, G_TX_CLAMP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsSPDisplayList(burn_smoke_seg4_dl_04022028),
    gsSPDisplayList(burn_smoke_seg4_dl_04022048),
    gsSPEndDisplayList(),
};
