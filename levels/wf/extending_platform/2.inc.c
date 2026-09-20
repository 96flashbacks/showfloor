const Vtx extending_platform_decal_dl_mesh_vtx_0[4] = {
	{{ {215, 102, -204}, 0, {990, 0}, {0, 127, 0, 255} }},
	{{ {-214, 102, 205}, 0, {0, 990}, {0, 127, 0, 255} }},
	{{ {215, 102, 205}, 0, {990, 990}, {0, 127, 0, 255} }},
	{{ {-214, 102, -204}, 0, {0, 0}, {0, 127, 0, 255} }},
};

const Gfx extending_platform_decal_dl_mesh_tri_0[] = {
	gsSPVertex(extending_platform_decal_dl_mesh_vtx_0 + 0, 4, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(0, 3, 1, 0),
	gsSPEndDisplayList(),
};

const Vtx extending_platform_decal_dl_mesh_vtx_1[16] = {
	{{ {-214, 102, -204}, 0, {0, 0}, {129, 0, 0, 255} }},
	{{ {-214, 0, -204}, 0, {0, 478}, {129, 0, 0, 255} }},
	{{ {-214, 0, 205}, 0, {2012, 478}, {129, 0, 0, 255} }},
	{{ {-214, 102, 205}, 0, {2012, 0}, {129, 0, 0, 255} }},
	{{ {-214, 102, 205}, 0, {0, 0}, {0, 0, 127, 255} }},
	{{ {215, 0, 205}, 0, {2012, 478}, {0, 0, 127, 255} }},
	{{ {215, 102, 205}, 0, {2012, 0}, {0, 0, 127, 255} }},
	{{ {-214, 0, 205}, 0, {0, 478}, {0, 0, 127, 255} }},
	{{ {215, 102, 205}, 0, {0, 0}, {127, 0, 0, 255} }},
	{{ {215, 0, -204}, 0, {2012, 478}, {127, 0, 0, 255} }},
	{{ {215, 102, -204}, 0, {2012, 0}, {127, 0, 0, 255} }},
	{{ {215, 0, 205}, 0, {0, 478}, {127, 0, 0, 255} }},
	{{ {215, 0, 205}, 0, {990, 990}, {0, 129, 0, 255} }},
	{{ {-214, 0, 205}, 0, {0, 990}, {0, 129, 0, 255} }},
	{{ {-214, 0, -204}, 0, {0, 0}, {0, 129, 0, 255} }},
	{{ {215, 0, -204}, 0, {990, 0}, {0, 129, 0, 255} }},
};

const Gfx extending_platform_decal_dl_mesh_tri_1[] = {
	gsSPVertex(extending_platform_decal_dl_mesh_vtx_1 + 0, 16, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(0, 2, 3, 0),
	gsSP1Triangle(4, 5, 6, 0),
	gsSP1Triangle(4, 7, 5, 0),
	gsSP1Triangle(8, 9, 10, 0),
	gsSP1Triangle(8, 11, 9, 0),
	gsSP1Triangle(12, 13, 14, 0),
	gsSP1Triangle(12, 14, 15, 0),
	gsSPEndDisplayList(),
};

const Gfx extending_platform_decal_dl_mesh[] = {
	gsSPDisplayList(mat_extending_platform_ArrowMaterial),
	gsSPDisplayList(extending_platform_decal_dl_mesh_tri_0),
	gsSPDisplayList(mat_mountain_TowerBrickMaterial),
	gsSPDisplayList(extending_platform_decal_dl_mesh_tri_1),
	gsDPPipeSync(),
	gsSPClearGeometryMode(G_TEXTURE_GEN),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
	gsSPTexture(65535, 65535, 0, 0, 0),
	gsSPEndDisplayList(),
};
