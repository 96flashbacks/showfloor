static const Vtx slider_8_dl_mesh_vtx_0[4] = {
	{{ {1128, 641, -4401}, 0, {-4, 996}, {229, 124, 0, 255} }},
	{{ {1128, 641, -5220}, 0, {2023, 1008}, {229, 124, 0, 255} }},
	{{ {907, 593, -5220}, 0, {2023, -10}, {229, 124, 0, 255} }},
	{{ {907, 593, -4401}, 0, {-4, -10}, {229, 124, 0, 255} }},
};

static const Gfx slider_8_dl_mesh_tri_0[] = {
	gsSPVertex(slider_8_dl_mesh_vtx_0 + 0, 4, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(2, 3, 0, 0),
	gsSPEndDisplayList(),
};

const Gfx slider_8_dl_mesh[] = {
	gsSPDisplayList(mat_slider_JumpMaterial),
	gsSPDisplayList(slider_8_dl_mesh_tri_0),
	gsSPDisplayList(mat_revert_slider_TextMaterial),
	gsDPPipeSync(),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPClearGeometryMode(G_TEXTURE_GEN),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsSPTexture(65535, 65535, 0, 0, 0),
	gsSPEndDisplayList(),
};

