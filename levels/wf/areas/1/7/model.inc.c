const Vtx mountain_7_dl_mesh_vtx_0[12] = {
	{{{256, 1075, 255},0, {0xBCA, 0xFFF0},{0x7F, 0x0, 0x0, 0xFF}}},
	{{{256, 0, -256},0, {0x13C6, 0xFF2},{0x7F, 0x0, 0x0, 0xFF}}},
	{{{256, 1075, -256},0, {0x13C6, 0xFFF0},{0x7F, 0x0, 0x0, 0xFF}}},
	{{{256, 0, 255},0, {0xBCA, 0xFF2},{0x7F, 0x0, 0x0, 0xFF}}},
	{{{-255, 1075, 255},0, {0xFBD4, 0xFFF0},{0x0, 0x0, 0x7F, 0xFF}}},
	{{{256, 0, 255},0, {0x3CE, 0xFF2},{0x0, 0x0, 0x7F, 0xFF}}},
	{{{256, 1075, 255},0, {0x3CE, 0xFFF0},{0x0, 0x0, 0x7F, 0xFF}}},
	{{{-255, 0, 255},0, {0xFBD4, 0xFF2},{0x0, 0x0, 0x7F, 0xFF}}},
	{{{256, 1075, -256},0, {0xFBD4, 0xFFF0},{0x0, 0x0, 0x81, 0xFF}}},
	{{{-255, 0, -256},0, {0x3CE, 0xFF2},{0x0, 0x0, 0x81, 0xFF}}},
	{{{-255, 1075, -256},0, {0x3CE, 0xFFF0},{0x0, 0x0, 0x81, 0xFF}}},
	{{{256, 0, -256},0, {0xFBD4, 0xFF2},{0x0, 0x0, 0x81, 0xFF}}},
};

const Gfx mountain_7_dl_mesh_tri_0[] = {
	gsSPVertex(mountain_7_dl_mesh_vtx_0 + 0, 12, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(0, 3, 1, 0),
	gsSP1Triangle(4, 5, 6, 0),
	gsSP1Triangle(4, 7, 5, 0),
	gsSP1Triangle(8, 9, 10, 0),
	gsSP1Triangle(8, 11, 9, 0),
	gsSPEndDisplayList(),
};

const Vtx mountain_7_dl_mesh_vtx_1[4] = {
	{{{256, 1075, -256},0, {0x7F0, 0xFC10},{0x0, 0x7F, 0x0, 0xFF}}},
	{{{-255, 1075, 255},0, {0x50, 0x3F0},{0x0, 0x7F, 0x0, 0xFF}}},
	{{{256, 1075, 255},0, {0x7F0, 0x3F0},{0x0, 0x7F, 0x0, 0xFF}}},
	{{{-255, 1075, -256},0, {0x50, 0xFC10},{0x0, 0x7F, 0x0, 0xFF}}},
};

const Gfx mountain_7_dl_mesh_tri_1[] = {
	gsSPVertex(mountain_7_dl_mesh_vtx_1 + 0, 4, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(0, 3, 1, 0),
	gsSPEndDisplayList(),
};

const Gfx mountain_7_dl_mesh[] = {
	gsSPDisplayList(mat_mountain_DarkGravelMaterial),
	gsSPDisplayList(mountain_7_dl_mesh_tri_0),
	gsSPDisplayList(mat_mountain_LightBlueGravelMaterial),
	gsSPDisplayList(mountain_7_dl_mesh_tri_1),
	gsDPPipeSync(),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPClearGeometryMode(G_TEXTURE_GEN),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsSPTexture(65535, 65535, 0, 0, 0),
	gsSPEndDisplayList(),
};
