#ifndef OBJECT_CONSTANTS_H
#define OBJECT_CONSTANTS_H

// This file contains macros that provide descriptive names for
// field-specific and object-specific constants, e.g. actions.

/* activeFlags */
#define ACTIVE_FLAG_DEACTIVATED            0         // 0x0000
#define ACTIVE_FLAG_ACTIVE                 (1 <<  0) // 0x0001
#define ACTIVE_FLAG_FAR_AWAY               (1 <<  1) // 0x0002
#define ACTIVE_FLAG_UNK2                   (1 <<  2) // 0x0004
#define ACTIVE_FLAG_IN_DIFFERENT_ROOM      (1 <<  3) // 0x0008
#define ACTIVE_FLAG_UNIMPORTANT            (1 <<  4) // 0x0010
#define ACTIVE_FLAG_INITIATED_TIME_STOP    (1 <<  5) // 0x0020
#define ACTIVE_FLAG_MOVE_THROUGH_GRATE     (1 <<  6) // 0x0040
#define ACTIVE_FLAG_DITHERED_ALPHA         (1 <<  7) // 0x0080
#define ACTIVE_FLAG_UNK8                   (1 <<  8) // 0x0100
#define ACTIVE_FLAG_UNK9                   (1 <<  9) // 0x0200
#define ACTIVE_FLAG_UNK10                  (1 << 10) // 0x0400

/* respawnInfoType */
#define RESPAWN_INFO_TYPE_NULL 0
#define RESPAWN_INFO_TYPE_32   1
#define RESPAWN_INFO_TYPE_16   2

/* respawnInfo */
#define RESPAWN_INFO_DONT_RESPAWN 0xFF

/* oFlags */
#define OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE         (1 <<  0) // 0x00000001
#define OBJ_FLAG_MOVE_XZ_USING_FVEL               (1 <<  1) // 0x00000002
#define OBJ_FLAG_MOVE_Y_WITH_TERMINAL_VEL         (1 <<  2) // 0x00000004
#define OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW         (1 <<  3) // 0x00000008
#define OBJ_FLAG_SET_FACE_ANGLE_TO_MOVE_ANGLE     (1 <<  4) // 0x00000010
#define OBJ_FLAG_0020                             (1 <<  5) // 0x00000020
#define OBJ_FLAG_COMPUTE_DIST_TO_MARIO            (1 <<  6) // 0x00000040
#define OBJ_FLAG_ACTIVE_FROM_AFAR                 (1 <<  7) // 0x00000080
#define OBJ_FLAG_0100                             (1 <<  8) // 0x00000100
#define OBJ_FLAG_TRANSFORM_RELATIVE_TO_PARENT     (1 <<  9) // 0x00000200
#define OBJ_FLAG_HOLDABLE                         (1 << 10) // 0x00000400
#define OBJ_FLAG_SET_THROW_MATRIX_FROM_TRANSFORM  (1 << 11) // 0x00000800
#define OBJ_FLAG_1000                             (1 << 12) // 0x00001000
#define OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO           (1 << 13) // 0x00002000
#define OBJ_FLAG_PERSISTENT_RESPAWN               (1 << 14) // 0x00004000
#define OBJ_FLAG_8000                             (1 << 15) // 0x00008000
#define OBJ_FLAG_30                               (1 << 30) // 0x40000000

/* oHeldState */
#define HELD_FREE    0
#define HELD_HELD    1
#define HELD_THROWN  2
#define HELD_DROPPED 3

/* oMoveFlags */
#define OBJ_MOVE_LANDED                (1 <<  0) // 0x0001
#define OBJ_MOVE_ON_GROUND             (1 <<  1) // 0x0002  // mutually exclusive to OBJ_MOVE_LANDED
#define OBJ_MOVE_LEFT_GROUND           (1 <<  2) // 0x0004
#define OBJ_MOVE_ENTERED_WATER         (1 <<  3) // 0x0008
#define OBJ_MOVE_AT_WATER_SURFACE      (1 <<  4) // 0x0010
#define OBJ_MOVE_UNDERWATER_OFF_GROUND (1 <<  5) // 0x0020
#define OBJ_MOVE_UNDERWATER_ON_GROUND  (1 <<  6) // 0x0040
#define OBJ_MOVE_IN_AIR                (1 <<  7) // 0x0080
#define OBJ_MOVE_OUT_SCOPE             (1 <<  8) // 0x0100
#define OBJ_MOVE_HIT_WALL              (1 <<  9) // 0x0200
#define OBJ_MOVE_HIT_EDGE              (1 << 10) // 0x0400
#define OBJ_MOVE_ABOVE_LAVA            (1 << 11) // 0x0800
#define OBJ_MOVE_LEAVING_WATER         (1 << 12) // 0x1000
#define OBJ_MOVE_BOUNCE                (1 << 13) // 0x2000

#define OBJ_MOVE_MASK_ON_GROUND (OBJ_MOVE_LANDED | OBJ_MOVE_ON_GROUND)
#define OBJ_MOVE_MASK_IN_WATER ( \
    OBJ_MOVE_ENTERED_WATER | \
    OBJ_MOVE_AT_WATER_SURFACE | \
    OBJ_MOVE_UNDERWATER_OFF_GROUND | \
    OBJ_MOVE_UNDERWATER_ON_GROUND)

/* oActiveParticleFlags */
#define ACTIVE_PARTICLE_DUST                 (1 <<  0) // 0x00000001
#define ACTIVE_PARTICLE_UNUSED_1             (1 <<  1) // 0x00000002 (stf_splash)
#define ACTIVE_PARTICLE_UNUSED_2             (1 <<  2) // 0x00000004 (stf_sleep)
#define ACTIVE_PARTICLE_SPARKLES             (1 <<  3) // 0x00000008
#define ACTIVE_PARTICLE_H_STAR               (1 <<  4) // 0x00000010
#define ACTIVE_PARTICLE_BUBBLE               (1 <<  5) // 0x00000020
#define ACTIVE_PARTICLE_WATER_SPLASH         (1 <<  6) // 0x00000040
#define ACTIVE_PARTICLE_IDLE_WATER_WAVE      (1 <<  7) // 0x00000080
#define ACTIVE_PARTICLE_SHALLOW_WATER_WAVE   (1 <<  8) // 0x00000100
#define ACTIVE_PARTICLE_PLUNGE_BUBBLE        (1 <<  9) // 0x00000200
#define ACTIVE_PARTICLE_WAVE_TRAIL           (1 << 10) // 0x00000400
#define ACTIVE_PARTICLE_FIRE                 (1 << 11) // 0x00000800
#define ACTIVE_PARTICLE_SHALLOW_WATER_SPLASH (1 << 12) // 0x00001000
#define ACTIVE_PARTICLE_LEAF                 (1 << 13) // 0x00002000
//#define ACTIVE_PARTICLE_DIRT                 (1 << 14) // 0x00004000
//#define ACTIVE_PARTICLE_MIST_CIRCLE          (1 << 15) // 0x00008000
//#define ACTIVE_PARTICLE_SNOW                 (1 << 16) // 0x00010000
//#define ACTIVE_PARTICLE_BREATH               (1 << 17) // 0x00020000
//#define ACTIVE_PARTICLE_V_STAR               (1 << 18) // 0x00040000
//#define ACTIVE_PARTICLE_TRIANGLE             (1 << 19) // 0x00080000

/* oAction */
#define OBJ_ACT_LAVA_DEATH        100
#define OBJ_ACT_DEATH_PLANE_DEATH 101

#define OBJ_ACT_HORIZONTAL_KNOCKBACK 100
#define OBJ_ACT_VERTICAL_KNOCKBACK   101
#define OBJ_ACT_SQUISHED             102

/* Slider Coin */
    /* oAction */
    #define SLD_COIN_ACT_STILL  0
    #define SLD_COIN_ACT_MOVING 1

/* Moving Coin */
    /* oAction */
    #define MOV_YCOIN_ACT_IDLE              0
    #define MOV_YCOIN_ACT_BLINKING          1
    #define MOV_YCOIN_ACT_LAVA_DEATH        100
    #define MOV_YCOIN_ACT_DEATH_PLANE_DEATH 101

/* Bowser */
    /* Tail oAction */
    #define BOWSER_ACT_TAIL_DEFAULT         0
    #define BOWSER_ACT_TAIL_THROWN          1
    #define BOWSER_ACT_TAIL_TOUCHED_MARIO   2

    /* oAction */
    #define BOWSER_ACT_DEFAULT              0
    #define BOWSER_ACT_THROWN               1
    #define BOWSER_ACT_JUMP_ONTO_STAGE      2
    #define BOWSER_ACT_DANCE                3
    #define BOWSER_ACT_DEAD                 4
    #define BOWSER_ACT_WAIT                 5
    #define BOWSER_ACT_HIT_EDGE             6
    #define BOWSER_ACT_TURN_FROM_EDGE       7
    #define BOWSER_ACT_HIT_MINE             8
    #define BOWSER_ACT_WALK_TO_MARIO        9
    #define BOWSER_ACT_BREATH_FIRE          10
    #define BOWSER_ACT_QUICK_JUMP           11
    #define BOWSER_ACT_IDLE                 12 // unused in the final game (BOWSER_ACT_UNUSED_SLOW_WALK)

    /* Animations */
    #define BOWSER_ANIM_STAND_UP            0
    #define BOWSER_ANIM_STAND_UP_UNUSED     1 // slightly different
    #define BOWSER_ANIM_SHAKING             2
    #define BOWSER_ANIM_GRABBED             3
    #define BOWSER_ANIM_BROKEN              4 // broken animation
    #define BOWSER_ANIM_FALL_DOWN           5 // unused
    #define BOWSER_ANIM_BREATH              6
    #define BOWSER_ANIM_JUMP                7 // unused, short jump, replaced by start/stop
    #define BOWSER_ANIM_JUMP_STOP           8
    #define BOWSER_ANIM_JUMP_START          9
    #define BOWSER_ANIM_DANCE               10
    #define BOWSER_ANIM_BREATH_UP           11
    #define BOWSER_ANIM_IDLE                12
    #define BOWSER_ANIM_SLOW_GAIT           13
    #define BOWSER_ANIM_LOOK_DOWN_STOP_WALK 14
    #define BOWSER_ANIM_LOOK_UP_START_WALK  15
    #define BOWSER_ANIM_FLIP_DOWN           16
    #define BOWSER_ANIM_LAY_DOWN            17
    #define BOWSER_ANIM_RUN_START           18
    #define BOWSER_ANIM_RUN                 19
    #define BOWSER_ANIM_RUN_STOP            20
    #define BOWSER_ANIM_RUN_SLIP            21
    #define BOWSER_ANIM_BREATH_QUICK        22
    #define BOWSER_ANIM_EDGE_MOVE           23
    #define BOWSER_ANIM_EDGE_STOP           24
    #define BOWSER_ANIM_FLIP                25
    #define BOWSER_ANIM_STAND_UP_FROM_FLIP  26

    /* oBowserCamAct */
    #define BOWSER_CAM_ACT_IDLE 0
    #define BOWSER_CAM_ACT_WALK 1
    #define BOWSER_CAM_ACT_END  2

    /* oBowserStatus */
    #define BOWSER_STATUS_ANGLE_MARIO   (1 <<  1) // 0x00000002
    #define BOWSER_STATUS_ANGLE_CENTER  (1 <<  2) // 0x00000004
    #define BOWSER_STATUS_DIST_MARIO    (1 <<  3) // 0x00000008
    #define BOWSER_STATUS_DIST_CENTER   (1 <<  4) // 0x00000010

    /* oBowserGrabbedStatus */
    #define BOWSER_GRAB_STATUS_NONE     0
    #define BOWSER_GRAB_STATUS_GRABBED  1
    #define BOWSER_GRAB_STATUS_HOLDING  2

    /* oSubAction */
    #define BOWSER_SUB_ACT_DEAD_FLY_BACK            0
    #define BOWSER_SUB_ACT_DEAD_BOUNCE              1
    #define BOWSER_SUB_ACT_DEAD_WAIT                2
    #define BOWSER_SUB_ACT_DEAD_DEFAULT_END         3
    #define BOWSER_SUB_ACT_DEAD_DEFAULT_END_OVER    4

    #define BOWSER_SUB_ACT_HIT_MINE_START           0
    #define BOWSER_SUB_ACT_HIT_MINE_FALL            1
    #define BOWSER_SUB_ACT_HIT_MINE_STOP            2

    #define BOWSER_SUB_ACT_JUMP_ON_STAGE_IDLE       0
    #define BOWSER_SUB_ACT_JUMP_ON_STAGE_START      1
    #define BOWSER_SUB_ACT_JUMP_ON_STAGE_LAND       2
    #define BOWSER_SUB_ACT_JUMP_ON_STAGE_STOP       3

/* Fish Spawer */
    /* oAction */
    #define FISH_SPAWNER_ACT_SPAWN   0
    #define FISH_SPAWNER_ACT_IDLE    1
    #define FISH_SPAWNER_ACT_RESPAWN 2

    /* oBhvParams2ndByte */
    #define FISH_SPAWNER_BP_MANY_BLUE 0
    #define FISH_SPAWNER_BP_FEW_BLUE  1
    #define FISH_SPAWNER_BP_MANY_CYAN 2
    #define FISH_SPAWNER_BP_FEW_CYAN  3

/* Fish */
    /* oAction */
    #define FISH_ACT_INIT 0
    #define FISH_ACT_ROAM 1
    #define FISH_ACT_FLEE 2

/* Blue Fish */
    /* oAction */
    #define BLUE_FISH_ACT_DIVE      0
    #define BLUE_FISH_ACT_TURN      1
    #define BLUE_FISH_ACT_ASCEND    2
    #define BLUE_FISH_ACT_TURN_BACK 3

    /* oAction: bhv_blue_fish_spawn_loop */
    #define BLUE_FISH_ACT_SPAWN     0
    #define BLUE_FISH_ACT_ROOM      1
    #define BLUE_FISH_ACT_DUPLICATE 2

/* Hoot */
    /* oHootAvailability */
    #define HOOT_AVAIL_ASLEEP_IN_TREE 0
    #define HOOT_AVAIL_WANTS_TO_TALK  1
    #define HOOT_AVAIL_READY_TO_FLY   2
    
    /* oAction */
    #define HOOT_ACT_CARRY 0

/* Bully (All variants) */
    /* oBhvParams2ndByte */
    #define BULLY_BP_SIZE_SMALL 0
    #define BULLY_BP_SIZE_BIG   1

    /* oAction */
    #define BULLY_ACT_PATROL            0
    #define BULLY_ACT_CHASE_MARIO       1
    #define BULLY_ACT_KNOCKBACK         2
    #define BULLY_ACT_BACK_UP           3
    #define BULLY_ACT_INACTIVE          4
    #define BULLY_ACT_ACTIVATE_AND_FALL 5
    #define BULLY_ACT_LAVA_DEATH        100
    #define BULLY_ACT_DEATH_PLANE_DEATH 101

    /* oBullySubtype */
    #define BULLY_STYPE_GENERIC 0
    #define BULLY_STYPE_MINION  1
    #define BULLY_STYPE_CHILL   16

/* Water Ring (All variants) */
    /* oAction */
    #define WATER_RING_ACT_NOT_COLLECTED 0
    #define WATER_RING_ACT_COLLECTED     1

/* Jet Stream Water Ring Spawner */
    /* oAction */
    #define JS_RING_SPAWNER_ACT_ACTIVE   0
    #define JS_RING_SPAWNER_ACT_INACTIVE 1

/* Celebration Star */
    /* oAction */
    #define CELEB_STAR_ACT_SPIN_AROUND_MARIO 0
    #define CELEB_STAR_ACT_FACE_CAMERA       1

/* LLL Drawbridge */
    /* oAction */
    #define LLL_DRAWBRIDGE_ACT_LOWER 0
    #define LLL_DRAWBRIDGE_ACT_RAISE 1

/* Bomp (Both variants) */
    /* oAction */
    #define BOMP_ACT_WAIT     0
    #define BOMP_ACT_POKE_OUT 1
    #define BOMP_ACT_EXTEND   2
    #define BOMP_ACT_RETRACT  3

/* WF Sliding Brick Platform */
    /* oBhvParams2ndByte */
    #define WF_SLID_BRICK_PTFM_BP_MOV_VEL_10 1
    #define WF_SLID_BRICK_PTFM_BP_MOV_VEL_15 2
    #define WF_SLID_BRICK_PTFM_BP_MOV_VEL_20 3

    /* oAction */
    #define WF_SLID_BRICK_PTFM_ACT_WAIT    0
    #define WF_SLID_BRICK_PTFM_ACT_EXTEND  1
    #define WF_SLID_BRICK_PTFM_ACT_RETRACT 2

/* Fake Moneybag Coin */
    /* oAction */
    #define FAKE_MONEYBAG_COIN_ACT_IDLE      0
    #define FAKE_MONEYBAG_COIN_ACT_TRANSFORM 1

/* Beta Chest Lid */
    /* oAction */
    #define BETA_CHEST_ACT_IDLE_CLOSED 0
    #define BETA_CHEST_ACT_OPENING     1
    #define BETA_CHEST_ACT_IDLE_OPEN   2

/* Boo */
    /* oBooDeathStatus */
    #define BOO_DEATH_STATUS_ALIVE 0
    #define BOO_DEATH_STATUS_DYING 1
    #define BOO_DEATH_STATUS_DEAD  2

    /* attackStatus */
    #define BOO_NOT_ATTACKED 0
    #define BOO_ATTACKED     1
    #define BOO_BOUNCED_ON   -1

    /* oBhvParams2ndByte */
    #define BOO_BP_GHOST_HUNT     0
    #define BOO_BP_GENERIC        1
    #define BOO_BP_MERRY_GO_ROUND 2

/* Beta Boo Key */
    /* oAction */
    #define BETA_BOO_KEY_ACT_IN_BOO   0
    #define BETA_BOO_KEY_ACT_DROPPING 1
    #define BETA_BOO_KEY_ACT_DROPPED  2

/* Piranha Plant */
    /* oAction */
    #define PIRANHA_PLANT_ACT_IDLE            0
    #define PIRANHA_PLANT_ACT_SLEEPING        1
    #define PIRANHA_PLANT_ACT_BITING          2
    #define PIRANHA_PLANT_ACT_WOKEN_UP        3
    #define PIRANHA_PLANT_ACT_STOPPED_BITING  4
    #define PIRANHA_PLANT_ACT_ATTACKED        5
    #define PIRANHA_PLANT_ACT_SHRINK_AND_DIE  6
    #define PIRANHA_PLANT_ACT_WAIT_TO_RESPAWN 7
    #define PIRANHA_PLANT_ACT_RESPAWN         8

    /* oPiranhaPlantSleepMusicState */
    #define PIRANHA_PLANT_SLEEP_MUSIC_PLAYING 0

/* Piranha Plant Bubble */
    /* oAction */
    #define PIRANHA_PLANT_BUBBLE_ACT_IDLE             0
    #define PIRANHA_PLANT_BUBBLE_ACT_GROW_SHRINK_LOOP 1
    #define PIRANHA_PLANT_BUBBLE_ACT_BURST            2

/* Penguins (General) */
    /* Walking sounds */
    #define PENGUIN_WALK_BABY 0
    #define PENGUIN_WALK_BIG  1

    /* Animations */
    #define PENGUIN_ANIM_WALK 0
    #define PENGUIN_ANIM_IDLE 3

/* Triplet Butterfly */
    /* oAction */
    #define TRIPLET_BUTTERFLY_ACT_INIT     0
    #define TRIPLET_BUTTERFLY_ACT_WANDER   1

    /* oBhvParams2ndByte */
    #define TRIPLET_BUTTERFLY_BP_SPAWN_TYPE_SPAWNER   (0 << 0)
    #define TRIPLET_BUTTERFLY_BP_SPAWN_TYPE_SPAWNED_1 (1 << 0)
    #define TRIPLET_BUTTERFLY_BP_SPAWN_TYPE_SPAWNED_2 (1 << 1)
    #define TRIPLET_BUTTERFLY_BP_SPAWN_TYPE_MASK      (TRIPLET_BUTTERFLY_BP_SPAWN_TYPE_SPAWNER | \
                                                       TRIPLET_BUTTERFLY_BP_SPAWN_TYPE_SPAWNED_1 | \
                                                       TRIPLET_BUTTERFLY_BP_SPAWN_TYPE_SPAWNED_2)

/* Bowser Puzzle */
    /* oAction */
    #define BOWSER_PUZZLE_ACT_SPAWN_PIECES      0
    #define BOWSER_PUZZLE_ACT_WAIT_FOR_COMPLETE 1
    #define BOWSER_PUZZLE_ACT_DONE              2

/* WF Rotating Wooden Platform */
    /* oAction */
    #define WF_ROTATING_WOODEN_PLATFORM_ACT_IDLE     0
    #define WF_ROTATING_WOODEN_PLATFORM_ACT_ROTATING 1

/* Rotating Platform */
    /* oBhvParams2ndByte */
    #define ROTATING_PLATFORM_BP_WF  0
    #define ROTATING_PLATFORM_BP_WDW 1

/* Tumbling Bridge */
    /* oBhvParams2ndByte */
    #define TUMBLING_BRIDGE_BP_WF    0
    #define TUMBLING_BRIDGE_BP_LLL   1

/* Thwomp */
    /* oAction */
    #define THWOMP_ACT_RAISE          0
    #define THWOMP_ACT_IDLE_AT_TOP    1
    #define THWOMP_ACT_LOWER          2
    #define THWOMP_ACT_LAND           3
    #define THWOMP_ACT_IDLE_AT_BOTTOM 4

#endif // OBJECT_CONSTANTS_H
