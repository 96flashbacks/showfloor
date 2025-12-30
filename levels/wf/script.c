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
#include "levels/wf/header.h"

// Unlike most levels, some of the level geometry objects in WF are defined as regular objects instead of terrain objects.
// Some of them are still terrain objects, however.
static const LevelScript script_func_local_1[] = {
    OBJECT(/*model*/ MODEL_LEVEL_GEOMETRY_03, /*pos*/  2305, 2432,  -255, /*angle*/ 0, 0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvStaticObject),
    OBJECT(/*model*/ MODEL_LEVEL_GEOMETRY_04, /*pos*/  3405, 1664, -1791, /*angle*/ 0, 0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvStaticObject),
    OBJECT(/*model*/ MODEL_LEVEL_GEOMETRY_05, /*pos*/  3840,    0, -2303, /*angle*/ 0, 0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvStaticObject),
    OBJECT(/*model*/ MODEL_LEVEL_GEOMETRY_05, /*pos*/  3840,    0, -1279, /*angle*/ 0, 0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvStaticObject),
    OBJECT(/*model*/ MODEL_LEVEL_GEOMETRY_06, /*pos*/     0,    0,     0, /*angle*/ 0, 0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvStaticObject),
    OBJECT(/*model*/ MODEL_LEVEL_GEOMETRY_07, /*pos*/     0, 3450, -2800, /*angle*/ 0, 0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvStaticObject),
    OBJECT(/*model*/ MODEL_LEVEL_GEOMETRY_0A, /*pos*/  3840,  794,  2688, /*angle*/ 0, 0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvStaticObject),
    OBJECT(/*model*/ MODEL_LEVEL_GEOMETRY_0C, /*pos*/  1408, 2522,  2431, /*angle*/ 0, 0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvStaticObject),
    OBJECT(/*model*/ MODEL_WF_GIANT_POLE,     /*pos*/ -1500, 2560,  2512, /*angle*/ 0, 0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvGiantPole),
    OBJECT(/*model*/ MODEL_WF_GIANT_POLE,     /*pos*/  1128, 2500, -1024, /*angle*/ 0, 0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvGiantPole),
    RETURN(),
};

static const LevelScript script_func_local_2[] = {
    OBJECT(/*model*/ MODEL_WF_SMALL_BOMP,               /*pos*/  3300, 1070,     1, /*angle*/ 0,  90, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvSmallBomp),
    OBJECT(/*model*/ MODEL_WF_SMALL_BOMP,               /*pos*/  3300, 1070,  1281, /*angle*/ 0,  90, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvSmallBomp),
    //OBJECT(/*model*/ MODEL_WF_LARGE_BOMP,               /*pos*/  3300, 1070,   641, /*angle*/ 0,   0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvLargeBomp),
    OBJECT(/*model*/ MODEL_WF_ROTATING_WOODEN_PLATFORM, /*pos*/  -255, 2560,  2304, /*angle*/ 0,   0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvWFRotatingWoodenPlatform),
    OBJECT(/*model*/ MODEL_WF_SLIDING_PLATFORM,         /*pos*/  3328, 1075, -1791, /*angle*/ 0,  90, 0, /*bhvParam*/ 0x00020000, /*bhv*/ bhvWFSlidingPlatform),
    OBJECT(/*model*/ MODEL_WF_SLIDING_PLATFORM,         /*pos*/  3328, 1075,  -767, /*angle*/ 0,  90, 0, /*bhvParam*/ 0x00010000, /*bhv*/ bhvWFSlidingPlatform),
    OBJECT(/*model*/ MODEL_WF_SLIDING_PLATFORM,         /*pos*/  3328, 1075, -2815, /*angle*/ 0,  90, 0, /*bhvParam*/ 0x00030000, /*bhv*/ bhvWFSlidingPlatform),
    OBJECT(/*model*/ MODEL_WF_TUMBLING_BRIDGE,          /*pos*/  1792, 2496,  1600, /*angle*/ 0,   0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvTumblingBridge),
    OBJECT(/*model*/ MODEL_NONE,                        /*pos*/ -2495, 1331,  -256, /*angle*/ 0,   0, 0, /*bhvParam*/ 0x003D0000, /*bhv*/ bhvPoleGrabbing),
    RETURN(),
};

static const LevelScript script_func_local_3[] = {
    OBJECT(/*model*/ MODEL_THWOMP,            /*pos*/  3562, 1939, -1500, /*angle*/ 0,   90, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvThwomp),
    OBJECT(/*model*/ MODEL_THWOMP,            /*pos*/  3530, 1075, -3320, /*angle*/ 0,  180, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvThwomp2),
    OBJECT(/*model*/ MODEL_NONE,              /*pos*/  -856,  922,  3819, /*angle*/ 0,    0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvBetaFishSplashSpawner),
    OBJECT(/*model*/ MODEL_PIRANHA_PLANT,     /*pos*/  1822, 2560,  -101, /*angle*/ 0,   90, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvPiranhaPlant),
    OBJECT(/*model*/ MODEL_WHOMP,             /*pos*/ -1985, 2560,   604, /*angle*/ 0,    0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvSmallWhomp),
    OBJECT(/*model*/ MODEL_BULLET_BILL,       /*pos*/  1280, 3712,   968, /*angle*/ 0,  180, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvBulletBill),
    OBJECT(/*model*/ MODEL_LEVEL_GEOMETRY_08, /*pos*/     0, 3584,     0, /*angle*/ 0,    0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvTower),
    OBJECT(/*model*/ MODEL_LEVEL_GEOMETRY_09, /*pos*/  1280, 3584,   896, /*angle*/ 0,    0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvBulletBillCannon),
    OBJECT(/*model*/ MODEL_NONE,              /*pos*/     0, 3483,     0, /*angle*/ 0,    0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvTowerPlatformGroup),
    OBJECT(/*model*/ MODEL_HOOT,              /*pos*/  2560,  700,  4608, /*angle*/ 0,    0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvHoot),
    RETURN(),
};

static const LevelScript script_func_local_4[] = {
    OBJECT(/*model*/ MODEL_STAR,  /*pos*/   300,  5550,     0, /*angle*/ 0, 0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvStar),
    OBJECT(/*model*/ MODEL_STAR,  /*pos*/     0,  3750, -2800, /*angle*/ 0, 0, 0, /*bhvParam*/ 0x01000000, /*bhv*/ bhvStar),
    RETURN(),
};

const LevelScript level_wf_entry[] = {
    INIT_LEVEL(),
    LOAD_MIO0        (/*seg*/ 0x07, _wf_segment_7SegmentRomStart, _wf_segment_7SegmentRomEnd),
    LOAD_MIO0        (/*seg*/ 0x0A, _cloud_floor_skybox_mio0SegmentRomStart, _cloud_floor_skybox_mio0SegmentRomEnd),
    LOAD_MIO0_TEXTURE(/*seg*/ 0x09, _generic_mio0SegmentRomStart, _generic_mio0SegmentRomEnd),
    LOAD_MIO0        (/*seg*/ 0x05, _group1_mio0SegmentRomStart, _group1_mio0SegmentRomEnd),
    LOAD_RAW         (/*seg*/ 0x0C, _group1_geoSegmentRomStart,  _group1_geoSegmentRomEnd),
    LOAD_MIO0        (/*seg*/ 0x06, _group14_mio0SegmentRomStart, _group14_mio0SegmentRomEnd),
    LOAD_RAW         (/*seg*/ 0x0D, _group14_geoSegmentRomStart,  _group14_geoSegmentRomEnd),
    LOAD_MIO0        (/*seg*/ 0x08, _common0_mio0SegmentRomStart, _common0_mio0SegmentRomEnd),
    LOAD_RAW         (/*seg*/ 0x0F, _common0_geoSegmentRomStart,  _common0_geoSegmentRomEnd),
    ALLOC_LEVEL_POOL(),
    MARIO(/*model*/ MODEL_MARIO, /*bhvParam*/ BPARAM4(0x01), /*bhv*/ bhvMario),
    JUMP_LINK(script_func_global_1),
    JUMP_LINK(script_func_global_2),
    JUMP_LINK(script_func_global_15),
    LOAD_MODEL_FROM_GEO(MODEL_BUBBLY_TREE,                      RCP_HmsMainTree),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_03,                wf_geo_5),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_04,                wf_geo_6),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_05,                wf_geo_7),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_06,                wf_geo_8),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_07,                wf_geo_9),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_08,                wf_geo_10),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_09,                wf_geo_0008E8),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_0A,                wf_geo_12),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_0C,                wf_geo_13),
    LOAD_MODEL_FROM_GEO(MODEL_WF_GIANT_POLE,                    wf_geo_000AE0),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_0E,                wf_geo_15),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_0F,                wf_geo_16),
    //LOAD_MODEL_FROM_GEO(MODEL_WF_ROTATING_PLATFORM,             wf_geo_0009B8),
    LOAD_MODEL_FROM_GEO(MODEL_WF_SMALL_BOMP,                    wf_geo_000A00),
    LOAD_MODEL_FROM_GEO(MODEL_WF_LARGE_BOMP,                    wf_geo_000A40),
    LOAD_MODEL_FROM_GEO(MODEL_WF_ROTATING_WOODEN_PLATFORM,      wf_geo_000A58),
    LOAD_MODEL_FROM_GEO(MODEL_WF_SLIDING_PLATFORM,              wf_geo_000A98),
    LOAD_MODEL_FROM_GEO(MODEL_WF_TUMBLING_BRIDGE_PART,          wf_geo_000AB0),
    LOAD_MODEL_FROM_GEO(MODEL_WF_TUMBLING_BRIDGE,               wf_geo_000AC8),
    LOAD_MODEL_FROM_GEO(MODEL_WF_TOWER_TRAPEZOID_PLATORM,       wf_geo_000AF8),
    LOAD_MODEL_FROM_GEO(MODEL_WF_TOWER_SQUARE_PLATORM,          wf_geo_000B10),
    LOAD_MODEL_FROM_GEO(MODEL_WF_TOWER_SQUARE_PLATORM_UNUSED,   wf_geo_000AF8),
    LOAD_MODEL_FROM_GEO(MODEL_WF_TOWER_SQUARE_PLATORM_ELEVATOR, wf_geo_000B60),

    AREA(/*index*/ 1, mountain_geo),
        OBJECT(/*model*/ MODEL_NONE, /*pos*/  2600, 1256,  5120, /*angle*/ 0, 100, 0, /*bhvParam*/ BPARAM2(WARP_NODE_0A), /*bhv*/ bhvSpinAirborneWarp),
        WARP_NODE(/*id*/ WARP_NODE_0A,      /*destLevel*/ LEVEL_WF,     /*destArea*/ 1, /*destNode*/ WARP_NODE_0A, /*flags*/ WARP_NO_CHECKPOINT),
        WARP_NODE(/*id*/ WARP_NODE_0B,      /*destLevel*/ LEVEL_WF,     /*destArea*/ 1, /*destNode*/ WARP_NODE_0C, /*flags*/ WARP_NO_CHECKPOINT),
        WARP_NODE(/*id*/ WARP_NODE_0C,      /*destLevel*/ LEVEL_WF,     /*destArea*/ 1, /*destNode*/ WARP_NODE_0B, /*flags*/ WARP_NO_CHECKPOINT),
        WARP_NODE(/*id*/ WARP_NODE_SUCCESS, /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 1, /*destNode*/ WARP_NODE_33, /*flags*/ WARP_NO_CHECKPOINT),
        WARP_NODE(/*id*/ WARP_NODE_DEATH,   /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 1, /*destNode*/ WARP_NODE_65, /*flags*/ WARP_NO_CHECKPOINT),
        JUMP_LINK(script_func_local_1),
        JUMP_LINK(script_func_local_2),
        JUMP_LINK(script_func_local_3),
        JUMP_LINK(script_func_local_4),
        TERRAIN(/*terrainData*/ mountain_collision),
        MACRO_OBJECTS(/*objList*/ mountain_macro),
        SHOW_DIALOG(/*index*/ 0x00, DIALOG_030),
        SET_BACKGROUND_MUSIC(/*settingsPreset*/ 0x0005, /*seq*/ SEQ_LEVEL_GRASS),
        TERRAIN_TYPE(/*terrainType*/ TERRAIN_STONE),
    END_AREA(),

    FREE_LEVEL_POOL(),
    MARIO_POS(/*area*/ 1, /*yaw*/ 90, /*pos*/ 2600, 256, 5120),
    CALL(/*arg*/ 0, /*func*/ lvl_init_or_update),
    CALL_LOOP(/*arg*/ 1, /*func*/ lvl_init_or_update),
    CLEAR_LEVEL(),
    SLEEP_BEFORE_EXIT(/*frames*/ 1),
    EXIT(),
};
