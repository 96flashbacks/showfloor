Vtx mountain_9_dl_mesh_vtx_0[20] = {
	{{{255, 64, -256},0, {2380, 286},{0x76, 0xD0, 0x0, 0xFF}}},
	{{{255, 64, 256},0, {-174, 286},{0x76, 0xD0, 0x0, 0xFF}}},
	{{{204, -63, 230},0, {-16, 974},{0x76, 0xD0, 0x0, 0xFF}}},
	{{{204, -63, -230},0, {2252, 974},{0x76, 0xD0, 0x0, 0xFF}}},
	{{{-256, 64, 256},0, {-1324, 322},{0x0, 0xE6, 0x7C, 0xFF}}},
	{{{-205, -63, 230},0, {-1068, 974},{0x0, 0xE6, 0x7C, 0xFF}}},
	{{{204, -63, 230},0, {974, 974},{0x0, 0xE6, 0x7C, 0xFF}}},
	{{{255, 64, 256},0, {1230, 322},{0x0, 0xE6, 0x7C, 0xFF}}},
	{{{-205, -63, 230},0, {1104, 976},{0x8A, 0xD0, 0x0, 0xFF}}},
	{{{-256, 64, 256},0, {1229, 304},{0x8A, 0xD0, 0x0, 0xFF}}},
	{{{-256, 64, -256},0, {-1328, 304},{0x8A, 0xD0, 0x0, 0xFF}}},
	{{{-205, -63, -230},0, {-1200, 976},{0x8A, 0xD0, 0x0, 0xFF}}},
	{{{204, -63, 230},0, {-1068, 974},{0x0, 0x81, 0x0, 0xFF}}},
	{{{-205, -63, 230},0, {974, 974},{0x0, 0x81, 0x0, 0xFF}}},
	{{{-205, -63, -230},0, {974, -1326},{0x0, 0x81, 0x0, 0xFF}}},
	{{{204, -63, -230},0, {-1068, -1326},{0x0, 0x81, 0x0, 0xFF}}},
	{{{255, 64, -256},0, {-1328, 304},{0x0, 0xE6, 0x84, 0xFF}}},
	{{{204, -63, -230},0, {-1200, 976},{0x0, 0xE6, 0x84, 0xFF}}},
	{{{-205, -63, -230},0, {1104, 976},{0x0, 0xE6, 0x84, 0xFF}}},
	{{{-256, 64, -256},0, {1229, 304},{0x0, 0xE6, 0x84, 0xFF}}},
};

Gfx mountain_9_dl_mesh_tri_0[] = {
	gsSPVertex(mountain_9_dl_mesh_vtx_0 + 0, 16, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(0, 2, 3, 0),
	gsSP1Triangle(4, 5, 6, 0),
	gsSP1Triangle(4, 6, 7, 0),
	gsSP1Triangle(8, 9, 10, 0),
	gsSP1Triangle(8, 10, 11, 0),
	gsSP1Triangle(12, 13, 14, 0),
	gsSP1Triangle(12, 14, 15, 0),
	gsSPVertex(mountain_9_dl_mesh_vtx_0 + 16, 4, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(0, 2, 3, 0),
	gsSPEndDisplayList(),
};

Vtx mountain_9_dl_mesh_vtx_1[4] = {
	{{{255, 64, 256},0, {-820, 1772},{0x0, 0x7F, 0x0, 0xFF}}},
	{{{255, 64, -256},0, {1684, 1772},{0x0, 0x7F, 0x0, 0xFF}}},
	{{{-256, 64, -256},0, {1684, -732},{0x0, 0x7F, 0x0, 0xFF}}},
	{{{-256, 64, 256},0, {-820, -732},{0x0, 0x7F, 0x0, 0xFF}}},
};

Gfx mountain_9_dl_mesh_tri_1[] = {
	gsSPVertex(mountain_9_dl_mesh_vtx_1 + 0, 4, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(0, 2, 3, 0),
	gsSPEndDisplayList(),
};

const Gfx mountain_9_dl_mesh[] = {
	gsSPDisplayList(mat_mountain_DarkDirtMaterial),
	gsSPDisplayList(mountain_9_dl_mesh_tri_0),
	gsSPDisplayList(mat_mountain_PolkaDotMaterial),
	gsSPDisplayList(mountain_9_dl_mesh_tri_1),
	gsDPPipeSync(),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPClearGeometryMode(G_TEXTURE_GEN),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsSPTexture(65535, 65535, 0, 0, 0),
	gsSPEndDisplayList(),
};
