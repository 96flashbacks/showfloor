#include <ultra64.h>
#include "sm64.h"

#include "make_const_nonconst.h"

#define	LX		 0
#define	LY		 0
#define	LZ		80

#define	DefMaterial(R,G,B) { {R/2,G/2,B/2,0, R/2,G/2,B/2,0}, {R,G,B,0, R,G,B,0, LX,LY,LZ,0}}
#define	DefOldMaterial(R1,G1,B1,R2,G2,B2,DX,DY,DZ) { {R1,G1,B1,0, R1,G1,B1,0}, {R2,G2,B2,0, R2,G2,B2,0, DX,DY,DZ,0}}

#include "debug_level_select/title_ultraU.sou"
#include "debug_level_select/title_ultraL.sou"
#include "debug_level_select/title_ultraT.sou"
#include "debug_level_select/title_ultraR.sou"
#include "debug_level_select/title_ultraA.sou"
#include "debug_level_select/title_marioM.sou"
#include "debug_level_select/title_marioA.sou"
#include "debug_level_select/title_marioR.sou"
#include "debug_level_select/title_marioI.sou"
#include "debug_level_select/title_marioO.sou"

//#include "debug_level_select/title_stagesel.sou"
//#include "debug_level_select/title_face.sou"
//#include "debug_level_select/title_arrow.sou"
