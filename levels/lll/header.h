#ifndef LLL_HEADER_H
#define LLL_HEADER_H

#include "types.h"
#include "game/moving_texture.h"
#include "game/areamap.h"

/* this could have a bunch of externs removed but that isn't super important right now */

/* i really hate how this level is sorted out, it's so messy. same as wf */

/* at least the others levels are somewhat clean in comparison, lol */

// geo
extern const GeoLayout lll_geo_0009E0[];
extern const GeoLayout lll_geo_0009F8[];
extern const GeoLayout lll_geo_000A10[];
extern const GeoLayout lll_geo_000A28[];
extern const GeoLayout lll_geo_000A40[];
extern const GeoLayout lll_geo_000A60[];
extern const GeoLayout lll_geo_000A78[];
extern const GeoLayout fire_bubble_platform_geo[];
extern const GeoLayout fire_bubble_volcano_geo[];
extern const GeoLayout lll_geo_000AC0[];
extern const GeoLayout lll_geo_000AD8[];
extern const GeoLayout lll_geo_000AF0[];
extern const GeoLayout lll_geo_000B08[];
extern const GeoLayout lll_geo_000B20[];
extern const GeoLayout lll_geo_000B38[];
extern const GeoLayout lll_geo_000B50[];
extern const GeoLayout lll_geo_000B68[];
extern const GeoLayout lll_geo_000B80[];
extern const GeoLayout lll_geo_000B98[];
extern const GeoLayout lll_geo_000BB0[];
extern const GeoLayout lll_geo_000BC8[];
extern const GeoLayout lll_geo_000BE0[];
extern const GeoLayout lll_geo_000BF8[];
extern const GeoLayout lll_geo_000C10[];
extern const GeoLayout lll_geo_000C30[];
extern const GeoLayout lll_geo_000C50[];
extern const GeoLayout lll_geo_000C70[];
extern const GeoLayout lll_geo_000C90[];
extern const GeoLayout lll_geo_000CB0[];
extern const GeoLayout lll_geo_000CD0[];
extern const GeoLayout lll_geo_000CF0[];
extern const GeoLayout lll_geo_000D10[];
extern const GeoLayout lll_geo_000D30[];
extern const GeoLayout lll_geo_000D50[];
extern const GeoLayout lll_geo_000D70[];
extern const GeoLayout lll_geo_000D90[];
extern const GeoLayout lll_geo_000DB0[];
extern const GeoLayout lll_geo_000DD0[];
extern const GeoLayout lll_geo_000DE8[];
extern const GeoLayout fire_bubble_geo[];

// leveldata
extern const Gfx fire_bubble_lava_plane_mesh[];
extern const Gfx fire_bubble_1_dl_mesh[];
extern const Gfx lll_seg7_dl_070137C0[];
extern const Gfx lll_seg7_dl_070138F8[];
extern const Gfx lll_seg7_dl_07013D28[];
extern const Gfx lll_seg7_dl_07014788[];
extern const Gfx lll_seg7_dl_07014BD8[];
extern const Gfx lll_seg7_dl_07015458[];
extern const Gfx lll_seg7_dl_07015C88[];
extern const Gfx lll_seg7_dl_07015E20[];
extern const Gfx lll_seg7_dl_07016250[];
extern const Gfx lll_seg7_dl_070165C8[];
extern const Gfx fire_bubble_2_dl_mesh[];
extern const Gfx fire_bubble_3_dl_mesh[];
extern const Gfx lll_seg7_dl_070178A8[];
extern const Gfx lll_seg7_dl_07017B50[];
extern const Gfx lll_seg7_dl_07017F40[];
extern const Gfx lll_seg7_dl_07018380[];
extern const Gfx lll_seg7_dl_07018680[];
extern const Gfx lll_seg7_dl_07018A30[];
extern const Gfx lll_seg7_dl_07018C90[];
extern const Gfx lll_seg7_dl_07018EF8[];
extern const Gfx lll_seg7_dl_07019160[];
extern const Gfx lll_seg7_dl_070193E0[];
extern const Gfx lll_seg7_dl_07019A08[];
extern const Gfx lll_seg7_dl_07019C08[];
extern const Gfx lll_seg7_dl_0701A010[];
extern const Gfx lll_seg7_dl_0701A1F0[];
extern const Gfx lll_seg7_dl_0701A388[];
extern const Gfx lll_seg7_dl_0701A3B8[];
extern const Gfx lll_seg7_dl_0701A3E8[];
extern const Gfx lll_seg7_dl_0701A418[];
extern const Gfx lll_seg7_dl_0701A448[];
extern const Gfx lll_seg7_dl_0701A478[];
extern const Gfx lll_seg7_dl_0701A4A8[];
extern const Gfx lll_seg7_dl_0701A4D8[];
extern const Gfx lll_seg7_dl_0701A508[];
extern const Gfx lll_seg7_dl_0701A538[];
extern const Gfx lll_seg7_dl_0701A568[];
extern const Gfx lll_seg7_dl_0701A598[];
extern const Gfx lll_seg7_dl_0701A5C8[];
extern const Gfx lll_seg7_dl_0701A5F8[];
extern const Gfx lll_seg7_dl_0701A628[];
extern const Gfx lll_seg7_dl_0701A878[];
extern const Gfx lll_seg7_dl_0701AD70[];
extern const Collision fire_bubble_collision[];
extern const MacroObject fire_bubble_macro[];
extern const Collision lll_seg7_collision_octagonal_moving_platform[];
extern const Collision lll_seg7_collision_drawbridge[];
extern const Collision lll_seg7_collision_rotating_fire_bars[];
extern const Collision lll_seg7_collision_wood_piece[];
extern const Collision lll_seg7_collision_0701D21C[];
extern const Collision lll_seg7_collision_rotating_platform[];
extern const Collision lll_seg7_collision_slow_tilting_platform[];
extern const Collision lll_seg7_collision_sinking_pyramids[];
extern const Collision lll_seg7_collision_inverted_pyramid[];
extern const Collision lll_seg7_collision_puzzle_piece[];
extern const Collision lll_seg7_collision_floating_block[];
extern const Collision lll_seg7_collision_hexagonal_platform[];
extern struct AreaMapData fire_bubble_areamap;

// script
extern const LevelScript level_lll_entry[];

#endif
