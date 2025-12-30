#ifndef CASTLE_HEADER_H
#define CASTLE_HEADER_H

#include "types.h"
#include "game/moving_texture.h"

// geo
extern const GeoLayout RCP_Stage1Scene1[];

// leveldata
extern const Gfx gfx_castle[];
extern const Collision castle_info[];
//extern static Movtex castle_movtex_water_data[];
extern const struct MovtexQuadCollection castle_movtex_water[];

// script
extern const LevelScript level_castle_entry[];

#endif
