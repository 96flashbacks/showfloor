#include <ultra64.h>
#include "sm64.h"
#include "surface_terrains.h"
#include "moving_texture_macros.h"
#include "level_misc_macros.h"
#include "macro_presets.h"
#include "special_presets.h"
#include "textures.h"
#include "dialog_ids.h"
#include "game/areamap.h"

#include "make_const_nonconst.h"

#include "levels/ddd/texture.inc.c"
#include "levels/ddd/material.inc.c"

// Static level geometry model data
#include "levels/ddd/areas/1/1/model.inc.c"
#include "levels/ddd/areas/1/2/model.inc.c"
#include "levels/ddd/areas/2/1/model.inc.c"
#include "levels/ddd/areas/2/2/model.inc.c"
#include "levels/ddd/areas/2/3/model.inc.c"
// Submarine model data
#include "levels/ddd/submarine/1.inc.c"
#include "levels/ddd/submarine/2.inc.c"

// Area 1 and 2 collison data
#include "levels/ddd/areas/1/collision.inc.c"
#include "levels/ddd/areas/2/collision.inc.c"

// Area 1 and 2 macro objects
#include "levels/ddd/areas/1/macro.inc.c"
#include "levels/ddd/areas/2/macro.inc.c"

// Submarine collision data
#include "levels/ddd/submarine/collision.inc.c"

// Water surface and areamap data
#include "levels/ddd/areas/1/movtext.inc.c"
#include "levels/ddd/areas/2/movtext.inc.c"
#include "levels/ddd/areas/1/areamap.inc.c"
#include "levels/ddd/areas/2/areamap.inc.c"
