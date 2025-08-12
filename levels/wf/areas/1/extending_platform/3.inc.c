const Vtx extending_platform_decal_dl_mesh_vtx_0[4] = {
	{{{-214, 102, 205},0, {86, 1006},{0x0, 0x7F, 0x0, 0xFF}}},
	{{{215, 102, 205},0, {906, 1006},{0x0, 0x7F, 0x0, 0xFF}}},
	{{{215, 102, -204},0, {906, -14},{0x0, 0x7F, 0x0, 0xFF}}},
	{{{-214, 102, -204},0, {86, -14},{0x0, 0x7F, 0x0, 0xFF}}},
};

const Gfx extending_platform_decal_dl_mesh_tri_0[] = {
	gsSPVertex(extending_platform_decal_dl_mesh_vtx_0 + 0, 4, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(0, 2, 3, 0),
	gsSPEndDisplayList(),
};

const Gfx extending_platform_decal_dl_mesh[] = {
	gsSPDisplayList(mat_extending_platform_ArrowMaterial),
	gsSPDisplayList(extending_platform_decal_dl_mesh_tri_0),
	gsDPPipeSync(),
	gsSPClearGeometryMode(G_TEXTURE_GEN),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsSPTexture(65535, 65535, 0, 0, 0),
	gsSPEndDisplayList(),
};
