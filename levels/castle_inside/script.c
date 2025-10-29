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

static const LevelScript script_func_local_1[] = {
    WARP_NODE(/*id*/ 0x00, /*destLevel*/ LEVEL_CASTLE_GROUNDS, /*destArea*/ 0x01, /*destNode*/ 0x00, /*flags*/ WARP_NO_CHECKPOINT),
    WARP_NODE(/*id*/ 0x01, /*destLevel*/ LEVEL_CASTLE_GROUNDS, /*destArea*/ 0x01, /*destNode*/ 0x01, /*flags*/ WARP_NO_CHECKPOINT),
    WARP_NODE(/*id*/ 0x02, /*destLevel*/ LEVEL_CASTLE_COURTYARD, /*destArea*/ 0x01, /*destNode*/ 0x01, /*flags*/ WARP_NO_CHECKPOINT),
    WARP_NODE(/*id*/ 0x03, /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 0x02, /*destNode*/ 0x00, /*flags*/ WARP_NO_CHECKPOINT),
    WARP_NODE(/*id*/ 0x04, /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 0x02, /*destNode*/ 0x01, /*flags*/ WARP_NO_CHECKPOINT),
    WARP_NODE(/*id*/ 0x05, /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 0x03, /*destNode*/ 0x00, /*flags*/ WARP_NO_CHECKPOINT),
    WARP_NODE(/*id*/ 0x06, /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 0x03, /*destNode*/ 0x01, /*flags*/ WARP_NO_CHECKPOINT),
    PAINTING_WARP_NODE(/*id*/ 0x0C, /*destLevel*/ LEVEL_LLL, /*destArea*/ 0x01, /*destNode*/ 0x0A, /*flags*/ WARP_NO_CHECKPOINT),
    PAINTING_WARP_NODE(/*id*/ 0x0D, /*destLevel*/ LEVEL_LLL, /*destArea*/ 0x01, /*destNode*/ 0x0A, /*flags*/ WARP_NO_CHECKPOINT),
    PAINTING_WARP_NODE(/*id*/ 0x0E, /*destLevel*/ LEVEL_LLL, /*destArea*/ 0x01, /*destNode*/ 0x0A, /*flags*/ WARP_NO_CHECKPOINT),
    PAINTING_WARP_NODE(/*id*/ 0x03, /*destLevel*/ LEVEL_CCM, /*destArea*/ 0x01, /*destNode*/ 0x0A, /*flags*/ WARP_NO_CHECKPOINT),
    PAINTING_WARP_NODE(/*id*/ 0x04, /*destLevel*/ LEVEL_CCM, /*destArea*/ 0x01, /*destNode*/ 0x0A, /*flags*/ WARP_NO_CHECKPOINT),
    PAINTING_WARP_NODE(/*id*/ 0x05, /*destLevel*/ LEVEL_CCM, /*destArea*/ 0x01, /*destNode*/ 0x0A, /*flags*/ WARP_NO_CHECKPOINT),
    PAINTING_WARP_NODE(/*id*/ 0x06, /*destLevel*/ LEVEL_WF, /*destArea*/ 0x01, /*destNode*/ 0x0A, /*flags*/ WARP_NO_CHECKPOINT),
    PAINTING_WARP_NODE(/*id*/ 0x07, /*destLevel*/ LEVEL_WF, /*destArea*/ 0x01, /*destNode*/ 0x0A, /*flags*/ WARP_NO_CHECKPOINT),
    PAINTING_WARP_NODE(/*id*/ 0x08, /*destLevel*/ LEVEL_WF, /*destArea*/ 0x01, /*destNode*/ 0x0A, /*flags*/ WARP_NO_CHECKPOINT),
    PAINTING_WARP_NODE(/*id*/ 0x09, /*destLevel*/ LEVEL_DDD, /*destArea*/ 0x01, /*destNode*/ 0x0A, /*flags*/ WARP_NO_CHECKPOINT),
    PAINTING_WARP_NODE(/*id*/ 0x0A, /*destLevel*/ LEVEL_DDD, /*destArea*/ 0x01, /*destNode*/ 0x0A, /*flags*/ WARP_NO_CHECKPOINT),
    PAINTING_WARP_NODE(/*id*/ 0x0B, /*destLevel*/ LEVEL_DDD, /*destArea*/ 0x01, /*destNode*/ 0x0A, /*flags*/ WARP_NO_CHECKPOINT),
    OBJECT(/*model*/ MODEL_NONE, /*pos*/  2013,  768, -4367, /*angle*/ 0,    0, 0, /*bhvParam*/ 0x000A0000, /*bhv*/ bhvWarp),
    WARP_NODE(/*id*/ 0x0A, /*destLevel*/ LEVEL_PSS, /*destArea*/ 0x01, /*destNode*/ 0x0A, /*flags*/ WARP_NO_CHECKPOINT),
    OBJECT(/*model*/ MODEL_NONE, /*pos*/    0,  178,  -6198, /*angle*/ 0,    0, 0, /*bhvParam*/ 0x300B0000, /*bhv*/ bhvWarp),
    WARP_NODE(/*id*/ 0x0B, /*destLevel*/ LEVEL_BOWSER_1, /*destArea*/ 0x01, /*destNode*/ 0x0A, /*flags*/ WARP_NO_CHECKPOINT),
    OBJECT(/*model*/ MODEL_NONE, /*pos*/  1963,  819,  1187, /*angle*/ 0,    0, 0, /*bhvParam*/ 0x050C0000, /*bhv*/ bhvWarp),
    WARP_NODE(/*id*/ 0x0C, /*destLevel*/ LEVEL_SA, /*destArea*/ 0x01, /*destNode*/ 0x0A, /*flags*/ WARP_NO_CHECKPOINT),
    WARP_NODE(/*id*/ 0xF2, /*destLevel*/ LEVEL_TOTWC, /*destArea*/ 0x01, /*destNode*/ 0x0A, /*flags*/ WARP_NO_CHECKPOINT),
    OBJECT(/*model*/ MODEL_NONE, /*pos*/ -1024,  512, -3003, /*angle*/ 0,    0, 0, /*bhvParam*/ 0x001E0000, /*bhv*/ bhvInstantActiveWarp),
    OBJECT(/*model*/ MODEL_NONE, /*pos*/     0,   0, -2013, /*angle*/ 0,  180, 0, /*bhvParam*/ 0x001F0000, /*bhv*/ bhvInstantActiveWarp), 
    OBJECT(/*model*/ MODEL_NONE, /*pos*/ -1024,  900, -1636, /*angle*/ 0,  180, 0, /*bhvParam*/ 0x00200000, /*bhv*/ bhvAirborneWarp), 
    OBJECT(/*model*/ MODEL_NONE, /*pos*/ -1024,  900, -1636, /*angle*/ 0,  180, 0, /*bhvParam*/ 0x00210000, /*bhv*/ bhvAirborneDeathWarp), 
    OBJECT(/*model*/ MODEL_NONE, /*pos*/ -1024,  900, -1636, /*angle*/ 0,  180, 0, /*bhvParam*/ 0x00220000, /*bhv*/ bhvHardAirKnockBackWarp),
    OBJECT(/*model*/ MODEL_NONE, /*pos*/ -1024,  900, -1636, /*angle*/ 0,  180, 0, /*bhvParam*/ 0x00230000, /*bhv*/ bhvDeathWarp), 
    OBJECT(/*model*/ MODEL_NONE, /*pos*/     0,  384, -6198, /*angle*/ 0,  180, 0, /*bhvParam*/ 0x00240000, /*bhv*/ bhvLaunchStarCollectWarp), 
    OBJECT(/*model*/ MODEL_NONE, /*pos*/     0,  384, -6198, /*angle*/ 0,  180, 0, /*bhvParam*/ 0x00250000, /*bhv*/ bhvLaunchDeathWarp),
    OBJECT(/*model*/ MODEL_NONE, /*pos*/ -1024,  900, -1636, /*angle*/ 0,    0, 0, /*bhvParam*/ 0x00260000, /*bhv*/ bhvAirborneStarCollectWarp), 
    OBJECT(/*model*/ MODEL_NONE, /*pos*/  2816, 1200, -2609, /*angle*/ 0,   90, 0, /*bhvParam*/ 0x00270000, /*bhv*/ bhvAirborneStarCollectWarp), 
    OBJECT(/*model*/ MODEL_NONE, /*pos*/  2816, 1200, -2609, /*angle*/ 0,  270, 0, /*bhvParam*/ 0x00280000, /*bhv*/ bhvDeathWarp), 
    WARP_NODE(/*id*/ 0x1E, /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 0x01, /*destNode*/ 0x1E, /*flags*/ WARP_NO_CHECKPOINT),
    WARP_NODE(/*id*/ 0x1F, /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 0x01, /*destNode*/ 0x1F, /*flags*/ WARP_NO_CHECKPOINT),
    WARP_NODE(/*id*/ 0x20, /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 0x01, /*destNode*/ 0x20, /*flags*/ WARP_NO_CHECKPOINT),
    WARP_NODE(/*id*/ 0x21, /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 0x01, /*destNode*/ 0x21, /*flags*/ WARP_NO_CHECKPOINT),
    WARP_NODE(/*id*/ 0x22, /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 0x01, /*destNode*/ 0x22, /*flags*/ WARP_NO_CHECKPOINT),
    WARP_NODE(/*id*/ 0x23, /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 0x01, /*destNode*/ 0x23, /*flags*/ WARP_NO_CHECKPOINT),
    WARP_NODE(/*id*/ 0x24, /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 0x01, /*destNode*/ 0x24, /*flags*/ WARP_NO_CHECKPOINT),
    WARP_NODE(/*id*/ 0x25, /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 0x01, /*destNode*/ 0x25, /*flags*/ WARP_NO_CHECKPOINT),
    WARP_NODE(/*id*/ 0x26, /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 0x01, /*destNode*/ 0x26, /*flags*/ WARP_NO_CHECKPOINT),
    WARP_NODE(/*id*/ 0x27, /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 0x01, /*destNode*/ 0x27, /*flags*/ WARP_NO_CHECKPOINT),
    WARP_NODE(/*id*/ 0x28, /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 0x01, /*destNode*/ 0x28, /*flags*/ WARP_NO_CHECKPOINT),
    OBJECT(/*model*/ MODEL_NONE, /*pos*/ -3279, 717, -2319, /*angle*/ 0, -90, 0, /*bhvParam*/ 0x00320000, /*bhv*/ bhvPaintingStarCollectWarp), //ddd
    OBJECT(/*model*/ MODEL_NONE, /*pos*/ -947, 99, -4907, /*angle*/ 0, 180, 0, /*bhvParam*/ 0x00330000, /*bhv*/ bhvPaintingStarCollectWarp), //wf
    OBJECT(/*model*/ MODEL_NONE, /*pos*/  947, 99, -4907, /*angle*/ 0, 180, 0, /*bhvParam*/ 0x00340000, /*bhv*/ bhvPaintingStarCollectWarp), //lll
    OBJECT(/*model*/ MODEL_NONE, /*pos*/  3279, 717, -2319, /*angle*/ 0,  90, 0, /*bhvParam*/ 0x00350000, /*bhv*/ bhvPaintingStarCollectWarp), //ccm
    WARP_NODE(/*id*/ 0x32, /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 0x01, /*destNode*/ 0x32, /*flags*/ WARP_NO_CHECKPOINT),
    WARP_NODE(/*id*/ 0x33, /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 0x01, /*destNode*/ 0x33, /*flags*/ WARP_NO_CHECKPOINT),
    WARP_NODE(/*id*/ 0x34, /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 0x01, /*destNode*/ 0x34, /*flags*/ WARP_NO_CHECKPOINT),
    WARP_NODE(/*id*/ 0x35, /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 0x01, /*destNode*/ 0x35, /*flags*/ WARP_NO_CHECKPOINT),
    OBJECT(/*model*/ MODEL_NONE, /*pos*/ -3279, 717, -2319, /*angle*/ 0, -90, 0, /*bhvParam*/ 0x00640000, /*bhv*/ bhvPaintingDeathWarp), //ddd
    OBJECT(/*model*/ MODEL_NONE, /*pos*/  -947, 99, -4907, /*angle*/ 0, 180, 0, /*bhvParam*/ 0x00650000, /*bhv*/ bhvPaintingDeathWarp), //wf
    OBJECT(/*model*/ MODEL_NONE, /*pos*/   947, 99, -4907, /*angle*/ 0, 180, 0, /*bhvParam*/ 0x00660000, /*bhv*/ bhvPaintingDeathWarp), //lll
    OBJECT(/*model*/ MODEL_NONE, /*pos*/  3279, 717, -2319, /*angle*/ 0,  90, 0, /*bhvParam*/ 0x00670000, /*bhv*/ bhvPaintingDeathWarp), //ccm
    WARP_NODE(/*id*/ 0x64, /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 0x01, /*destNode*/ 0x64, /*flags*/ WARP_NO_CHECKPOINT),
    WARP_NODE(/*id*/ 0x65, /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 0x01, /*destNode*/ 0x65, /*flags*/ WARP_NO_CHECKPOINT),
    WARP_NODE(/*id*/ 0x66, /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 0x01, /*destNode*/ 0x66, /*flags*/ WARP_NO_CHECKPOINT),
    WARP_NODE(/*id*/ 0x67, /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 0x01, /*destNode*/ 0x67, /*flags*/ WARP_NO_CHECKPOINT),
    RETURN(),
};

const LevelScript level_castle_inside_entry[] = {
    INIT_LEVEL(),
    LOAD_MIO0        (/*seg*/ 0x07, _castle_inside_segment_7SegmentRomStart, _castle_inside_segment_7SegmentRomEnd),
    LOAD_MIO0_TEXTURE(/*seg*/ 0x09, _inside_mio0SegmentRomStart, _inside_mio0SegmentRomEnd),
    ALLOC_LEVEL_POOL(),
    MARIO(/*model*/ MODEL_MARIO, /*bhvParam*/ BPARAM4(0x01), /*bhv*/ bhvMario),
    LOAD_MODEL_FROM_GEO(MODEL_CASTLE_BOWSER_TRAP,        castle_geo_000F18),
    LOAD_MODEL_FROM_GEO(MODEL_CASTLE_DOOR_WARP,          RCP_HmsMainDoor),
    LOAD_MODEL_FROM_GEO(MODEL_CASTLE_DOOR,               RCP_HmsMainDoor),
    LOAD_MODEL_FROM_GEO(MODEL_CASTLE_DOOR_A,             RCP_HmsMainroomDoorA),
    LOAD_MODEL_FROM_GEO(MODEL_CASTLE_DOOR_B,             RCP_HmsMainroomDoorB),
    LOAD_MODEL_FROM_GEO(MODEL_CASTLE_DOOR_C,             RCP_HmsMainroomDoorC),
    LOAD_MODEL_FROM_GEO(MODEL_CASTLE_DOOR_D,             RCP_HmsMainroomDoorD),
    // The different sets of star doors all use different model IDs, despite them all loading the same geo layout.
    // It is possible that star doors were originally going to have numbers on them, similar to the other locked doors.

    AREA(/*index*/ 1, castle_geo),
        OBJECT(/*model*/ MODEL_NONE,              /*pos*/    0, 614, -6261, /*angle*/ 0,   0, 0, /*bhvParam*/ 0x00140000, /*bhv*/ bhvCastleFloorTrap),
        OBJECT(/*model*/ MODEL_CASTLE_DOOR_A,     /*pos*/-1690, 205, -2319, /*angle*/ 0,  90, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvDoor),
        OBJECT(/*model*/ MODEL_CASTLE_DOOR_B,     /*pos*/ -947,   0, -3061, /*angle*/ 0,   0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvDoor),
        OBJECT(/*model*/ MODEL_CASTLE_DOOR_C,     /*pos*/  947,   0, -3061, /*angle*/ 0,   0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvDoor),
        OBJECT(/*model*/ MODEL_CASTLE_DOOR_D,     /*pos*/ 1690, 205, -2319, /*angle*/ 0, -90, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvDoor),
        OBJECT(/*model*/ MODEL_CASTLE_DOOR,       /*pos*/ -329,   0, -2853, /*angle*/ 0, -45, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvDoor),
        OBJECT(/*model*/ MODEL_CASTLE_DOOR,       /*pos*/  329,   0, -2853, /*angle*/ 0,  45, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvDoor),
        OBJECT(/*model*/ MODEL_CASTLE_DOOR,       /*pos*/  -77, 410, -3061, /*angle*/ 0,   0, 0, /*bhvParam*/ 0x00010000, /*bhv*/ bhvDoor),
        OBJECT(/*model*/ MODEL_CASTLE_DOOR,       /*pos*/   76, 410, -3061, /*angle*/ 0,-180, 0, /*bhvParam*/ 0x00020000, /*bhv*/ bhvDoor),
        OBJECT(/*model*/ MODEL_CASTLE_DOOR,       /*pos*/  -79,   0, -1116, /*angle*/ 0,   0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvDoorWarp),
        OBJECT(/*model*/ MODEL_CASTLE_DOOR,       /*pos*/   75,   0, -1116, /*angle*/ 0,-180, 0, /*bhvParam*/ 0x00010000, /*bhv*/ bhvDoorWarp),
        OBJECT(/*model*/ MODEL_CASTLE_DOOR,       /*pos*/    0,   0, -5416, /*angle*/ 0,   0, 0, /*bhvParam*/ 0x00020000, /*bhv*/ bhvDoorWarp),
        JUMP_LINK(script_func_local_1),
        WARP_NODE(/*id*/ 0xF1, /*destLevel*/ LEVEL_CASTLE_GROUNDS, /*destArea*/ 0x01, /*destNode*/ 0x03, /*flags*/ WARP_NO_CHECKPOINT),
        TERRAIN(/*terrainData*/ castle_inside_collision),
        ROOMS(/*surfaceRooms*/ castle_inside_collision_rooms),
        SHOW_DIALOG(/*index*/ 0x00, DIALOG_021),
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
