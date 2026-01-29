#ifndef BOB_HEADER_H
#define BOB_HEADER_H

#include "types.h"
#include "game/moving_texture.h"
#include "game/areamap.h"

// geo
extern const GeoLayout bob_geo_000458[];
extern const GeoLayout bob_geo_000470[];
extern const GeoLayout bob_geo_000488[];

// leveldata
extern const Gfx bob_1_dl_mesh[];
extern const Gfx bob_pazair_mesh[];
extern const Gfx bob_3_dl_mesh[];
extern const Gfx bob_4_dl_mesh[];
extern const Gfx bob_5_dl_mesh[];
extern const Gfx bob_seg7_dl_0700E768[];
extern const Gfx bob_seg7_dl_0700E8A0[];
extern const Collision bob_collision[];
extern const MacroObject bob_seg7_macro_objs[];
//extern const Collision bob_seg7_collision_chain_chomp_gate[];
extern const Collision bob_seg7_collision_bridge[];
extern const Collision bob_seg7_collision_gate[];
extern const Trajectory bob_seg7_metal_ball_path0[];
extern const Trajectory bob_seg7_metal_ball_path1[];
extern const Trajectory bob_seg7_trajectory_koopa[];
extern Movtex bob_movtex_tris_waterfall[];
extern const Gfx bob_dl_waterfall[];
extern const struct MovtexQuadCollection bob_movtex_water[];
extern struct AreaMapData areaMap_27_01;

// script
extern const LevelScript level_bob_entry[];

#endif
