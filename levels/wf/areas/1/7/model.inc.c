static const Vtx mountain_7_dl_mesh_vtx_0[12] = {
	{{ {256, 1075, 255}, 0, {3018, -3}, {127, 0, 0, 255} }},
	{{ {256, 0, -256}, 0, {5062, 4254}, {127, 0, 0, 255} }},
	{{ {256, 1075, -256}, 0, {5062, -3}, {127, 0, 0, 255} }},
	{{ {256, 0, 255}, 0, {3018, 4254}, {127, 0, 0, 255} }},
	{{ {-255, 1075, 255}, 0, {-1068, -3}, {0, 0, 127, 255} }},
	{{ {256, 0, 255}, 0, {974, 4254}, {0, 0, 127, 255} }},
	{{ {256, 1075, 255}, 0, {974, -3}, {0, 0, 127, 255} }},
	{{ {-255, 0, 255}, 0, {-1068, 4254}, {0, 0, 127, 255} }},
	{{ {256, 1075, -256}, 0, {-1068, -3}, {0, 0, 129, 255} }},
	{{ {-255, 0, -256}, 0, {974, 4254}, {0, 0, 129, 255} }},
	{{ {-255, 1075, -256}, 0, {974, -3}, {0, 0, 129, 255} }},
	{{ {256, 0, -256}, 0, {-1068, 4254}, {0, 0, 129, 255} }},
};

static const Gfx mountain_7_dl_mesh_tri_0[] = {
	gsSPVertex(mountain_7_dl_mesh_vtx_0 + 0, 12, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(0, 3, 1, 0),
	gsSP1Triangle(4, 5, 6, 0),
	gsSP1Triangle(4, 7, 5, 0),
	gsSP1Triangle(8, 9, 10, 0),
	gsSP1Triangle(8, 11, 9, 0),
	gsSPEndDisplayList(),
};

static const Vtx mountain_7_dl_mesh_vtx_1[4] = {
	{{ {256, 1075, -256}, 0, {2032, -1008}, {0, 127, 0, 255} }},
	{{ {-255, 1075, 255}, 0, {80, 1008}, {0, 127, 0, 255} }},
	{{ {256, 1075, 255}, 0, {2032, 1008}, {0, 127, 0, 255} }},
	{{ {-255, 1075, -256}, 0, {80, -1008}, {0, 127, 0, 255} }},
};

static const Gfx mountain_7_dl_mesh_tri_1[] = {
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
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
	gsSPTexture(65535, 65535, 0, 0, 0),
	gsSPEndDisplayList(),
};
