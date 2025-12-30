/* ========================================================================================
		: Water surface records in 1 - 1.
===========================================================================================	*/
static Movtex castle_movtex_water_data[] = {
	MOV_TEX_INIT_LOAD(    2),
    MOV_TEX_ROT_SPEED(   32), // larger pool?
    MOV_TEX_ROT_SCALE(   25),
    MOV_TEX_4_BOX_TRIS(-3584, -3584),
    MOV_TEX_4_BOX_TRIS(-3584,  3584),
    MOV_TEX_4_BOX_TRIS( 3800,  3584),
    MOV_TEX_4_BOX_TRIS( 3800, -3584),
    MOV_TEX_ROT(     ROTATE_CLOCKWISE),
    MOV_TEX_ALPHA(    160),
    MOV_TEX_DEFINE(  TEXTURE_WATER),
	MOV_TEX_END(),
    MOV_TEX_ROT_SPEED(   32), // smaller pool?
    MOV_TEX_ROT_SCALE(    5),
    MOV_TEX_4_BOX_TRIS( 3072,  3072),
    MOV_TEX_4_BOX_TRIS( 3072,  6144),
    MOV_TEX_4_BOX_TRIS( 6144,  6144),
    MOV_TEX_4_BOX_TRIS( 6144,  3072),
    MOV_TEX_ROT(     ROTATE_COUNTER_CLOCKWISE),
    MOV_TEX_ALPHA(    160),
    MOV_TEX_DEFINE(  TEXTURE_WATER),
    MOV_TEX_END(),
};

const struct MovtexQuadCollection castle_movtex_water[] = {
	{0, castle_movtex_water_data},
	{0xffff,	NULL},
};