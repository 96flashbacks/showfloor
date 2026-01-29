Vtx bttdw_3_dl_mesh_vtx_0[15] = {
	{{ {-52, 412, -1230}, 0, {912, -30}, {175, 206, 22, 255} }},
	{{ {1020, 494, -1395}, 0, {-10012, -24}, {175, 206, 22, 255} }},
	{{ {1020, 392, -1395}, 0, {-9878, 990}, {175, 206, 22, 255} }},
	{{ {-52, 309, -1230}, 0, {990, 990}, {175, 206, 22, 255} }},
	{{ {-52, 412, -1230}, 0, {990, 0}, {175, 206, 22, 255} }},
	{{ {-257, 309, -820}, 0, {-3578, 990}, {175, 206, 22, 255} }},
	{{ {-265, 411, -820}, 0, {-3614, -28}, {175, 206, 22, 255} }},
	{{ {-265, 411, -820}, 0, {786, 0}, {175, 206, 22, 255} }},
	{{ {-163, 419, -820}, 0, {-234, 0}, {175, 206, 22, 255} }},
	{{ {-155, 317, -820}, 0, {-234, 990}, {175, 206, 22, 255} }},
	{{ {-257, 309, -820}, 0, {786, 990}, {175, 206, 22, 255} }},
	{{ {145, 443, -820}, 0, {800, -30}, {175, 206, 22, 255} }},
	{{ {1073, 514, -820}, 0, {-8488, -28}, {175, 206, 22, 255} }},
	{{ {1073, 412, -820}, 0, {-8410, 990}, {175, 206, 22, 255} }},
	{{ {145, 340, -820}, 0, {878, 990}, {175, 206, 22, 255} }},
};

Gfx bttdw_3_dl_mesh_tri_0[] = {
	gsSPVertex(bttdw_3_dl_mesh_vtx_0 + 0, 15, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(0, 2, 3, 0),
	gsSP1Triangle(4, 5, 6, 0),
	gsSP1Triangle(4, 3, 5, 0),
	gsSP1Triangle(7, 8, 9, 0),
	gsSP1Triangle(7, 9, 10, 0),
	gsSP1Triangle(11, 12, 13, 0),
	gsSP1Triangle(11, 13, 14, 0),
	gsSPEndDisplayList(),
};

Vtx bttdw_3_dl_mesh_vtx_1[9] = {
	{{ {154, -215, 1058}, 0, {-8900, -86}, {223, 191, 31, 255} }},
	{{ {154, -442, 1090}, 0, {-9070, 990}, {223, 191, 31, 255} }},
	{{ {153, 442, -820}, 0, {1000, -121}, {223, 191, 31, 255} }},
	{{ {154, 205, -818}, 0, {990, 990}, {223, 191, 31, 255} }},
	{{ {153, 442, -820}, 0, {1001, -112}, {223, 191, 31, 255} }},
	{{ {-153, 205, -818}, 0, {990, 990}, {223, 191, 31, 255} }},
	{{ {-153, -442, 1090}, 0, {-9072, 990}, {223, 191, 31, 255} }},
	{{ {-153, -215, 1058}, 0, {-8900, -86}, {223, 191, 31, 255} }},
	{{ {-155, 420, -820}, 0, {1001, 92}, {223, 191, 31, 255} }},
};

Gfx bttdw_3_dl_mesh_tri_1[] = {
	gsSPVertex(bttdw_3_dl_mesh_vtx_1 + 0, 9, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(1, 3, 4, 0),
	gsSP1Triangle(5, 6, 7, 0),
	gsSP1Triangle(8, 5, 7, 0),
	gsSPEndDisplayList(),
};

Vtx bttdw_3_dl_mesh_vtx_2[5] = {
	{{ {154, -306, 1024}, 0, {0, 0}, {255, 212, 0, 255} }},
	{{ {145, 340, -820}, 0, {6473, 34}, {255, 212, 0, 255} }},
	{{ {-155, 317, -820}, 0, {6447, 1001}, {255, 212, 0, 255} }},
	{{ {-153, -306, 1024}, 0, {0, 988}, {255, 212, 0, 255} }},
	{{ {-155, 317, -820}, 0, {6447, 1001}, {255, 212, 0, 255} }},
};

Gfx bttdw_3_dl_mesh_tri_2[] = {
	gsSPVertex(bttdw_3_dl_mesh_vtx_2 + 0, 5, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(3, 0, 4, 0),
	gsSPEndDisplayList(),
};

Gfx bttdw_3_dl_mesh[] = {
	gsSPDisplayList(mat_bttdw_Fence),
	gsSPDisplayList(bttdw_3_dl_mesh_tri_0),
	gsSPDisplayList(mat_revert_bttdw_Fence),
	gsSPDisplayList(mat_bttdw_Bridge_Fence),
	gsSPDisplayList(bttdw_3_dl_mesh_tri_1),
	gsSPDisplayList(mat_revert_bttdw_Bridge_Fence),
	gsSPDisplayList(mat_bttdw_Bridge),
	gsSPDisplayList(bttdw_3_dl_mesh_tri_2),
	gsSPDisplayList(mat_revert_bttdw_Bridge),
	gsDPPipeSync(),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPClearGeometryMode(G_TEXTURE_GEN),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsSPTexture(65535, 65535, 0, 0, 0),
	gsSPEndDisplayList(),
};
