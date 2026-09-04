// The gfx data has to be separated like Mario's model

extern const Texture meye1_txt[];
extern const Texture meye2_txt[];
extern const Texture meye3_txt[];
extern const Texture meye4_txt[];
extern const Texture meye5_txt[];
extern const Texture m_button_txt[];

/********************************************************************************/
/*	Luigi texture																*/ 																
/********************************************************************************/

ALIGNED8 static const Texture l_mark_txt[] = {
#include "actors/player/l_mark_txt.rgba16.inc.c"
};

ALIGNED8 static const Texture l_momi_txt[] = {
#include "actors/player/l_momi_txt.rgba16.inc.c"
};

ALIGNED8 static const Texture l_hige_txt[] = {
#include "actors/player/l_hige_txt.rgba16.inc.c"
};
/********************************************************************************/
/*	Luigi gfx list.																*/
/********************************************************************************/

#define	SCALE	4
#define ShapeColor_luigi(R,G,B) {{R/SCALE ,G/SCALE ,B/SCALE ,0 ,R/SCALE ,G/SCALE ,B/SCALE ,0 },{ R, G, B, 0, R, G, B, 0, LIGHT_X, LIGHT_Y, LIGHT_Z, 0} }

// All of the .sou parts of Luigi were modified to remove the "static" keyword on the "RCP" parts
#include "luigi/luigi_near_poly.sou"
#include "luigi/luigi_near_body.sou"
#include "luigi/luigi_near_head.sou"
#include "luigi/luigi_mid_poly.sou"
#include "luigi/luigi_mid_body.sou"
#include "luigi/luigi_far_poly.sou"
#include "luigi/luigi_swim_hand.sou"
