/********************************************************************************/
/*	Snow Slider																	*/
/********************************************************************************/

static const Lights1 snow_slider_lights = gdSPDefLights1(
	0x3F, 0x3F, 0x3F,
	0xFF, 0xFF, 0xFF, 0x28, 0x28, 0x28);

static const Gfx mat_snow_slider_SnowMaterial[] = {
	gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsSPSetLights1(snow_slider_lights),
	gsSPEndDisplayList(),
};

static const Gfx mat_revert_snow_slider_SnowMaterial[] = {
	gsDPPipeSync(),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsSPEndDisplayList(),
};

static const Gfx mat_snow_slider_IceMaterial[] = {
	gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_MODULATERGB, G_CC_MODULATERGB),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, ccm_seg7_texture_07000000),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 1023, 256),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0),
	gsDPSetTileSize(0, 0, 0, 124, 124),
	gsSPSetLights1(snow_slider_lights),
	gsSPEndDisplayList(),
};

static const Gfx mat_revert_snow_slider_IceMaterial[] = {
	gsDPPipeSync(),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsSPEndDisplayList(),
};

static const Gfx mat_snow_slider_GraniteRockMaterial[] = {
	gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_MODULATERGB, G_CC_MODULATERGB),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, ccm_seg7_texture_07000900),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 1023, 256),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0),
	gsDPSetTileSize(0, 0, 0, 124, 124),
	gsSPSetLights1(snow_slider_lights),
	gsSPEndDisplayList(),
};

static const Gfx mat_revert_snow_slider_GraniteRockMaterial[] = {
	gsDPPipeSync(),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsSPEndDisplayList(),
};

static const Gfx mat_snow_slider_DirtySnowMaterial[] = {
	gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_MODULATERGB, G_CC_MODULATERGB),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, ccm_seg7_texture_07001100),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 2047, 128),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 0, 0, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, 0, G_TX_WRAP | G_TX_NOMIRROR, 6, 0),
	gsDPSetTileSize(0, 0, 0, 252, 124),
	gsSPSetLights1(snow_slider_lights),
	gsSPEndDisplayList(),
};

static const Gfx mat_revert_snow_slider_DirtySnowMaterial[] = {
	gsDPPipeSync(),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsSPEndDisplayList(),
};

static const Gfx mat_snow_slider_IceHazardMaterial[] = {
	gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_MODULATERGB, G_CC_MODULATERGB),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, ccm_seg7_texture_07000800),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 1023, 256),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0),
	gsDPSetTileSize(0, 0, 0, 124, 124),
	gsSPSetLights1(snow_slider_lights),
	gsSPEndDisplayList(),
};

static const Gfx mat_revert_snow_slider_IceHazardMaterial[] = {
	gsDPPipeSync(),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsSPEndDisplayList(),
};

static const Gfx mat_snow_slider_IceBrickGradientMaterial[] = {
	gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_MODULATERGB, G_CC_MODULATERGB),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 1, ccm_seg7_texture_07002100),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 1023, 256),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, 0, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0),
	gsDPSetTileSize(0, 0, 0, 124, 124),
	gsSPSetLights1(snow_slider_lights),
	gsSPEndDisplayList(),
};

static const Gfx mat_revert_snow_slider_IceBrickGradientMaterial[] = {
	gsDPPipeSync(),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsSPEndDisplayList(),
};

static const Gfx mat_snow_slider_VoidMaterial[] = {
	gsDPPipeSync(),
	gsSPClearGeometryMode(G_LIGHTING),
    gsDPSetCombineMode(G_CC_PRIMITIVE, G_CC_PRIMITIVE),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetPrimColor(0, 0, 0, 0, 0, 255),
	gsSPEndDisplayList(),
};

static const Gfx mat_revert_snow_slider_VoidMaterial[] = {
	gsSPSetGeometryMode(G_LIGHTING),
	gsDPPipeSync(),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsSPEndDisplayList(),
};

static const Gfx mat_snow_slider_IceBrickMaterial[] = {
	gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_MODULATERGB, G_CC_MODULATERGB),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, ccm_seg7_texture_07001900),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 1023, 256),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0),
	gsDPSetTileSize(0, 0, 0, 124, 124),
	gsSPSetLights1(snow_slider_lights),
	gsSPEndDisplayList(),
};

static const Gfx mat_revert_snow_slider_IceBrickMaterial[] = {
	gsDPPipeSync(),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsSPEndDisplayList(),
};

static const Gfx mat_snow_slider_RockMaterial[] = {
	gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_MODULATERGB, G_CC_MODULATERGB),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, ccm_seg7_texture_07003100),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 1023, 256),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0),
	gsDPSetTileSize(0, 0, 0, 124, 124),
	gsSPSetLights1(snow_slider_lights),
	gsSPEndDisplayList(),
};

static const Gfx mat_revert_snow_slider_RockMaterial[] = {
	gsDPPipeSync(),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsSPEndDisplayList(),
};


static const Gfx mat_snow_slider_FenceMaterial[] = {
	gsDPPipeSync(),
	gsSPClearGeometryMode(G_CULL_BACK),
    gsDPSetCombineMode(G_CC_DECALRGBA, G_CC_DECALRGBA),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 1, ccm_seg7_texture_07002900),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 2047, 128),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 0, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0, G_TX_WRAP | G_TX_NOMIRROR, 6, 0),
	gsDPSetTileSize(0, 0, 0, 252, 124),
	gsSPEndDisplayList(),
};

static const Gfx mat_revert_snow_slider_FenceMaterial[] = {
	gsSPSetGeometryMode(G_CULL_BACK),
	gsDPPipeSync(),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsSPEndDisplayList(),
};

/********************************************************************************/
/*	Slider																		*/
/********************************************************************************/

static const Lights1 slider_00_lights = gdSPDefLights1(
	0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x28, 0x28, 0x28);

static const Lights1 slider_22_lights = gdSPDefLights1(
	0x8, 0x8, 0x8,
	0x22, 0x22, 0x22, 0x28, 0x28, 0x28);

static const Lights1 slider_33_lights = gdSPDefLights1(
	0xC, 0xC, 0xC,
	0x33, 0x33, 0x33, 0x28, 0x28, 0x28);

static const Lights1 slider_44_lights = gdSPDefLights1(
	0x11, 0x11, 0x11,
	0x44, 0x44, 0x44, 0x28, 0x28, 0x28);

static const Lights1 slider_55_lights = gdSPDefLights1(
	0x15, 0x15, 0x15,
	0x55, 0x55, 0x55, 0x28, 0x28, 0x28);

static const Lights1 slider_66_lights = gdSPDefLights1(
	0x19, 0x19, 0x19,
	0x66, 0x66, 0x66, 0x28, 0x28, 0x28);

static const Lights1 slider_77_lights = gdSPDefLights1(
	0x1D, 0x1D, 0x1D,
	0x77, 0x77, 0x77, 0x28, 0x28, 0x28);

static const Lights1 slider_77_22_lights = gdSPDefLights1(
	0x22, 0x22, 0x22,
	0x77, 0x77, 0x77, 0x28, 0x28, 0x28);

static const Lights1 slider_77_2A_lights = gdSPDefLights1(
	0x2A, 0x2A, 0x2A,
	0x77, 0x77, 0x77, 0x28, 0x28, 0x28);

static const Lights1 slider_88_lights = gdSPDefLights1(
	0x22, 0x22, 0x22,
	0x88, 0x88, 0x88, 0x28, 0x28, 0x28);

static const Lights1 slider_99_lights = gdSPDefLights1(
	0x26, 0x26, 0x26,
	0x99, 0x99, 0x99, 0x28, 0x28, 0x28);

static const Lights1 slider_AA_lights = gdSPDefLights1(
	0x2A, 0x2A, 0x2A,
	0xAA, 0xAA, 0xAA, 0x28, 0x28, 0x28);

static const Lights1 slider_BB_lights = gdSPDefLights1(
	0x2E, 0x2E, 0x2E,
	0xBB, 0xBB, 0xBB, 0x28, 0x28, 0x28);

static const Lights1 slider_CC_lights = gdSPDefLights1(
	0x33, 0x33, 0x33,
	0xCC, 0xCC, 0xCC, 0x28, 0x28, 0x28);

static const Lights1 slider_DD_lights = gdSPDefLights1(
	0x37, 0x37, 0x37,
	0xDD, 0xDD, 0xDD, 0x28, 0x28, 0x28);

static const Lights1 slider_EE_lights = gdSPDefLights1(
	0x3B, 0x3B, 0x3B,
	0xEE, 0xEE, 0xEE, 0x28, 0x28, 0x28);

static const Lights1 slider_FF_lights = gdSPDefLights1(
	0x3F, 0x3F, 0x3F,
	0xFF, 0xFF, 0xFF, 0x28, 0x28, 0x28);

static const Lights1 slider_yellow_lights = gdSPDefLights1(
	0x3F, 0x3F, 0x0,
	0xFF, 0xFF, 0x0, 0x28, 0x28, 0x28);

static const Gfx mat_slider_fog_start[] = {
	gsDPPipeSync(),
	gsDPSetCycleType(G_CYC_2CYCLE),
	gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
	gsDPSetDepthSource(G_ZS_PIXEL),
	gsDPSetFogColor(0, 0, 0, 255),
	gsSPFogPosition(980, 1000),
	gsSPSetGeometryMode(G_FOG),
	gsDPSetCombineMode(G_CC_MODULATERGB, G_CC_PASS2),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsSPEndDisplayList(),
};

static const Gfx mat_slider_bricks_load_block[] = {
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 1, slide_09006800),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 1023, 256),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0),
	gsDPSetTileSize(0, 0, 0, 124, 124),
	gsSPEndDisplayList(),
};

static const Gfx mat_slider_slide_load_block[] = {
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 1, slide_0900B800),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 1023, 256),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0),
	gsDPSetTileSize(0, 0, 0, 124, 124),
	gsSPEndDisplayList(),
};

static const Gfx mat_slider_striped_bricks_load_block[] = {
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 1, slide_0900A800),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 1023, 256),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0),
	gsDPSetTileSize(0, 0, 0, 124, 124),
	gsSPEndDisplayList(),
};

static const Gfx mat_slider_metal_load_block[] = {
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 1, slide_09008800),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 2047, 128),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 0, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0, G_TX_WRAP | G_TX_NOMIRROR, 6, 0),
	gsDPSetTileSize(0, 0, 0, 252, 124),
	gsSPEndDisplayList(),
};

static const Gfx mat_slider_sides_load_block[] = {
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 1, slide_09000800),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 2047, 128),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 0, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0, G_TX_CLAMP | G_TX_NOMIRROR, 6, 0),
	gsDPSetTileSize(0, 0, 0, 252, 124),
	gsSPEndDisplayList(),
};

static const Gfx mat_slider_wood_load_block[] = {
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 1, slide_09007000),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 2047, 128),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 0, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0, G_TX_WRAP | G_TX_NOMIRROR, 6, 0),
	gsDPSetTileSize(0, 0, 0, 252, 124),
	gsSPEndDisplayList(),
};

static const Gfx mat_slider_BricksMaterial_22[] = {
	gsSPDisplayList(mat_slider_fog_start),
	gsSPDisplayList(mat_slider_bricks_load_block),
	gsSPSetLights1(slider_22_lights),
	gsSPEndDisplayList(),
};

static const Gfx mat_slider_BricksMaterial_33[] = {
	gsSPDisplayList(mat_slider_fog_start),
	gsSPDisplayList(mat_slider_bricks_load_block),
	gsSPSetLights1(slider_33_lights),
	gsSPEndDisplayList(),
};

static const Gfx mat_slider_BricksMaterial_44[] = {
	gsSPDisplayList(mat_slider_fog_start),
	gsSPDisplayList(mat_slider_bricks_load_block),
	gsSPSetLights1(slider_44_lights),
	gsSPEndDisplayList(),
};

static const Gfx mat_slider_BricksMaterial_55[] = {
	gsSPDisplayList(mat_slider_fog_start),
	gsSPDisplayList(mat_slider_bricks_load_block),
	gsSPSetLights1(slider_55_lights),
	gsSPEndDisplayList(),
};

static const Gfx mat_slider_BricksMaterial_66[] = {
	gsSPDisplayList(mat_slider_fog_start),
	gsSPDisplayList(mat_slider_bricks_load_block),
	gsSPSetLights1(slider_66_lights),
	gsSPEndDisplayList(),
};

static const Gfx mat_slider_BricksMaterial_FF[] = {
	gsSPDisplayList(mat_slider_fog_start),
	gsSPDisplayList(mat_slider_bricks_load_block),
	gsSPSetLights1(slider_FF_lights),
	gsSPEndDisplayList(),
};

static const Gfx mat_slider_SlideMaterial_55[] = {
	gsSPDisplayList(mat_slider_fog_start),
	gsSPDisplayList(mat_slider_slide_load_block),
	gsSPSetLights1(slider_55_lights),
	gsSPEndDisplayList(),
};

static const Gfx mat_slider_SlideMaterial_66[] = {
	gsSPDisplayList(mat_slider_fog_start),
	gsSPDisplayList(mat_slider_slide_load_block),
	gsSPSetLights1(slider_66_lights),
	gsSPEndDisplayList(),
};

static const Gfx mat_slider_SlideMaterial_77[] = {
	gsSPDisplayList(mat_slider_fog_start),
	gsSPDisplayList(mat_slider_slide_load_block),
	gsSPSetLights1(slider_77_lights),
	gsSPEndDisplayList(),
};

static const Gfx mat_slider_SlideMaterial_77_2A[] = {
	gsSPDisplayList(mat_slider_fog_start),
	gsSPDisplayList(mat_slider_slide_load_block),
	gsSPSetLights1(slider_77_2A_lights),
	gsSPEndDisplayList(),
};

static const Gfx mat_slider_SlideMaterial_88[] = {
	gsSPDisplayList(mat_slider_fog_start),
	gsSPDisplayList(mat_slider_slide_load_block),
	gsSPSetLights1(slider_88_lights),
	gsSPEndDisplayList(),
};

static const Gfx mat_slider_SlideMaterial_99[] = {
	gsSPDisplayList(mat_slider_fog_start),
	gsSPDisplayList(mat_slider_slide_load_block),
	gsSPSetLights1(slider_99_lights),
	gsSPEndDisplayList(),
};

static const Gfx mat_slider_SlideMaterial_AA[] = {
	gsSPDisplayList(mat_slider_fog_start),
	gsSPDisplayList(mat_slider_slide_load_block),
	gsSPSetLights1(slider_AA_lights),
	gsSPEndDisplayList(),
};

static const Gfx mat_slider_SlideMaterial_BB[] = {
	gsSPDisplayList(mat_slider_fog_start),
	gsSPDisplayList(mat_slider_slide_load_block),
	gsSPSetLights1(slider_BB_lights),
	gsSPEndDisplayList(),
};

static const Gfx mat_slider_SlideMaterial_CC[] = {
	gsSPDisplayList(mat_slider_fog_start),
	gsSPDisplayList(mat_slider_slide_load_block),
	gsSPSetLights1(slider_CC_lights),
	gsSPEndDisplayList(),
};

static const Gfx mat_slider_SlideMaterial_DD[] = {
	gsSPDisplayList(mat_slider_fog_start),
	gsSPDisplayList(mat_slider_slide_load_block),
	gsSPSetLights1(slider_DD_lights),
	gsSPEndDisplayList(),
};

static const Gfx mat_slider_SlideMaterial_FF[] = {
	gsSPDisplayList(mat_slider_fog_start),
	gsSPDisplayList(mat_slider_slide_load_block),
	gsSPSetLights1(slider_FF_lights),
	gsSPEndDisplayList(),
};

static const Gfx mat_slider_StripedBricksMaterial_BB[] = {
	gsSPDisplayList(mat_slider_fog_start),
	gsSPDisplayList(mat_slider_striped_bricks_load_block),
	gsSPSetLights1(slider_BB_lights),
	gsSPEndDisplayList(),
};

static const Gfx mat_slider_StripedBricksMaterial_CC[] = {
	gsSPDisplayList(mat_slider_fog_start),
	gsSPDisplayList(mat_slider_striped_bricks_load_block),
	gsSPSetLights1(slider_CC_lights),
	gsSPEndDisplayList(),
};

static const Gfx mat_slider_StripedBricksMaterial_FF[] = {
	gsSPDisplayList(mat_slider_fog_start),
	gsSPDisplayList(mat_slider_striped_bricks_load_block),
	gsSPSetLights1(slider_FF_lights),
	gsSPEndDisplayList(),
};

static const Gfx mat_slider_MetalMaterial_00[] = {
	gsSPDisplayList(mat_slider_fog_start),
	gsSPDisplayList(mat_slider_metal_load_block),
	gsSPSetLights1(slider_00_lights),
	gsSPEndDisplayList(),
};

static const Gfx mat_slider_MetalMaterial_44[] = {
	gsSPDisplayList(mat_slider_fog_start),
	gsSPDisplayList(mat_slider_metal_load_block),
	gsSPSetLights1(slider_44_lights),
	gsSPEndDisplayList(),
};

static const Gfx mat_slider_MetalMaterial_55[] = {
	gsSPDisplayList(mat_slider_fog_start),
	gsSPDisplayList(mat_slider_metal_load_block),
	gsSPSetLights1(slider_55_lights),
	gsSPEndDisplayList(),
};

static const Gfx mat_slider_MetalMaterial_66[] = {
	gsSPDisplayList(mat_slider_fog_start),
	gsSPDisplayList(mat_slider_metal_load_block),
	gsSPSetLights1(slider_66_lights),
	gsSPEndDisplayList(),
};

static const Gfx mat_slider_MetalMaterial_77[] = {
	gsSPDisplayList(mat_slider_fog_start),
	gsSPDisplayList(mat_slider_metal_load_block),
	gsSPSetLights1(slider_77_lights),
	gsSPEndDisplayList(),
};

static const Gfx mat_slider_MetalMaterial_77_22[] = {
	gsSPDisplayList(mat_slider_fog_start),
	gsSPDisplayList(mat_slider_metal_load_block),
	gsSPSetLights1(slider_77_22_lights),
	gsSPEndDisplayList(),
};

static const Gfx mat_slider_MetalMaterial_88[] = {
	gsSPDisplayList(mat_slider_fog_start),
	gsSPDisplayList(mat_slider_metal_load_block),
	gsSPSetLights1(slider_88_lights),
	gsSPEndDisplayList(),
};

static const Gfx mat_slider_MetalMaterial_99[] = {
	gsSPDisplayList(mat_slider_fog_start),
	gsSPDisplayList(mat_slider_metal_load_block),
	gsSPSetLights1(slider_99_lights),
	gsSPEndDisplayList(),
};

static const Gfx mat_slider_MetalMaterial_AA[] = {
	gsSPDisplayList(mat_slider_fog_start),
	gsSPDisplayList(mat_slider_metal_load_block),
	gsSPSetLights1(slider_AA_lights),
	gsSPEndDisplayList(),
};

static const Gfx mat_slider_MetalMaterial_BB[] = {
	gsSPDisplayList(mat_slider_fog_start),
	gsSPDisplayList(mat_slider_metal_load_block),
	gsSPSetLights1(slider_BB_lights),
	gsSPEndDisplayList(),
};

static const Gfx mat_slider_MetalMaterial_CC[] = {
	gsSPDisplayList(mat_slider_fog_start),
	gsSPDisplayList(mat_slider_metal_load_block),
	gsSPSetLights1(slider_CC_lights),
	gsSPEndDisplayList(),
};

static const Gfx mat_slider_MetalMaterial_DD[] = {
	gsSPDisplayList(mat_slider_fog_start),
	gsSPDisplayList(mat_slider_metal_load_block),
	gsSPSetLights1(slider_DD_lights),
	gsSPEndDisplayList(),
};

static const Gfx mat_slider_MetalMaterial_EE[] = {
	gsSPDisplayList(mat_slider_fog_start),
	gsSPDisplayList(mat_slider_metal_load_block),
	gsSPSetLights1(slider_EE_lights),
	gsSPEndDisplayList(),
};

static const Gfx mat_slider_MetalMaterial_FF[] = {
	gsSPDisplayList(mat_slider_fog_start),
	gsSPDisplayList(mat_slider_metal_load_block),
	gsSPSetLights1(slider_FF_lights),
	gsSPEndDisplayList(),
};

static const Gfx mat_slider_SidesMaterial_00[] = {
	gsSPDisplayList(mat_slider_fog_start),
	gsSPDisplayList(mat_slider_sides_load_block),
	gsSPSetLights1(slider_00_lights),
	gsSPEndDisplayList(),
};

static const Gfx mat_slider_SidesMaterial_FF[] = {
	gsSPDisplayList(mat_slider_fog_start),
	gsSPDisplayList(mat_slider_sides_load_block),
	gsSPSetLights1(slider_FF_lights),
	gsSPEndDisplayList(),
};

static const Gfx mat_slider_WoodMaterial[] = {
	gsSPDisplayList(mat_slider_fog_start),
	gsSPDisplayList(mat_slider_wood_load_block),
	gsSPSetLights1(slider_FF_lights),
	gsSPEndDisplayList(),
};

static const Gfx mat_slider_SnowSlideArrowMaterial[] = {
	gsSPDisplayList(mat_slider_fog_start),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 1, slide_SnowSlideArrow_rgba16),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 2047, 128),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 0, 0, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, 0, G_TX_CLAMP | G_TX_NOMIRROR, 6, 0),
	gsDPSetTileSize(0, 0, 0, 252, 124),
	gsSPSetLights1(slider_FF_lights),
	gsSPEndDisplayList(),
};

static const Gfx mat_revert_slider_Material[] = {
	gsDPPipeSync(),
	gsDPSetCycleType(G_CYC_1CYCLE),
	gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_NOOP2),
	gsSPClearGeometryMode(G_FOG),
	gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
	gsSPEndDisplayList(),
};

static const Gfx mat_slider_StartMaterial[] = {
	gsDPPipeSync(),
	gsSPClearGeometryMode(G_CULL_BACK),
    gsDPSetCombineMode(G_CC_MODULATEIFADEA, G_CC_MODULATEIFADEA),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetEnvColor(255, 255, 255, 255),
	gsDPSetTextureImage(G_IM_FMT_IA, G_IM_SIZ_16b_LOAD_BLOCK, 1, ccm_seg7_texture_start),
	gsDPSetTile(G_IM_FMT_IA, G_IM_SIZ_16b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 2047, 128),
	gsDPSetTile(G_IM_FMT_IA, G_IM_SIZ_16b, 16, 0, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0, G_TX_WRAP | G_TX_NOMIRROR, 6, 0),
	gsDPSetTileSize(0, 0, 0, 252, 124),
	gsSPSetLights1(slider_yellow_lights),
	gsSPEndDisplayList(),
};

static const Gfx mat_slider_JumpMaterial[] = {
	gsDPPipeSync(),
	gsSPClearGeometryMode(G_CULL_BACK),
    gsDPSetCombineMode(G_CC_MODULATEIFADEA, G_CC_MODULATEIFADEA),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetEnvColor(255, 255, 255, 255),
	gsDPSetTextureImage(G_IM_FMT_IA, G_IM_SIZ_16b_LOAD_BLOCK, 1, ccm_seg7_texture_jump),
	gsDPSetTile(G_IM_FMT_IA, G_IM_SIZ_16b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 2047, 128),
	gsDPSetTile(G_IM_FMT_IA, G_IM_SIZ_16b, 16, 0, 0, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, 0, G_TX_CLAMP | G_TX_NOMIRROR, 6, 0),
	gsDPSetTileSize(0, 0, 0, 252, 124),
	gsSPSetLights1(slider_yellow_lights),
	gsSPEndDisplayList(),
};

static const Gfx mat_revert_slider_TextMaterial[] = {
	gsSPSetGeometryMode(G_CULL_BACK),
	gsDPPipeSync(),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsSPEndDisplayList(),
};
