static const Vtx slider_3_dl_mesh_vtx_0[4] = {
	{{ {5819, 5240, 5820}, 0, {-16, 751}, {90, 0, 90, 255} }},
	{{ {6109, 5342, 5531}, 0, {2044, 204}, {90, 0, 90, 255} }},
	{{ {5819, 5342, 5820}, 0, {-16, 204}, {90, 0, 90, 255} }},
	{{ {6109, 5240, 5531}, 0, {2044, 751}, {90, 0, 90, 255} }},
};

static const Gfx slider_3_dl_mesh_tri_0[] = {
	gsSPVertex(slider_3_dl_mesh_vtx_0 + 0, 4, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(0, 3, 1, 0),
	gsSPEndDisplayList(),
};

const Gfx slider_3_dl_mesh[] = {
	gsSPDisplayList(mat_slider_StartMaterial),
	gsSPDisplayList(slider_3_dl_mesh_tri_0),
	gsSPDisplayList(mat_revert_slider_TextMaterial),
	gsDPPipeSync(),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPClearGeometryMode(G_TEXTURE_GEN),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
	gsSPTexture(65535, 65535, 0, 0, 0),
	gsSPEndDisplayList(),
};

