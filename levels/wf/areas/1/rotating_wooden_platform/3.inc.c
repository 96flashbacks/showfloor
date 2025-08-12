const Vtx rotating_wooden_platform_decal_dl_mesh_vtx_0[4] = {
	{{{-167, 0, 184},0, {0xFFF0, 0x7F0},{0x0, 0x7F, 0x0, 0xFF}}},
	{{{167, 0, 184},0, {0x7F0, 0x7F0},{0x0, 0x7F, 0x0, 0xFF}}},
	{{{167, 0, -184},0, {0x7F0, 0xFFF0},{0x0, 0x7F, 0x0, 0xFF}}},
	{{{-167, 0, -184},0, {0xFFF0, 0xFFF0},{0x0, 0x7F, 0x0, 0xFF}}},
};

const Gfx rotating_wooden_platform_decal_dl_mesh_tri_0[] = {
	gsSPVertex(rotating_wooden_platform_decal_dl_mesh_vtx_0 + 0, 4, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(0, 2, 3, 0),
	gsSPEndDisplayList(),
};

const Gfx rotating_wooden_platform_decal_dl_mesh[] = {
	gsSPDisplayList(mat_rotating_wooden_platform_DecalMaterial),
	gsSPDisplayList(rotating_wooden_platform_decal_dl_mesh_tri_0),
	gsDPPipeSync(),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPClearGeometryMode(G_TEXTURE_GEN),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsSPTexture(65535, 65535, 0, 0, 0),
	gsSPEndDisplayList(),
};
