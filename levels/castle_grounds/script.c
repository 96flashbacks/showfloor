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

static const LevelScript script_func_local_1[] = { // Ports (warps)
/* Door warps */
    WARP_NODE(/*id*/ WARP_NODE_00, /*destLevel*/ LEVEL_CASTLE,         /*destArea*/ 1, /*destNode*/ WARP_NODE_00, /*flags*/ WARP_NO_CHECKPOINT),
    WARP_NODE(/*id*/ WARP_NODE_01, /*destLevel*/ LEVEL_CASTLE,         /*destArea*/ 1, /*destNode*/ WARP_NODE_01, /*flags*/ WARP_NO_CHECKPOINT),
/* Death warp */
    OBJECT(/*model*/ MODEL_NONE, /*pos*/     0,   900, -1710, /*angle*/ 0, 180, 0, /*bhvParam*/ BPARAM2(WARP_NODE_02), /*bhv*/ bhvDeathWarp),
    WARP_NODE(/*id*/ WARP_NODE_02, /*destLevel*/ LEVEL_CASTLE_GROUNDS, /*destArea*/ 1, /*destNode*/ WARP_NODE_02, /*flags*/ WARP_NO_CHECKPOINT),
/* Spawn spin warp (unused) */
    OBJECT(/*model*/ MODEL_NONE, /*pos*/ -1515,   260,  4962, /*angle*/ 0, 180, 0, /*bhvParam*/ BPARAM2(WARP_NODE_03), /*bhv*/ bhvSpinAirborneCircleWarp),
    WARP_NODE(/*id*/ WARP_NODE_03, /*destLevel*/ LEVEL_CASTLE_GROUNDS, /*destArea*/ 1, /*destNode*/ WARP_NODE_03, /*flags*/ WARP_NO_CHECKPOINT),
    RETURN(),
};

static const LevelScript script_func_local_2[] = { // BGParts
    OBJECT(/*model*/ MODEL_NONE,                        /*pos*/ -7430, 1500,   873, /*angle*/ 0,   0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvBirdsSoundLoop),
    OBJECT(/*model*/ MODEL_NONE,                        /*pos*/   -80, 1500,  5004, /*angle*/ 0,   0, 0, /*bhvParam*/ 0x00010000, /*bhv*/ bhvBirdsSoundLoop),
    OBJECT(/*model*/ MODEL_NONE,                        /*pos*/  7131, 1500, -2989, /*angle*/ 0,   0, 0, /*bhvParam*/ 0x00020000, /*bhv*/ bhvBirdsSoundLoop),
    OBJECT(/*model*/ MODEL_NONE,                        /*pos*/ -7430, 1500, -5937, /*angle*/ 0,   0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvAmbientSounds),
    RETURN(),
};

const LevelScript level_castle_grounds_entry[] = { // SEQ_DoStage16
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
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_03, castle_grounds_tower_geo),
    LOAD_MODEL_FROM_GEO(MODEL_BUBBLY_TREE,       RCP_HmsMainTree),
    LOAD_MODEL_FROM_GEO(MODEL_CASTLE_DOOR_WARP,  RCP_HmsMainDoor),

    AREA(/*index*/ 1, castle_grounds_geo),
        WARP_NODE(/*id*/ WARP_NODE_DEATH, /*destLevel*/ LEVEL_CASTLE_GROUNDS, /*destArea*/ 1, /*destNode*/ WARP_NODE_02, /*flags*/ WARP_NO_CHECKPOINT),
        JUMP_LINK(script_func_local_1),
        JUMP_LINK(script_func_local_2),
        TERRAIN(/*terrainData*/ castle_grounds_collision),
        SET_BACKGROUND_MUSIC(/*settingsPreset*/ 0x0000, /*seq*/ SEQ_SOUND_PLAYER),
        TERRAIN_TYPE(/*terrainType*/ TERRAIN_GRASS),
    END_AREA(),

    FREE_LEVEL_POOL(),
    MARIO_POS(/*area*/ 1, /*yaw*/ 180, /*pos*/ -1515, 260, 4962),
    CALL(/*arg*/ 0, /*func*/ lvl_init_or_update),
    CALL_LOOP(/*arg*/ 1, /*func*/ lvl_init_or_update),
    CLEAR_LEVEL(),
    SLEEP_BEFORE_EXIT(/*frames*/ 1),
    EXIT(),
};
