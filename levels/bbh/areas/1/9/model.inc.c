Vtx ghost_house_painting_dl_mesh_vtx_0[8] = {
	{{ {1519, 304, 878}, 0, {4, 1002}, {0, 0, 129, 255} }},
	{{ {1320, 304, 878}, 0, {2052, 1002}, {0, 0, 129, 255} }},
	{{ {1320, 403, 878}, 0, {2052, -21}, {0, 0, 129, 255} }},
	{{ {1519, 403, 878}, 0, {4, -21}, {0, 0, 129, 255} }},
	{{ {1933, 403, 878}, 0, {2052, -21}, {0, 0, 129, 255} }},
	{{ {2132, 403, 878}, 0, {4, -21}, {0, 0, 129, 255} }},
	{{ {2132, 304, 878}, 0, {4, 1002}, {0, 0, 129, 255} }},
	{{ {1933, 304, 878}, 0, {2052, 1002}, {0, 0, 129, 255} }},
};

Gfx ghost_house_painting_dl_mesh_tri_0[] = {
	gsSPVertex(ghost_house_painting_dl_mesh_vtx_0 + 0, 8, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(0, 2, 3, 0),
	gsSP1Triangle(4, 5, 6, 0),
	gsSP1Triangle(4, 6, 7, 0),
	gsSPEndDisplayList(),
};

Vtx ghost_house_painting_dl_mesh_vtx_1[8] = {
	{{ {1519, 205, 878}, 0, {4, 1002}, {0, 0, 129, 255} }},
	{{ {1320, 205, 878}, 0, {2052, 1002}, {0, 0, 129, 255} }},
	{{ {1320, 304, 878}, 0, {2052, -21}, {0, 0, 129, 255} }},
	{{ {1519, 304, 878}, 0, {4, -21}, {0, 0, 129, 255} }},
	{{ {1933, 304, 878}, 0, {2052, -21}, {0, 0, 129, 255} }},
	{{ {2132, 304, 878}, 0, {4, -21}, {0, 0, 129, 255} }},
	{{ {2132, 205, 878}, 0, {4, 1002}, {0, 0, 129, 255} }},
	{{ {1933, 205, 878}, 0, {2052, 1002}, {0, 0, 129, 255} }},
};

Gfx ghost_house_painting_dl_mesh_tri_1[] = {
	gsSPVertex(ghost_house_painting_dl_mesh_vtx_1 + 0, 8, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(0, 2, 3, 0),
	gsSP1Triangle(4, 5, 6, 0),
	gsSP1Triangle(4, 6, 7, 0),
	gsSPEndDisplayList(),
};

Gfx ghost_house_painting_dl_mesh[] = {
	gsSPDisplayList(mat_ghost_house_Crash1),
	gsSPDisplayList(ghost_house_painting_dl_mesh_tri_0),
	gsSPDisplayList(mat_revert_ghost_house_Crash1),
	gsSPDisplayList(mat_ghost_house_Crash2),
	gsSPDisplayList(ghost_house_painting_dl_mesh_tri_1),
	gsSPDisplayList(mat_revert_ghost_house_Crash2),
	gsDPPipeSync(),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPClearGeometryMode(G_TEXTURE_GEN),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsSPTexture(65535, 65535, 0, 0, 0),
	gsSPEndDisplayList(),
};
