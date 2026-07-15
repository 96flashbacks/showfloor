// 0x0700D780 - 0x0700D798
static const Lights1 wf_seg7_lights_0700D780 = gdSPDefLights1(
    0x3f, 0x3f, 0x3f,
    0xff, 0xff, 0xff, 0x28, 0x28, 0x28
);

Vtx bomp_lod_dl_mesh_vtx_0[22] = {
	{{ {-252, 210, -128}, 0, {2522, -34}, {0, 0, 129, 255} }},
	{{ {259, 5, -128}, 0, {0, 990}, {0, 0, 129, 255} }},
	{{ {-252, 5, -128}, 0, {2520, 990}, {0, 0, 129, 255} }},
	{{ {-252, 210, -128}, 0, {2522, -34}, {7, 19, 131, 255} }},
	{{ {259, 210, -96}, 0, {468, -34}, {7, 19, 131, 255} }},
	{{ {259, 5, -128}, 0, {0, 990}, {7, 19, 131, 255} }},
	{{ {-252, 261, -77}, 0, {4056, 412}, {5, 90, 166, 255} }},
	{{ {259, 210, -96}, 0, {478, 990}, {5, 90, 166, 255} }},
	{{ {-252, 210, -128}, 0, {4056, 990}, {5, 90, 166, 255} }},
	{{ {-252, 210, 127}, 0, {-3096, 990}, {5, 91, 89, 255} }},
	{{ {259, 210, 95}, 0, {480, 990}, {5, 91, 89, 255} }},
	{{ {-252, 261, 75}, 0, {-3096, 412}, {5, 91, 89, 255} }},
	{{ {-252, 5, 127}, 0, {0, 990}, {0, 0, 127, 255} }},
	{{ {259, 5, 127}, 0, {2012, 990}, {0, 0, 127, 255} }},
	{{ {-252, 210, 127}, 0, {0, 182}, {0, 0, 127, 255} }},
	{{ {259, 5, 127}, 0, {2012, 990}, {7, 19, 125, 255} }},
	{{ {259, 210, 95}, 0, {2012, 162}, {7, 19, 125, 255} }},
	{{ {-252, 210, 127}, 0, {0, 182}, {7, 19, 125, 255} }},
	{{ {-252, 261, 75}, 0, {0, -3138}, {12, 126, 0, 255} }},
	{{ {259, 210, -96}, 0, {1194, 0}, {12, 126, 0, 255} }},
	{{ {-252, 261, -77}, 0, {1194, -3138}, {12, 126, 0, 255} }},
	{{ {259, 210, 95}, 0, {0, 0}, {12, 126, 0, 255} }},
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
	{{ {259, 5, -128}, 0, {990, 830}, {127, 0, 0, 255} }},
	{{ {259, 210, 95}, 0, {96, 13}, {127, 0, 0, 255} }},
	{{ {259, 5, 127}, 0, {0, 830}, {127, 0, 0, 255} }},
	{{ {259, 210, -96}, 0, {862, 13}, {127, 0, 0, 255} }},
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
