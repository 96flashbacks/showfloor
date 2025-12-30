// Power Meter HUD

#ifdef POWBUILD
ALIGNED8 static const u8 texture_power_meter_eight_left[] = {
#include "actors/power_meter/pow_meter_left_side_full.rgba16.inc.c"
};

ALIGNED8 static const u8 texture_power_meter_eight_right[] = {
#include "actors/power_meter/pow_meter_right_side_full.rgba16.inc.c"
};

ALIGNED8 static const u8 texture_power_meter_seven_left[] = {
#include "actors/power_meter/pow_meter_left_side_7.rgba16.inc.c"
};

ALIGNED8 static const u8 texture_power_meter_seven_right[] = {
#include "actors/power_meter/pow_meter_right_side_7.rgba16.inc.c"
};

ALIGNED8 static const u8 texture_power_meter_six_left[] = {
#include "actors/power_meter/pow_meter_left_side_6.rgba16.inc.c"
};

ALIGNED8 static const u8 texture_power_meter_six_right[] = {
#include "actors/power_meter/pow_meter_right_side_6.rgba16.inc.c"
};

ALIGNED8 static const u8 texture_power_meter_five_left[] = {
#include "actors/power_meter/pow_meter_left_side_5.rgba16.inc.c"
};

ALIGNED8 static const u8 texture_power_meter_five_right[] = {
#include "actors/power_meter/pow_meter_right_side_5.rgba16.inc.c"
};

ALIGNED8 static const u8 texture_power_meter_four_left[] = {
#include "actors/power_meter/pow_meter_left_side_4.rgba16.inc.c"
};

ALIGNED8 static const u8 texture_power_meter_four_right[] = {
#include "actors/power_meter/pow_meter_right_side_4.rgba16.inc.c"
};

ALIGNED8 static const u8 texture_power_meter_three_left[] = {
#include "actors/power_meter/pow_meter_left_side_3.rgba16.inc.c"
};

ALIGNED8 static const u8 texture_power_meter_three_right[] = {
#include "actors/power_meter/pow_meter_right_side_3.rgba16.inc.c"
};

ALIGNED8 static const u8 texture_power_meter_two_left[] = {
#include "actors/power_meter/pow_meter_left_side_2.rgba16.inc.c"
};

ALIGNED8 static const u8 texture_power_meter_two_right[] = {
#include "actors/power_meter/pow_meter_right_side_2.rgba16.inc.c"
};

ALIGNED8 static const u8 texture_power_meter_one_left[] = {
#include "actors/power_meter/pow_meter_left_side_1.rgba16.inc.c"
};

ALIGNED8 static const u8 texture_power_meter_one_right[] = {
#include "actors/power_meter/pow_meter_right_side_1.rgba16.inc.c"
};

ALIGNED8 static const u8 texture_power_meter_zero_left[] = {
#include "actors/power_meter/pow_meter_left_side_empty.rgba16.inc.c"
};

ALIGNED8 static const u8 texture_power_meter_zero_right[] = {
#include "actors/power_meter/pow_meter_right_side_empty.rgba16.inc.c"
};

#else

ALIGNED8 static const u8 texture_power_meter_eight_left[] = {
#include "actors/power_meter/power_meter_left_side_full.rgba16.inc.c"
};

ALIGNED8 static const u8 texture_power_meter_eight_right[] = {
#include "actors/power_meter/power_meter_right_side_full.rgba16.inc.c"
};

ALIGNED8 static const u8 texture_power_meter_seven_left[] = {
#include "actors/power_meter/power_meter_left_side_7.rgba16.inc.c"
};

ALIGNED8 static const u8 texture_power_meter_seven_right[] = {
#include "actors/power_meter/power_meter_right_side_7.rgba16.inc.c"
};

ALIGNED8 static const u8 texture_power_meter_six_left[] = {
#include "actors/power_meter/power_meter_left_side_6.rgba16.inc.c"
};

ALIGNED8 static const u8 texture_power_meter_six_right[] = {
#include "actors/power_meter/power_meter_right_side_6.rgba16.inc.c"
};

ALIGNED8 static const u8 texture_power_meter_five_left[] = {
#include "actors/power_meter/power_meter_left_side_5.rgba16.inc.c"
};

ALIGNED8 static const u8 texture_power_meter_five_right[] = {
#include "actors/power_meter/power_meter_right_side_5.rgba16.inc.c"
};

ALIGNED8 static const u8 texture_power_meter_four_left[] = {
#include "actors/power_meter/power_meter_left_side_4.rgba16.inc.c"
};

ALIGNED8 static const u8 texture_power_meter_four_right[] = {
#include "actors/power_meter/power_meter_right_side_4.rgba16.inc.c"
};

ALIGNED8 static const u8 texture_power_meter_three_left[] = {
#include "actors/power_meter/power_meter_left_side_3.rgba16.inc.c"
};

ALIGNED8 static const u8 texture_power_meter_three_right[] = {
#include "actors/power_meter/power_meter_right_side_3.rgba16.inc.c"
};

ALIGNED8 static const u8 texture_power_meter_two_left[] = {
#include "actors/power_meter/power_meter_left_side_2.rgba16.inc.c"
};

ALIGNED8 static const u8 texture_power_meter_two_right[] = {
#include "actors/power_meter/power_meter_right_side_2.rgba16.inc.c"
};

ALIGNED8 static const u8 texture_power_meter_one_left[] = {
#include "actors/power_meter/power_meter_left_side_1.rgba16.inc.c"
};

ALIGNED8 static const u8 texture_power_meter_one_right[] = {
#include "actors/power_meter/power_meter_right_side_1.rgba16.inc.c"
};

ALIGNED8 static const u8 texture_power_meter_zero_left[] = {
#include "actors/power_meter/power_meter_left_side_empty.rgba16.inc.c"
};

ALIGNED8 static const u8 texture_power_meter_zero_right[] = {
#include "actors/power_meter/power_meter_right_side_empty.rgba16.inc.c"
};
#endif
const u8 *const power_meter_health_segments_lut[] = {
    texture_power_meter_zero_left,
    texture_power_meter_zero_right,
    texture_power_meter_one_left,
    texture_power_meter_one_right,
    texture_power_meter_two_left,
    texture_power_meter_two_right,
    texture_power_meter_three_left,
    texture_power_meter_three_right,
    texture_power_meter_four_left,
    texture_power_meter_four_right,
    texture_power_meter_five_left,
    texture_power_meter_five_right,
    texture_power_meter_six_left,
    texture_power_meter_six_right,
    texture_power_meter_seven_left,
    texture_power_meter_seven_right,
    texture_power_meter_eight_left,
    texture_power_meter_eight_right,
};

static const Vtx vertex_power_meter_base[] = {
    {{{   -32,    -32,      0}, 0, {     0,   2016}, {0xff, 0xff, 0xff, 0xdc}}},
    {{{     0,    -32,      0}, 0, {   992,   2016}, {0xff, 0xff, 0xff, 0xdc}}},
    {{{     0,     32,      0}, 0, {   992,      0}, {0xff, 0xff, 0xff, 0xdc}}},
    {{{   -32,     32,      0}, 0, {     0,      0}, {0xff, 0xff, 0xff, 0xdc}}},
    {{{     0,    -32,      0}, 0, {     1,   2016}, {0xff, 0xff, 0xff, 0xdc}}},
    {{{    32,    -32,      0}, 0, {  1024,   2016}, {0xff, 0xff, 0xff, 0xdc}}},
    {{{    32,     32,      0}, 0, {  1024,      0}, {0xff, 0xff, 0xff, 0xdc}}},
    {{{     0,     32,      0}, 0, {     1,      0}, {0xff, 0xff, 0xff, 0xdc}}},
};

const Gfx dl_power_meter_base[] = {
    gsDPPipeSync(),
    gsSPClearGeometryMode(G_LIGHTING),
    gsDPSetCombineMode(G_CC_MODULATERGBA, G_CC_MODULATERGBA),
    gsDPSetRenderMode(G_RM_XLU_SURF, G_RM_XLU_SURF2),
    gsDPSetTextureFilter(G_TF_POINT),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPVertex(vertex_power_meter_base, 8, 0),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD),
    gsDPTileSync(),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 6, G_TX_NOLOD, G_TX_CLAMP, 5, G_TX_NOLOD),
    gsDPSetTileSize(0, 0, 0, (32 - 1) << G_TEXTURE_IMAGE_FRAC, (64 - 1) << G_TEXTURE_IMAGE_FRAC),
    gsSPEndDisplayList(),
};
