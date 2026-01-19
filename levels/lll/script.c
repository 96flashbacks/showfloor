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
#include "levels/lll/header.h"

// Unlike most levels, level geometry objects in LLL are defined as regular objects instead of terrain objects.
static const LevelScript script_func_local_1[] = {
    OBJECT(/*model*/ MODEL_LEVEL_GEOMETRY_03, /*pos*/  3326,   0, -2110, /*angle*/ 0,   0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvStaticObject),
    OBJECT(/*model*/ MODEL_LEVEL_GEOMETRY_04, /*pos*/  3454,   0,   261, /*angle*/ 0,   0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvStaticObject),
    // OBJECT(/*model*/ MODEL_LEVEL_GEOMETRY_05, /*pos*/  7168,   0,  1408, /*angle*/ 0,   0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvStaticObject),
    OBJECT(/*model*/ MODEL_LEVEL_GEOMETRY_06, /*pos*/     0,   0,  3712, /*angle*/ 0,   0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvStaticObject),
    OBJECT(/*model*/ MODEL_LEVEL_GEOMETRY_07, /*pos*/ -3199,   0,  3456, /*angle*/ 0,   0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvStaticObject),
    OBJECT(/*model*/ MODEL_LEVEL_GEOMETRY_08, /*pos*/ -3367,   0,    43, /*angle*/ 0,   0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvStaticObject),
    OBJECT(/*model*/ MODEL_LEVEL_GEOMETRY_0A, /*pos*/     0,   0,     0, /*angle*/ 0,   0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvStaticObject),
    OBJECT(/*model*/ MODEL_LEVEL_GEOMETRY_0B, /*pos*/     0,   0,  6272, /*angle*/ 0,   0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvStaticObject),
    // OBJECT(/*model*/ MODEL_LEVEL_GEOMETRY_0C, /*pos*/  5632,   0,  1408, /*angle*/ 0, 270, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvStaticObject),
    OBJECT(/*model*/ MODEL_LEVEL_GEOMETRY_0C, /*pos*/  2048,   0,  3456, /*angle*/ 0, 180, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvStaticObject),
    // OBJECT(/*model*/ MODEL_LEVEL_GEOMETRY_0C, /*pos*/ -4607,   0,  3456, /*angle*/ 0, 270, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvStaticObject),
    OBJECT(/*model*/ MODEL_LEVEL_GEOMETRY_0C, /*pos*/ -3327,   0,  1579, /*angle*/ 0,   0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvStaticObject),
    //OBJECT(/*model*/ MODEL_LEVEL_GEOMETRY_0D, /*pos*/     0,   0, -2047, /*angle*/ 0,   0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvStaticObject),
    // This behavior differs from the rest in that it actually has collision rather than being purely visual.
    // A likely explanation is that this object was added in after the collision model was made, and they didn't want
    // to redo the collision so they just added a new behavior for it.
    // OBJECT(/*model*/ MODEL_LEVEL_GEOMETRY_0E, /*pos*/ -5115, 300, -3200, /*angle*/ 0,  90, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvLLLHexagonalMesh),
    RETURN(),
};

static const LevelScript script_func_local_2[] = {
    OBJECT(/*model*/ MODEL_LLL_DRAWBRIDGE_PART,                /*pos*/ -1919,  307,  3648, /*angle*/ 0,  0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvLLLDrawbridgeSpawner),
    OBJECT(/*model*/ MODEL_LLL_ROTATING_BLOCK_FIRE_BARS,       /*pos*/ -2694,  307, -1805, /*angle*/ 0,  0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvLLLRotatingBlockWithFireBars),
    OBJECT(/*model*/ MODEL_LLL_ROTATING_HEXAGONAL_RING,        /*pos*/     0,    0,     0, /*angle*/ 0,  0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvLLLRotatingHexagonalRing),
    // OBJECT(/*model*/ MODEL_LLL_SINKING_RECTANGULAR_PLATFORM,   /*pos*/  3968,    0,  1408, /*angle*/ 0, 90, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvLLLSinkingRectangularPlatform),
    // OBJECT(/*model*/ MODEL_LLL_SINKING_RECTANGULAR_PLATFORM,   /*pos*/ -5759,    0,  3072, /*angle*/ 0,  0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvLLLSinkingRectangularPlatform),
    // OBJECT(/*model*/ MODEL_LLL_SINKING_RECTANGULAR_PLATFORM,   /*pos*/  2816,    0,   512, /*angle*/ 0, 90, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvLLLSinkingRectangularPlatform),
    OBJECT(/*model*/ MODEL_LLL_SINKING_RECTANGULAR_PLATFORM,   /*pos*/ -1791,    0, -3385, /*angle*/ 0, 90, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvLLLSinkingRectangularPlatform),
    // OBJECT(/*model*/ MODEL_LLL_SINKING_SQUARE_PLATFORMS,       /*pos*/  3840,    0, -3199, /*angle*/ 0,  0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvLLLSinkingSquarePlatforms),
    OBJECT(/*model*/ MODEL_LLL_TILTING_SQUARE_PLATFORM,        /*pos*/   922, -153,  2150, /*angle*/ 0,  0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvLLLTiltingInvertedPyramid),
    OBJECT(/*model*/ MODEL_LLL_TILTING_SQUARE_PLATFORM,        /*pos*/  1741, -153,  1741, /*angle*/ 0,  0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvLLLTiltingInvertedPyramid),
    OBJECT(/*model*/ MODEL_LLL_TILTING_SQUARE_PLATFORM,        /*pos*/  1741, -153,  2560, /*angle*/ 0,  0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvLLLTiltingInvertedPyramid),
    // OBJECT(/*model*/ MODEL_LLL_TILTING_SQUARE_PLATFORM,        /*pos*/  2099, -153,  -306, /*angle*/ 0,  0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvLLLTiltingInvertedPyramid),
    OBJECT(/*model*/ MODEL_NONE,                               /*pos*/  0,     194, -4980, /*angle*/ 0,  0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvLLLTumblingBridge),
    // OBJECT(/*model*/ MODEL_NONE,                               /*pos*/ -5119,  102,  1024, /*angle*/ 0,  0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvLLLBowserPuzzle),
    RETURN(),
};

static const LevelScript script_func_local_3[] = {
    OBJECT(/*model*/ MODEL_LLL_ROTATING_HEXAGONAL_PLATFORM,    /*pos*/ -2694,    0, -1805, /*angle*/ 0,  0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvLLLRotatingHexagonalPlatform),
    // OBJECT(/*model*/ MODEL_NONE,                               /*pos*/ -3180,    0,  1906, /*angle*/ 0,  0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvLLLFloatingWoodBridge),
    RETURN(),
};

static const LevelScript script_func_local_4[] = {
    OBJECT(/*model*/ MODEL_NONE,            /*pos*/ -3199,  307,  3456, /*angle*/ 0,   0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvMrI),
    //OBJECT(/*model*/ MODEL_BULLY_BOSS,      /*pos*/  3372, 2234, -2906, /*angle*/ 0,   0, 0, /*bhvParam*/ 0x01000000, /*bhv*/ bhvBigBullyWithMinions),
    OBJECT(/*model*/ MODEL_BULLY,           /*pos*/ -3363,  307,  -474, /*angle*/ 0,   0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvSmallBully),
    OBJECT(/*model*/ MODEL_BULLY,           /*pos*/   -20,  307, -3666, /*angle*/ 0,   0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvSmallBully),
    // OBJECT(/*model*/ MODEL_BULLY,           /*pos*/  6813,  307,  1613, /*angle*/ 0,   0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvSmallBully),
    // OBJECT(/*model*/ MODEL_BULLY,           /*pos*/  7168,  307,   998, /*angle*/ 0,   0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvSmallBully),
    OBJECT(/*model*/ MODEL_BULLY,           /*pos*/ -3332,  307,   522, /*angle*/ 0,   0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvSmallBully),
    OBJECT(/*model*/ MODEL_BULLY,           /*pos*/    -0,  307,  3712, /*angle*/ 0,   0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvSmallBully),
    OBJECT(/*model*/ MODEL_NONE,            /*pos*/   500,    2,  5000, /*angle*/ 0, 270, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvBouncingFireball),
    OBJECT(/*model*/ MODEL_NONE,            /*pos*/  -700,    2,  4500, /*angle*/ 0,  90, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvBouncingFireball),
    OBJECT(/*model*/ MODEL_NONE,            /*pos*/ -6300,    2,  2625, /*angle*/ 0,  90, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvBouncingFireball),
    // OBJECT(/*model*/ MODEL_NONE,            /*pos*/ -3280,    2, -4854, /*angle*/ 0,   0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvBouncingFireball),
    OBJECT(/*model*/ MODEL_NONE,            /*pos*/  4417,    2,   479, /*angle*/ 0, 315, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvBouncingFireball),
    OBJECT(/*model*/ MODEL_NONE,            /*pos*/  3909,    2, -1083, /*angle*/ 0, 315, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvBouncingFireball),
    OBJECT(/*model*/ MODEL_NONE,            /*pos*/  3459,    2,  -601, /*angle*/ 0,  90, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvBouncingFireball),
    RETURN(),
};

static const LevelScript script_func_local_5[] = {
    OBJECT(/*model*/ MODEL_STAR, /*pos*/ 0, 400,  -6100, /*angle*/ 0, 0, 0, /*bhvParam*/ 0x00000000, /*bhv*/ bhvStar),
    RETURN(),
};

const LevelScript level_lll_entry[] = {
    INIT_LEVEL(),
    LOAD_MIO0        (/*seg*/ 0x07, _lll_segment_7SegmentRomStart, _lll_segment_7SegmentRomEnd),
    LOAD_MIO0_TEXTURE(/*seg*/ 0x09, _generic_mio0SegmentRomStart, _generic_mio0SegmentRomEnd),
    LOAD_MIO0        (/*seg*/ 0x0A, _bitfs_skybox_mio0SegmentRomStart, _bitfs_skybox_mio0SegmentRomEnd),
    LOAD_MIO0        (/*seg*/ 0x0B, _effect_mio0SegmentRomStart, _effect_mio0SegmentRomEnd),
    LOAD_MIO0        (/*seg*/ 0x05, _group2_mio0SegmentRomStart, _group2_mio0SegmentRomEnd),
    LOAD_RAW         (/*seg*/ 0x0C, _group2_geoSegmentRomStart,  _group2_geoSegmentRomEnd),
    LOAD_MIO0        (/*seg*/ 0x06, _group17_mio0SegmentRomStart, _group17_mio0SegmentRomEnd),
    LOAD_RAW         (/*seg*/ 0x0D, _group17_geoSegmentRomStart,  _group17_geoSegmentRomEnd),
    LOAD_MIO0        (/*seg*/ 0x08, _common0_mio0SegmentRomStart, _common0_mio0SegmentRomEnd),
    LOAD_RAW         (/*seg*/ 0x0F, _common0_geoSegmentRomStart,  _common0_geoSegmentRomEnd),
    ALLOC_LEVEL_POOL(),
    MARIO(/*model*/ MODEL_MARIO, /*bhvParam*/ 0x00000001, /*bhv*/ bhvMario),
    JUMP_LINK(script_func_global_1),
    JUMP_LINK(script_func_global_3),
    JUMP_LINK(script_func_global_18),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_03,                  lll_geo_0009E0),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_04,                  lll_geo_0009F8),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_05,                  lll_geo_000A10),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_06,                  lll_geo_000A28),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_07,                  lll_geo_000A40),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_08,                  lll_geo_000A60),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_0A,                  fire_bubble_volcano_geo),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_0B,                  fire_bubble_platform_geo),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_0C,                  lll_geo_000AC0),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_0D,                  lll_geo_000AD8),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_0E,                  lll_geo_000AF0),
    LOAD_MODEL_FROM_GEO(MODEL_LLL_DRAWBRIDGE_PART,                lll_geo_000B20),
    LOAD_MODEL_FROM_GEO(MODEL_LLL_ROTATING_BLOCK_FIRE_BARS,       lll_geo_000B38),
    LOAD_MODEL_FROM_GEO(MODEL_LLL_ROTATING_HEXAGONAL_RING,        lll_geo_000BB0),
    LOAD_MODEL_FROM_GEO(MODEL_LLL_SINKING_RECTANGULAR_PLATFORM,   lll_geo_000BC8),
    LOAD_MODEL_FROM_GEO(MODEL_LLL_SINKING_SQUARE_PLATFORMS,       lll_geo_000BE0),
    LOAD_MODEL_FROM_GEO(MODEL_LLL_TILTING_SQUARE_PLATFORM,        lll_geo_000BF8),
    LOAD_MODEL_FROM_GEO(MODEL_LLL_BOWSER_PIECE_1,                 lll_geo_000C10),
    LOAD_MODEL_FROM_GEO(MODEL_LLL_BOWSER_PIECE_2,                 lll_geo_000C30),
    LOAD_MODEL_FROM_GEO(MODEL_LLL_BOWSER_PIECE_3,                 lll_geo_000C50),
    LOAD_MODEL_FROM_GEO(MODEL_LLL_BOWSER_PIECE_4,                 lll_geo_000C70),
    LOAD_MODEL_FROM_GEO(MODEL_LLL_BOWSER_PIECE_5,                 lll_geo_000C90),
    LOAD_MODEL_FROM_GEO(MODEL_LLL_BOWSER_PIECE_6,                 lll_geo_000CB0),
    LOAD_MODEL_FROM_GEO(MODEL_LLL_BOWSER_PIECE_7,                 lll_geo_000CD0),
    LOAD_MODEL_FROM_GEO(MODEL_LLL_BOWSER_PIECE_8,                 lll_geo_000CF0),
    LOAD_MODEL_FROM_GEO(MODEL_LLL_BOWSER_PIECE_9,                 lll_geo_000D10),
    LOAD_MODEL_FROM_GEO(MODEL_LLL_BOWSER_PIECE_10,                lll_geo_000D30),
    LOAD_MODEL_FROM_GEO(MODEL_LLL_BOWSER_PIECE_11,                lll_geo_000D50),
    LOAD_MODEL_FROM_GEO(MODEL_LLL_BOWSER_PIECE_12,                lll_geo_000D70),
    LOAD_MODEL_FROM_GEO(MODEL_LLL_BOWSER_PIECE_13,                lll_geo_000D90),
    LOAD_MODEL_FROM_GEO(MODEL_LLL_BOWSER_PIECE_14,                lll_geo_000DB0),
    LOAD_MODEL_FROM_GEO(MODEL_LLL_ROTATING_HEXAGONAL_PLATFORM,    lll_geo_000A78),
    LOAD_MODEL_FROM_GEO(MODEL_LLL_WOOD_BRIDGE,                    lll_geo_000B50),
    LOAD_MODEL_FROM_GEO(MODEL_LLL_FALLING_PLATFORM,               lll_geo_000B80),

    AREA(/*index*/ 1, fire_bubble_geo),
        OBJECT(/*model*/ MODEL_NONE, /*pos*/ 1285, 1154, 6273, /*angle*/ 0,   -90, 0, /*bhvParam*/ BPARAM2(WARP_NODE_0A), /*bhv*/ bhvSpinAirborneWarp),
        WARP_NODE(/*id*/ WARP_NODE_0A,      /*destLevel*/ LEVEL_LLL,    /*destArea*/ 1, /*destNode*/ WARP_NODE_0A, /*flags*/ WARP_NO_CHECKPOINT),
        WARP_NODE(/*id*/ WARP_NODE_0C,      /*destLevel*/ LEVEL_LLL,    /*destArea*/ 1, /*destNode*/ WARP_NODE_0D, /*flags*/ WARP_NO_CHECKPOINT),
        WARP_NODE(/*id*/ WARP_NODE_0D,      /*destLevel*/ LEVEL_LLL,    /*destArea*/ 1, /*destNode*/ WARP_NODE_0C, /*flags*/ WARP_NO_CHECKPOINT),
        WARP_NODE(/*id*/ WARP_NODE_SUCCESS, /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 1, /*destNode*/ WARP_NODE_34, /*flags*/ WARP_NO_CHECKPOINT),
        WARP_NODE(/*id*/ WARP_NODE_DEATH,   /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 1, /*destNode*/ WARP_NODE_66, /*flags*/ WARP_NO_CHECKPOINT),
        JUMP_LINK(script_func_local_1),
        JUMP_LINK(script_func_local_2),
        JUMP_LINK(script_func_local_3),
        JUMP_LINK(script_func_local_4),
        JUMP_LINK(script_func_local_5),
        TERRAIN(/*terrainData*/ fire_bubble_collision),
        MACRO_OBJECTS(/*objList*/ fire_bubble_macro),
        SHOW_DIALOG(/*index*/ 0x00, DIALOG_097),
        SET_BACKGROUND_MUSIC(/*settingsPreset*/ 0x0000, /*seq*/ SEQ_LEVEL_HOT),
        TERRAIN_TYPE(/*terrainType*/ TERRAIN_STONE),
    END_AREA(),

    FREE_LEVEL_POOL(),
    MARIO_POS(/*area*/ 1, /*yaw*/ -90, /*pos*/ 1252, 154, 6273),
    CALL(/*arg*/ 0, /*func*/ lvl_init_or_update),
    CALL_LOOP(/*arg*/ 1, /*func*/ lvl_init_or_update),
    CLEAR_LEVEL(),
    SLEEP_BEFORE_EXIT(/*frames*/ 1),
    EXIT(),
};
