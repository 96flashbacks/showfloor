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
#include "levels/ttm/header.h"

const LevelScript level_ttm_entry[] = {
    INIT_LEVEL(),
    LOAD_MIO0        (/*seg*/ 0x07, _ttm_segment_7SegmentRomStart, _ttm_segment_7SegmentRomEnd),
    LOAD_MIO0_TEXTURE(/*seg*/ 0x09, _mountain_mio0SegmentRomStart, _mountain_mio0SegmentRomEnd),
    LOAD_MIO0        (/*seg*/ 0x0A, _water_skybox_mio0SegmentRomStart, _water_skybox_mio0SegmentRomEnd),
    //LOAD_MIO0        (/*seg*/ 0x05, _group6_mio0SegmentRomStart, _group6_mio0SegmentRomEnd),
    //LOAD_RAW         (/*seg*/ 0x0C, _group6_geoSegmentRomStart,  _group6_geoSegmentRomEnd),
    LOAD_MIO0        (/*seg*/ 0x08, _common0_mio0SegmentRomStart, _common0_mio0SegmentRomEnd),
    LOAD_RAW         (/*seg*/ 0x0F, _common0_geoSegmentRomStart,  _common0_geoSegmentRomEnd),
    ALLOC_LEVEL_POOL(),
    MARIO(/*model*/ MODEL_MARIO, /*bhvParam*/ BPARAM4(0x01), /*bhv*/ bhvMario),
    JUMP_LINK(script_func_global_1),
    //JUMP_LINK(script_func_global_7),
    LOAD_MODEL_FROM_GEO(MODEL_TTM_SLIDE_EXIT_PODIUM, ttm_geo_000DF4),
    LOAD_MODEL_FROM_GEO(MODEL_TTM_ROLLING_LOG,       ttm_geo_000730),
    LOAD_MODEL_FROM_GEO(MODEL_TTM_STAR_CAGE,         ttm_geo_000710),
    LOAD_MODEL_FROM_GEO(MODEL_TTM_BLUE_SMILEY,       ttm_geo_000D14),
    LOAD_MODEL_FROM_GEO(MODEL_TTM_YELLOW_SMILEY,     ttm_geo_000D4C),
    LOAD_MODEL_FROM_GEO(MODEL_TTM_STAR_SMILEY,       ttm_geo_000D84),
    LOAD_MODEL_FROM_GEO(MODEL_TTM_MOON_SMILEY,       ttm_geo_000DBC),
    /*LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_03,     ttm_geo_000748),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_04,     ttm_geo_000778),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_05,     ttm_geo_0007A8),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_06,     ttm_geo_0007D8),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_07,     ttm_geo_000808),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_08,     ttm_geo_000830),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_09,     ttm_geo_000858),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_0A,     ttm_geo_000880),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_0B,     ttm_geo_0008A8),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_0C,     ttm_geo_0008D0),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_0D,     ttm_geo_0008F8),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_0F,     ttm_geo_000920),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_10,     ttm_geo_000948),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_11,     ttm_geo_000970),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_12,     ttm_geo_000990),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_13,     ttm_geo_0009C0),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_14,     ttm_geo_0009F0),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_15,     ttm_geo_000A18),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_16,     ttm_geo_000A40),*/

    AREA(/*index*/ 1, ttm_geo_000A70),
        OBJECT(/*model*/ MODEL_NONE, /*pos*/   102, -3332,  5734, /*angle*/ 0,   45, 0, /*bhvParam*/ BPARAM2(WARP_NODE_0A), /*bhv*/ bhvSpinAirborneWarp),
        OBJECT(/*model*/ MODEL_NONE, /*pos*/ -2447, -2457,  3952, /*angle*/ 0, -105, 0, /*bhvParam*/ BPARAM2(WARP_NODE_14), /*bhv*/ bhvAirborneWarp),
        OBJECT(/*model*/ MODEL_NONE, /*pos*/  2267, -3006, -3788, /*angle*/ 0,  148, 0, /*bhvParam*/ BPARAM2(WARP_NODE_15), /*bhv*/ bhvFadingWarp),
        OBJECT(/*model*/ MODEL_NONE, /*pos*/  -557, -3448, -4146, /*angle*/ 0, -168, 0, /*bhvParam*/ BPARAM2(WARP_NODE_16), /*bhv*/ bhvFadingWarp),
        WARP_NODE(/*id*/ WARP_NODE_0A, /*destLevel*/ LEVEL_TTM, /*destArea*/ 1, /*destNode*/ WARP_NODE_0A, /*flags*/ WARP_NO_CHECKPOINT),
        WARP_NODE(/*id*/ WARP_NODE_14, /*destLevel*/ LEVEL_TTM, /*destArea*/ 1, /*destNode*/ WARP_NODE_14, /*flags*/ WARP_NO_CHECKPOINT),
        WARP_NODE(/*id*/ WARP_NODE_15, /*destLevel*/ LEVEL_TTM, /*destArea*/ 1, /*destNode*/ WARP_NODE_16, /*flags*/ WARP_NO_CHECKPOINT),
        WARP_NODE(/*id*/ WARP_NODE_16, /*destLevel*/ LEVEL_TTM, /*destArea*/ 1, /*destNode*/ WARP_NODE_15, /*flags*/ WARP_NO_CHECKPOINT),
        WARP_NODE(/*id*/ WARP_NODE_SUCCESS,    /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 2, /*destNode*/ WARP_NODE_34, /*flags*/ WARP_NO_CHECKPOINT),
        WARP_NODE(/*id*/ WARP_NODE_DEATH,      /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 2, /*destNode*/ WARP_NODE_66, /*flags*/ WARP_NO_CHECKPOINT),

        TERRAIN(/*terrainData*/ ttm_collision),
        //MACRO_OBJECTS(/*objList*/ ttm_seg7_area_1_macro_objs),
        SET_BACKGROUND_MUSIC(/*settingsPreset*/ 0x0000, /*seq*/ SEQ_LEVEL_GRASS),
        TERRAIN_TYPE(/*terrainType*/ TERRAIN_STONE),
    END_AREA(),

    FREE_LEVEL_POOL(),
    MARIO_POS(/*area*/ 1, /*yaw*/ 45, /*pos*/ 102, -4332, 5734),
    CALL(/*arg*/ 0, /*func*/ lvl_init_or_update),
    CALL_LOOP(/*arg*/ 1, /*func*/ lvl_init_or_update),
    CLEAR_LEVEL(),
    SLEEP_BEFORE_EXIT(/*frames*/ 1),
    EXIT(),
};
