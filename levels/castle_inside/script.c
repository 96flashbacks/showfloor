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
#include "levels/castle_inside/header.h"

static const LevelScript script_func_local_1[] = { // Ports (Warps)
/* Door warps */
    WARP_NODE(/*id*/ WARP_NODE_00, /*destLevel*/ LEVEL_CASTLE_GROUNDS,   /*destArea*/ 1, /*destNode*/ WARP_NODE_00, /*flags*/ WARP_NO_CHECKPOINT),
    WARP_NODE(/*id*/ WARP_NODE_01, /*destLevel*/ LEVEL_CASTLE_GROUNDS,   /*destArea*/ 1, /*destNode*/ WARP_NODE_01, /*flags*/ WARP_NO_CHECKPOINT),
    WARP_NODE(/*id*/ WARP_NODE_02, /*destLevel*/ LEVEL_CASTLE_COURTYARD, /*destArea*/ 1, /*destNode*/ WARP_NODE_01, /*flags*/ WARP_NO_CHECKPOINT),

/* Level entry warps */
    // Fire Bubble painting warps
    PAINTING_WARP_NODE(/*id*/ WARP_NODE_0C, /*destLevel*/ LEVEL_LLL, /*destArea*/ 1, /*destNode*/ WARP_NODE_0A, /*flags*/ WARP_NO_CHECKPOINT),
    PAINTING_WARP_NODE(/*id*/ WARP_NODE_0D, /*destLevel*/ LEVEL_LLL, /*destArea*/ 1, /*destNode*/ WARP_NODE_0A, /*flags*/ WARP_NO_CHECKPOINT),
    PAINTING_WARP_NODE(/*id*/ WARP_NODE_0E, /*destLevel*/ LEVEL_LLL, /*destArea*/ 1, /*destNode*/ WARP_NODE_0A, /*flags*/ WARP_NO_CHECKPOINT),
    // Snow Slider painting warps
    PAINTING_WARP_NODE(/*id*/ WARP_NODE_03, /*destLevel*/ LEVEL_CCM, /*destArea*/ 1, /*destNode*/ WARP_NODE_0A, /*flags*/ WARP_NO_CHECKPOINT),
    PAINTING_WARP_NODE(/*id*/ WARP_NODE_04, /*destLevel*/ LEVEL_CCM, /*destArea*/ 1, /*destNode*/ WARP_NODE_0A, /*flags*/ WARP_NO_CHECKPOINT),
    PAINTING_WARP_NODE(/*id*/ WARP_NODE_05, /*destLevel*/ LEVEL_CCM, /*destArea*/ 1, /*destNode*/ WARP_NODE_0A, /*flags*/ WARP_NO_CHECKPOINT),
    // Mountain painting warps
    PAINTING_WARP_NODE(/*id*/ WARP_NODE_06, /*destLevel*/ LEVEL_WF, /*destArea*/ 1, /*destNode*/ WARP_NODE_0A, /*flags*/ WARP_NO_CHECKPOINT),
    PAINTING_WARP_NODE(/*id*/ WARP_NODE_07, /*destLevel*/ LEVEL_WF, /*destArea*/ 1, /*destNode*/ WARP_NODE_0A, /*flags*/ WARP_NO_CHECKPOINT),
    PAINTING_WARP_NODE(/*id*/ WARP_NODE_08, /*destLevel*/ LEVEL_WF, /*destArea*/ 1, /*destNode*/ WARP_NODE_0A, /*flags*/ WARP_NO_CHECKPOINT),
    // Water Land painting warps
    PAINTING_WARP_NODE(/*id*/ WARP_NODE_09, /*destLevel*/ LEVEL_DDD, /*destArea*/ 1, /*destNode*/ WARP_NODE_0A, /*flags*/ WARP_NO_CHECKPOINT),
    PAINTING_WARP_NODE(/*id*/ WARP_NODE_0A, /*destLevel*/ LEVEL_DDD, /*destArea*/ 1, /*destNode*/ WARP_NODE_0A, /*flags*/ WARP_NO_CHECKPOINT),
    PAINTING_WARP_NODE(/*id*/ WARP_NODE_0B, /*destLevel*/ LEVEL_DDD, /*destArea*/ 1, /*destNode*/ WARP_NODE_0A, /*flags*/ WARP_NO_CHECKPOINT),
    // Koopa 1 warp
    OBJECT(/*model*/ MODEL_NONE, /*pos*/    0,  178,  -6198, /*angle*/ 0,    0, 0, /*bhvParam*/ BPARAM1(0x30) | BPARAM2(WARP_NODE_0B), /*bhv*/ bhvWarp),
    WARP_NODE(/*id*/ WARP_NODE_0B, /*destLevel*/ LEVEL_BOWSER_1, /*destArea*/ 1, /*destNode*/ WARP_NODE_0A, /*flags*/ WARP_NO_CHECKPOINT),

/* Non-painting exit warps */
    // Pause exit warp
    OBJECT(/*model*/ MODEL_NONE, /*pos*/    0,    0, -2013, /*angle*/ 0,  180, 0, /*bhvParam*/ BPARAM2(WARP_NODE_1F), /*bhv*/ bhvInstantActiveWarp),
    // Koopa 1 exit warps
    OBJECT(/*model*/ MODEL_NONE, /*pos*/    0,  384, -6301, /*angle*/ 0,  180, 0, /*bhvParam*/ BPARAM2(WARP_NODE_24), /*bhv*/ bhvLaunchStarCollectWarp),
    OBJECT(/*model*/ MODEL_NONE, /*pos*/    0,  384, -6301, /*angle*/ 0,  180, 0, /*bhvParam*/ BPARAM2(WARP_NODE_25), /*bhv*/ bhvLaunchDeathWarp),
    // Pause exit warp node
    WARP_NODE(/*id*/ WARP_NODE_1F, /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 1, /*destNode*/ WARP_NODE_1F, /*flags*/ WARP_NO_CHECKPOINT),
    // Koopa 1 exit warp nodes
    WARP_NODE(/*id*/ WARP_NODE_24, /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 1, /*destNode*/ WARP_NODE_24, /*flags*/ WARP_NO_CHECKPOINT),
    WARP_NODE(/*id*/ WARP_NODE_25, /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 1, /*destNode*/ WARP_NODE_25, /*flags*/ WARP_NO_CHECKPOINT),

/* Painting star exit warps */
    OBJECT(/*model*/ MODEL_NONE, /*pos*/ -3304, 717, -2310, /*angle*/ 0, -90, 0, /*bhvParam*/ BPARAM2(WARP_NODE_32), /*bhv*/ bhvPaintingStarCollectWarp),
    OBJECT(/*model*/ MODEL_NONE, /*pos*/  -950, 107, -4916, /*angle*/ 0, 180, 0, /*bhvParam*/ BPARAM2(WARP_NODE_33), /*bhv*/ bhvPaintingStarCollectWarp),
    OBJECT(/*model*/ MODEL_NONE, /*pos*/   950, 107, -4916, /*angle*/ 0, 180, 0, /*bhvParam*/ BPARAM2(WARP_NODE_34), /*bhv*/ bhvPaintingStarCollectWarp),
    OBJECT(/*model*/ MODEL_NONE, /*pos*/  3304, 717, -2310, /*angle*/ 0,  90, 0, /*bhvParam*/ BPARAM2(WARP_NODE_35), /*bhv*/ bhvPaintingStarCollectWarp),
    WARP_NODE(/*id*/ WARP_NODE_32, /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 1, /*destNode*/ WARP_NODE_32, /*flags*/ WARP_NO_CHECKPOINT), // Snow Slider
    WARP_NODE(/*id*/ WARP_NODE_33, /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 1, /*destNode*/ WARP_NODE_33, /*flags*/ WARP_NO_CHECKPOINT), // Mountain
    WARP_NODE(/*id*/ WARP_NODE_34, /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 1, /*destNode*/ WARP_NODE_34, /*flags*/ WARP_NO_CHECKPOINT), // Fire Bubble
    WARP_NODE(/*id*/ WARP_NODE_35, /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 1, /*destNode*/ WARP_NODE_35, /*flags*/ WARP_NO_CHECKPOINT), // Water Land
/* Painting death exit warps */
    OBJECT(/*model*/ MODEL_NONE, /*pos*/ -3304, 717, -2310, /*angle*/ 0, -90, 0, /*bhvParam*/ BPARAM2(WARP_NODE_64), /*bhv*/ bhvPaintingDeathWarp),
    OBJECT(/*model*/ MODEL_NONE, /*pos*/  -950, 107, -4916, /*angle*/ 0, 180, 0, /*bhvParam*/ BPARAM2(WARP_NODE_65), /*bhv*/ bhvPaintingDeathWarp),
    OBJECT(/*model*/ MODEL_NONE, /*pos*/   950, 107, -4916, /*angle*/ 0, 180, 0, /*bhvParam*/ BPARAM2(WARP_NODE_66), /*bhv*/ bhvPaintingDeathWarp),
    OBJECT(/*model*/ MODEL_NONE, /*pos*/  3304, 717, -2310, /*angle*/ 0,  90, 0, /*bhvParam*/ BPARAM2(WARP_NODE_67), /*bhv*/ bhvPaintingDeathWarp),
    WARP_NODE(/*id*/ WARP_NODE_64, /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 1, /*destNode*/ WARP_NODE_64, /*flags*/ WARP_NO_CHECKPOINT), // Snow Slider
    WARP_NODE(/*id*/ WARP_NODE_65, /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 1, /*destNode*/ WARP_NODE_65, /*flags*/ WARP_NO_CHECKPOINT), // Mountain
    WARP_NODE(/*id*/ WARP_NODE_66, /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 1, /*destNode*/ WARP_NODE_66, /*flags*/ WARP_NO_CHECKPOINT), // Fire Bubble
    WARP_NODE(/*id*/ WARP_NODE_67, /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 1, /*destNode*/ WARP_NODE_67, /*flags*/ WARP_NO_CHECKPOINT), // Water Land
    RETURN(),
};

const LevelScript level_castle_inside_entry[] = { // SEQ_DoStage06
    INIT_LEVEL(),
    LOAD_MIO0        (/*seg*/ 0x07, _castle_inside_segment_7SegmentRomStart, _castle_inside_segment_7SegmentRomEnd),
    LOAD_MIO0_TEXTURE(/*seg*/ 0x09, _inside_mio0SegmentRomStart, _inside_mio0SegmentRomEnd),
    ALLOC_LEVEL_POOL(),
    MARIO(/*model*/ MODEL_MARIO, /*bhvParam*/ BPARAM4(0x01), /*bhv*/ bhvMario),
    LOAD_MODEL_FROM_GEO(MODEL_CASTLE_BOWSER_TRAP, castle_geo_000F18),
    LOAD_MODEL_FROM_GEO(MODEL_CASTLE_DOOR_WARP,   RCP_HmsMainDoor),
    LOAD_MODEL_FROM_GEO(MODEL_CASTLE_DOOR,        RCP_HmsMainDoor),
    LOAD_MODEL_FROM_GEO(MODEL_CASTLE_DOOR_A,      RCP_HmsMainroomDoorA),
    LOAD_MODEL_FROM_GEO(MODEL_CASTLE_DOOR_B,      RCP_HmsMainroomDoorB),
    LOAD_MODEL_FROM_GEO(MODEL_CASTLE_DOOR_C,      RCP_HmsMainroomDoorC),
    LOAD_MODEL_FROM_GEO(MODEL_CASTLE_DOOR_D,      RCP_HmsMainroomDoorD),

    AREA(/*index*/ 1, castle_geo),
        OBJECT(/*model*/ MODEL_NONE,              /*pos*/    0, 614, -6364, /*angle*/ 0,   0, 0, /*bhvParam*/ 0x00140000, /*bhv*/ bhvCastleFloorTrap),
        OBJECT(/*model*/ MODEL_CASTLE_DOOR_A,     /*pos*/-1690, 205, -2320, /*angle*/ 0,  90, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvDoor),
        OBJECT(/*model*/ MODEL_CASTLE_DOOR_B,     /*pos*/ -946,   0, -3060, /*angle*/ 0,   0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvDoor),
        OBJECT(/*model*/ MODEL_CASTLE_DOOR_C,     /*pos*/  946,   0, -3060, /*angle*/ 0,   0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvDoor),
        OBJECT(/*model*/ MODEL_CASTLE_DOOR_D,     /*pos*/ 1690, 205, -2320, /*angle*/ 0, -90, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvDoor),
        OBJECT(/*model*/ MODEL_CASTLE_DOOR,       /*pos*/ -332,   0, -2856, /*angle*/ 0, -45, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvDoor),
        OBJECT(/*model*/ MODEL_CASTLE_DOOR,       /*pos*/  332,   0, -2856, /*angle*/ 0,  45, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvDoor),
        OBJECT(/*model*/ MODEL_CASTLE_DOOR,       /*pos*/  -77, 410, -3060, /*angle*/ 0,   0, 0, /*bhvParam*/ 0x00010000, /*bhv*/ bhvDoor),
        OBJECT(/*model*/ MODEL_CASTLE_DOOR,       /*pos*/   77, 410, -3060, /*angle*/ 0,-180, 0, /*bhvParam*/ 0x00020000, /*bhv*/ bhvDoor),
        OBJECT(/*model*/ MODEL_CASTLE_DOOR,       /*pos*/  -79,   0, -1116, /*angle*/ 0,   0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvDoorWarp),
        OBJECT(/*model*/ MODEL_CASTLE_DOOR,       /*pos*/   75,   0, -1116, /*angle*/ 0,-180, 0, /*bhvParam*/ 0x00010000, /*bhv*/ bhvDoorWarp),
        OBJECT(/*model*/ MODEL_CASTLE_DOOR,       /*pos*/    0,   0, -5416, /*angle*/ 0,   0, 0, /*bhvParam*/ 0x00020000, /*bhv*/ bhvDoorWarp),
        JUMP_LINK(script_func_local_1),
        WARP_NODE(/*id*/ WARP_NODE_DEATH, /*destLevel*/ LEVEL_CASTLE_GROUNDS, /*destArea*/ 1, /*destNode*/ WARP_NODE_03, /*flags*/ WARP_NO_CHECKPOINT),
        TERRAIN(/*terrainData*/ castle_inside_collision),
        ROOMS(/*surfaceRooms*/ castle_inside_collision_rooms),
        SHOW_DIALOG(/*index*/ 0x00, MESS_MAIN_IN),
        SET_BACKGROUND_MUSIC(/*settingsPreset*/ 0x0001, /*seq*/ SEQ_LEVEL_INSIDE_CASTLE),
        TERRAIN_TYPE(/*terrainType*/ TERRAIN_STONE),
    END_AREA(),

    FREE_LEVEL_POOL(),
    MARIO_POS(/*area*/ 1, /*yaw*/ 180, /*pos*/ 0, 0, -2013),
    CALL(/*arg*/ 0, /*func*/ lvl_init_or_update),
    CALL_LOOP(/*arg*/ 1, /*func*/ lvl_init_or_update),
    CLEAR_LEVEL(),
    SLEEP_BEFORE_EXIT(/*frames*/ 1),
    EXIT(),
};
