#ifndef DDD_HEADER_H
#define DDD_HEADER_H

#include "types.h"
#include "game/moving_texture.h"
#include "game/areamap.h"

// geo
extern const GeoLayout water_land_submarine_geo[];
extern const GeoLayout water_land_area_1_geo[];
extern const GeoLayout water_land_area_2_geo[];

// leveldata
extern const Gfx water_land_1_dl_mesh[];
extern const Gfx water_land_2_dl_mesh[];
extern const Gfx water_land_3_dl_mesh[];
extern const Gfx water_land_4_dl_mesh[];
extern const Gfx water_land_5_dl_mesh[];
extern const Gfx submarine_1_dl_mesh[];
extern const Gfx submarine_2_dl_mesh[];

extern const Collision water_land_area_1_collision[];
extern const Collision water_land_area_2_collision[];

extern const MacroObject water_land_area_1_macro[];
extern const MacroObject water_land_area_2_macro[];

extern const Collision water_land_submarine_collision[];

extern const struct MovtexQuadCollection ddd_movtex_area1_water[];
extern const struct MovtexQuadCollection ddd_movtex_area2_water[];
extern struct AreaMapData water_land_area_1_areamap;
extern struct AreaMapData water_land_area_2_areamap;

// script
extern const LevelScript level_ddd_entry[];

#endif
