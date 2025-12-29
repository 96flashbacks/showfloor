#include <ultra64.h>
#include "sm64.h"
#include "behavior_data.h"
#include "model_ids.h"
#include "seq_ids.h"
#include "dialog_ids.h"
#include "segment_symbols.h"
#include "level_commands.h"

#include "game/level_update.h"

#include "levels/scripts.h"

#include "actors/common1.h"

#include "make_const_nonconst.h"
#include "levels/ccm/header.h"

static const LevelScript script_func_local_1[] = {
    OBJECT(/*model*/ MODEL_PENGUIN,          /*pos*/  7346, -4096,  6186, /*angle*/ 0,   0, 0, /*bhvParam*/ 0x00020000, /*bhv*/ bhvSmallPenguin),
    OBJECT(/*model*/ MODEL_PENGUIN,          /*pos*/  7488, -4096,  6083, /*angle*/ 0,   0, 0, /*bhvParam*/ 0x00020000, /*bhv*/ bhvSmallPenguin),
    OBJECT(/*model*/ MODEL_PENGUIN,          /*pos*/  7863, -4096, -7915, /*angle*/ 0,  90, 0, /*bhvParam*/ 0x00020000, /*bhv*/ bhvSmallPenguin),
    OBJECT(/*model*/ MODEL_PENGUIN,          /*pos*/  2147,  4096, -1261, /*angle*/ 0, 180, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvSmallPenguin),
    RETURN(),
};

static const LevelScript script_func_local_2[] = {
    OBJECT(/*model*/ MODEL_PENGUIN,          /*pos*/  7296, -4096,  6085, /*angle*/ 0, 0, 0, /*bhvParam*/ 0x01000000, /*bhv*/ bhvTuxiesMother),
    OBJECT(/*model*/ MODEL_STAR,             /*pos*/  5625, -3578,  5379, /*angle*/ 0, 0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvStar),
    RETURN(),
};

static const LevelScript script_func_local_area_2[] = {
    OBJECT(/*model*/ MODEL_TTM_BLUE_SMILEY,   /*pos*/  4389,  3160,   624, /*angle*/ 0, 0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvStaticObject),
    OBJECT(/*model*/ MODEL_TTM_YELLOW_SMILEY, /*pos*/ -1251,  2050,  2224, /*angle*/ 0, 0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvStaticObject),
    OBJECT(/*model*/ MODEL_TTM_STAR_SMILEY,   /*pos*/ -2547,   876,  -520, /*angle*/ 0, 0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvStaticObject),
    OBJECT(/*model*/ MODEL_TTM_MOON_SMILEY,   /*pos*/  -324,   500, -4090, /*angle*/ 0, 0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvStaticObject),
    RETURN(),
};

static const LevelScript script_func_local_area_3[] = {
    OBJECT(/*model*/ MODEL_TTM_BLUE_SMILEY,   /*pos*/  7867, -1559, -6085, /*angle*/ 0, 0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvStaticObject),
    OBJECT(/*model*/ MODEL_TTM_BLUE_SMILEY,   /*pos*/ -5241,  4700,  9466, /*angle*/ 0, 0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvStaticObject),
    OBJECT(/*model*/ MODEL_TTM_YELLOW_SMILEY, /*pos*/ -1869, -5800,  7358, /*angle*/ 0, 0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvStaticObject),
    OBJECT(/*model*/ MODEL_TTM_STAR_SMILEY,   /*pos*/ -9095,  3650,  5348, /*angle*/ 0, 0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvStaticObject),
    OBJECT(/*model*/ MODEL_TTM_MOON_SMILEY,   /*pos*/ -8477,   300, -7122, /*angle*/ 0, 0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvStaticObject),
    OBJECT(/*model*/ MODEL_TTM_MOON_SMILEY,   /*pos*/  6160, -6640,  7861, /*angle*/ 0, 0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvStaticObject),
    RETURN(),
};

static const LevelScript script_func_local_area_4[] = {
    OBJECT(/*model*/ MODEL_TTM_YELLOW_SMILEY, /*pos*/  5157,  1474, -8292, /*angle*/ 0, 0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvStaticObject),
    OBJECT(/*model*/ MODEL_TTM_STAR_SMILEY,   /*pos*/ 11106,  2200,   381, /*angle*/ 0, 0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvStaticObject),
    OBJECT(/*model*/ MODEL_TTM_MOON_SMILEY,   /*pos*/    37,  1474, -1124, /*angle*/ 0, 0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvStaticObject),
    RETURN(),
};

const LevelScript level_ccm_entry[] = {
    INIT_LEVEL(),
    LOAD_MIO0        (/*seg*/ 0x07, _ccm_segment_7SegmentRomStart, _ccm_segment_7SegmentRomEnd),
    LOAD_MIO0_TEXTURE(/*seg*/ 0x09, _slide_mio0SegmentRomStart, _slide_mio0SegmentRomEnd),
    LOAD_MIO0        (/*seg*/ 0x0B, _effect_mio0SegmentRomStart, _effect_mio0SegmentRomEnd),
    LOAD_MIO0        (/*seg*/ 0x0A, _ccm_skybox_mio0SegmentRomStart, _ccm_skybox_mio0SegmentRomEnd),
    LOAD_MIO0        (/*seg*/ 0x05, _group7_mio0SegmentRomStart, _group7_mio0SegmentRomEnd),
    LOAD_RAW         (/*seg*/ 0x0C, _group7_geoSegmentRomStart,  _group7_geoSegmentRomEnd),
    LOAD_MIO0        (/*seg*/ 0x08, _common0_mio0SegmentRomStart, _common0_mio0SegmentRomEnd),
    LOAD_RAW         (/*seg*/ 0x0F, _common0_geoSegmentRomStart,  _common0_geoSegmentRomEnd),
    ALLOC_LEVEL_POOL(),
    MARIO(/*model*/ MODEL_MARIO, /*bhvParam*/ BPARAM4(0x01), /*bhv*/ bhvMario),
    JUMP_LINK(script_func_global_1),
    JUMP_LINK(script_func_global_8),
    LOAD_MODEL_FROM_GEO(MODEL_TTM_SLIDE_EXIT_PODIUM, ccm_geo_000DF4),
    LOAD_MODEL_FROM_GEO(MODEL_TTM_BLUE_SMILEY,       ccm_geo_000D14),
    LOAD_MODEL_FROM_GEO(MODEL_TTM_YELLOW_SMILEY,     ccm_geo_000D4C),
    LOAD_MODEL_FROM_GEO(MODEL_TTM_STAR_SMILEY,       ccm_geo_000D84),
    LOAD_MODEL_FROM_GEO(MODEL_TTM_MOON_SMILEY,       ccm_geo_000DBC),

    AREA(/*index*/ 1, snow_slider_geo),
        OBJECT(/*model*/ MODEL_NONE, /*pos*/  1846,  5420, -1641, /*angle*/ 0,   0, 0, /*bhvParam*/ 0x000A0000, /*bhv*/ bhvSpinAirborneWarp),
        OBJECT(/*model*/ MODEL_NONE, /*pos*/  7987, -3993,  4638, /*angle*/ 0,   0, 0, /*bhvParam*/ 0x00140000, /*bhv*/ bhvWarp),
        OBJECT(/*model*/ MODEL_NONE, /*pos*/  2868,  4220,  -204, /*angle*/ 0, -90, 0, /*bhvParam*/ 0x0F1E0000, /*bhv*/ bhvWarp),
        WARP_NODE(/*id*/ WARP_NODE_0A,      /*destLevel*/ LEVEL_CCM,    /*destArea*/ 1, /*destNode*/ WARP_NODE_0A, /*flags*/ WARP_NO_CHECKPOINT),
        WARP_NODE(/*id*/ WARP_NODE_14,      /*destLevel*/ LEVEL_CCM,    /*destArea*/ 1, /*destNode*/ WARP_NODE_1E, /*flags*/ WARP_NO_CHECKPOINT),
        WARP_NODE(/*id*/ WARP_NODE_1E,      /*destLevel*/ LEVEL_CCM,    /*destArea*/ 2, /*destNode*/ WARP_NODE_0A, /*flags*/ WARP_NO_CHECKPOINT),
        WARP_NODE(/*id*/ WARP_NODE_1F,      /*destLevel*/ LEVEL_CCM,    /*destArea*/ 1, /*destNode*/ WARP_NODE_20, /*flags*/ WARP_NO_CHECKPOINT),
        WARP_NODE(/*id*/ WARP_NODE_20,      /*destLevel*/ LEVEL_CCM,    /*destArea*/ 1, /*destNode*/ WARP_NODE_1F, /*flags*/ WARP_NO_CHECKPOINT),
        WARP_NODE(/*id*/ WARP_NODE_SUCCESS, /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 1, /*destNode*/ WARP_NODE_32, /*flags*/ WARP_NO_CHECKPOINT),
        WARP_NODE(/*id*/ WARP_NODE_DEATH,   /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 1, /*destNode*/ WARP_NODE_64, /*flags*/ WARP_NO_CHECKPOINT),
        JUMP_LINK(script_func_local_1),
        JUMP_LINK(script_func_local_2),
        TERRAIN(/*terrainData*/ snow_slider_collision),
        SHOW_DIALOG(/*index*/ 0x00, DIALOG_048),
        SET_BACKGROUND_MUSIC(/*settingsPreset*/ 0x0000, /*seq*/ SEQ_LEVEL_GRASS),
        TERRAIN_TYPE(/*terrainType*/ TERRAIN_SNOW),
    END_AREA(),

    AREA(/*index*/ 2, ccm_geo_000B5C),
        OBJECT(/*model*/ MODEL_NONE, /*pos*/ 7014, 5381, 6761, /*angle*/ 0, 225, 0, /*bhvParam*/ 0x000A0000, /*bhv*/ bhvAirborneWarp),
        WARP_NODE(/*id*/ WARP_NODE_0A,      /*destLevel*/ LEVEL_CCM,    /*destArea*/ 2, /*destNode*/ WARP_NODE_0A, /*flags*/ WARP_NO_CHECKPOINT),
        WARP_NODE(/*id*/ WARP_NODE_SUCCESS, /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 1, /*destNode*/ WARP_NODE_32, /*flags*/ WARP_NO_CHECKPOINT),
        WARP_NODE(/*id*/ WARP_NODE_DEATH,   /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 1, /*destNode*/ WARP_NODE_64, /*flags*/ WARP_NO_CHECKPOINT),
        JUMP_LINK(script_func_local_area_2),
        TERRAIN(/*terrainData*/ ccm_seg7_area_2_collision),
        MACRO_OBJECTS(/*objList*/ ccm_seg7_area_2_macro_objs),
        INSTANT_WARP(/*index*/ 2, /*destArea*/ 3, /*displace*/ 10240, 7168, 10240),
        SET_BACKGROUND_MUSIC(/*settingsPreset*/ 0x0000, /*seq*/ SEQ_LEVEL_GRASS),
        TERRAIN_TYPE(/*terrainType*/ TERRAIN_SLIDE),
    END_AREA(),

    AREA(/*index*/ 3, ccm_geo_000BEC),
        WARP_NODE(/*id*/ 0xF0, /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 0x01, /*destNode*/ 0x32, /*flags*/ WARP_NO_CHECKPOINT),
        WARP_NODE(/*id*/ 0xF1, /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 0x01, /*destNode*/ 0x64, /*flags*/ WARP_NO_CHECKPOINT),
        JUMP_LINK(script_func_local_area_3),
        TERRAIN(/*terrainData*/ ccm_seg7_area_3_collision),
        MACRO_OBJECTS(/*objList*/ ccm_seg7_area_3_macro_objs),
        INSTANT_WARP(/*index*/ 3, /*destArea*/ 4, /*displace*/ -11264, 13312, 3072),
        SET_BACKGROUND_MUSIC(/*settingsPreset*/ 0x0000, /*seq*/ SEQ_LEVEL_GRASS),
        TERRAIN_TYPE(/*terrainType*/ TERRAIN_SLIDE),
    END_AREA(),

    AREA(/*index*/ 4, ccm_geo_000C84),
        OBJECT(/*model*/ MODEL_TTM_SLIDE_EXIT_PODIUM, /*pos*/ -7285, -1866, -4812, /*angle*/ 0, 0, 0, /*bhvParam*/ 0x000A0000, /*bhv*/ bhvExitPodiumWarp),
        WARP_NODE(/*id*/ 0x0A, /*destLevel*/ LEVEL_CCM, /*destArea*/ 0x01, /*destNode*/ 0x14, /*flags*/ WARP_NO_CHECKPOINT),
        WARP_NODE(/*id*/ 0xF0, /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 0x01, /*destNode*/ 0x32, /*flags*/ WARP_NO_CHECKPOINT),
        WARP_NODE(/*id*/ 0xF1, /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 0x01, /*destNode*/ 0x64, /*flags*/ WARP_NO_CHECKPOINT),
        JUMP_LINK(script_func_local_area_4),
        TERRAIN(/*terrainData*/ ccm_seg7_area_4_collision),
        MACRO_OBJECTS(/*objList*/ ccm_seg7_area_4_macro_objs),
        SET_BACKGROUND_MUSIC(/*settingsPreset*/ 0x0000, /*seq*/ SEQ_LEVEL_GRASS),
        TERRAIN_TYPE(/*terrainType*/ TERRAIN_SLIDE),
    END_AREA(),

    FREE_LEVEL_POOL(),
     MARIO_POS(/*area*/ 1, /*yaw*/ 0, /*pos*/  1846,  4250, -1641),
    CALL(/*arg*/ 0, /*func*/ lvl_init_or_update),
    CALL_LOOP(/*arg*/ 1, /*func*/ lvl_init_or_update),
    CLEAR_LEVEL(),
    SLEEP_BEFORE_EXIT(/*frames*/ 1),
    EXIT(),
};
