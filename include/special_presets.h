#ifndef SPECIAL_PRESETS_H
#define SPECIAL_PRESETS_H

enum SpecialPresets {
    special_null_start,                         // OBJSETCODE_PLAYER
    special_yellow_coin,                        // OBJSETCODE_COIN
    special_yellow_coin_2,                      // OBJSETCODE_STAR
    special_unknown_3,                          // OBJSETCODE_ELEVATOR
    special_boo,                                // OBJSETCODE_TERESA
    special_castle_floor_trap,                  // OBJSETCODE_MAINROOM_TRAP
    special_lll_moving_octagonal_mesh_platform, // OBJSETCODE_MOTOS_OBJ01
    special_empty_7,                            // OBJSETCODE_MOTOS_OBJ02
    special_lll_drawbridge_spawner,             // OBJSETCODE_MOTOS_OBJ03
    special_empty_9,                            // OBJSETCODE_MOTOS_OBJ04
    special_lll_rotating_block_with_fire_bars,  // OBJSETCODE_MOTOS_OBJ05
    special_lll_floating_wood_bridge,           // OBJSETCODE_MOTOS_OBJ06
    special_tumbling_platform,                  // OBJSETCODE_MOTOS_OBJ07
    special_lll_rotating_hexagonal_ring,        // OBJSETCODE_MOTOS_OBJ08
    special_lll_sinking_rectangular_platform,   // OBJSETCODE_MOTOS_OBJ09
    special_lll_sinking_square_platforms,       // OBJSETCODE_MOTOS_OBJ10
    special_lll_tilting_square_platform,        // OBJSETCODE_MOTOS_OBJ11
    special_lll_bowser_puzzle,                  // OBJSETCODE_MOTOS_OBJ12
    special_mr_i,                               // OBJSETCODE_MEDAMA
    special_small_bully,                        // OBJSETCODE_MOTOS
    special_big_bully,                          // OBJSETCODE_BIGMOTOS
    special_empty_21,                           // OBJSETCODE_BLOCK
    special_empty_22,                           // OBJSETCODE_FIREMOTOS
    special_empty_23,                           // OBJSETCODE_FIREBAR
    special_empty_24,                           // OBJSETCODE_LAVA
    special_empty_25,                           // OBJSETCODE_FIRE
    special_moving_blue_coin,                   // OBJSETCODE_SLIDER_COIN
    special_jrb_chest,                          // OBJSETCODE_TAKARA_BOX
    special_water_ring,                         // OBJSETCODE_RING
    special_mine,                               // OBJSETCODE_KIRAI
    special_empty_30,                           // OBJSETCODE_BIRD
    special_empty_31,                           // OBJSETCODE_FLOWER
    special_butterfly,                          // OBJSETCODE_BUTTERFLY
    special_bowser,                             // OBJSETCODE_KOPA
    special_wf_rotating_wooden_platform,        // OBJSETCODE_CASTLE_ROTEBAR
    special_small_bomp,                         // OBJSETCODE_CASTLE_DOSSUNBAR
    special_wf_sliding_platform,                // OBJSETCODE_CASTLE_TRANSBAR50
    special_tower_platform_group,               // OBJSETCODE_CASTLE_GOALBAR
    special_rotating_counter_clockwise,         // OBJSETCODE_CASTLE_SIDEBAR
    special_wf_tumbling_bridge,                 // OBJSETCODE_CASTLE_DOWNBRIDGE
    special_large_bomp,                         // OBJSETCODE_CASTLE_DOSSUNBAR2

    special_level_geo_03 = 101,                 // OBJSETCODE_BG01
    special_level_geo_04,                       // OBJSETCODE_BG02
    special_level_geo_05,                       // OBJSETCODE_BG03
    special_level_geo_06,                       // OBJSETCODE_BG04
    special_level_geo_07,                       // OBJSETCODE_BG05
    special_level_geo_08,                       // OBJSETCODE_BG06
    special_level_geo_09,                       // OBJSETCODE_BG07
    special_level_geo_0A,                       // OBJSETCODE_BG08
    special_level_geo_0B,                       // OBJSETCODE_BG09
    special_level_geo_0C,                       // OBJSETCODE_BG10
    special_level_geo_0D,                       // OBJSETCODE_BG11
    special_level_geo_0E,                       // OBJSETCODE_BG12
    special_level_geo_0F,                       // OBJSETCODE_BG13
    special_level_geo_10,                       // OBJSETCODE_BG14
    special_level_geo_11,                       // OBJSETCODE_BG15
    special_level_geo_12,                       // OBJSETCODE_BG16
    special_level_geo_13,                       // OBJSETCODE_BG17
    special_level_geo_14,                       // OBJSETCODE_BG18
    special_level_geo_15,                       // OBJSETCODE_BG19
    special_level_geo_16,                       // OBJSETCODE_BG20
    special_bubble_tree,                        // OBJSETCODE_TREE1
    // There were likely no trees 2-5 or doors 1-5 in the demo
    special_castle_door_warp,                   // OBJSETCODE_TRIPDOOR0
    special_castle_door,                        // OBJSETCODE_DOOR0
    special_castle_door_A,                      // OBJSETCODE_DOOR_A
    special_castle_door_B,                      // OBJSETCODE_DOOR_B
    special_castle_door_C,                      // OBJSETCODE_DOOR_C
    special_castle_door_D,                      // OBJSETCODE_DOOR_D

    special_null_end = 0xFF
};

#endif // SPECIAL_PRESETS_H
