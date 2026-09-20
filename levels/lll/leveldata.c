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

#include "levels/lll/texture.inc.c"
#include "levels/lll/material.inc.c"

// Unlike other levels LLL has a specific file with lights inside
#include "levels/lll/areas/1/light.inc.c"
// Static level geometry model data
#include "levels/lll/areas/1/1/model.inc.c"
#include "levels/lll/areas/1/2/model.inc.c"
#include "levels/lll/areas/1/3/model.inc.c"
// Level geometry objects model data
#include "levels/lll/areas/1/4/model.inc.c"
#include "levels/lll/areas/1/5/model.inc.c"
#include "levels/lll/areas/1/6/model.inc.c"
#include "levels/lll/areas/1/7/model.inc.c"
#include "levels/lll/areas/1/8/model.inc.c"
#include "levels/lll/areas/1/9/model.inc.c"
#include "levels/lll/areas/1/10/model.inc.c"
#include "levels/lll/areas/1/11/model.inc.c"
#include "levels/lll/areas/1/12/model.inc.c"
#include "levels/lll/areas/1/13/model.inc.c"
#include "levels/lll/areas/1/14/model.inc.c"
#include "levels/lll/areas/1/15/model.inc.c"

// "MoveBG" model data
#include "levels/lll/moving_octagonal_mesh_platform/model.inc.c"
#include "levels/lll/drawbridge_part/model.inc.c"
#include "levels/lll/rotating_block_fire_bars/model.inc.c"
#include "levels/lll/wooden_float_small/model.inc.c"
#include "levels/lll/wooden_float_large/model.inc.c"
#include "levels/lll/collapsing_wooden_platform/model.inc.c"
#include "levels/lll/long_wooden_bridge/model.inc.c"
#include "levels/lll/rotating_hexagonal_ring/model.inc.c"
#include "levels/lll/sinking_rectangular_platform/model.inc.c"
#include "levels/lll/sinking_square_platform/model.inc.c"
#include "levels/lll/tilting_square_platform/model.inc.c"
#include "levels/lll/puzzle_piece/model.inc.c"

// Main level collision data and macro objects
#include "levels/lll/areas/1/collision.inc.c"
#include "levels/lll/areas/1/macro.inc.c"

// "MoveBG" collision data
#include "levels/lll/moving_octagonal_mesh_platform/collision.inc.c"
#include "levels/lll/drawbridge_part/collision.inc.c"
#include "levels/lll/rotating_block_fire_bars/collision.inc.c"
#include "levels/lll/wooden_float_small/collision.inc.c"
#include "levels/lll/collapsing_wooden_platform/collision.inc.c"
#include "levels/lll/rotating_hexagonal_ring/collision.inc.c"
#include "levels/lll/sinking_rectangular_platform/collision.inc.c"
#include "levels/lll/sinking_square_platform/collision.inc.c"
#include "levels/lll/tilting_square_platform/collision.inc.c"
#include "levels/lll/puzzle_piece/collision.inc.c"
#include "levels/lll/areas/1/10/collision.inc.c"

// Areamap data
#include "levels/lll/areas/1/areamap.inc.c"
