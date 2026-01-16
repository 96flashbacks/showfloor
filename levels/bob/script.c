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
#include "actors/common0.h"

#include "make_const_nonconst.h"
#include "levels/bob/header.h"

static const LevelScript script_func_local_1[] = {
    //OBJECT(/*model*/ MODEL_BOB_CHAIN_CHOMP_GATE,  /*pos*/  1456,   768,   446, /*angle*/ 0, 326, 0,  /*behParam*/ 0x00000000, /*beh*/ bhvChainChompGate),
    //OBJECT(/*model*/ MODEL_BOB_SEESAW_PLATFORM,   /*pos*/ -2303,   717,  1024, /*angle*/ 0, 45, 0,   /*behParam*/ 0x00030000, /*beh*/ bhvSeesawPlatform),
    //OBJECT(/*model*/ MODEL_NONE,                  /*pos*/ -2050,     0, -3069, /*angle*/ 0, 25, 0,   /*behParam*/ 0x00000000, /*beh*/ bhvOpenableGrill),
    //OBJECT(/*model*/ MODEL_PURPLE_SWITCH,         /*pos*/ -2283,     0, -3682, /*angle*/ 0, 27, 0,   /*behParam*/ 0x00000000, /*beh*/ bhvFloorSwitchGrills),
    //OBJECT(/*model*/ MODEL_CHECKERBOARD_PLATFORM, /*pos*/  1612,   300,  4611, /*angle*/ 0, 0, 0,    /*behParam*/ 0x00280000, /*beh*/ bhvCheckerboardElevatorGroup),
    RETURN(),
};

static const LevelScript script_func_local_2[] = {
    //OBJECT_WITH_ACTS(/*model*/ MODEL_NONE,                  /*pos*/  1535, 3840, -5561, /*angle*/ 0, 0, 0,   /*behParam*/ 0x00000000, /*beh*/ bhvBoBBowlingBallSpawner,  /*acts*/ ACT_1 | ACT_2),
    //OBJECT_WITH_ACTS(/*model*/ MODEL_NONE,                  /*pos*/  1535, 3840, -5561, /*angle*/ 0, 0, 0,   /*behParam*/ 0x00000000, /*beh*/ bhvTTMBowlingBallSpawner,  /*acts*/ ACT_3 | ACT_4 | ACT_5 | ACT_6),
    //OBJECT_WITH_ACTS(/*model*/ MODEL_NONE,                  /*pos*/   524, 2825, -5400, /*angle*/ 0, 0, 0,   /*behParam*/ 0x00020000, /*beh*/ bhvBoBBowlingBallSpawner,  /*acts*/ ACT_1 | ACT_2),
    //OBJECT_WITH_ACTS(/*model*/ MODEL_NONE,                  /*pos*/   524, 2825, -5400, /*angle*/ 0, 0, 0,   /*behParam*/ 0x00020000, /*beh*/ bhvTTMBowlingBallSpawner,  /*acts*/ ACT_3 | ACT_4 | ACT_5 | ACT_6),
    //OBJECT          (/*model*/ MODEL_BOWLING_BALL,          /*pos*/  -993,  886, -3565, /*angle*/ 0, 0, 0,   /*behParam*/ 0x00000000, /*beh*/ bhvPitBowlingBall),
    //OBJECT          (/*model*/ MODEL_BOWLING_BALL,          /*pos*/  -785,  886, -4301, /*angle*/ 0, 0, 0,   /*behParam*/ 0x00000000, /*beh*/ bhvPitBowlingBall),
    //OBJECT_WITH_ACTS(/*model*/ MODEL_BOWLING_BALL,          /*pos*/   -93,  886, -3414, /*angle*/ 0, 0, 0,   /*behParam*/ 0x00000000, /*beh*/ bhvPitBowlingBall,         /*acts*/ ALL_ACTS),
    //OBJECT_WITH_ACTS(/*model*/ MODEL_BOBOMB_BUDDY,          /*pos*/ -4554,    0,  4994, /*angle*/ 0, 0, 0,   /*behParam*/ DIALOG_002 << 16, /*beh*/ bhvBobombBuddy,            /*acts*/ ACT_1),
    //OBJECT_WITH_ACTS(/*model*/ MODEL_BOBOMB_BUDDY,          /*pos*/ -4554,    0,  4994, /*angle*/ 0, 0, 0,   /*behParam*/ 0x00000000,       /*beh*/ bhvBobombBuddyOpensCannon, /*acts*/ ACT_2 | ACT_3 | ACT_4 | ACT_5 | ACT_6),
    //OBJECT_WITH_ACTS(/*model*/ MODEL_CANNON_BASE,           /*pos*/ -5694,  128,  5600, /*angle*/ 0, 135, 0, /*behParam*/ 0x00010000, /*beh*/ bhvWaterBombCannon,        /*acts*/ ACT_1),
    //OBJECT_WITH_ACTS(/*model*/ MODEL_DL_CANNON_LID,         /*pos*/ -5694,  128,  5600, /*angle*/ 0, 180, 0, /*behParam*/ 0x00000000, /*beh*/ bhvCannonClosed,           /*acts*/ ACT_1 | ACT_2 | ACT_3 | ACT_4 | ACT_5 | ACT_6),
    //OBJECT_WITH_ACTS(/*model*/ MODEL_NONE,                  /*pos*/  3217, 4242, -5136, /*angle*/ 0, 0, 0,   /*behParam*/ 0x00000000, /*beh*/ bhvKoopaRaceEndpoint,      /*acts*/ ALL_ACTS),
    OBJECT(/*model*/ MODEL_KOOPA_WITH_SHELL,      /*pos*/  4228,  816,  5869, /*angle*/ 0, 0, 0,   /*behParam*/ 0x00010000, /*beh*/ bhvKoopa),
    RETURN(),
};

static const LevelScript script_func_local_3[] = {
    //OBJECT_WITH_ACTS(/*model*/ MODEL_KING_BOBOMB,           /*pos*/  1636, 4242, -5567, /*angle*/ 0, -147, 0, /*behParam*/ 0x00000000, /*beh*/ bhvKingBobomb,            /*acts*/ ACT_1),
    OBJECT(/*model*/ MODEL_STAR,                  /*pos*/ 1161,  4414,  -101, /*angle*/ 0, 0, 0,    /*behParam*/ 0x01000000, /*beh*/ bhvStar),
    //OBJECT_WITH_ACTS(/*model*/ MODEL_KOOPA_WITH_SHELL,      /*pos*/ -3000,  121,  5300, /*angle*/ 0, 0, 0,    /*behParam*/ 0x02020000, /*beh*/ bhvKoopa),
    //OBJECT_WITH_ACTS(/*model*/ MODEL_NONE,                  /*pos*/  -600, 4400,   600, /*angle*/ 0, 0, 0,    /*behParam*/ 0x03040000, /*beh*/ bhvHiddenStar,            /*acts*/ ALL_ACTS), 
    //OBJECT_WITH_ACTS(/*model*/ MODEL_STAR,                  /*pos*/  1550, 1200,   300, /*angle*/ 0, 0, 0,    /*behParam*/ 0x04000000, /*beh*/ bhvStar,                  /*acts*/ ALL_ACTS),
    RETURN(),
};

const LevelScript level_bob_entry[] = {
    INIT_LEVEL(),
    LOAD_MIO0(        /*seg*/ 0x07, _bob_segment_7SegmentRomStart, _bob_segment_7SegmentRomEnd),
    LOAD_MIO0_TEXTURE(/*seg*/ 0x09, _generic_mio0SegmentRomStart, _generic_mio0SegmentRomEnd),
    LOAD_MIO0        (/*seg*/ 0x0A, _water_skybox_mio0SegmentRomStart, _water_skybox_mio0SegmentRomEnd),
    //LOAD_MIO0(        /*seg*/ 0x05, _group3_mio0SegmentRomStart, _group3_mio0SegmentRomEnd),
    //LOAD_RAW(         /*seg*/ 0x0C, _group3_geoSegmentRomStart,  _group3_geoSegmentRomEnd),
    LOAD_MIO0(        /*seg*/ 0x06, _group14_mio0SegmentRomStart, _group14_mio0SegmentRomEnd),
    LOAD_RAW(         /*seg*/ 0x0D, _group14_geoSegmentRomStart,  _group14_geoSegmentRomEnd),
    LOAD_MIO0(        /*seg*/ 0x08, _common0_mio0SegmentRomStart, _common0_mio0SegmentRomEnd),
    LOAD_RAW(         /*seg*/ 0x0F, _common0_geoSegmentRomStart,  _common0_geoSegmentRomEnd),
    ALLOC_LEVEL_POOL(),
    MARIO(/*model*/ MODEL_MARIO, /*behParam*/ 0x00000001, /*beh*/ bhvMario),
    JUMP_LINK(script_func_global_1),
    JUMP_LINK(script_func_global_4),
    JUMP_LINK(script_func_global_15),
    LOAD_MODEL_FROM_GEO(MODEL_BUBBLY_TREE,      RCP_HmsMainTree),
    //LOAD_MODEL_FROM_GEO(MODEL_HANA,      RCP_HmsEnemyhana),
    //LOAD_MODEL_FROM_GEO(MODEL_BOB_SEESAW_PLATFORM,  bob_geo_000458),
    //LOAD_MODEL_FROM_GEO(MODEL_BOB_BARS_GRILLS,      bob_geo_000470),

    AREA(/*index*/ 1, bob_geo_000488),
        JUMP_LINK(script_func_local_1),
        JUMP_LINK(script_func_local_2),
        JUMP_LINK(script_func_local_3),
        OBJECT(/*model*/ MODEL_NONE, /*pos*/ -5694,  1128,  5600, /*angle*/ 0, 90, 0,  /*behParam*/ 0x000A0000, /*beh*/ bhvSpinAirborneWarp),
        OBJECT(/*model*/ MODEL_NONE, /*pos*/   583,  2683, -5387, /*angle*/ 0, -154, 0, /*behParam*/ 0x000B0000, /*beh*/ bhvFadingWarp),
        OBJECT(/*model*/ MODEL_NONE, /*pos*/  1680,  3835, -5523, /*angle*/ 0, -153, 0, /*behParam*/ 0x000C0000, /*beh*/ bhvFadingWarp),
        OBJECT(/*model*/ MODEL_NONE, /*pos*/ -6612,  1024, -3351, /*angle*/ 0, 107, 0,  /*behParam*/ 0x000D0000, /*beh*/ bhvFadingWarp),
        OBJECT(/*model*/ MODEL_NONE, /*pos*/  1980,   768,  6618, /*angle*/ 0, -151, 0, /*behParam*/ 0x000E0000, /*beh*/ bhvFadingWarp),
        //OBJECT(/*model*/ MODEL_CHUCKYA,      /*pos*/  3066,  4293,  -6527, /*angle*/ 0, 0, 0,   /*behParam*/ 0x00000000, /*beh*/ bhvChuckya),
        //OBJECT(/*model*/ MODEL_HANA,      /*pos*/  -1900,     0,  3450, /*angle*/ 0, 0, 0,   /*behParam*/ 0x00000000, /*beh*/ bhvHana),
        WARP_NODE(/*id*/ 0x0A, /*destLevel*/ LEVEL_BOB, /*destArea*/ 0x01, /*destNode*/ 0x0A, /*flags*/ WARP_NO_CHECKPOINT),
        WARP_NODE(/*id*/ 0x0B, /*destLevel*/ LEVEL_BOB, /*destArea*/ 0x01, /*destNode*/ 0x0C, /*flags*/ WARP_NO_CHECKPOINT),
        WARP_NODE(/*id*/ 0x0C, /*destLevel*/ LEVEL_BOB, /*destArea*/ 0x01, /*destNode*/ 0x0B, /*flags*/ WARP_NO_CHECKPOINT),
        WARP_NODE(/*id*/ 0x0D, /*destLevel*/ LEVEL_BOB, /*destArea*/ 0x01, /*destNode*/ 0x0E, /*flags*/ WARP_NO_CHECKPOINT),
        WARP_NODE(/*id*/ 0x0E, /*destLevel*/ LEVEL_BOB, /*destArea*/ 0x01, /*destNode*/ 0x0D, /*flags*/ WARP_NO_CHECKPOINT),
        WARP_NODE(/*id*/ 0xF0, /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 0x01, /*destNode*/ 0x32, /*flags*/ WARP_NO_CHECKPOINT),
        WARP_NODE(/*id*/ 0xF1, /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 0x01, /*destNode*/ 0x64, /*flags*/ WARP_NO_CHECKPOINT),
        TERRAIN(/*terrainData*/ bobo_collision),
        MACRO_OBJECTS(/*objList*/ bob_seg7_macro_objs),
        SHOW_DIALOG(/*index*/ 0x00, DIALOG_000),
        SET_BACKGROUND_MUSIC(/*settingsPreset*/ 0x0000, /*seq*/ SEQ_LEVEL_GRASS),
        TERRAIN_TYPE(/*terrainType*/ TERRAIN_GRASS),
    END_AREA(),

    FREE_LEVEL_POOL(),
    MARIO_POS(/*area*/ 1, /*yaw*/ 90, /*pos*/ -5694,  128,  5600),
    CALL(/*arg*/ 0, /*func*/ lvl_init_or_update),
    CALL_LOOP(/*arg*/ 1, /*func*/ lvl_init_or_update),
    CLEAR_LEVEL(),
    SLEEP_BEFORE_EXIT(/*frames*/ 1),
    EXIT(),
};
