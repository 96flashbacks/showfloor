
static const Vtx submarine_2_dl_mesh_vtx_0[4] = {
	{{ {4438, 147, -1084}, 0, {2072, -8}, {125, 0, 231, 255} }},
	{{ {4643, 147, -60}, 0, {8, -8}, {125, 0, 231, 255} }},
	{{ {4438, -113, -1084}, 0, {2072, 1011}, {125, 0, 231, 255} }},
	{{ {4643, -113, -60}, 0, {8, 1011}, {125, 0, 231, 255} }},
};

static const Gfx submarine_2_dl_mesh_tri_0[] = {
	gsSPVertex(submarine_2_dl_mesh_vtx_0 + 0, 4, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(1, 3, 2, 0),
	gsSPEndDisplayList(),
};

static const Vtx submarine_2_dl_mesh_vtx_1[8] = {
	{{ {3812, 759, 1959}, 0, {995, 999}, {129, 0, 0, 255} }},
	{{ {3812, 1117, 1603}, 0, {1, -12}, {129, 0, 0, 255} }},
	{{ {3812, 759, 1603}, 0, {1, 999}, {129, 0, 0, 255} }},
	{{ {3812, 1117, 1959}, 0, {995, -12}, {129, 0, 0, 255} }},
	{{ {4026, 759, 1603}, 0, {1, 999}, {127, 0, 0, 255} }},
	{{ {4026, 1117, 1959}, 0, {995, -12}, {127, 0, 0, 255} }},
	{{ {4026, 759, 1959}, 0, {995, 999}, {127, 0, 0, 255} }},
	{{ {4026, 1117, 1603}, 0, {1, -12}, {127, 0, 0, 255} }},
};

static const Gfx submarine_2_dl_mesh_tri_1[] = {
	gsSPVertex(submarine_2_dl_mesh_vtx_1 + 0, 8, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(0, 3, 1, 0),
	gsSP1Triangle(4, 5, 6, 0),
	gsSP1Triangle(4, 7, 5, 0),
	gsSPEndDisplayList(),
};

const Gfx submarine_2_dl_mesh[] = {
	gsSPDisplayList(mat_submarine_SubmarineTextDecalMaterial),
	gsSPDisplayList(submarine_2_dl_mesh_tri_0),
	gsSPDisplayList(mat_revert_submarine_SubmarineTextDecalMaterial),
	gsSPDisplayList(mat_submarine_SubmarineBowserDecalMaterial),
	gsSPDisplayList(submarine_2_dl_mesh_tri_1),
	gsSPDisplayList(mat_revert_submarine_SubmarineBowserDecalMaterial),
	gsDPPipeSync(),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPClearGeometryMode(G_TEXTURE_GEN),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
	gsSPTexture(65535, 65535, 0, 0, 0),
	gsSPEndDisplayList(),
};
