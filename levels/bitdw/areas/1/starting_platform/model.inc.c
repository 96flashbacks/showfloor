Lights1 bitdw_f3dlite_material_004_lights = gdSPDefLights1(
	0x17, 0x22, 0x37,
	0x79, 0x9F, 0xEB, 0x28, 0x28, 0x28);

Lights1 bitdw_f3dlite_material_005_lights = gdSPDefLights1(
	0x1A, 0x1A, 0x1F,
	0x83, 0x85, 0x96, 0x28, 0x28, 0x28);

Lights1 bitdw_f3dlite_material_006_lights = gdSPDefLights1(
	0x3F, 0x3F, 0x3F,
	0xFF, 0xFF, 0xFF, 0x28, 0x28, 0x28);

Lights1 bitdw_f3dlite_material_007_lights = gdSPDefLights1(
	0x17, 0x22, 0x37,
	0x79, 0x9F, 0xEB, 0x28, 0x28, 0x28);

Vtx bitdw_seg7_dl_070028A0_mesh_vtx_0[16] = {
	{{ {-613, 819, 514}, 0, {1808, 376}, {127, 0, 0, 255} }},
	{{ {-613, 512, -509}, 0, {1808, 990}, {127, 0, 0, 255} }},
	{{ {-613, 819, -509}, 0, {1808, 376}, {127, 0, 0, 255} }},
	{{ {-613, 512, 514}, 0, {1808, 990}, {127, 0, 0, 255} }},
	{{ {-1535, 819, 514}, 0, {1808, 376}, {129, 0, 0, 255} }},
	{{ {-1535, 819, -509}, 0, {1808, 376}, {129, 0, 0, 255} }},
	{{ {-1535, 512, -509}, 0, {1808, 990}, {129, 0, 0, 255} }},
	{{ {-1535, 512, 514}, 0, {1808, 990}, {129, 0, 0, 255} }},
	{{ {-613, 512, -509}, 0, {-882, 990}, {0, 0, 129, 255} }},
	{{ {-1535, 512, -509}, 0, {-882, 990}, {0, 0, 129, 255} }},
	{{ {-1535, 819, -509}, 0, {-882, 376}, {0, 0, 129, 255} }},
	{{ {-613, 819, -509}, 0, {-882, 376}, {0, 0, 129, 255} }},
	{{ {-1535, 512, 514}, 0, {820, 990}, {0, 0, 127, 255} }},
	{{ {-613, 819, 514}, 0, {820, 376}, {0, 0, 127, 255} }},
	{{ {-1535, 819, 514}, 0, {820, 376}, {0, 0, 127, 255} }},
	{{ {-613, 512, 514}, 0, {820, 990}, {0, 0, 127, 255} }},
};

Gfx bitdw_seg7_dl_070028A0_mesh_tri_0[] = {
	gsSPVertex(bitdw_seg7_dl_070028A0_mesh_vtx_0 + 0, 16, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(0, 3, 1, 0),
	gsSP1Triangle(4, 5, 6, 0),
	gsSP1Triangle(4, 6, 7, 0),
	gsSP1Triangle(8, 9, 10, 0),
	gsSP1Triangle(8, 10, 11, 0),
	gsSP1Triangle(12, 13, 14, 0),
	gsSP1Triangle(12, 15, 13, 0),
	gsSPEndDisplayList(),
};

Vtx bitdw_seg7_dl_070028A0_mesh_vtx_1[14] = {
	{{ {1741, 512, -51}, 0, {0, -372}, {0, 2, 129, 255} }},
	{{ {1331, 614, -48}, 0, {990, 990}, {0, 2, 129, 255} }},
	{{ {1741, 614, -49}, 0, {990, -372}, {0, 2, 129, 255} }},
	{{ {1741, 512, -51}, 0, {0, -372}, {255, 1, 129, 255} }},
	{{ {1253, 512, -48}, 0, {0, 990}, {255, 1, 129, 255} }},
	{{ {1331, 614, -48}, 0, {990, 990}, {255, 1, 129, 255} }},
	{{ {1331, 614, 53}, 0, {480, 990}, {0, 127, 0, 255} }},
	{{ {1741, 614, -49}, 0, {-36, 0}, {0, 127, 0, 255} }},
	{{ {1331, 614, -48}, 0, {0, 990}, {0, 127, 0, 255} }},
	{{ {1741, 614, 53}, 0, {480, 0}, {0, 127, 0, 255} }},
	{{ {1253, 512, 53}, 0, {0, 990}, {0, 0, 127, 255} }},
	{{ {1741, 614, 53}, 0, {990, -372}, {0, 0, 127, 255} }},
	{{ {1331, 614, 53}, 0, {990, 990}, {0, 0, 127, 255} }},
	{{ {1741, 512, 53}, 0, {0, -372}, {0, 0, 127, 255} }},
};

Gfx bitdw_seg7_dl_070028A0_mesh_tri_1[] = {
	gsSPVertex(bitdw_seg7_dl_070028A0_mesh_vtx_1 + 0, 14, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(3, 4, 5, 0),
	gsSP1Triangle(6, 7, 8, 0),
	gsSP1Triangle(6, 9, 7, 0),
	gsSP1Triangle(10, 11, 12, 0),
	gsSP1Triangle(10, 13, 11, 0),
	gsSPEndDisplayList(),
};

Vtx bitdw_seg7_dl_070028A0_mesh_vtx_2[57] = {
	{{ {102, 819, -305}, 0, {5590, -1054}, {0, 0, 129, 255} }},
	{{ {-613, -716, -304}, 0, {0, 990}, {0, 0, 129, 255} }},
	{{ {-613, 819, -304}, 0, {3800, -2842}, {0, 0, 129, 255} }},
	{{ {-613, -716, -304}, 0, {0, 990}, {0, 0, 129, 255} }},
	{{ {-1125, 512, -304}, 0, {1756, -3354}, {0, 0, 129, 255} }},
	{{ {-613, 512, -304}, 0, {3034, -2076}, {0, 0, 129, 255} }},
	{{ {717, 614, -304}, 0, {6612, 990}, {0, 0, 129, 255} }},
	{{ {-613, -716, -304}, 0, {0, 990}, {0, 0, 129, 255} }},
	{{ {102, 717, -305}, 0, {5334, -798}, {0, 0, 129, 255} }},
	{{ {-613, -716, -304}, 0, {1500, -542}, {0, 0, 129, 255} }},
	{{ {102, 819, -305}, 0, {2864, 3202}, {0, 0, 129, 255} }},
	{{ {102, 717, -305}, 0, {2694, 3032}, {0, 0, 129, 255} }},
	{{ {717, 614, -304}, 0, {1500, 3884}, {0, 0, 129, 255} }},
	{{ {102, 717, -305}, 0, {2694, 3032}, {0, 0, 129, 255} }},
	{{ {717, 717, -304}, 0, {1672, 4054}, {0, 0, 129, 255} }},
	{{ {-613, 819, 309}, 0, {3800, -2842}, {0, 0, 127, 255} }},
	{{ {-613, -716, 309}, 0, {0, 990}, {0, 0, 127, 255} }},
	{{ {102, 819, 309}, 0, {5590, -1054}, {0, 0, 127, 255} }},
	{{ {-613, 512, 309}, 0, {3034, -2076}, {0, 0, 127, 255} }},
	{{ {-1125, 512, 309}, 0, {1756, -3354}, {0, 0, 127, 255} }},
	{{ {-613, -716, 309}, 0, {480, 1498}, {0, 0, 127, 255} }},
	{{ {102, 717, 309}, 0, {-712, -2076}, {0, 0, 127, 255} }},
	{{ {102, 819, 309}, 0, {-882, -2246}, {0, 0, 127, 255} }},
	{{ {717, 614, -304}, 0, {982, 3024}, {127, 0, 0, 255} }},
	{{ {717, 717, -304}, 0, {1236, 2768}, {127, 0, 0, 255} }},
	{{ {717, 717, 309}, 0, {-294, 1234}, {127, 0, 0, 255} }},
	{{ {717, 614, 309}, 0, {-550, 1490}, {127, 0, 0, 255} }},
	{{ {102, 717, 309}, 0, {-294, 1234}, {127, 0, 0, 255} }},
	{{ {102, 819, -305}, 0, {1496, 2516}, {127, 0, 0, 255} }},
	{{ {102, 819, 309}, 0, {-38, 982}, {127, 0, 0, 255} }},
	{{ {102, 717, -305}, 0, {1240, 2770}, {127, 0, 0, 255} }},
	{{ {-613, -716, -304}, 0, {0, 990}, {0, 0, 129, 255} }},
	{{ {717, 614, -304}, 0, {6612, 990}, {0, 0, 129, 255} }},
	{{ {307, -716, -304}, 0, {2268, 3288}, {0, 0, 129, 255} }},
	{{ {1331, 614, -304}, 0, {8144, 2522}, {0, 0, 129, 255} }},
	{{ {102, 717, 309}, 0, {5334, -798}, {0, 0, 127, 255} }},
	{{ {717, 614, 309}, 0, {6612, 990}, {0, 0, 127, 255} }},
	{{ {717, 717, 309}, 0, {6866, 734}, {0, 0, 127, 255} }},
	{{ {-613, -716, 309}, 0, {0, 990}, {0, 0, 127, 255} }},
	{{ {307, -716, 309}, 0, {2268, 3288}, {0, 0, 127, 255} }},
	{{ {1331, 614, 309}, 0, {8144, 2522}, {0, 0, 127, 255} }},
	{{ {307, -716, -304}, 0, {-2338, 6344}, {101, 179, 0, 255} }},
	{{ {1331, 614, -304}, 0, {982, 3024}, {101, 179, 0, 255} }},
	{{ {1331, 614, 309}, 0, {-550, 1490}, {101, 179, 0, 255} }},
	{{ {307, -716, 309}, 0, {-3872, 4812}, {101, 179, 0, 255} }},
	{{ {717, 717, -304}, 0, {4822, -798}, {0, 127, 0, 255} }},
	{{ {102, 717, 309}, 0, {1756, -798}, {0, 127, 0, 255} }},
	{{ {717, 717, 309}, 0, {3290, -2332}, {0, 127, 0, 255} }},
	{{ {102, 717, -304}, 0, {3290, 734}, {0, 127, 0, 255} }},
	{{ {1331, 614, -304}, 0, {6356, -2332}, {0, 127, 0, 255} }},
	{{ {717, 614, -304}, 0, {4822, -798}, {0, 127, 0, 255} }},
	{{ {717, 614, 309}, 0, {3290, -2332}, {0, 127, 0, 255} }},
	{{ {1331, 614, 309}, 0, {4822, -3864}, {0, 127, 0, 255} }},
	{{ {102, 819, -304}, 0, {3290, 734}, {0, 127, 0, 255} }},
	{{ {-613, 819, 309}, 0, {0, 990}, {0, 127, 0, 255} }},
	{{ {102, 819, 309}, 0, {1756, -798}, {0, 127, 0, 255} }},
	{{ {-613, 819, -304}, 0, {1502, 2522}, {0, 127, 0, 255} }},
};

Gfx bitdw_seg7_dl_070028A0_mesh_tri_2[] = {
	gsSPVertex(bitdw_seg7_dl_070028A0_mesh_vtx_2 + 0, 15, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(3, 4, 5, 0),
	gsSP1Triangle(6, 7, 8, 0),
	gsSP1Triangle(9, 10, 11, 0),
	gsSP1Triangle(12, 13, 14, 0),
	gsSPVertex(bitdw_seg7_dl_070028A0_mesh_vtx_2 + 15, 16, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(3, 4, 1, 0),
	gsSP1Triangle(5, 6, 7, 0),
	gsSP1Triangle(8, 9, 10, 0),
	gsSP1Triangle(8, 10, 11, 0),
	gsSP1Triangle(12, 13, 14, 0),
	gsSP1Triangle(12, 15, 13, 0),
	gsSPVertex(bitdw_seg7_dl_070028A0_mesh_vtx_2 + 31, 14, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(1, 3, 2, 0),
	gsSP1Triangle(4, 5, 6, 0),
	gsSP1Triangle(7, 5, 4, 0),
	gsSP1Triangle(5, 7, 8, 0),
	gsSP1Triangle(9, 5, 8, 0),
	gsSP1Triangle(10, 11, 12, 0),
	gsSP1Triangle(10, 12, 13, 0),
	gsSPVertex(bitdw_seg7_dl_070028A0_mesh_vtx_2 + 45, 12, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(0, 3, 1, 0),
	gsSP1Triangle(4, 5, 6, 0),
	gsSP1Triangle(4, 6, 7, 0),
	gsSP1Triangle(8, 9, 10, 0),
	gsSP1Triangle(8, 11, 9, 0),
	gsSPEndDisplayList(),
};

Vtx bitdw_seg7_dl_070028A0_mesh_vtx_3[4] = {
	{{ {-613, 819, -509}, 0, {4566, -4120}, {0, 127, 0, 255} }},
	{{ {-1535, 819, -509}, 0, {0, -4120}, {0, 127, 0, 255} }},
	{{ {-1535, 819, 514}, 0, {0, 990}, {0, 127, 0, 255} }},
	{{ {-613, 819, 514}, 0, {4568, 990}, {0, 127, 0, 255} }},
};

Gfx bitdw_seg7_dl_070028A0_mesh_tri_3[] = {
	gsSPVertex(bitdw_seg7_dl_070028A0_mesh_vtx_3 + 0, 4, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(0, 2, 3, 0),
	gsSPEndDisplayList(),
};

Gfx mat_bitdw_f3dlite_material_004[] = {
	gsSPSetLights1(bitdw_f3dlite_material_004_lights),
	gsDPPipeSync(),
	gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, ENVIRONMENT, TEXEL0, 0, SHADE, 0, 0, 0, 0, ENVIRONMENT),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 1, sky_09001000),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 1023, 256),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0),
	gsDPSetTileSize(0, 0, 0, 124, 124),
	gsSPEndDisplayList(),
};

Gfx mat_revert_bitdw_f3dlite_material_004[] = {
	gsDPPipeSync(),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsSPEndDisplayList(),
};

Gfx mat_bitdw_f3dlite_material_005[] = {
	gsSPSetLights1(bitdw_f3dlite_material_005_lights),
	gsDPPipeSync(),
	gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, ENVIRONMENT, TEXEL0, 0, SHADE, 0, 0, 0, 0, ENVIRONMENT),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 1, sky_09000800),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 1023, 256),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0),
	gsDPSetTileSize(0, 0, 0, 124, 124),
	gsSPEndDisplayList(),
};

Gfx mat_revert_bitdw_f3dlite_material_005[] = {
	gsDPPipeSync(),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsSPEndDisplayList(),
};

Gfx mat_bitdw_f3dlite_material_006[] = {
	gsSPSetLights1(bitdw_f3dlite_material_006_lights),
	gsDPPipeSync(),
	gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, ENVIRONMENT, TEXEL0, 0, SHADE, 0, 0, 0, 0, ENVIRONMENT),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 1, sky_09007000),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 1023, 256),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0),
	gsDPSetTileSize(0, 0, 0, 124, 124),
	gsSPEndDisplayList(),
};

Gfx mat_revert_bitdw_f3dlite_material_006[] = {
	gsDPPipeSync(),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsSPEndDisplayList(),
};

Gfx mat_bitdw_f3dlite_material_007[] = {
	gsSPSetLights1(bitdw_f3dlite_material_007_lights),
	gsDPPipeSync(),
	gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, ENVIRONMENT, TEXEL0, 0, SHADE, 0, 0, 0, 0, ENVIRONMENT),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 1, sky_09007800),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 1023, 256),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0),
	gsDPSetTileSize(0, 0, 0, 124, 124),
	gsSPEndDisplayList(),
};

Gfx mat_revert_bitdw_f3dlite_material_007[] = {
	gsDPPipeSync(),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsSPEndDisplayList(),
};

Gfx bitdw_seg7_dl_070028A0[] = {
	gsSPDisplayList(mat_bitdw_f3dlite_material_004),
	gsSPDisplayList(bitdw_seg7_dl_070028A0_mesh_tri_0),
	gsSPDisplayList(mat_revert_bitdw_f3dlite_material_004),
	gsSPDisplayList(mat_bitdw_f3dlite_material_005),
	gsSPDisplayList(bitdw_seg7_dl_070028A0_mesh_tri_1),
	gsSPDisplayList(mat_revert_bitdw_f3dlite_material_005),
	gsSPDisplayList(mat_bitdw_f3dlite_material_006),
	gsSPDisplayList(bitdw_seg7_dl_070028A0_mesh_tri_2),
	gsSPDisplayList(mat_revert_bitdw_f3dlite_material_006),
	gsSPDisplayList(mat_bitdw_f3dlite_material_007),
	gsSPDisplayList(bitdw_seg7_dl_070028A0_mesh_tri_3),
	gsSPDisplayList(mat_revert_bitdw_f3dlite_material_007),
	gsDPPipeSync(),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPClearGeometryMode(G_TEXTURE_GEN),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsSPTexture(65535, 65535, 0, 0, 0),
	gsSPEndDisplayList(),
};

