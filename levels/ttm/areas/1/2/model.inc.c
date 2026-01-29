Vtx ttm_2_dl_mesh_vtx_0[16] = {
	{{ {1397, 2315, 926}, 0, {-246, 990}, {68, 0, 107, 255} }},
	{{ {2048, 2406, 513}, 0, {7428, 0}, {68, 0, 107, 255} }},
	{{ {1397, 2416, 926}, 0, {-258, 0}, {68, 0, 107, 255} }},
	{{ {1397, 2315, 926}, 0, {-246, 990}, {68, 0, 107, 255} }},
	{{ {2048, 2305, 513}, 0, {7442, 990}, {68, 0, 107, 255} }},
	{{ {2048, 2406, 513}, 0, {7428, 0}, {68, 0, 107, 255} }},
	{{ {257, 2305, 257}, 0, {-262, 990}, {154, 255, 76, 255} }},
	{{ {1024, 2406, 1281}, 0, {12512, 0}, {154, 255, 76, 255} }},
	{{ {239, 2406, 234}, 0, {-560, 0}, {154, 255, 76, 255} }},
	{{ {257, 2305, 257}, 0, {-262, 990}, {154, 0, 76, 255} }},
	{{ {1024, 2305, 1281}, 0, {12512, 990}, {154, 0, 76, 255} }},
	{{ {1024, 2406, 1281}, 0, {12512, 0}, {154, 0, 76, 255} }},
	{{ {1024, 2406, 1281}, 0, {-720, 0}, {88, 0, 92, 255} }},
	{{ {1397, 2315, 926}, 0, {4404, 990}, {88, 0, 92, 255} }},
	{{ {1397, 2416, 926}, 0, {4424, 0}, {88, 0, 92, 255} }},
	{{ {1024, 2305, 1281}, 0, {-740, 990}, {88, 0, 92, 255} }},
};

Gfx ttm_2_dl_mesh_tri_0[] = {
	gsSPVertex(ttm_2_dl_mesh_vtx_0 + 0, 16, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(3, 4, 5, 0),
	gsSP1Triangle(6, 7, 8, 0),
	gsSP1Triangle(9, 10, 11, 0),
	gsSP1Triangle(12, 13, 14, 0),
	gsSP1Triangle(12, 15, 13, 0),
	gsSPEndDisplayList(),
};

Gfx mat_ttm_f3dlite_material_056[] = {
	gsSPClearGeometryMode(G_CULL_BACK),
	gsDPPipeSync(),
	gsDPSetCombineLERP(0, 0, 0, TEXEL0, 0, 0, 0, TEXEL0, 0, 0, 0, TEXEL0, 0, 0, 0, TEXEL0),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 1, mountain_09008000),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 1023, 256),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, 0, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0),
	gsDPSetTileSize(0, 0, 0, 124, 124),
	gsSPEndDisplayList(),
};

Gfx mat_revert_ttm_f3dlite_material_056[] = {
	gsSPSetGeometryMode(G_CULL_BACK),
	gsDPPipeSync(),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsSPEndDisplayList(),
};

Gfx ttm_2_dl_mesh[] = {
	gsSPDisplayList(mat_ttm_f3dlite_material_056),
	gsSPDisplayList(ttm_2_dl_mesh_tri_0),
	gsSPDisplayList(mat_revert_ttm_f3dlite_material_056),
	gsDPPipeSync(),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPClearGeometryMode(G_TEXTURE_GEN),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsSPTexture(65535, 65535, 0, 0, 0),
	gsSPEndDisplayList(),
};

