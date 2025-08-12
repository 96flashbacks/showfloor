#ifndef CASTLE_GROUNDS_HEADER_H
#define CASTLE_GROUNDS_HEADER_H

#include "types.h"
#include "game/moving_texture.h"
#include "game/areamap.h"

// geo
extern const GeoLayout castle_grounds_tower_geo[];
extern const GeoLayout castle_grounds_geo[];

// leveldata
extern const Gfx castle_grounds_1_dl_mesh[];
extern const Gfx castle_grounds_2_dl_mesh[];
extern const Gfx castle_grounds_3_dl_mesh[];
extern const Gfx castle_grounds_4_dl_mesh[];
extern const Gfx castle_grounds_5_dl_mesh[];
extern const Gfx castle_grounds_tower_1_dl_mesh[];
extern const Gfx castle_grounds_tower_2_dl_mesh[];
extern const Collision castle_grounds_collision[];
extern const struct MovtexQuadCollection castle_grounds_movtex_water[];
extern Movtex castle_grounds_movtex_tris_waterfall[];
extern const Gfx castle_grounds_dl_waterfall[];
extern struct AreaMapData areaMap_16_01;

// script
extern const LevelScript level_castle_grounds_entry[];

#endif
