// 0x0700D28C
static Movtex jrb_movtex_area1_water_data[] = {
    MOV_TEX_INIT_LOAD(    1),
    MOV_TEX_ROT_SPEED(   20),
    MOV_TEX_ROT_SCALE(    6),
    MOV_TEX_4_BOX_TRIS(-4095, -5631),
    MOV_TEX_4_BOX_TRIS(-4095,  3753),
    MOV_TEX_4_BOX_TRIS( 5631,  3753),
    MOV_TEX_4_BOX_TRIS( 5631, -5631),
    MOV_TEX_ROT(     ROTATE_COUNTER_CLOCKWISE),
    MOV_TEX_ALPHA(    0xB4),
    MOV_TEX_DEFINE(  TEXTURE_JRB_WATER),
    MOV_TEX_END(),
};

// 0x0700D2CC
const struct MovtexQuadCollection jrb_movtex_area1_water[] = {
    {0, jrb_movtex_area1_water_data},
    {-1, NULL},
};
