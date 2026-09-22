#ifndef CASTLE_COURTYARD_HEADER_H
#define CASTLE_COURTYARD_HEADER_H

#include "types.h"
#include "game/moving_texture.h"
#include "game/areamap.h"

// geo
extern const GeoLayout courtyard_geo[];

// leveldata
extern const Gfx courtyard_1_dl_mesh[];
extern const Gfx courtyard_2_dl_mesh[];
extern const Gfx courtyard_3_dl_mesh[];

extern const Collision courtyard_collision[];

extern struct AreaMapData courtyard_areamap;

// script
extern const LevelScript level_castle_courtyard_entry[];

#endif
