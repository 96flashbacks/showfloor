#ifndef THI_HEADER_H
#define THI_HEADER_H

#include "types.h"
#include "game/moving_texture.h"
#include "game/areamap.h"

// geo
extern const GeoLayout thi_geo_0005B0[];
extern const GeoLayout thi_geo_0005C8[];
extern const GeoLayout thi_geo_0005F0[];
extern const GeoLayout thi_geo_000608[];
extern const GeoLayout thi_geo_0006D4[];
extern const GeoLayout thi_geo_00079C[];

// leveldata
extern const Gfx big_world_1_dl_mesh[];
extern const Gfx big_world_2_dl_mesh[];
extern const Gfx big_world_3_dl_mesh[];
extern const Gfx big_world_4_dl_mesh[];
extern const Gfx big_world_5_dl_mesh[];
//extern const Gfx thi_seg7_dl_07007538[];
extern const Gfx thi_seg7_dl_07007648[];
extern const Gfx thi_seg7_dl_07007930[];
//extern const Gfx thi_seg7_dl_07007C20[];
//extern const Gfx thi_seg7_dl_07009670[];
//extern const Gfx thi_seg7_dl_07009D50[];
//extern const Gfx thi_seg7_dl_07009F58[];
extern const Collision big_world_collision[];
extern const Collision thi_seg7_area_2_collision[];
extern const Collision thi_seg7_area_3_collision[];
extern const MacroObject thi_seg7_area_1_macro_objs[];
extern const MacroObject thi_seg7_area_2_macro_objs[];
extern const MacroObject thi_seg7_area_3_macro_objs[];
extern const Collision thi_seg7_collision_top_trap[];
extern const Trajectory thi_seg7_trajectory_koopa[];
extern const struct MovtexQuadCollection thi_movtex_area1_water[];
extern const struct MovtexQuadCollection thi_movtex_area2_water[];
extern struct AreaMapData areaMap_13_01;

// script
extern const LevelScript level_thi_entry[];

#endif
