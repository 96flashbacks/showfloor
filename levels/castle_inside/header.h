#ifndef CASTLE_INSIDE_HEADER_H
#define CASTLE_INSIDE_HEADER_H

#include "types.h"
#include "game/moving_texture.h"

// geo
extern const GeoLayout castle_geo[];
extern const GeoLayout castle_geo_000F18[]; //trap door

// leveldata
extern struct Painting ccm_painting;
extern struct Painting wf_painting;
extern struct Painting jrb_painting;
extern struct Painting lll_painting;
extern const Gfx trapdoor_mesh[]; //trap door
extern Gfx castle_inside_main_dl_mesh[];
extern Gfx castle_inside_decal_dl_mesh[];
extern Gfx castle_inside_bowser_dl_mesh[];
extern Gfx castle_inside_bowser_painting_dl_mesh[];
extern Gfx castle_inside_mountain_dl_mesh[];
extern Gfx castle_inside_fire_bubble_dl_mesh[];
extern Gfx castle_inside_fire_bubble_decal_dl_mesh[];
extern Gfx castle_inside_water_land_dl_mesh[];
extern Gfx castle_inside_snow_slider_dl_mesh[];
extern Gfx castle_inside_hallway_dl_mesh[];
extern const Collision castle_inside_collision[];
extern const u8 castle_inside_collision_rooms[];
extern const Collision inside_castle_seg7_collision_floor_trap[]; //trap door

/*  don't forget to fully remove these later:

extern const Collision inside_castle_seg7_collision_star_door[];
extern const Collision inside_castle_seg7_collision_water_level_pillar[];
extern const Trajectory *const inside_castle_seg7_trajectory_mips[];

*/

// script
extern const LevelScript level_castle_inside_entry[];

#endif
