/********************************************************************************
						Ultra 64 MARIO Brothers

						stage 1 sequence module

			Copyright 1995 Nintendo co., ltd.  All rights reserved

							December 8, 1995
 ********************************************************************************/

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
#include "levels/castle/header.h"

#define seqPort(a,b,c,d) WARP_NODE(a,b,c,d,WARP_NO_CHECKPOINT)

const LevelScript level_castle_entry[] = {

    INIT_LEVEL(),
    LOAD_MIO0(         /*seg*/ 0x07, _castle_segment_7SegmentRomStart, _castle_segment_7SegmentRomEnd),

    ALLOC_LEVEL_POOL(),
    MARIO(/*model*/ MODEL_MARIO, /*behParam*/ 0x00000001, /*beh*/ bhvMario),

    AREA(/*index*/ 1, RCP_Stage1Scene1),
        TERRAIN(/*terrainData*/ castle_info),
        SET_BACKGROUND_MUSIC(/*settingsPreset*/ 0x0000, /*seq*/ SEQ_SOUND_PLAYER),
        TERRAIN_TYPE(/*terrainType*/ TERRAIN_GRASS),

    END_AREA(),

    FREE_LEVEL_POOL(),

    MARIO_POS(/*area*/ 1, /*yaw*/ 0, /*pos*/ 0, 1024, 0),
    CALL(/*arg*/ 0, /*func*/ lvl_init_or_update),
    CALL_LOOP(/*arg*/ 1, /*func*/ lvl_init_or_update),
    CLEAR_LEVEL(),
    SLEEP_BEFORE_EXIT(/*frames*/ 1),
    EXIT(),
};
