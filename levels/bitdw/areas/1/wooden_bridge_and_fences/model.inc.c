Lights1 bitdw_f3dlite_material_008_lights = gdSPDefLights1(
	0x3F, 0x3F, 0x3F,
	0xFF, 0xFF, 0xFF, 0x49, 0x49, 0x49);

Lights1 bitdw_f3dlite_material_009_lights = gdSPDefLights1(
	0x3F, 0x3F, 0x3F,
	0xFF, 0xFF, 0xFF, 0x49, 0x49, 0x49);

Lights1 bitdw_f3dlite_material_010_lights = gdSPDefLights1(
	0x3F, 0x3F, 0x3F,
	0xFF, 0xFF, 0xFF, 0x49, 0x49, 0x49);

Vtx bitdw_seg7_dl_07003608_mesh_vtx_0[18] = {
	{{ {-52, 412, -1230}, 0, {990, 0}, {144, 247, 198, 255} }},
	{{ {-257, 309, -820}, 0, {-3578, 990}, {144, 247, 198, 255} }},
	{{ {-265, 411, -820}, 0, {-3614, -28}, {144, 247, 198, 255} }},
	{{ {-52, 412, -1230}, 0, {990, 0}, {142, 0, 199, 255} }},
	{{ {-52, 309, -1230}, 0, {990, 990}, {142, 0, 199, 255} }},
	{{ {-257, 309, -820}, 0, {-3578, 990}, {142, 0, 199, 255} }},
	{{ {-52, 412, -1230}, 0, {912, -30}, {237, 0, 130, 255} }},
	{{ {1020, 494, -1395}, 0, {-10012, -24}, {237, 0, 130, 255} }},
	{{ {1020, 392, -1395}, 0, {-9878, 990}, {237, 0, 130, 255} }},
	{{ {-52, 309, -1230}, 0, {990, 990}, {237, 0, 130, 255} }},
	{{ {-265, 411, -820}, 0, {786, 0}, {0, 0, 129, 255} }},
	{{ {-163, 419, -820}, 0, {-234, 0}, {0, 0, 129, 255} }},
	{{ {-155, 317, -820}, 0, {-234, 990}, {0, 0, 129, 255} }},
	{{ {-257, 309, -820}, 0, {786, 990}, {0, 0, 129, 255} }},
	{{ {145, 443, -820}, 0, {800, -30}, {0, 0, 129, 255} }},
	{{ {1073, 514, -820}, 0, {-8488, -28}, {0, 0, 129, 255} }},
	{{ {1073, 412, -820}, 0, {-8410, 990}, {0, 0, 129, 255} }},
	{{ {145, 340, -820}, 0, {878, 990}, {0, 0, 129, 255} }},
};

Gfx bitdw_seg7_dl_07003608_mesh_tri_0[] = {
	gsSPVertex(bitdw_seg7_dl_07003608_mesh_vtx_0 + 0, 14, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(3, 4, 5, 0),
	gsSP1Triangle(6, 7, 8, 0),
	gsSP1Triangle(6, 8, 9, 0),
	gsSP1Triangle(10, 11, 12, 0),
	gsSP1Triangle(10, 12, 13, 0),
	gsSPVertex(bitdw_seg7_dl_07003608_mesh_vtx_0 + 14, 4, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(0, 2, 3, 0),
	gsSPEndDisplayList(),
};

Vtx bitdw_seg7_dl_07003608_mesh_vtx_1[12] = {
	{{ {-153, 205, -818}, 0, {990, 990}, {129, 0, 0, 255} }},
	{{ {-153, -442, 1090}, 0, {-9072, 990}, {129, 0, 0, 255} }},
	{{ {-153, -215, 1058}, 0, {-8900, -86}, {129, 0, 0, 255} }},
	{{ {-155, 420, -820}, 0, {1001, 92}, {129, 255, 0, 255} }},
	{{ {-153, 205, -818}, 0, {990, 990}, {129, 255, 0, 255} }},
	{{ {-153, -215, 1058}, 0, {-8900, -86}, {129, 255, 0, 255} }},
	{{ {154, -215, 1058}, 0, {-8900, -86}, {127, 0, 0, 255} }},
	{{ {154, -442, 1090}, 0, {-9070, 990}, {127, 0, 0, 255} }},
	{{ {153, 442, -820}, 0, {1000, -121}, {127, 0, 0, 255} }},
	{{ {154, -442, 1090}, 0, {-9070, 990}, {127, 1, 0, 255} }},
	{{ {154, 205, -818}, 0, {990, 990}, {127, 1, 0, 255} }},
	{{ {153, 442, -820}, 0, {1001, -112}, {127, 1, 0, 255} }},
};

Gfx bitdw_seg7_dl_07003608_mesh_tri_1[] = {
	gsSPVertex(bitdw_seg7_dl_07003608_mesh_vtx_1 + 0, 12, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(3, 4, 5, 0),
	gsSP1Triangle(6, 7, 8, 0),
	gsSP1Triangle(9, 10, 11, 0),
	gsSPEndDisplayList(),
};

Vtx bitdw_seg7_dl_07003608_mesh_vtx_2[6] = {
	{{ {154, -306, 1024}, 0, {0, 0}, {247, 120, 42, 255} }},
	{{ {145, 340, -820}, 0, {6473, 34}, {247, 120, 42, 255} }},
	{{ {-155, 317, -820}, 0, {6447, 1001}, {247, 120, 42, 255} }},
	{{ {-153, -306, 1024}, 0, {0, 988}, {0, 120, 41, 255} }},
	{{ {154, -306, 1024}, 0, {0, 0}, {0, 120, 41, 255} }},
	{{ {-155, 317, -820}, 0, {6447, 1001}, {0, 120, 41, 255} }},
};

Gfx bitdw_seg7_dl_07003608_mesh_tri_2[] = {
	gsSPVertex(bitdw_seg7_dl_07003608_mesh_vtx_2 + 0, 6, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(3, 4, 5, 0),
	gsSPEndDisplayList(),
};

Gfx mat_bitdw_f3dlite_material_008[] = {
	gsSPClearGeometryMode(G_CULL_BACK),
	gsSPSetLights1(bitdw_f3dlite_material_008_lights),
	gsDPPipeSync(),
	gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, ENVIRONMENT, 0),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 1, sky_09005000),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 1023, 256),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, 0, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0),
	gsDPSetTileSize(0, 0, 0, 124, 124),
	gsSPEndDisplayList(),
};

Gfx mat_revert_bitdw_f3dlite_material_008[] = {
	gsSPSetGeometryMode(G_CULL_BACK),
	gsDPPipeSync(),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsSPEndDisplayList(),
};

Gfx mat_bitdw_f3dlite_material_009[] = {
	gsSPClearGeometryMode(G_CULL_BACK),
	gsSPSetLights1(bitdw_f3dlite_material_009_lights),
	gsDPPipeSync(),
	gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, ENVIRONMENT, 0),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 1, sky_09006000),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 1023, 256),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, 0, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0),
	gsDPSetTileSize(0, 0, 0, 124, 124),
	gsSPEndDisplayList(),
};

Gfx mat_revert_bitdw_f3dlite_material_009[] = {
	gsSPSetGeometryMode(G_CULL_BACK),
	gsDPPipeSync(),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsSPEndDisplayList(),
};

Gfx mat_bitdw_f3dlite_material_010[] = {
	gsSPClearGeometryMode(G_CULL_BACK),
	gsSPSetLights1(bitdw_f3dlite_material_010_lights),
	gsDPPipeSync(),
	gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, ENVIRONMENT, 0),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 1, sky_09000000),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 1023, 256),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, 0, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0),
	gsDPSetTileSize(0, 0, 0, 124, 124),
	gsSPEndDisplayList(),
};

Gfx mat_revert_bitdw_f3dlite_material_010[] = {
	gsSPSetGeometryMode(G_CULL_BACK),
	gsDPPipeSync(),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsSPEndDisplayList(),
};

Gfx bitdw_seg7_dl_07003608[] = {
	gsSPDisplayList(mat_bitdw_f3dlite_material_008),
	gsSPDisplayList(bitdw_seg7_dl_07003608_mesh_tri_0),
	gsSPDisplayList(mat_revert_bitdw_f3dlite_material_008),
	gsSPDisplayList(mat_bitdw_f3dlite_material_009),
	gsSPDisplayList(bitdw_seg7_dl_07003608_mesh_tri_1),
	gsSPDisplayList(mat_revert_bitdw_f3dlite_material_009),
	gsSPDisplayList(mat_bitdw_f3dlite_material_010),
	gsSPDisplayList(bitdw_seg7_dl_07003608_mesh_tri_2),
	gsSPDisplayList(mat_revert_bitdw_f3dlite_material_010),
	gsDPPipeSync(),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPClearGeometryMode(G_TEXTURE_GEN),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsSPTexture(65535, 65535, 0, 0, 0),
	gsSPEndDisplayList(),
};

