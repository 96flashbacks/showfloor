Vtx big_world_5_dl_mesh_vtx_0[4] = {
	{{ {4690, -511, 2131}, 0, {0, 0}, {0, 0, 0, 180} }},
	{{ {4690, -511, 2377}, 0, {0, 990}, {0, 0, 0, 180} }},
	{{ {4936, -511, 2377}, 0, {990, 990}, {0, 0, 0, 180} }},
	{{ {4936, -511, 2131}, 0, {990, 0}, {0, 0, 0, 180} }},
};

Gfx big_world_5_dl_mesh_tri_0[] = {
	gsSPVertex(big_world_5_dl_mesh_vtx_0 + 0, 4, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(0, 2, 3, 0),
	gsSPEndDisplayList(),
};

Gfx mat_big_world_f3dlite_material_027[] = {
	gsSPClearGeometryMode(G_LIGHTING),
	gsDPPipeSync(),
	gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetTextureImage(G_IM_FMT_IA, G_IM_SIZ_8b_LOAD_BLOCK, 1, grass_0900B000),
	gsDPSetTile(G_IM_FMT_IA, G_IM_SIZ_8b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 511, 512),
	gsDPSetTile(G_IM_FMT_IA, G_IM_SIZ_8b, 4, 0, 0, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, 0),
	gsDPSetTileSize(0, 0, 0, 124, 124),
	gsSPEndDisplayList(),
};

Gfx mat_revert_big_world_f3dlite_material_027[] = {
	gsSPSetGeometryMode(G_LIGHTING),
	gsDPPipeSync(),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsSPEndDisplayList(),
};

Gfx big_world_5_dl_mesh[] = {
	gsSPDisplayList(mat_big_world_f3dlite_material_027),
	gsSPDisplayList(big_world_5_dl_mesh_tri_0),
	gsSPDisplayList(mat_revert_big_world_f3dlite_material_027),
	gsDPPipeSync(),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPClearGeometryMode(G_TEXTURE_GEN),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsSPTexture(65535, 65535, 0, 0, 0),
	gsSPEndDisplayList(),
};
