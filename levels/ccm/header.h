#ifndef CCM_HEADER_H
#define CCM_HEADER_H

#include "types.h"
#include "game/moving_texture.h"
#include "game/areamap.h"

// geo
extern const GeoLayout snow_slider_geo[];
extern const GeoLayout ccm_geo_000B5C[];
extern const GeoLayout ccm_geo_000BEC[];
extern const GeoLayout ccm_geo_000C84[];
extern const GeoLayout ccm_geo_000D14[];
extern const GeoLayout ccm_geo_000D4C[];
extern const GeoLayout ccm_geo_000D84[];
extern const GeoLayout ccm_geo_000DBC[];
extern const GeoLayout ccm_geo_000DF4[];
extern const GeoLayout chimney_geo[];


// leveldata
extern const Gfx snow_slider_1_dl_mesh[];
extern const Gfx snow_slider_2_dl_mesh[];
extern const Gfx snow_slider_3_dl_mesh[];
extern const Collision snow_slider_collision[];
extern const Gfx slider_1_dl_mesh[];
extern const Gfx slider_3_dl_mesh[];
extern const Gfx slider_4_dl_mesh[];
extern const Gfx slider_5_dl_mesh[];
extern const Gfx slider_6_dl_mesh[];
extern const Gfx slider_7_dl_mesh[];
extern const Gfx slider_8_dl_mesh[];
extern const Gfx ccm_seg7_dl_0702A9C0[];
extern const Gfx ccm_seg7_dl_0702AAA8[];
extern const Gfx ccm_seg7_dl_0702AB90[];
extern const Gfx ccm_seg7_dl_0702AC78[];
extern const Gfx ccm_seg7_dl_0702BB60[];
extern const Gfx snow_slider_obj_00_mesh[];
extern const Collision ccm_seg7_area_2_collision[];
extern const Collision ccm_seg7_area_3_collision[];
extern const Collision ccm_seg7_area_4_collision[];
extern const Collision ccm_seg7_collision_podium_warp[];
extern const MacroObject ccm_seg7_area_2_macro_objs[];
extern const MacroObject ccm_seg7_area_3_macro_objs[];
extern const MacroObject ccm_seg7_area_4_macro_objs[];
extern struct AreaMapData areaMap_05_01;

// script
extern const LevelScript level_ccm_entry[];

#endif
