
typedef struct {
#ifndef GBI_FLOATS
	short		x, y, z;
#else
	float		x, y, z;
#endif
	unsigned short	flag;
	short		tcu, tcv;
	unsigned char	cn0, cn1, cn2, cn3;
} BetaVtx_t;

typedef struct {
#ifndef GBI_FLOATS
	short		x, y, z;
#else
	float		x, y, z;
#endif
	unsigned short	flag;
	short		tcu, tcv;
	unsigned char	n0, n1, n2;
	unsigned char a;
} BetaVtx_tn;

/*typedef union {
    BetaVtx_t		v;  / * Use this one for colors  * /
    BetaVtx_tn              n;  / * Use this one for normals * /
    long long int	force_structure_alignment;
} BetaVtx;*/
typedef BetaVtx_t BetaVtx;
