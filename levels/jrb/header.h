#ifndef JRB_HEADER_H
#define JRB_HEADER_H

#include "types.h"
#include "game/moving_texture.h"
#include "game/areamap.h"

// geo

extern const GeoLayout jrb_geo_000A18[];


// leveldata
extern const Gfx sunken_ship_1_dl_mesh[];
extern const Gfx sunken_ship_2_dl_mesh[];
extern const Gfx sunken_ship_3_dl_mesh[];


extern const Collision sunken_ship_collision[];
extern struct AreaMapData areaMap_12_01;


// script
extern const LevelScript level_jrb_entry[];

#endif
