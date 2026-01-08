Lights1 big_world_f3dlite_material_026_lights = gdSPDefLights1(
	0x7F, 0x7F, 0x3C,
	0xFF, 0xFF, 0x78, 0x49, 0x49, 0x49);

Vtx big_world_4_dl_mesh_vtx_0[27] = {
	{{ {-8191, -3583, -8191}, 0, {1502, 990}, {0, 127, 0, 255} }},
	{{ {-5119, -3583, -4607}, 0, {0, 96}, {0, 127, 0, 255} }},
	{{ {-5119, -3583, -8191}, 0, {0, 990}, {0, 127, 0, 255} }},
	{{ {5632, -6655, -6655}, 0, {76, 806}, {0, 127, 0, 255} }},
	{{ {5120, -6655, -7167}, 0, {0, 990}, {0, 127, 0, 255} }},
	{{ {4608, -6655, -5631}, 0, {506, 990}, {0, 127, 0, 255} }},
	{{ {5632, -6655, -4607}, 0, {722, 620}, {0, 127, 0, 255} }},
	{{ {-8191, -3583, -8191}, 0, {0, 990}, {0, 127, 0, 255} }},
	{{ {-8191, -3583, 8192}, 0, {8144, 990}, {0, 127, 0, 255} }},
	{{ {-5119, -3583, 8192}, 0, {8144, 0}, {0, 127, 0, 255} }},
	{{ {-5119, -3583, -4607}, 0, {1756, 0}, {0, 127, 0, 255} }},
	{{ {-5119, -3583, 8192}, 0, {0, 990}, {0, 127, 0, 255} }},
	{{ {6963, -3583, 8192}, 0, {5998, 990}, {0, 127, 0, 255} }},
	{{ {6963, -3583, 4608}, 0, {5998, 96}, {0, 127, 0, 255} }},
	{{ {-5119, -3583, 4608}, 0, {0, 96}, {0, 127, 0, 255} }},
	{{ {7680, -6655, -6655}, 0, {224, 786}, {252, 216, 136, 255} }},
	{{ {8192, -6655, -7167}, 0, {0, 990}, {0, 127, 0, 255} }},
	{{ {5120, -6655, -7167}, 0, {1502, 990}, {0, 127, 0, 255} }},
	{{ {5632, -6655, -6655}, 0, {1246, 786}, {0, 127, 0, 255} }},
	{{ {-921, -6655, -5426}, 0, {1940, 990}, {0, 127, 0, 255} }},
	{{ {-921, -6655, -4607}, 0, {1952, 844}, {0, 127, 0, 255} }},
	{{ {5632, -6655, -4607}, 0, {-382, 800}, {0, 127, 0, 255} }},
	{{ {4608, -6655, -5631}, 0, {0, 990}, {0, 127, 0, 255} }},
	{{ {7680, -6655, -6655}, 0, {970, 790}, {252, 216, 136, 255} }},
	{{ {7680, -6655, -5631}, 0, {470, 890}, {0, 0, 129, 255} }},
	{{ {7680, -6655, -4607}, 0, {0, 990}, {4, 126, 12, 255} }},
	{{ {8192, -6655, -7167}, 0, {1270, 990}, {0, 127, 0, 255} }},
};

Gfx big_world_4_dl_mesh_tri_0[] = {
	gsSPVertex(big_world_4_dl_mesh_vtx_0 + 0, 15, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(3, 4, 5, 0),
	gsSP1Triangle(3, 5, 6, 0),
	gsSP1Triangle(7, 8, 9, 0),
	gsSP1Triangle(7, 9, 10, 0),
	gsSP1Triangle(11, 12, 13, 0),
	gsSP1Triangle(11, 13, 14, 0),
	gsSPVertex(big_world_4_dl_mesh_vtx_0 + 15, 12, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(0, 2, 3, 0),
	gsSP1Triangle(4, 5, 6, 0),
	gsSP1Triangle(4, 6, 7, 0),
	gsSP1Triangle(8, 9, 10, 0),
	gsSP1Triangle(8, 10, 11, 0),
	gsSPEndDisplayList(),
};

Gfx mat_big_world_f3dlite_material_026[] = {
	gsSPSetLights1(big_world_f3dlite_material_026_lights),
	gsDPPipeSync(),
	gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetTextureImage(G_IM_FMT_IA, G_IM_SIZ_16b_LOAD_BLOCK, 1, grass_0900B800),
	gsDPSetTile(G_IM_FMT_IA, G_IM_SIZ_16b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 1023, 256),
	gsDPSetTile(G_IM_FMT_IA, G_IM_SIZ_16b, 8, 0, 0, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0),
	gsDPSetTileSize(0, 0, 0, 124, 124),
	gsSPEndDisplayList(),
};

Gfx mat_revert_big_world_f3dlite_material_026[] = {
	gsDPPipeSync(),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsSPEndDisplayList(),
};

// 0x070072E8 - 0x07007348
Gfx big_world_4_dl_mesh[] = {
	gsSPDisplayList(mat_big_world_f3dlite_material_026),
	gsSPDisplayList(big_world_4_dl_mesh_tri_0),
	gsSPDisplayList(mat_revert_big_world_f3dlite_material_026),
	gsDPPipeSync(),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPClearGeometryMode(G_TEXTURE_GEN),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsSPTexture(65535, 65535, 0, 0, 0),
	gsSPEndDisplayList(),
};

