#ifndef CASTLE_INSIDE_HEADER_H
#define CASTLE_INSIDE_HEADER_H

#include "types.h"
#include "game/moving_texture.h"

// geo
extern const GeoLayout castle_geo_000F18[]; // Trap Door
extern const GeoLayout castle_geo[];

// leveldata
extern struct Painting ccm_painting;
extern struct Painting wf_painting;
extern struct Painting jrb_painting;
extern struct Painting lll_painting;

extern const Gfx trapdoor_mesh[];            // Trap Door
extern Gfx castle_inside_gfx_sel_a_mesh[];   // Lobby Room
extern Gfx castle_inside_gfx_sel_a_a_mesh[]; // Lobby Room (Decals)
extern Gfx castle_inside_gfx_sel_b_mesh[];   // Courtyard Hallway
extern Gfx castle_inside_gfx_sel_c_mesh[];   // Bowser Hallway
extern Gfx gfx_sr_mipmap[];                  // Bowser Hallway (Mipmap Painting)
extern Gfx castle_inside_gfx_sel_d_mesh[];   // Mountain Room
extern Gfx castle_inside_gfx_sel_e_mesh[];   // Fire Bubble Room
extern Gfx castle_inside_gfx_sel_e_a_mesh[]; // Fire Bubble Room (Decals)
extern Gfx castle_inside_gfx_sel_f_mesh[];   // Snow Slider Room
extern Gfx castle_inside_gfx_sel_g_mesh[];   // Water Land Room

extern const Collision castle_inside_collision[];
extern const u8 castle_inside_collision_rooms[];
extern const Collision inside_castle_seg7_collision_floor_trap[]; // Trap Door

// script
extern const LevelScript level_castle_inside_entry[];

#endif
