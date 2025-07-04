// 0x0700DE98 - 0x0700DEB0
static const Lights1 wf_seg7_lights_0700DE98 = gdSPDefLights1(
    0x3f, 0x3f, 0x3f,
    0xff, 0xff, 0xff, 0x28, 0x28, 0x28
);

// 0x0700DEB0 - 0x0700DFA0
static const Vtx wf_seg7_vertex_0700DEB0[] = {
    {{{  -127,    -63,     64}, 0, {   990,   -542}, {0x00, 0x81, 0x00, 0xff}}},
    {{{   127,    -63,    -63}, 0, {     0,      0}, {0x00, 0x81, 0x00, 0xff}}},
    {{{   127,    -63,     64}, 0, {     0,   -542}, {0x00, 0x81, 0x00, 0xff}}},
    {{{   127,    -63,     64}, 0, {  5078,    990}, {0x7f, 0x00, 0x00, 0xff}}},
    {{{   127,    -63,    -63}, 0, {  6100,    990}, {0x7f, 0x00, 0x00, 0xff}}},
    {{{   127,     64,    -63}, 0, {  6100,    480}, {0x7f, 0x00, 0x00, 0xff}}},
    {{{   127,     64,     64}, 0, {  5078,    480}, {0x7f, 0x00, 0x00, 0xff}}},
    {{{   127,     64,    -63}, 0, {   880,    990}, {0x00, 0x00, 0x81, 0xff}}},
    {{{   127,    -63,    -63}, 0, {   880,      0}, {0x00, 0x00, 0x81, 0xff}}},
    {{{  -127,    -63,    -63}, 0, {     0,      0}, {0x00, 0x00, 0x81, 0xff}}},
    {{{  -127,     64,    -63}, 0, {     0,    990}, {0x00, 0x00, 0x81, 0xff}}},
    {{{  -127,     64,    -63}, 0, {   990,      0}, {0x00, 0x7f, 0x00, 0xff}}},
    {{{  -127,     64,     64}, 0, {   990,   -542}, {0x00, 0x7f, 0x00, 0xff}}},
    {{{   127,     64,     64}, 0, {     0,   -544}, {0x00, 0x7f, 0x00, 0xff}}},
    {{{   127,     64,    -63}, 0, {     0,      0}, {0x00, 0x7f, 0x00, 0xff}}},
};

// 0x0700DFA0 - 0x0700E050
static const Vtx wf_seg7_vertex_0700DFA0[] = {
    {{{  -127,     64,     64}, 0, {     0,    990}, {0x00, 0x00, 0x7f, 0xff}}},
    {{{   127,    -63,     64}, 0, {   880,      0}, {0x00, 0x00, 0x7f, 0xff}}},
    {{{   127,     64,     64}, 0, {   880,    990}, {0x00, 0x00, 0x7f, 0xff}}},
    {{{  -127,    -63,     64}, 0, {   990,   -542}, {0x00, 0x81, 0x00, 0xff}}},
    {{{  -127,    -63,    -63}, 0, {   990,      0}, {0x00, 0x81, 0x00, 0xff}}},
    {{{   127,    -63,    -63}, 0, {     0,      0}, {0x00, 0x81, 0x00, 0xff}}},
    {{{  -127,    -63,    -63}, 0, {  3034,    990}, {0x81, 0x00, 0x00, 0xff}}},
    {{{  -127,    -63,     64}, 0, {  4056,    990}, {0x81, 0x00, 0x00, 0xff}}},
    {{{  -127,     64,     64}, 0, {  4056,    480}, {0x81, 0x00, 0x00, 0xff}}},
    {{{  -127,     64,    -63}, 0, {  3034,    480}, {0x81, 0x00, 0x00, 0xff}}},
    {{{  -127,    -63,     64}, 0, {     0,      0}, {0x00, 0x00, 0x7f, 0xff}}},
};

// 0x0700E050 - 0x0700E0F0
static const Gfx wf_seg7_dl_0700E050[] = {
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, generic_09001800),
    gsDPLoadSync(),
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, 32 * 32 - 1, CALC_DXT(32, G_IM_SIZ_16b_BYTES)),
    gsSPLight(&wf_seg7_lights_0700DE98.l, 1),
    gsSPLight(&wf_seg7_lights_0700DE98.a, 2),
    gsSPVertex(wf_seg7_vertex_0700DEB0, 15, 0),
    gsSP2Triangles( 0,  1,  2, 0x0,  3,  4,  5, 0x0),
    gsSP2Triangles( 3,  5,  6, 0x0,  7,  8,  9, 0x0),
    gsSP2Triangles( 7,  9, 10, 0x0, 11, 12, 13, 0x0),
    gsSP1Triangle(11, 13, 14, 0x0),
    gsSPVertex(wf_seg7_vertex_0700DFA0, 11, 0),
    gsSP2Triangles( 0,  1,  2, 0x0,  3,  4,  5, 0x0),
    gsSP2Triangles( 6,  7,  8, 0x0,  6,  8,  9, 0x0),
    gsSP1Triangle( 0, 10,  1, 0x0),
    gsSPEndDisplayList(),
};

// 0x0700E0F0 - 0x0700E160
const Gfx wf_seg7_dl_0700E0F0[] = {
    gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_MODULATERGB, G_CC_MODULATERGB),
    gsSPClearGeometryMode(G_SHADING_SMOOTH),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPTileSync(),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, G_TX_RENDERTILE, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_WRAP | G_TX_NOMIRROR, 5, G_TX_NOLOD),
    gsDPSetTileSize(0, 0, 0, (32 - 1) << G_TEXTURE_IMAGE_FRAC, (32 - 1) << G_TEXTURE_IMAGE_FRAC),
    gsSPDisplayList(wf_seg7_dl_0700E050),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsSPSetGeometryMode(G_SHADING_SMOOTH),
    gsSPEndDisplayList(),
};
