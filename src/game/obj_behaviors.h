#ifndef OBJ_BEHAVIORS_H
#define OBJ_BEHAVIORS_H

#include <PR/ultratypes.h>

#include "engine/surface_collision.h"
#include "macros.h"
#include "types.h"

// Iwamoto iQue source equivalents in the comments above the functions

// iwacoin.s
void bhv_moving_coin_init(void);
void bhv_moving_coin_loop(void);
void bhv_slider_coin_init(void);
void bhv_slider_coin_loop(void);
// uzumaki.s
void bhv_jet_stream_loop(void);
// bird.s
void bhv_hoot_init(void);
void bhv_hoot_loop(void);
// kame.s
void bhv_beta_holdable_object_init(void); /* unused */
void bhv_beta_holdable_object_loop(void); /* unused */
// effect.s
void bhv_object_bubble_init(void);
void bhv_object_bubble_loop(void);
void bhv_object_water_wave_init(void);
void bhv_object_water_wave_loop(void);
void bhv_explosion_init(void);
void bhv_explosion_loop(void);
void bhv_bobomb_bully_death_smoke_init(void);
void bhv_bobomb_explosion_bubble_init(void);
void bhv_bobomb_explosion_bubble_loop(void);
// otos.s
void bhv_small_bully_init(void);
void bhv_bully_loop(void);
void bhv_big_bully_init(void);
// ring.s
void bhv_jet_stream_ring_spawner_loop(void);
void bhv_jet_stream_water_ring_init(void);
void bhv_jet_stream_water_ring_loop(void);
// kirai.s
void bhv_bowser_bomb_loop(void);
void bhv_bowser_bomb_explosion_loop(void);
void bhv_bowser_bomb_smoke_loop(void);
// V_star.s
void bhv_celebration_star_init(void);
void bhv_celebration_star_loop(void);
void bhv_celebration_star_sparkle_loop(void);
void bhv_star_dust_loop(void);
// movebg.s
void bhv_lll_drawbridge_spawner_loop(void);
void bhv_lll_drawbridge_loop(void);
void bhv_small_bomp_init(void);
void bhv_small_bomp_loop(void);
void bhv_large_bomp_init(void);
void bhv_large_bomp_loop(void);
void bhv_wf_sliding_platform_init(void);
void bhv_wf_sliding_platform_loop(void);
// others.s
void bhv_birds_sound_loop(void);
void bhv_ambient_sounds_init(void);

#endif // OBJ_BEHAVIORS_H
