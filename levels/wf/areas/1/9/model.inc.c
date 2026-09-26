static const Vtx mountain_9_dl_mesh_vtx_0[20] = {
	{{ {255, 64, -256}, 0, {2380, 286}, {69, 154, 30, 255} }},
	{{ {255, 64, 256}, 0, {-174, 286}, {72, 187, 79, 255} }},
	{{ {204, -63, 230}, 0, {-16, 974}, {57, 164, 67, 255} }},
	{{ {204, -63, -230}, 0, {2252, 974}, {60, 146, 19, 255} }},
	{{ {-256, 64, 256}, 0, {-1324, 322}, {150, 196, 219, 255} }},
	{{ {-205, -63, 230}, 0, {-1068, 974}, {161, 171, 7, 255} }},
	{{ {204, -63, 230}, 0, {974, 974}, {57, 164, 67, 255} }},
	{{ {255, 64, 256}, 0, {1230, 322}, {72, 187, 79, 255} }},
	{{ {-205, -63, 230}, 0, {1104, 976}, {161, 171, 7, 255} }},
	{{ {-256, 64, 256}, 0, {1229, 304}, {150, 196, 219, 255} }},
	{{ {-256, 64, -256}, 0, {-1328, 304}, {247, 142, 56, 255} }},
	{{ {-205, -63, -230}, 0, {-1200, 976}, {81, 177, 58, 255} }},
	{{ {204, -63, 230}, 0, {-1068, 974}, {57, 164, 67, 255} }},
	{{ {-205, -63, 230}, 0, {974, 974}, {161, 171, 7, 255} }},
	{{ {-205, -63, -230}, 0, {974, -1326}, {81, 177, 58, 255} }},
	{{ {204, -63, -230}, 0, {-1068, -1326}, {60, 146, 19, 255} }},
	{{ {255, 64, -256}, 0, {-1328, 304}, {69, 154, 30, 255} }},
	{{ {204, -63, -230}, 0, {-1200, 976}, {60, 146, 19, 255} }},
	{{ {-205, -63, -230}, 0, {1104, 976}, {81, 177, 58, 255} }},
	{{ {-256, 64, -256}, 0, {1229, 304}, {247, 142, 56, 255} }},
};

static const Gfx mountain_9_dl_mesh_tri_0[] = {
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

static const Vtx mountain_9_dl_mesh_vtx_1[4] = {
	{{{255, 64, 256},0, {-820, 1772},{0x0, 0x7F, 0x0, 0xFF}}},
	{{{255, 64, -256},0, {1684, 1772},{0x0, 0x7F, 0x0, 0xFF}}},
	{{{-256, 64, -256},0, {1684, -732},{0x0, 0x7F, 0x0, 0xFF}}},
	{{{-256, 64, 256},0, {-820, -732},{0x0, 0x7F, 0x0, 0xFF}}},
};

static const Gfx mountain_9_dl_mesh_tri_1[] = {
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
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
	gsSPTexture(65535, 65535, 0, 0, 0),
	gsSPEndDisplayList(),
};
