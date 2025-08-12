// 0x0700D780 - 0x0700D798
static const Lights1 wf_seg7_lights_0700D780 = gdSPDefLights1(
    0x3f, 0x3f, 0x3f,
    0xff, 0xff, 0xff, 0x28, 0x28, 0x28
);

Vtx bomp_lod_dl_mesh_vtx_0[22] = {
	{{{-252, 210, -128},0, {2506, -50},{0x0, 0x0, 0x81, 0xFF}}},
	{{{259, 5, -128},0, {-16, 974},{0x0, 0x0, 0x81, 0xFF}}},
	{{{-252, 5, -128},0, {2504, 974},{0x0, 0x0, 0x81, 0xFF}}},
	{{{-252, 210, -128},0, {2506, -50},{0x8, 0x14, 0x83, 0xFF}}},
	{{{259, 210, -96},0, {452, -50},{0x8, 0x14, 0x83, 0xFF}}},
	{{{259, 5, -128},0, {-16, 974},{0x8, 0x14, 0x83, 0xFF}}},
	{{{-252, 261, -77},0, {4040, 396},{0x6, 0x5A, 0xA6, 0xFF}}},
	{{{259, 210, -96},0, {462, 974},{0x6, 0x5A, 0xA6, 0xFF}}},
	{{{-252, 210, -128},0, {4040, 974},{0x6, 0x5A, 0xA6, 0xFF}}},
	{{{-252, 210, 127},0, {-3112, 974},{0x6, 0x5A, 0x59, 0xFF}}},
	{{{259, 210, 95},0, {464, 974},{0x6, 0x5A, 0x59, 0xFF}}},
	{{{-252, 261, 75},0, {-3112, 396},{0x6, 0x5A, 0x59, 0xFF}}},
	{{{-252, 5, 127},0, {-16, 974},{0x0, 0x0, 0x7F, 0xFF}}},
	{{{259, 5, 127},0, {1996, 974},{0x0, 0x0, 0x7F, 0xFF}}},
	{{{-252, 210, 127},0, {-16, 166},{0x0, 0x0, 0x7F, 0xFF}}},
	{{{259, 5, 127},0, {1996, 974},{0x8, 0x14, 0x7D, 0xFF}}},
	{{{259, 210, 95},0, {1996, 146},{0x8, 0x14, 0x7D, 0xFF}}},
	{{{-252, 210, 127},0, {-16, 166},{0x8, 0x14, 0x7D, 0xFF}}},
	{{{-252, 261, 75},0, {-16, -3154},{0xD, 0x7E, 0x0, 0xFF}}},
	{{{259, 210, -96},0, {1178, -16},{0xD, 0x7E, 0x0, 0xFF}}},
	{{{-252, 261, -77},0, {1178, -3154},{0xD, 0x7E, 0x0, 0xFF}}},
	{{{259, 210, 95},0, {-16, -16},{0xD, 0x7E, 0x0, 0xFF}}},
};

Gfx bomp_lod_dl_mesh_tri_0[] = {
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, generic_09003000),
    gsDPLoadSync(),
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, 32 * 32 - 1, CALC_DXT(32, G_IM_SIZ_16b_BYTES)),
    gsSPLight(&wf_seg7_lights_0700D370.l, 1),
    gsSPLight(&wf_seg7_lights_0700D370.a, 2),
	gsSPVertex(bomp_lod_dl_mesh_vtx_0 + 0, 15, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(3, 4, 5, 0),
	gsSP1Triangle(6, 7, 8, 0),
	gsSP1Triangle(9, 10, 11, 0),
	gsSP1Triangle(12, 13, 14, 0),
	gsSPVertex(bomp_lod_dl_mesh_vtx_0 + 15, 7, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(3, 4, 5, 0),
	gsSP1Triangle(3, 6, 4, 0),
	gsSPEndDisplayList(),
};

Vtx bomp_lod_dl_mesh_vtx_1[4] = {
	{{{259, 5, -128},0, {991, 812},{0x7F, 0x0, 0x0, 0xFF}}},
	{{{259, 210, 95},0, {102, 15},{0x7F, 0x0, 0x0, 0xFF}}},
	{{{259, 5, 127},0, {-21, 812},{0x7F, 0x0, 0x0, 0xFF}}},
	{{{259, 210, -96},0, {867, 15},{0x7F, 0x0, 0x0, 0xFF}}},
};

Gfx bomp_lod_dl_mesh_tri_1[] = {
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, wf_seg7_texture_07001800),
    gsDPLoadSync(),
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, 32 * 32 - 1, CALC_DXT(32, G_IM_SIZ_16b_BYTES)),
	gsSPVertex(bomp_lod_dl_mesh_vtx_1 + 0, 4, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(0, 3, 1, 0),
	gsSPEndDisplayList(),
};

// 0x0700D9F0 - 0x0700DA68
const Gfx wf_seg7_dl_0700D9F0[] = {
    gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_MODULATERGB, G_CC_MODULATERGB),
    gsSPClearGeometryMode(G_SHADING_SMOOTH),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPTileSync(),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, G_TX_RENDERTILE, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_WRAP | G_TX_NOMIRROR, 5, G_TX_NOLOD),
    gsDPSetTileSize(0, 0, 0, (32 - 1) << G_TEXTURE_IMAGE_FRAC, (32 - 1) << G_TEXTURE_IMAGE_FRAC),
    gsSPDisplayList(bomp_lod_dl_mesh_tri_0),
    gsSPDisplayList(bomp_lod_dl_mesh_tri_1),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsSPSetGeometryMode(G_SHADING_SMOOTH),
    gsSPEndDisplayList(),
};
