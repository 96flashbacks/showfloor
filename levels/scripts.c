#include <ultra64.h>
#include "sm64.h"
#include "game/level_update.h"
#include "level_commands.h"
#include "game/area.h"

#include "make_const_nonconst.h"

#include "segment_symbols.h"

#include "actors/common0.h"
#include "actors/common1.h"
#include "actors/group0.h"
#include "actors/group1.h"
#include "actors/group2.h"
#include "actors/group4.h"
#include "actors/group7.h"
#include "actors/group9.h"
#include "actors/group10.h"
#include "actors/group12.h"
#include "actors/group13.h"
#include "actors/group14.h"
#include "actors/group15.h"
#include "actors/group17.h"

#include "levels/intro/header.h"

#include "level_headers.h"

#include "level_table.h"

#define STUB_LEVEL(_0, _1, _2, _3, _4, _5, _6, _7, _8)
#define DEFINE_LEVEL(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10) + 3
static const LevelScript script_exec_level_table[2
  #include "level_defines.h"
];
#undef DEFINE_LEVEL
#undef STUB_LEVEL

static const LevelScript script_L1[4];
static const LevelScript goto_mario_head_regular[4];
static const LevelScript goto_mario_head_dizzy[4];
static const LevelScript script_L5[4];

#define STUB_LEVEL(_0, _1, _2, _3, _4, _5, _6, _7, _8)
#define DEFINE_LEVEL(_0, _1, _2, folder, _4, _5, _6, _7, _8, _9, _10) static const LevelScript script_exec_ ## folder [4 + 1];

#include "level_defines.h"

#undef DEFINE_LEVEL
#undef STUB_LEVEL

const LevelScript level_main_scripts_entry[] = {
    LOAD_MIO0(/*seg*/ 0x04, _group0_mio0SegmentRomStart, _group0_mio0SegmentRomEnd),
    LOAD_MIO0(/*seg*/ 0x03, _common1_mio0SegmentRomStart, _common1_mio0SegmentRomEnd),
    LOAD_RAW (/*seg*/ 0x17, _group0_geoSegmentRomStart, _group0_geoSegmentRomEnd),
    LOAD_RAW (/*seg*/ 0x16, _common1_geoSegmentRomStart, _common1_geoSegmentRomEnd),
    LOAD_RAW (/*seg*/ 0x13, _behaviorSegmentRomStart, _behaviorSegmentRomEnd),
    ALLOC_LEVEL_POOL(),
    LOAD_MODEL_FROM_GEO(MODEL_MARIO,                   RCP_MarioHierarchy),
    LOAD_MODEL_FROM_GEO(MODEL_SMOKE,                   smoke_geo),
    LOAD_MODEL_FROM_GEO(MODEL_SPARKLES,                sparkles_geo),
    LOAD_MODEL_FROM_GEO(MODEL_BUBBLE,                  bubble_geo),
    LOAD_MODEL_FROM_GEO(MODEL_SMALL_WATER_SPLASH,      small_water_splash_geo),
    LOAD_MODEL_FROM_GEO(MODEL_IDLE_WATER_WAVE,         idle_water_wave_geo),
    LOAD_MODEL_FROM_GEO(MODEL_WATER_SPLASH,            water_splash_geo),
    LOAD_MODEL_FROM_GEO(MODEL_WAVE_TRAIL,              wave_trail_geo),
    LOAD_MODEL_FROM_GEO(MODEL_YELLOW_COIN,             RCP_HmsItemCoin),
    LOAD_MODEL_FROM_GEO(MODEL_STAR,                    RCP_HmsItemStar),
    LOAD_MODEL_FROM_GEO(MODEL_STAR_DUST,               RCP_HmsItemStarDust),
    LOAD_MODEL_FROM_GEO(MODEL_TRANSPARENT_STAR,        RCP_HmsDotstar),
    LOAD_MODEL_FROM_DL (MODEL_WHITE_PARTICLE_SMALL,    white_particle_small_dl, LAYER_ALPHA),
    LOAD_MODEL_FROM_GEO(MODEL_RED_FLAME,               red_flame_geo),
    LOAD_MODEL_FROM_GEO(MODEL_YELLOW_FLAME,            yellow_flame_geo),
    LOAD_MODEL_FROM_GEO(MODEL_BURN_SMOKE,              burn_smoke_geo),
    LOAD_MODEL_FROM_GEO(MODEL_LEAVES,                  leaves_geo),
    LOAD_MODEL_FROM_GEO(MODEL_PURPLE_MARBLE,           purple_marble_geo),
    LOAD_MODEL_FROM_GEO(MODEL_FISH,                    fish_geo),
    LOAD_MODEL_FROM_GEO(MODEL_FISH_SHADOW,             fish_shadow_geo),
    LOAD_MODEL_FROM_GEO(MODEL_SPARKLES_ANIMATION,      sparkles_animation_geo),
    LOAD_MODEL_FROM_DL (MODEL_SAND_DUST,               sand_seg3_dl_0302BCD0,   LAYER_ALPHA),
    LOAD_MODEL_FROM_GEO(MODEL_BURN_SMOKE_UNUSED,       burn_smoke_geo),
    LOAD_MODEL_FROM_GEO(MODEL_MIST,                    mist_geo),
    LOAD_MODEL_FROM_GEO(MODEL_WHITE_PUFF,              white_puff_geo),
    LOAD_MODEL_FROM_DL (MODEL_WHITE_PARTICLE_DL,       white_particle_dl,       LAYER_ALPHA),
    LOAD_MODEL_FROM_GEO(MODEL_WHITE_PARTICLE,          white_particle_geo),
    LOAD_MODEL_FROM_GEO(MODEL_RED_FLAME_SHADOW,        red_flame_shadow_geo),
    LOAD_MODEL_FROM_GEO(MODEL_NUMBER,                  number_geo),
    LOAD_MODEL_FROM_GEO(MODEL_EXPLOSION,               RCP_HmsItembombfire),
    LOAD_MODEL_FROM_GEO(MODEL_DIRT_ANIMATION,          dirt_animation_geo),
    LOAD_MODEL_FROM_GEO(MODEL_CARTOON_STAR,            cartoon_star_geo),
    FREE_LEVEL_POOL(),
    CALL(/*arg*/ 0, /*func*/ lvl_init_from_save_file),
    LOOP_BEGIN(),
        CALL(/*arg*/ 0, /*func*/ lvl_set_current_level),
        GET_OR_SET(/*op*/ OP_SET, /*var*/ VAR_CURR_ACT_NUM),
        JUMP_LINK(script_exec_level_table),
        SLEEP(/*frames*/ 1),
    LOOP_UNTIL(/*op*/ OP_LT, /*arg*/ 0),
    JUMP_IF(/*op*/ OP_EQ, /*arg*/ -2, goto_mario_head_regular),
    JUMP_IF(/*op*/ OP_EQ, /*arg*/ -3, goto_mario_head_dizzy),
    JUMP_IF(/*op*/ OP_EQ, /*arg*/ -8, script_L1),
    JUMP_IF(/*op*/ OP_EQ, /*arg*/ -9, script_L5),
};

static const LevelScript script_L1[] = {
    EXIT_AND_EXECUTE(/*seg*/ 0x14, _introSegmentRomStart, _introSegmentRomEnd, level_intro_splash_screen),
};

static const LevelScript goto_mario_head_regular[] = {
    EXIT_AND_EXECUTE(/*seg*/ 0x14, _introSegmentRomStart, _introSegmentRomEnd, level_intro_mario_head_regular),
};

static const LevelScript goto_mario_head_dizzy[] = {
    EXIT_AND_EXECUTE(/*seg*/ 0x14, _introSegmentRomStart, _introSegmentRomEnd, level_intro_mario_head_dizzy),
};

static const LevelScript script_L5[] = {
    EXIT_AND_EXECUTE(/*seg*/ 0x14, _introSegmentRomStart, _introSegmentRomEnd, level_intro_entry_4),
};

// Include the level jumptable.

#define STUB_LEVEL(_0, _1, _2, _3, _4, _5, _6, _7, _8)

#define DEFINE_LEVEL(_0, levelenum, _2, folder, _4, _5, _6, _7, _8, _9, _10) JUMP_IF(OP_EQ, levelenum, script_exec_ ## folder),

static const LevelScript script_exec_level_table[] = {
    GET_OR_SET(/*op*/ OP_GET, /*var*/ VAR_CURR_LEVEL_NUM),
    #include "levels/level_defines.h"
    EXIT(),
};
#undef DEFINE_LEVEL

#define DEFINE_LEVEL(_0, _1, _2, folder, _4, _5, _6, _7, _8, _9, _10) \
static const LevelScript script_exec_ ## folder [] = { \
    EXECUTE(0x0E, _ ## folder ## SegmentRomStart, _ ## folder ## SegmentRomEnd, level_ ## folder ## _entry), \
    RETURN(), \
};

#include "levels/level_defines.h"
#undef STUB_LEVEL
#undef DEFINE_LEVEL

const LevelScript script_func_global_1[] = {
    LOAD_MODEL_FROM_GEO(MODEL_CANNON_BARREL,           RCP_HmsCannonBody),
    LOAD_MODEL_FROM_GEO(MODEL_CANNON_BASE,             RCP_HmsCannonBase),
    LOAD_MODEL_FROM_GEO(MODEL_GOOMBA,                  RCP_HmsEnemykuribo),
    LOAD_MODEL_FROM_GEO(MODEL_CHUCKYA,                 RCP_HmsEnemycarryboy),
    RETURN(),
};

const LevelScript script_func_global_2[] = {
    LOAD_MODEL_FROM_GEO(MODEL_BULLET_BILL,             bullet_bill_geo),
    LOAD_MODEL_FROM_GEO(MODEL_YELLOW_SPHERE,           yellow_sphere_geo),
    LOAD_MODEL_FROM_GEO(MODEL_HOOT,                    hoot_geo),
    LOAD_MODEL_FROM_GEO(MODEL_THWOMP,                  RCP_HmsDosun),
    LOAD_MODEL_FROM_GEO(MODEL_HEAVE_HO,                RCP_HmsEnemyomurobo),
    RETURN(),
};

const LevelScript script_func_global_3[] = {
    LOAD_MODEL_FROM_GEO(MODEL_BLARGG,                  blargg_geo),
    LOAD_MODEL_FROM_GEO(MODEL_BULLY,                   RCP_HmsEnemyotos_basedata),
    LOAD_MODEL_FROM_GEO(MODEL_BULLY_BOSS,              RCP_HmsEnemyBigotos_basedata),
    RETURN(),
};
const LevelScript script_func_global_4[] = {
        LOAD_MODEL_FROM_GEO(MODEL_BLACK_BOBOMB,          RCP_HmsEnemybom_basedata),
        RETURN(),
};

const LevelScript script_func_global_5[] = {
    LOAD_MODEL_FROM_GEO(MODEL_SUSHI,                   RCP_HmsEnemyShark),
    RETURN(),
};

const LevelScript script_func_global_8[] = {
    LOAD_MODEL_FROM_GEO(MODEL_PENGUIN,                 RCP_HmsEnemyping_base),
    RETURN(),
};

const LevelScript script_func_global_10[] = {
    LOAD_MODEL_FROM_GEO(MODEL_BOO,                     boo_geo),
    RETURN(),
};

const LevelScript script_func_global_11[] = {
    LOAD_MODEL_FROM_GEO(MODEL_BIRDS,                   birds_geo),
    RETURN(),
};

const LevelScript script_func_global_12[] = {
    LOAD_MODEL_FROM_GEO(MODEL_MIPS,                    RCP_rabbitHierarchy),
    RETURN(),
};

const LevelScript script_func_global_13[] = {
    LOAD_MODEL_FROM_GEO(MODEL_BOWSER,                  bowser_geo),
    LOAD_MODEL_FROM_GEO(MODEL_BOWSER_BOMB_CHILD_OBJ,   bowser_bomb_geo),
    LOAD_MODEL_FROM_GEO(MODEL_BOWSER_BOMB,             bowser_bomb_geo),
    LOAD_MODEL_FROM_GEO(MODEL_BOWSER_SMOKE,            bowser_impact_smoke_geo),
    LOAD_MODEL_FROM_GEO(MODEL_BOWSER_FLAMES,           bowser_flames_geo),
    LOAD_MODEL_FROM_GEO(MODEL_BOWSER_NO_SHADOW,        bowser_geo_no_shadow),
    RETURN(),
};

const LevelScript script_func_global_14[] = {
    LOAD_MODEL_FROM_GEO(MODEL_BUB,                     bub_geo),
    LOAD_MODEL_FROM_GEO(MODEL_TREASURE_CHEST_BASE,     treasure_chest_base_geo),
    LOAD_MODEL_FROM_GEO(MODEL_TREASURE_CHEST_LID,      treasure_chest_lid_geo),
    LOAD_MODEL_FROM_GEO(MODEL_CYAN_FISH,               cyan_fish_geo),
    LOAD_MODEL_FROM_GEO(MODEL_WATER_RING,              water_ring_geo),
    LOAD_MODEL_FROM_GEO(MODEL_WATER_MINE,              water_mine_geo),
    RETURN(),
};

const LevelScript script_func_global_15[] = {
    LOAD_MODEL_FROM_GEO(MODEL_KOOPA_WITH_SHELL,        RCP_HmsEnemynokonoko),
    LOAD_MODEL_FROM_GEO(MODEL_PIRANHA_PLANT,           RCP_HmsEnemypakun_angry),
    LOAD_MODEL_FROM_GEO(MODEL_WHOMP,                   RCP_HmsEnemywallman),
    RETURN(),
};

const LevelScript script_func_global_18[] = {
    LOAD_MODEL_FROM_GEO(MODEL_MR_I_IRIS,               mr_i_iris_geo),
    LOAD_MODEL_FROM_GEO(MODEL_MR_I,                    mr_i_geo),
    RETURN(),
};
