#ifndef WF_HEADER_H
#define WF_HEADER_H

#include "types.h"
#include "game/moving_texture.h"
#include "game/areamap.h"

// geo
extern const GeoLayout wf_geo_5[];
extern const GeoLayout wf_geo_6[];
extern const GeoLayout wf_geo_7[];
extern const GeoLayout wf_geo_8[];
extern const GeoLayout wf_geo_9[];
extern const GeoLayout wf_geo_10[];
extern const GeoLayout wf_geo_12[];
extern const GeoLayout wf_geo_13[];
extern const GeoLayout wf_geo_15[];
extern const GeoLayout wf_geo_16[];
extern const GeoLayout wf_geo_0008E8[];
extern const GeoLayout wf_geo_000A00[];
extern const GeoLayout wf_geo_000A40[];
extern const GeoLayout wf_geo_000A58[];
extern const GeoLayout wf_geo_000A98[];
extern const GeoLayout wf_geo_000AB0[];
extern const GeoLayout wf_geo_000AC8[];
extern const GeoLayout wf_geo_000AE0[];
extern const GeoLayout wf_geo_000AF8[];
extern const GeoLayout wf_geo_000B10[];
extern const GeoLayout wf_geo_000B38[];
extern const GeoLayout wf_geo_000B60[];
extern const GeoLayout mountain_geo[];

// leveldata
extern const Gfx mountain_1_dl_mesh[];
extern const Gfx mountain_5_dl_mesh[];
extern const Gfx mountain_5_lod_dl_mesh[];
extern const Gfx mountain_6_dl_mesh[];
extern const Gfx mountain_6_lod_dl_mesh[];
extern const Gfx mountain_7_dl_mesh[];
extern const Gfx mountain_8_dl_mesh[];
extern const Gfx mountain_9_dl_mesh[];
extern const Gfx mountain_10_dl_mesh[];
extern const Gfx mountain_12_dl_mesh[];
extern const Gfx mountain_12_lod_dl_mesh[];
extern const Gfx mountain_13_dl_mesh[];
extern const Gfx mountain_15_dl_mesh[];
extern const Gfx mountain_16_dl_mesh[];
extern const Gfx rotating_wooden_platform_decal_dl_mesh[];
extern const Gfx extending_platform_decal_dl_mesh[];

extern const Gfx wf_seg7_dl_070052B8[]; //keep

extern const Gfx wf_seg7_dl_07008FE8[]; //keep
extern const Gfx wf_seg7_dl_07009278[]; //keep
extern const Gfx wf_seg7_dl_0700A170[]; //keep
extern const Gfx wf_seg7_dl_0700AEC8[]; //keep
extern const Gfx wf_seg7_dl_0700AFB8[]; //keep

extern const Gfx wf_seg7_dl_0700D708[]; //keep
extern const Gfx wf_seg7_dl_0700D9F0[]; //keep
extern const Gfx wf_seg7_dl_0700DE20[]; //keep
extern const Gfx wf_seg7_dl_0700E0F0[]; //keep
extern const Gfx wf_seg7_dl_0700E3D0[]; //keep
extern const Gfx wf_seg7_dl_0700E760[]; //keep
extern const Gfx wf_seg7_dl_0700E9B8[]; //keep
extern const Gfx wf_seg7_dl_0700EB40[]; //keep
extern const Gfx beta_extending_platform_dl_mesh[];
extern const Gfx wf_seg7_dl_0700F018[]; //keep

extern const Collision wf_seg7_collision_small_bomp[];
extern const Collision wf_seg7_collision_large_bomp[];
extern const Collision wf_seg7_collision_clocklike_rotation[];
extern const Collision wf_seg7_collision_sliding_brick_platform[];
extern const Collision wf_seg7_collision_tumbling_bridge[];
extern const Collision wf_seg7_collision_trapezoid[];
extern const Collision wf_seg7_collision_platform[];
extern const Collision wf_seg7_collision_rotating_platform[];
extern const Collision wf_seg7_collision_tower[];
extern const Collision wf_seg7_collision_bullet_bill_cannon[];
extern const Collision mountain_10_collision[];
extern const Collision mountain_collision[];
extern const MacroObject mountain_macro[];
extern const struct MovtexQuadCollection wf_movtex_water[];
extern struct AreaMapData areaMap_24_01;

// script
extern const LevelScript level_wf_entry[];

#endif
