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
#include "levels/bbh/header.h"


static const LevelScript script_func_local_1[] = {
  
    RETURN(),
};

static const LevelScript script_func_local_2[] = {
   
    RETURN(),
};

static const LevelScript script_func_local_3[] = {
    OBJECT_WITH_ACTS(/*model*/ MODEL_BOO,                        /*pos*/    20,   100,  -908, /*angle*/ 0, 0, 0,   /*bhvParam*/ 0, /*bhv*/ bhvBoo,                 /*acts*/ ACT_2 | ACT_3 | ACT_4 | ACT_5 | ACT_6),
    OBJECT_WITH_ACTS(/*model*/ MODEL_BOO,                        /*pos*/  3150,   100,   398, /*angle*/ 0, 0, 0,   /*bhvParam*/ 0, /*bhv*/ bhvBoo,                 /*acts*/ ACT_2 | ACT_3 | ACT_4 | ACT_5 | ACT_6),
    OBJECT_WITH_ACTS(/*model*/ MODEL_BOO,                        /*pos*/ -2000,   150,  -800, /*angle*/ 0, 0, 0,   /*bhvParam*/ 0, /*bhv*/ bhvBoo,                 /*acts*/ ACT_2 | ACT_3 | ACT_4 | ACT_5 | ACT_6),
    OBJECT_WITH_ACTS(/*model*/ MODEL_BOO,                        /*pos*/  2851,   100,  2289, /*angle*/ 0, 0, 0,   /*bhvParam*/ 0, /*bhv*/ bhvBoo,                 /*acts*/ ACT_2 | ACT_3 | ACT_4 | ACT_5 | ACT_6),
    OBJECT_WITH_ACTS(/*model*/ MODEL_BOO,                        /*pos*/ -1551,   100, -1018, /*angle*/ 0, 0, 0,   /*bhvParam*/ 0, /*bhv*/ bhvBoo,                 /*acts*/ ACT_2 | ACT_3 | ACT_4 | ACT_5 | ACT_6),
    OBJECT_WITH_ACTS(/*model*/ MODEL_BOO,                        /*pos*/   581,  1850,  -206, /*angle*/ 0, -90, 0, /*bhvParam*/ 0, /*bhv*/ bhvBoo,                 /*acts*/ ALL_ACTS),

    RETURN(),
};

static const LevelScript script_func_local_4[] = {
    OBJECT_WITH_ACTS(/*model*/ MODEL_STAR, /*pos*/ -2030, 1350,  1940, /*angle*/ 0, 0, 0,  /*bhvParam*/ BPARAM1(STAR_INDEX_ACT_3), /*bhv*/ bhvStar,                    /*acts*/ ALL_ACTS),
    //OBJECT_WITH_ACTS(/*model*/ MODEL_NONE, /*pos*/  -204, 1100,  1576, /*angle*/ 0, 0, 0,  /*bhvParam*/ BPARAM1(STAR_INDEX_ACT_4), /*bhv*/ bhvHiddenRedCoinStar,    /*acts*/ ALL_ACTS),
    OBJECT_WITH_ACTS(/*model*/ MODEL_NONE, /*pos*/   923, 1741,  -332, /*angle*/ 0, 18, 0, /*bhvParam*/ BPARAM1(STAR_INDEX_ACT_6) | BPARAM2(0x01), /*bhv*/ bhvMrI,                    /*acts*/ ALL_ACTS),
    RETURN(),
};

const LevelScript level_bbh_entry[] = {
    INIT_LEVEL(),
    LOAD_MIO0        (/*seg*/ 0x07, _bbh_segment_7SegmentRomStart, _bbh_segment_7SegmentRomEnd),
    //LOAD_MIO0        (/*seg*/ 0x0A, _bbh_skybox_mio0SegmentRomStart, _bbh_skybox_mio0SegmentRomEnd),
    LOAD_MIO0_TEXTURE(/*seg*/ 0x09, _spooky_mio0SegmentRomStart, _spooky_mio0SegmentRomEnd),
    LOAD_MIO0        (/*seg*/ 0x05, _group9_mio0SegmentRomStart, _group9_mio0SegmentRomEnd),
    LOAD_RAW         (/*seg*/ 0x0C, _group9_geoSegmentRomStart,  _group9_geoSegmentRomEnd),
    LOAD_MIO0        (/*seg*/ 0x06, _group17_mio0SegmentRomStart, _group17_mio0SegmentRomEnd),
    LOAD_RAW         (/*seg*/ 0x0D, _group17_geoSegmentRomStart, _group17_geoSegmentRomEnd),
    LOAD_MIO0        (/*seg*/ 0x08, _common0_mio0SegmentRomStart, _common0_mio0SegmentRomEnd),
    LOAD_RAW         (/*seg*/ 0x0F, _common0_geoSegmentRomStart,  _common0_geoSegmentRomEnd),
    ALLOC_LEVEL_POOL(),
    MARIO(/*model*/ MODEL_MARIO, /*bhvParam*/ BPARAM4(0x01), /*bhv*/ bhvMario),
    JUMP_LINK(script_func_global_1),
    JUMP_LINK(script_func_global_10),
    JUMP_LINK(script_func_global_18),
    //LOAD_MODEL_FROM_GEO(MODEL_BBH_HAUNTED_DOOR,           haunted_door_geo),
    /*LOAD_MODEL_FROM_GEO(MODEL_BBH_STAIRCASE_STEP,         geo_bbh_0005B0),
    LOAD_MODEL_FROM_GEO(MODEL_BBH_TILTING_FLOOR_PLATFORM, geo_bbh_0005C8),
    LOAD_MODEL_FROM_GEO(MODEL_BBH_TUMBLING_PLATFORM,      geo_bbh_0005E0),
    LOAD_MODEL_FROM_GEO(MODEL_BBH_TUMBLING_PLATFORM_PART, geo_bbh_0005F8),
    LOAD_MODEL_FROM_GEO(MODEL_BBH_MOVING_BOOKSHELF,       geo_bbh_000610),
    LOAD_MODEL_FROM_GEO(MODEL_BBH_MESH_ELEVATOR,          geo_bbh_000628),
    LOAD_MODEL_FROM_GEO(MODEL_BBH_MERRY_GO_ROUND,         geo_bbh_000640),
    LOAD_MODEL_FROM_GEO(MODEL_BBH_WOODEN_TOMB,            geo_bbh_000658),*/

    AREA(/*index*/ 1, geo_bbh_000F00),
        JUMP_LINK(script_func_local_1),
        JUMP_LINK(script_func_local_2),
        JUMP_LINK(script_func_local_3),
        JUMP_LINK(script_func_local_4),
        OBJECT(/*model*/ MODEL_NONE, /*pos*/ 666, 796, 5350, /*angle*/ 0, 180, 0, /*bhvParam*/ BPARAM2(WARP_NODE_0A), /*bhv*/ bhvSpinAirborneWarp),
        WARP_NODE(/*id*/ WARP_NODE_0A,      /*destLevel*/ LEVEL_BBH,              /*destArea*/ 1, /*destNode*/ WARP_NODE_0A, /*flags*/ WARP_NO_CHECKPOINT),
        WARP_NODE(/*id*/ WARP_NODE_SUCCESS, /*destLevel*/ LEVEL_CASTLE_COURTYARD, /*destArea*/ 1, /*destNode*/ WARP_NODE_0A, /*flags*/ WARP_NO_CHECKPOINT),
        WARP_NODE(/*id*/ WARP_NODE_DEATH,   /*destLevel*/ LEVEL_CASTLE_COURTYARD, /*destArea*/ 1, /*destNode*/ WARP_NODE_0B, /*flags*/ WARP_NO_CHECKPOINT),
        TERRAIN(/*terrainData*/ ghost_house_collision),
        MACRO_OBJECTS(/*objList*/ bbh_seg7_macro_objs),
        ROOMS(/*surfaceRooms*/ bbh_seg7_rooms),
        SHOW_DIALOG(/*index*/ 0x00, DIALOG_098),
        SET_BACKGROUND_MUSIC(/*settingsPreset*/ 0x0006, /*seq*/ SEQ_LEVEL_SPOOKY),
        TERRAIN_TYPE(/*terrainType*/ TERRAIN_SPOOKY),
    END_AREA(),

    FREE_LEVEL_POOL(),
    MARIO_POS(/*area*/ 1, /*yaw*/ 180, /*pos*/ 0, 0, 0),
    CALL(/*arg*/ 0, /*func*/ lvl_init_or_update),
    CALL_LOOP(/*arg*/ 1, /*func*/ lvl_init_or_update),
    CLEAR_LEVEL(),
    SLEEP_BEFORE_EXIT(/*frames*/ 1),
    EXIT(),
};
