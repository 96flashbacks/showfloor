Lights1 bobo_summit_warp_layer1_lights = gdSPDefLights1(
	0x14, 0x14, 0x14,
	0x64, 0x64, 0x64, 0x28, 0x28, 0x28);

Vtx bobo_3_dl_mesh_vtx_0[12] = {
	{{ {-921, 0, 5888}, 0, {-966, 2252}, {0, 127, 0, 255} }},
	{{ {768, 0, 5376}, 0, {718, 1742}, {0, 127, 0, 255} }},
	{{ {-921, 0, 5377}, 0, {-966, 1744}, {0, 127, 0, 255} }},
	{{ {768, 0, 5888}, 0, {718, 2252}, {0, 127, 0, 255} }},
	{{ {-1791, 0, 1792}, 0, {-1834, -1834}, {0, 127, 0, 255} }},
	{{ {-2815, 0, 256}, 0, {-2856, -3368}, {0, 127, 0, 255} }},
	{{ {-3071, 0, 512}, 0, {-3112, -3112}, {0, 127, 0, 255} }},
	{{ {-1535, 0, 1536}, 0, {-1580, -2090}, {0, 127, 0, 255} }},
	{{ {-1764, 0, -3213}, 0, {-1808, -6830}, {0, 127, 0, 255} }},
	{{ {-2679, 0, -5045}, 0, {-2720, -8658}, {0, 127, 0, 255} }},
	{{ {-3251, 0, -4759}, 0, {-3292, -8374}, {0, 127, 0, 255} }},
	{{ {-2336, 0, -2926}, 0, {-2378, -6544}, {0, 127, 0, 255} }},
};

Gfx bobo_3_dl_mesh_tri_0[] = {
	gsSPVertex(bobo_3_dl_mesh_vtx_0 + 0, 12, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(0, 3, 1, 0),
	gsSP1Triangle(4, 5, 6, 0),
	gsSP1Triangle(4, 7, 5, 0),
	gsSP1Triangle(8, 9, 10, 0),
	gsSP1Triangle(8, 10, 11, 0),
	gsSPEndDisplayList(),
};

Gfx mat_bobo_summit_warp_layer1[] = {
	gsSPSetGeometryMode(G_FOG),
	gsSPSetLights1(bobo_summit_warp_layer1_lights),
	gsDPPipeSync(),
	gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, ENVIRONMENT, 0, 0, 0, COMBINED, 0, 0, 0, COMBINED),
	gsDPSetFogColor(160, 160, 160, 255),
	gsSPFogPosition(980, 1000),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsDPSetCycleType(G_CYC_2CYCLE),
	gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 1, generic_09009000),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 1023, 256),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0),
	gsDPSetTileSize(0, 0, 0, 124, 124),
	gsSPEndDisplayList(),
};

Gfx mat_revert_bobo_summit_warp_layer1[] = {
	gsSPClearGeometryMode(G_FOG),
	gsDPPipeSync(),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsDPSetCycleType(G_CYC_1CYCLE),
	gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
	gsSPEndDisplayList(),
};

Gfx bobo_3_dl_mesh[] = {
	gsSPDisplayList(mat_bobo_summit_warp_layer1),
	gsSPDisplayList(bobo_3_dl_mesh_tri_0),
	gsSPDisplayList(mat_revert_bobo_summit_warp_layer1),
	gsDPPipeSync(),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPClearGeometryMode(G_TEXTURE_GEN),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsSPTexture(65535, 65535, 0, 0, 0),
	gsSPEndDisplayList(),
};

