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
#include "levels/castle_grounds/header.h"

// This has so many unused warps it's kind of disgusting
static const LevelScript script_func_local_1[] = {
    WARP_NODE(/*id*/ WARP_NODE_00, /*destLevel*/ LEVEL_CASTLE,         /*destArea*/ 1, /*destNode*/ WARP_NODE_00, /*flags*/ WARP_NO_CHECKPOINT),
    WARP_NODE(/*id*/ WARP_NODE_01, /*destLevel*/ LEVEL_CASTLE,         /*destArea*/ 1, /*destNode*/ WARP_NODE_01, /*flags*/ WARP_NO_CHECKPOINT),
    WARP_NODE(/*id*/ WARP_NODE_02, /*destLevel*/ LEVEL_CASTLE,         /*destArea*/ 3, /*destNode*/ WARP_NODE_02, /*flags*/ WARP_NO_CHECKPOINT),
    OBJECT(/*model*/ MODEL_NONE, /*pos*/     0,   900, -1710, /*angle*/ 0, 180, 0, /*bhvParam*/ BPARAM2(WARP_NODE_03), /*bhv*/ bhvDeathWarp),
    WARP_NODE(/*id*/ WARP_NODE_03, /*destLevel*/ LEVEL_CASTLE_GROUNDS, /*destArea*/ 1, /*destNode*/ WARP_NODE_03, /*flags*/ WARP_NO_CHECKPOINT),
    OBJECT(/*model*/ MODEL_NONE, /*pos*/ -1528,   260,  4890, /*angle*/ 0, 180, 0, /*bhvParam*/ BPARAM2(WARP_NODE_04), /*bhv*/ bhvSpinAirborneCircleWarp),
    WARP_NODE(/*id*/ WARP_NODE_04, /*destLevel*/ LEVEL_CASTLE_GROUNDS, /*destArea*/ 1, /*destNode*/ WARP_NODE_04, /*flags*/ WARP_NO_CHECKPOINT),
    OBJECT(/*model*/ MODEL_NONE, /*pos*/ -3379,  -815, -2025, /*angle*/ 0,   0, 0, /*bhvParam*/ BPARAM1(60) | BPARAM2(WARP_NODE_05), /*bhv*/ bhvWarp),
    OBJECT(/*model*/ MODEL_NONE, /*pos*/ -3379,  -500, -2025, /*angle*/ 0, 180, 0, /*bhvParam*/ BPARAM2(WARP_NODE_06), /*bhv*/ bhvLaunchDeathWarp),
    OBJECT(/*model*/ MODEL_NONE, /*pos*/ -3799, -1199, -5816, /*angle*/ 0,   0, 0, /*bhvParam*/ BPARAM2(WARP_NODE_07), /*bhv*/ bhvSwimmingWarp),
    OBJECT(/*model*/ MODEL_NONE, /*pos*/ -3379,  -500, -2025, /*angle*/ 0, 180, 0, /*bhvParam*/ BPARAM2(WARP_NODE_08), /*bhv*/ bhvLaunchStarCollectWarp),
    WARP_NODE(/*id*/ WARP_NODE_05, /*destLevel*/ LEVEL_VCUTM,          /*destArea*/ 1, /*destNode*/ WARP_NODE_0A, /*flags*/ WARP_NO_CHECKPOINT),
    WARP_NODE(/*id*/ WARP_NODE_06, /*destLevel*/ LEVEL_CASTLE_GROUNDS, /*destArea*/ 1, /*destNode*/ WARP_NODE_06, /*flags*/ WARP_NO_CHECKPOINT),
    WARP_NODE(/*id*/ WARP_NODE_07, /*destLevel*/ LEVEL_CASTLE_GROUNDS, /*destArea*/ 1, /*destNode*/ WARP_NODE_07, /*flags*/ WARP_NO_CHECKPOINT),
    WARP_NODE(/*id*/ WARP_NODE_08, /*destLevel*/ LEVEL_CASTLE_GROUNDS, /*destArea*/ 1, /*destNode*/ WARP_NODE_08, /*flags*/ WARP_NO_CHECKPOINT),
    OBJECT(/*model*/ MODEL_NONE, /*pos*/  5408,  4500,  3637, /*angle*/ 0, 225, 0, /*bhvParam*/ BPARAM2(WARP_NODE_0A), /*bhv*/ bhvAirborneWarp),
    WARP_NODE(/*id*/ WARP_NODE_0A, /*destLevel*/ LEVEL_CASTLE_GROUNDS, /*destArea*/ 1, /*destNode*/ WARP_NODE_0A, /*flags*/ WARP_NO_CHECKPOINT),
    OBJECT(/*model*/ MODEL_NONE, /*pos*/ -6901,  2376, -6509, /*angle*/ 0, 230, 0, /*bhvParam*/ BPARAM2(WARP_NODE_14), /*bhv*/ bhvAirborneWarp),
    WARP_NODE(/*id*/ WARP_NODE_14, /*destLevel*/ LEVEL_CASTLE_GROUNDS, /*destArea*/ 1, /*destNode*/ WARP_NODE_14, /*flags*/ WARP_NO_CHECKPOINT),
    OBJECT(/*model*/ MODEL_NONE, /*pos*/  4997, -1250,  2258, /*angle*/ 0, 210, 0, /*bhvParam*/ BPARAM2(WARP_NODE_1E), /*bhv*/ bhvSwimmingWarp),
    WARP_NODE(/*id*/ WARP_NODE_1E, /*destLevel*/ LEVEL_CASTLE_GROUNDS, /*destArea*/ 1, /*destNode*/ WARP_NODE_1E, /*flags*/ WARP_NO_CHECKPOINT),
    RETURN(),
};

static const LevelScript script_func_local_2[] = {
    OBJECT(/*model*/ MODEL_NONE,                        /*pos*/ -7430, 1500,   873, /*angle*/ 0,   0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvBirdsSoundLoop),
    OBJECT(/*model*/ MODEL_NONE,                        /*pos*/   -80, 1500,  5004, /*angle*/ 0,   0, 0, /*bhvParam*/ 0x00010000, /*bhv*/ bhvBirdsSoundLoop),
    OBJECT(/*model*/ MODEL_NONE,                        /*pos*/  7131, 1500, -2989, /*angle*/ 0,   0, 0, /*bhvParam*/ 0x00020000, /*bhv*/ bhvBirdsSoundLoop),
    OBJECT(/*model*/ MODEL_NONE,                        /*pos*/ -7430, 1500, -5937, /*angle*/ 0,   0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvAmbientSounds),
    RETURN(),
};

const LevelScript level_castle_grounds_entry[] = {
    INIT_LEVEL(),
    LOAD_MIO0        (/*seg*/ 0x07, _castle_grounds_segment_7SegmentRomStart, _castle_grounds_segment_7SegmentRomEnd),
    LOAD_MIO0        (/*seg*/ 0x0A, _water_skybox_mio0SegmentRomStart, _water_skybox_mio0SegmentRomEnd),
    LOAD_MIO0_TEXTURE(/*seg*/ 0x09, _outside_mio0SegmentRomStart, _outside_mio0SegmentRomEnd),
    LOAD_MIO0        (/*seg*/ 0x05, _group10_mio0SegmentRomStart, _group10_mio0SegmentRomEnd),
    LOAD_RAW         (/*seg*/ 0x0C, _group10_geoSegmentRomStart,  _group10_geoSegmentRomEnd),
    LOAD_MIO0        (/*seg*/ 0x08, _common0_mio0SegmentRomStart, _common0_mio0SegmentRomEnd),
    LOAD_RAW         (/*seg*/ 0x0F, _common0_geoSegmentRomStart,  _common0_geoSegmentRomEnd),
    ALLOC_LEVEL_POOL(),
    MARIO(/*model*/ MODEL_MARIO, /*bhvParam*/ BPARAM4(0x01), /*bhv*/ bhvMario),
    JUMP_LINK(script_func_global_11),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_03,           castle_grounds_tower_geo),
    LOAD_MODEL_FROM_GEO(MODEL_BUBBLY_TREE,                 RCP_HmsMainTree),
    LOAD_MODEL_FROM_GEO(MODEL_CASTLE_DOOR_WARP,            RCP_HmsMainDoor),

    AREA(/*index*/ 1, castle_grounds_geo),
        WARP_NODE(/*id*/ WARP_NODE_DEATH, /*destLevel*/ LEVEL_CASTLE_GROUNDS, /*destArea*/ 1, /*destNode*/ WARP_NODE_03, /*flags*/ WARP_NO_CHECKPOINT),
        JUMP_LINK(script_func_local_1),
        JUMP_LINK(script_func_local_2),
        TERRAIN(/*terrainData*/ castle_grounds_collision),
        SET_BACKGROUND_MUSIC(/*settingsPreset*/ 0x0000, /*seq*/ SEQ_SOUND_PLAYER),
        TERRAIN_TYPE(/*terrainType*/ TERRAIN_GRASS),
    END_AREA(),

    FREE_LEVEL_POOL(),
    MARIO_POS(/*area*/ 1, /*yaw*/ 180, /*pos*/ -1506, 260, 4955),
    CALL(/*arg*/ 0, /*func*/ lvl_init_or_update),
    CALL_LOOP(/*arg*/ 1, /*func*/ lvl_init_or_update),
    CLEAR_LEVEL(),
    SLEEP_BEFORE_EXIT(/*frames*/ 1),
    EXIT(),
};
