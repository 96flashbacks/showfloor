// 0x07006E4C - 0x07006E7C
static Movtex bob_movtex_water_data[] = {
    MOV_TEX_INIT_LOAD(   1),
    MOV_TEX_ROT_SPEED(   52),
    MOV_TEX_ROT_SCALE(   16),
    MOV_TEX_4_BOX_TRIS(-4402,  6589),
    MOV_TEX_4_BOX_TRIS(-4402, -7489), 
    MOV_TEX_4_BOX_TRIS( 1997, -7489),
    MOV_TEX_4_BOX_TRIS( 1997,  6589),
    MOV_TEX_ROT(     ROTATE_CLOCKWISE),
    MOV_TEX_ALPHA(    0x96),
    MOV_TEX_DEFINE(  TEXTURE_WATER),
    MOV_TEX_END(),
};

const struct MovtexQuadCollection bob_movtex_water[] = {
    {0, bob_movtex_water_data},
    {-1, NULL},
};

Movtex bob_movtex_tris_waterfall[] = {
    MOV_TEX_SPD(32),
    MOV_TEX_TRIS(-1096,  2631, -5922,  1, 10),
    MOV_TEX_TRIS(-1811,  2440, -7143,  0,  7),
    MOV_TEX_TRIS(-1142,  2440, -5891,  0, 10),
    MOV_TEX_TRIS(-1016,  2640, -5970,  1, 10),
    MOV_TEX_TRIS(-1765,  2631, -7166,  1,  7),
    MOV_TEX_TRIS(-1811,  1018, -7143, -2,  7),
    MOV_TEX_TRIS(-1142,  1018, -5891, -2, 10),
    MOV_TEX_TRIS(-1685,  2640, -7222,  1,  7),
    MOV_TEX_TRIS(-2511,    46, -6783, -4,  7),
    MOV_TEX_TRIS(-1842,    46, -5531, -4, 10),
};

// 0x07010F30 - 0x07010FC0
const Gfx bob_dl_waterfall[] = {
    gsSP2Triangles( 0,  1,  2, 0x0,  3,  4,  0, 0x0),
    gsSP2Triangles( 2,  5,  6, 0x0,  0,  4,  1, 0x0),
    gsSP2Triangles( 3,  7,  4, 0x0,  2,  1,  5, 0x0),
    gsSP2Triangles( 5,  8,  6, 0x0,  6,  8,  9, 0x0),
    gsSPEndDisplayList(),
};
