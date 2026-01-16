#include <ultra64.h>
#include "sm64.h"
#include "behavior_data.h"
#include "model_ids.h"
#include "seq_ids.h"
#include "segment_symbols.h"
#include "level_commands.h"

#include "game/level_update.h"

#include "levels/scripts.h"

#include "actors/common1.h"

#include "make_const_nonconst.h"
#include "levels/thi/header.h"

static const LevelScript script_func_local_4[] = {
    OBJECT(/*model*/ MODEL_STAR,  /*pos*/   400, 4300, -1200,  /*angle*/ 0, 0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvStar),
    OBJECT(/*model*/ MODEL_STAR,  /*pos*/   -1900, -900, 6800, /*angle*/ 0, 0, 0, /*bhvParam*/ 0x01000000, /*bhv*/ bhvStar),
    OBJECT(/*model*/ MODEL_CHUCKYA,  /*pos*/   -1908, 2202, -595, /*angle*/ 0, 0, 0, /*bhvParam*/ 0x01000000, /*bhv*/ bhvChuckya),
    OBJECT(/*model*/ MODEL_HANA,  /*pos*/   -6193, -2969, 6121, /*angle*/ 0, 0, 0, /*bhvParam*/ 0x01000000, /*bhv*/ bhvHana),
    RETURN(),
};

const LevelScript level_thi_entry[] = {
    INIT_LEVEL(),
    LOAD_MIO0        (/*seg*/ 0x07, _thi_segment_7SegmentRomStart, _thi_segment_7SegmentRomEnd),
    LOAD_MIO0_TEXTURE(/*seg*/ 0x09, _grass_mio0SegmentRomStart, _grass_mio0SegmentRomEnd),
    LOAD_MIO0        (/*seg*/ 0x0A, _bbh_skybox_mio0SegmentRomStart, _bbh_skybox_mio0SegmentRomEnd),
    //LOAD_MIO0        (/*seg*/ 0x05, _group11_mio0SegmentRomStart, _group11_mio0SegmentRomEnd),
    //LOAD_RAW         (/*seg*/ 0x0C, _group11_geoSegmentRomStart,  _group11_geoSegmentRomEnd),
    LOAD_MIO0        (/*seg*/ 0x06, _group14_mio0SegmentRomStart, _group14_mio0SegmentRomEnd),
    LOAD_RAW         (/*seg*/ 0x0D, _group14_geoSegmentRomStart,  _group14_geoSegmentRomEnd),
    LOAD_MIO0        (/*seg*/ 0x08, _common0_mio0SegmentRomStart, _common0_mio0SegmentRomEnd),
    LOAD_RAW         (/*seg*/ 0x0F, _common0_geoSegmentRomStart,  _common0_geoSegmentRomEnd),
    ALLOC_LEVEL_POOL(),
    MARIO(/*model*/ MODEL_MARIO, /*bhvParam*/ BPARAM4(0x01), /*bhv*/ bhvMario),
    JUMP_LINK(script_func_global_1),
    LOAD_MODEL_FROM_GEO(MODEL_HANA,      RCP_HmsEnemyhana),
    JUMP_LINK(script_func_local_4),
    //JUMP_LINK(script_func_global_15),
    //LOAD_MODEL_FROM_GEO(MODEL_THI_BUBBLY_TREE,     bubbly_tree_geo),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_03,   thi_geo_0005F0),
    //LOAD_MODEL_FROM_GEO(MODEL_THI_WARP_PIPE,       warp_pipe_geo),
    LOAD_MODEL_FROM_GEO(MODEL_THI_HUGE_ISLAND_TOP, thi_geo_0005B0),
    LOAD_MODEL_FROM_GEO(MODEL_THI_TINY_ISLAND_TOP, thi_geo_0005C8),

    AREA(/*index*/ 1, thi_geo_000608),
        OBJECT(/*model*/ MODEL_NONE, /*pos*/ -7372, -2969,  7373, /*angle*/ 0, 106, 0, /*bhvParam*/ BPARAM2(WARP_NODE_0A), /*bhv*/ bhvSpinAirborneWarp),
        OBJECT(/*model*/ MODEL_NONE, /*pos*/   410,  -512,   922, /*angle*/ 0,   0, 0, /*bhvParam*/ BPARAM2(WARP_NODE_0B), /*bhv*/ bhvInstantActiveWarp),
        OBJECT(/*model*/ MODEL_NONE, /*pos*/   410,  -512,   717, /*angle*/ 0,   0, 0, /*bhvParam*/ BPARAM1(5)  | BPARAM2(WARP_NODE_0C), /*bhv*/ bhvWarp),
        OBJECT(/*model*/ MODEL_NONE, /*pos*/     0,  3170, -1570, /*angle*/ 0,   0, 0, /*bhvParam*/ BPARAM1(10) | BPARAM2(WARP_NODE_0D), /*bhv*/ bhvWarp),
        WARP_NODE(/*id*/ WARP_NODE_0A,      /*destLevel*/ LEVEL_THI,    /*destArea*/ 1, /*destNode*/ WARP_NODE_0A, /*flags*/ WARP_NO_CHECKPOINT),
        WARP_NODE(/*id*/ WARP_NODE_0B,      /*destLevel*/ LEVEL_THI,    /*destArea*/ 1, /*destNode*/ WARP_NODE_0B, /*flags*/ WARP_NO_CHECKPOINT),
        WARP_NODE(/*id*/ WARP_NODE_0C,      /*destLevel*/ LEVEL_THI,    /*destArea*/ 3, /*destNode*/ WARP_NODE_0A, /*flags*/ WARP_NO_CHECKPOINT),
        WARP_NODE(/*id*/ WARP_NODE_0D,      /*destLevel*/ LEVEL_THI,    /*destArea*/ 3, /*destNode*/ WARP_NODE_0B, /*flags*/ WARP_NO_CHECKPOINT),
        WARP_NODE(/*id*/ WARP_NODE_SUCCESS, /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 2, /*destNode*/ WARP_NODE_37, /*flags*/ WARP_NO_CHECKPOINT),
        WARP_NODE(/*id*/ WARP_NODE_DEATH,   /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 2, /*destNode*/ WARP_NODE_69, /*flags*/ WARP_NO_CHECKPOINT),
        /*JUMP_LINK(script_func_local_7),
        JUMP_LINK(script_func_local_1),
        JUMP_LINK(script_func_local_5),*/
        JUMP_LINK(script_func_local_4),
        TERRAIN(/*terrainData*/ big_world_collision),
        MACRO_OBJECTS(/*objList*/ thi_seg7_area_1_macro_objs),
        SET_BACKGROUND_MUSIC(/*settingsPreset*/ 0x0000, /*seq*/ SEQ_LEVEL_GRASS),
        TERRAIN_TYPE(/*terrainType*/ TERRAIN_GRASS),
    END_AREA(),

    FREE_LEVEL_POOL(),
    MARIO_POS(/*area*/ 1, /*yaw*/ 106, /*pos*/ -7372, -2969,  7373),
    CALL(/*arg*/ 0, /*func*/ lvl_init_or_update),
    CALL_LOOP(/*arg*/ 1, /*func*/ lvl_init_or_update),
    CLEAR_LEVEL(),
    SLEEP_BEFORE_EXIT(/*frames*/ 1),
    EXIT(),
};
