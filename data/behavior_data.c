#define OBJECT_FIELDS_INDEX_DIRECTLY

#include "sm64.h"

#include "object_constants.h"
#include "game/object_list_processor.h"
#include "game/interaction.h"
#include "game/behavior_actions.h"
#include "game/mario_actions_cutscene.h"
#include "game/mario_misc.h"
#include "game/object_helpers.h"
#include "game/debug.h"
#include "engine/surface_load.h"

#include "actors/common0.h"
#include "actors/common1.h"
#include "actors/group1.h"
#include "actors/group2.h"
#include "actors/group4.h"
#include "actors/group7.h"
#include "actors/group9.h"
#include "actors/group10.h"
#include "actors/group12.h"
#include "actors/group13.h"
#include "actors/group14.h"
#include "actors/group17.h"
#include "levels/castle_inside/header.h"
#include "levels/castle_grounds/header.h"
#include "levels/lll/header.h"
#include "levels/ddd/header.h"
#include "levels/wf/header.h"
#include "levels/ccm/header.h"
#include "levels/ttc/header.h"

#include "make_const_nonconst.h"
#include "behavior_data.h"

#define _SHIFTL(v, s, w)	\
    ((unsigned int) (((unsigned int)(v) & ((0x01 << (w)) - 1)) << (s)))

#define BC_B(a) _SHIFTL(a, 24, 8)
#define BC_BB(a, b) (_SHIFTL(a, 24, 8) | _SHIFTL(b, 16, 8))
#define BC_BBBB(a, b, c, d) (_SHIFTL(a, 24, 8) | _SHIFTL(b, 16, 8) | _SHIFTL(c, 8, 8) | _SHIFTL(d, 0, 8))
#define BC_BBH(a, b, c) (_SHIFTL(a, 24, 8) | _SHIFTL(b, 16, 8) | _SHIFTL(c, 0, 16))
#define BC_B0H(a, b) (_SHIFTL(a, 24, 8) | _SHIFTL(b, 0, 16))
#define BC_H(a) _SHIFTL(a, 16, 16)
#define BC_HH(a, b) (_SHIFTL(a, 16, 16) | _SHIFTL(b, 0, 16))
#define BC_W(a) ((uintptr_t)(u32)(a))
#define BC_PTR(a) ((uintptr_t)(a))

// Defines the start of the behavior script as well as the object list the object belongs to.
// Has some special behavior for certain objects.
#define BEGIN(objList) \
    BC_BB(0x00, objList)

// Delays the behavior script for a certain number of frames.
#define DELAY(num) \
    BC_B0H(0x01, num)

// Jumps to a new behavior command and stores the return address in the object's stack.
#define CALL(addr) \
    BC_B(0x02), \
    BC_PTR(addr)

// Jumps back to the behavior command stored in the object's stack.
#define RETURN() \
    BC_B(0x03)

// Jumps to a new behavior script without saving anything.
#define GOTO(addr) \
    BC_B(0x04), \
    BC_PTR(addr)

// Marks the start of a loop that will repeat a certain number of times.
#define BEGIN_REPEAT(count) \
    BC_B0H(0x05, count)

// Marks the end of a repeating loop.
#define END_REPEAT() \
    BC_B(0x06)

// Also marks the end of a repeating loop, but continues executing commands following the loop on the same frame.
#define END_REPEAT_CONTINUE() \
    BC_B(0x07)

// Marks the beginning of an infinite loop.
#define BEGIN_LOOP() \
    BC_B(0x08)

// Marks the end of an infinite loop.
#define END_LOOP() \
    BC_B(0x09)

// Exits the behavior script.
// Often used to end behavior scripts that do not contain an infinite loop.
#define BREAK() \
    BC_B(0x0A)

// Exits the behavior script, unused.
#define BREAK_UNUSED() \
    BC_B(0x0B)

// Executes a native game function.
#define CALL_NATIVE(func) \
    BC_B(0x0C), \
    BC_PTR(func)

// Adds a float to the specified field.
#define ADD_FLOAT(field, value) \
    BC_BBH(0x0D, field, value)

// Sets the specified field to a float.
#define SET_FLOAT(field, value) \
    BC_BBH(0x0E, field, value)

// Adds an integer to the specified field.
#define ADD_INT(field, value) \
    BC_BBH(0x0F, field, value)

// Sets the specified field to an integer.
#define SET_INT(field, value) \
    BC_BBH(0x10, field, value)

// Performs a bitwise OR with the specified field and the given integer.
// Usually used to set an object's flags.
#define OR_INT(field, value) \
    BC_BBH(0x11, field, value)

// Performs a bit clear with the specified short. Unused in favor of the 32-bit version.
#define BIT_CLEAR(field, value) \
    BC_BBH(0x12, field, value)

// TODO: this one needs a better name / labelling
// Gets a random short, right shifts it the specified amount and adds min to it, then sets the specified field to that value.
#define SET_INT_RAND_RSHIFT(field, min, rshift) \
    BC_BBH(0x13, field, min), \
    BC_H(rshift)

// Sets the specified field to a random float in the given range.
#define SET_RANDOM_FLOAT(field, min, range) \
    BC_BBH(0x14, field, min), \
    BC_H(range)

// Sets the specified field to a random integer in the given range.
#define SET_RANDOM_INT(field, min, range) \
    BC_BBH(0x15, field, min), \
    BC_H(range)

// Adds a random float in the given range to the specified field.
#define ADD_RANDOM_FLOAT(field, min, range) \
    BC_BBH(0x16, field, min), \
    BC_H(range)

// TODO: better name (unused anyway)
// Gets a random short, right shifts it the specified amount and adds min to it, then adds the value to the specified field. Unused.
#define ADD_INT_RAND_RSHIFT(field, min, rshift) \
    BC_BBH(0x17, field, min), \
    BC_H(rshift)

// No operation. Unused.
#define CMD_NOP_1(field) \
    BC_BB(0x18, field)

// No operation. Unused.
#define CMD_NOP_2(field) \
    BC_BB(0x19, field)

// No operation. Unused.
#define CMD_NOP_3(field) \
    BC_BB(0x1A, field)

// Sets the current model ID of the object.
#define SET_MODEL(modelID) \
    BC_B0H(0x1B, modelID)

// Spawns a child object with the specified model and behavior.
#define SPAWN_CHILD(modelID, behavior) \
    BC_B(0x1C), \
    BC_W(modelID), \
    BC_PTR(behavior)

// Exits the behavior script and despawns the object.
// Often used to end behavior scripts that do not contain an infinite loop.
#define DEACTIVATE() \
    BC_B(0x1D)

// Finds the floor triangle directly under the object and moves the object down to it.
#define DROP_TO_FLOOR() \
    BC_B(0x1E)

// Sets the destination float field to the sum of the values of the given float fields.
#define SUM_FLOAT(fieldDst, fieldSrc1, fieldSrc2) \
    BC_BBBB(0x1F, fieldDst, fieldSrc1, fieldSrc2)

// Sets the destination integer field to the sum of the values of the given integer fields. Unused.
#define SUM_INT(fieldDst, fieldSrc1, fieldSrc2) \
    BC_BBBB(0x20, fieldDst, fieldSrc1, fieldSrc2)

// Billboards the current object, making it always face the camera.
#define BILLBOARD() \
    BC_B(0x21)

// Hides the current object.
#define HIDE() \
    BC_B(0x22)

// Sets the size of the object's cylindrical hitbox.
#define SET_HITBOX(radius, height) \
    BC_B(0x23), \
    BC_HH(radius, height)

// No operation. Unused.
#define CMD_NOP_4(field, value) \
    BC_BBH(0x24, field, value)

// Delays the behavior script for the number of frames given by the value of the specified field.
#define DELAY_VAR(field) \
    BC_BB(0x25, field)

// Unused. Marks the start of a loop that will repeat a certain number of times.
// Uses a u8 as the argument, instead of a s16 like the other version does.
#define BEGIN_REPEAT_UNUSED(count) \
    BC_BB(0x26, count)

// Loads the animations for the object. <field> is always set to oAnimations.
#define LOAD_ANIMATIONS(field, anims) \
    BC_BB(0x27, field), \
    BC_PTR(anims)

// Begins animation and sets the object's current animation index to the specified value.
#define ANIMATE(animIndex) \
    BC_BB(0x28, animIndex)

// Spawns a child object with the specified model and behavior, plus a behavior param.
#define SPAWN_CHILD_WITH_PARAM(bhvParam, modelID, behavior) \
    BC_B0H(0x29, bhvParam), \
    BC_W(modelID), \
    BC_PTR(behavior)

// Loads collision data for the object.
#define LOAD_COLLISION_DATA(collisionData) \
    BC_B(0x2A), \
    BC_PTR(collisionData)

// Sets the size of the object's cylindrical hitbox, and applies a downwards offset.
#define SET_HITBOX_WITH_OFFSET(radius, height, downOffset) \
    BC_B(0x2B), \
    BC_HH(radius, height), \
    BC_H(downOffset)

// Spawns a new object with the specified model and behavior.
#define SPAWN_OBJ(modelID, behavior) \
    BC_B(0x2C), \
    BC_W(modelID), \
    BC_PTR(behavior)

// Sets the home position of the object to its current position.
#define SET_HOME() \
    BC_B(0x2D)

// Sets the size of the object's cylindrical hurtbox.
#define SET_HURTBOX(radius, height) \
    BC_B(0x2E), \
    BC_HH(radius, height)

// Sets the object's interaction type.
#define SET_INTERACT_TYPE(type) \
    BC_B(0x2F), \
    BC_W(type)

// Sets various parameters that the object uses for calculating physics.
#define SET_OBJ_PHYSICS(wallHitboxRadius, gravity, bounciness, dragStrength, friction, buoyancy, unused1, unused2) \
    BC_B(0x30), \
    BC_HH(wallHitboxRadius, gravity), \
    BC_HH(bounciness, dragStrength), \
    BC_HH(friction, buoyancy), \
    BC_HH(unused1, unused2)

// Sets the object's interaction subtype. Unused.
#define SET_INTERACT_SUBTYPE(subtype) \
    BC_B(0x31), \
    BC_W(subtype)

// Sets the object's size to the specified percentage.
#define SCALE(unusedField, percent) \
    BC_BBH(0x32, unusedField, percent)

// Performs a bit clear on the object's parent's field with the specified value.
// Used for clearing active particle flags fron Mario's object.
#define PARENT_BIT_CLEAR(field, flags) \
    BC_BB(0x33, field), \
    BC_W(flags)

// Animates an object using texture animation. <field> is always set to oAnimState.
#define ANIMATE_TEXTURE(field, rate) \
    BC_BBH(0x34, field, rate)

// Disables rendering for the object.
#define DISABLE_RENDERING() \
    BC_B(0x35)

// Unused. Sets the specified field to an integer. Wastes 4 bytes of space for no reason at all.
#define SET_INT_UNUSED(field, value) \
    BC_BB(0x36, field), \
    BC_HH(0, value)

// Spawns a water droplet with the given parameters.
#define SPAWN_WATER_DROPLET(dropletParams) \
    BC_B(0x37), \
    BC_PTR(dropletParams)

const BehaviorScript bhvMrI[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_MOVE_XZ_USING_FVEL | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SET_HOME(),
    SPAWN_CHILD(/*Model*/ MODEL_MR_I_IRIS, /*Behavior*/ bhvMrIBody),
    SET_MODEL(MODEL_MR_I),
    BILLBOARD(),
    CALL_NATIVE(bhv_init_room),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_mr_i_loop),
    END_LOOP(),
};

const BehaviorScript bhvBobomb[] = {
    BEGIN(OBJ_LIST_DESTRUCTIVE),
    OR_INT(oFlags, (OBJ_FLAG_PERSISTENT_RESPAWN | OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_HOLDABLE | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_ANIMATIONS(oAnimations, bom_anime),
    DROP_TO_FLOOR(),
    ANIMATE(0),
    SET_INT(oIntangibleTimer, 0),
    SET_HOME(),
    CALL_NATIVE(bhv_bobomb_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_bobomb_loop),
    END_LOOP(),
};

const BehaviorScript bhvBobombFuseSmoke[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BILLBOARD(),
    SET_INT(oAnimState, -1),
    CALL_NATIVE(bhv_bobomb_fuse_smoke_init),
    DELAY(1),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_dust_smoke_loop),
        ADD_INT(oAnimState, 1),
    END_LOOP(),
};

const BehaviorScript bhvKoopa[] = {
    BEGIN(OBJ_LIST_PUSHABLE),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    DROP_TO_FLOOR(),
    LOAD_ANIMATIONS(oAnimations, nokonoko_anime),
    ANIMATE(9),
    SET_HOME(),
    SET_OBJ_PHYSICS(/*Wall hitbox radius*/ 50, /*Gravity*/ -400, /*Bounciness*/ 0, /*Drag strength*/ 0, /*Friction*/ 1000, /*Buoyancy*/ 200, /*Unused*/ 0, 0),
    SCALE(/*Unused*/ 0, /*Field*/ 150),
    SET_FLOAT(oKoopaAgility, 1),
    CALL_NATIVE(bhv_koopa_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_koopa_update),
    END_LOOP(),
};

const BehaviorScript bhvMrIBody[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    CALL_NATIVE(bhv_init_room),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_mr_i_body_loop),
    END_LOOP(),
};

const BehaviorScript bhvMrIParticle[] = {
    BEGIN(OBJ_LIST_LEVEL),
    BILLBOARD(),
    OR_INT(oFlags, (OBJ_FLAG_MOVE_XZ_USING_FVEL | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SET_INT(oIntangibleTimer, 0),
    SET_HITBOX(50, 50),
    SET_INT(oDamageOrCoinValue, 1),
    SET_INT(oInteractType, INTERACT_DAMAGE),
    SET_OBJ_PHYSICS(/*Wall hitbox radius*/ 30, /*Gravity*/ 0, /*Bounciness*/ 0, /*Drag strength*/ 0, /*Friction*/ 0, /*Buoyancy*/ 0, /*Unused*/ 0, 0),
    CALL_NATIVE(bhv_init_room),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_mr_i_particle_loop),
    END_LOOP(),
};

const BehaviorScript bhvPurpleParticle[] = {
    BEGIN(OBJ_LIST_UNIMPORTANT),
    BILLBOARD(),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BEGIN_REPEAT(10),
        CALL_NATIVE(bhv_piranha_particle_loop),
    END_REPEAT(),
    DEACTIVATE(),
};

const BehaviorScript bhvGiantPole[] = {
    BEGIN(OBJ_LIST_POLELIKE),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    SET_INT(oInteractType, INTERACT_POLE),
    SET_HITBOX(/*Radius*/ 80, /*Height*/ 1350),
    SET_HOME(),
    SET_INT(oIntangibleTimer, 0),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_giant_pole_loop),
    END_LOOP(),
};

const BehaviorScript bhvPoleGrabbing[] = {
    BEGIN(OBJ_LIST_POLELIKE),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    SET_INT(oInteractType, INTERACT_POLE),
    SET_HITBOX(/*Radius*/ 80, /*Height*/ 1500),
    CALL_NATIVE(bhv_pole_init),
    SET_INT(oIntangibleTimer, 0),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_pole_base_loop),
    END_LOOP(),
};

const BehaviorScript bhvBetaChestBottom[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, (OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    DROP_TO_FLOOR(),
    CALL_NATIVE(bhv_beta_chest_bottom_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_beta_chest_bottom_loop),
    END_LOOP(),
};

const BehaviorScript bhvBetaChestLid[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, (OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SET_HOME(),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_beta_chest_lid_loop),
    END_LOOP(),
};

const BehaviorScript bhvBubbleParticleSpawner[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    DISABLE_RENDERING(),
    SET_RANDOM_INT(oWaterObjUnkF4, /*Minimum*/ 2, /*Range*/ 9),
    DELAY_VAR(oWaterObjUnkF4),
    SPAWN_CHILD(/*Model*/ MODEL_BUBBLE, /*Behavior*/ bhvSmallWaterWave),
    PARENT_BIT_CLEAR(oActiveParticleFlags, ACTIVE_PARTICLE_BUBBLE),
    DEACTIVATE(),
};

const BehaviorScript bhvBubbleMaybe[] = {
    BEGIN(OBJ_LIST_UNIMPORTANT),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BILLBOARD(),
    CALL_NATIVE(bhv_bubble_wave_init),
    SET_RANDOM_FLOAT(oWaterObjUnkF4, /*Minimum*/ -75, /*Range*/ 150),
    SET_RANDOM_FLOAT(oWaterObjUnkF8, /*Minimum*/ -75, /*Range*/ 150),
    SET_RANDOM_FLOAT(oWaterObjUnkFC, /*Minimum*/ -75, /*Range*/ 150),
    SUM_FLOAT(/*Dest*/ oPosX, /*Value 1*/ oPosX, /*Value 2*/ oWaterObjUnkF4),
    SUM_FLOAT(/*Dest*/ oPosZ, /*Value 1*/ oPosZ, /*Value 2*/ oWaterObjUnkF8),
    SUM_FLOAT(/*Dest*/ oPosY, /*Value 1*/ oPosY, /*Value 2*/ oWaterObjUnkFC),
    SET_INT(oAnimState, -1),
    BEGIN_REPEAT(60),
        ADD_INT(oAnimState, 1),
        CALL_NATIVE(bhv_bubble_maybe_loop),
    END_REPEAT(),
    DEACTIVATE(),
};

const BehaviorScript bhvSmallWaterWave[] = {
    BEGIN(OBJ_LIST_UNIMPORTANT),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BILLBOARD(),
    CALL_NATIVE(bhv_bubble_wave_init),
    SET_RANDOM_FLOAT(oWaterObjUnkF4, /*Minimum*/ -50, /*Range*/ 100),
    SET_RANDOM_FLOAT(oWaterObjUnkF8, /*Minimum*/ -50, /*Range*/ 100),
    SUM_FLOAT(/*Dest*/ oPosX, /*Value 1*/ oPosX, /*Value 2*/ oWaterObjUnkF4),
    SUM_FLOAT(/*Dest*/ oPosZ, /*Value 1*/ oPosZ, /*Value 2*/ oWaterObjUnkF8),
    SET_RANDOM_FLOAT(oWaterObjUnkFC, /*Minimum*/ 0, /*Range*/ 50),
    SUM_FLOAT(/*Dest*/ oPosY, /*Value 1*/ oPosY, /*Value 2*/ oWaterObjUnkFC),
    SET_INT(oAnimState, -1),
    CALL(bhvSmallWaterWave398),
    BEGIN_REPEAT(60),
        CALL(bhvSmallWaterWave398),
        CALL_NATIVE(bhv_small_water_wave_loop),
    END_REPEAT(),
    DEACTIVATE(),
};

const BehaviorScript bhvSmallWaterWave398[] = {
    ADD_INT(oAnimState, 1),
    ADD_FLOAT(oPosY, 7),
    SET_RANDOM_FLOAT(oWaterObjUnkF4, /*Minimum*/ -2, /*Range*/ 5),
    SET_RANDOM_FLOAT(oWaterObjUnkF8, /*Minimum*/ -2, /*Range*/ 5),
    SUM_FLOAT(/*Dest*/ oPosX, /*Value 1*/ oPosX, /*Value 2*/ oWaterObjUnkF4),
    SUM_FLOAT(/*Dest*/ oPosZ, /*Value 1*/ oPosZ, /*Value 2*/ oWaterObjUnkF8),
    RETURN(),
};

const BehaviorScript bhvWaterAirBubble[] = {
    BEGIN(OBJ_LIST_LEVEL),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BILLBOARD(),
    SET_HITBOX_WITH_OFFSET(/*Radius*/ 400, /*Height*/ 150, /*Downwards offset*/ -150),
    SET_INT(oIntangibleTimer, 0),
    SET_INTERACT_TYPE(INTERACT_WATER_RING),
    SET_INT(oDamageOrCoinValue, 5),
    CALL_NATIVE(bhv_water_air_bubble_init),
    SET_INT(oAnimState, -1),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_water_air_bubble_loop),
    END_LOOP(),
};

const BehaviorScript bhvSmallParticle[] = {
    BEGIN(OBJ_LIST_UNIMPORTANT),
    BILLBOARD(),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    CALL_NATIVE(bhv_particle_init),
    BEGIN_REPEAT(70),
        CALL_NATIVE(bhv_particle_loop),
    END_REPEAT(),
    DEACTIVATE(),
};

const BehaviorScript bhvPlungeBubble[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    PARENT_BIT_CLEAR(oActiveParticleFlags, ACTIVE_PARTICLE_PLUNGE_BUBBLE),
    DISABLE_RENDERING(),
    CALL_NATIVE(bhv_water_waves_init),
    DEACTIVATE(),
};

const BehaviorScript bhvSmallParticleSnow[] = {
    BEGIN(OBJ_LIST_UNIMPORTANT),
    BILLBOARD(),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    CALL_NATIVE(bhv_particle_init),
    BEGIN_REPEAT(30),
        CALL_NATIVE(bhv_particle_loop),
    END_REPEAT(),
    DEACTIVATE(),
};

const BehaviorScript bhvSmallParticleBubbles[] = {
    BEGIN(OBJ_LIST_UNIMPORTANT),
    BILLBOARD(),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    CALL_NATIVE(bhv_particle_init),
    BEGIN_REPEAT(70),
        CALL_NATIVE(bhv_small_bubbles_loop),
    END_REPEAT(),
    DEACTIVATE(),
};

const BehaviorScript bhvFishGroup[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_fish_group_loop),
    END_LOOP(),
};

const BehaviorScript bhvCannon[] = {
    BEGIN(OBJ_LIST_LEVEL),
    OR_INT(oFlags, (OBJ_FLAG_ACTIVE_FROM_AFAR | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SPAWN_CHILD(/*Model*/ MODEL_CANNON_BARREL, /*Behavior*/ bhvCannonBarrel),
    SET_INT(oInteractType, INTERACT_CANNON_BASE),
    ADD_FLOAT(oPosY, -340),
    SET_HOME(),
    SET_HITBOX(/*Radius*/ 150, /*Height*/ 150),
    SET_INT(oIntangibleTimer, 0),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_cannon_base_loop),
    END_LOOP(),
};

const BehaviorScript bhvCannonBarrel[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, (OBJ_FLAG_ACTIVE_FROM_AFAR | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    DROP_TO_FLOOR(),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_cannon_barrel_loop),
    END_LOOP(),
};

const BehaviorScript bhvCannonBaseUnused[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, (OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_MOVE_XZ_USING_FVEL | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    BILLBOARD(),
    SET_INT(oAnimState, -1),
    BEGIN_REPEAT(8),
        CALL_NATIVE(bhv_cannon_base_unused_loop),
        ADD_INT(oAnimState, 1),
    END_REPEAT(),
    DEACTIVATE(),
};

const BehaviorScript bhvChuckya[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, (OBJ_FLAG_HOLDABLE | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_ANIMATIONS(oAnimations, carryboy_anime),
    ANIMATE(5),
    SET_INT(oInteractType, INTERACT_GRABBABLE),
    SET_HITBOX(/*Radius*/ 150, /*Height*/ 100),
    SET_OBJ_PHYSICS(/*Wall hitbox radius*/ 30, /*Gravity*/ -400, /*Bounciness*/ -50, /*Drag strength*/ 1000, /*Friction*/ 1000, /*Buoyancy*/ 200, /*Unused*/ 0, 0),
    SPAWN_OBJ(/*Model*/ MODEL_NONE, /*Behavior*/ bhvChuckyaAnchorMario),
    SET_INT(oNumLootCoins, 5),
    SET_INT(oIntangibleTimer, 0),
    SET_HOME(),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_chuckya_loop),
    END_LOOP(),
};

const BehaviorScript bhvChuckyaAnchorMario[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, (OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    BILLBOARD(),
    SET_FLOAT(oParentRelativePosY, -60),
    SET_FLOAT(oParentRelativePosZ, 150),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_chuckya_anchor_mario_loop),
    END_LOOP(),
};

const BehaviorScript bhvGoomba[] = {
    BEGIN(OBJ_LIST_PUSHABLE),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    DROP_TO_FLOOR(),
    LOAD_ANIMATIONS(oAnimations, kuribo_anime),
    SET_HOME(),
    SET_OBJ_PHYSICS(/*Wall hitbox radius*/ 40, /*Gravity*/ -400, /*Bounciness*/ -50, /*Drag strength*/ 1000, /*Friction*/ 1000, /*Buoyancy*/ 0, /*Unused*/ 0, 0),
    CALL_NATIVE(bhv_goomba_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_goomba_update),
    END_LOOP(),
};

const BehaviorScript bhvGoombaTripletSpawner[] = {
    BEGIN(OBJ_LIST_PUSHABLE),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    DROP_TO_FLOOR(),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_goomba_triplet_spawner_update),
    END_LOOP(),
};

const BehaviorScript bhvRotatingPlatform[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    SET_HOME(),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_rotating_platform_loop),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};

const BehaviorScript bhvTower[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    LOAD_COLLISION_DATA(mountain_10_collision),
    SET_FLOAT(oCollisionDistance, 3000),
    SET_FLOAT(oDrawingDistance, 20000),
    BEGIN_LOOP(),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};

const BehaviorScript bhvBulletBillCannon[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    LOAD_COLLISION_DATA(wf_seg7_collision_bullet_bill_cannon),
    SET_FLOAT(oCollisionDistance, 300),
    BEGIN_LOOP(),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};

const BehaviorScript bhvRotatingCounterClockwise[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    BREAK(),
};

const BehaviorScript bhvWFRotatingWoodenPlatform[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    LOAD_COLLISION_DATA(wf_seg7_collision_clocklike_rotation),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_wf_rotating_wooden_platform_loop),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};

const BehaviorScript bhvExitPodiumWarp[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, (OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SET_INT(oInteractType, INTERACT_WARP),
    DROP_TO_FLOOR(),
    SET_FLOAT(oCollisionDistance, 8000),
    LOAD_COLLISION_DATA(ccm_seg7_collision_podium_warp),
    SET_INT(oIntangibleTimer, 0),
    SET_HITBOX(/*Radius*/ 50, /*Height*/ 50),
    BEGIN_LOOP(),
        CALL_NATIVE(load_object_collision_model),
        SET_INT(oInteractStatus, 0),
    END_LOOP(),
};

const BehaviorScript bhvFadingWarp[] = {
    BEGIN(OBJ_LIST_LEVEL),
    SET_INT(oInteractionSubtype, INT_SUBTYPE_FADING_WARP),
    OR_INT(oFlags, (OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SET_INT(oInteractType, INTERACT_WARP),
    SET_INT(oIntangibleTimer, 0),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_fading_warp_loop),
    END_LOOP(),
};

const BehaviorScript bhvWarp[] = {
    BEGIN(OBJ_LIST_LEVEL),
    OR_INT(oFlags, (OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SET_INT(oInteractType, INTERACT_WARP),
    SET_INT(oIntangibleTimer, 0),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_warp_loop),
    END_LOOP(),
};

const BehaviorScript bhvWhitePuffExplosion[] = {
    BEGIN(OBJ_LIST_UNIMPORTANT),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BILLBOARD(),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_white_puff_exploding_loop),
    END_LOOP(),
};

const BehaviorScript bhvSpawnedStar[] = {
    BEGIN(OBJ_LIST_LEVEL),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    SET_INT(oBhvParams2ndByte, 1),
    GOTO(bhvSpawnedStarNoLevelExit + 1 + 1),
};

const BehaviorScript bhvSpawnedStarNoLevelExit[] = {
    BEGIN(OBJ_LIST_LEVEL),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    // Spawned star - common:
    SET_HOME(),
    CALL_NATIVE(bhv_spawned_star_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_spawned_star_loop),
    END_LOOP(),
};

const BehaviorScript bhvCoinInsideBoo[] = {
    BEGIN(OBJ_LIST_LEVEL),
    SET_HITBOX(/*Radius*/ 100, /*Height*/ 64),
    SET_INT(oInteractType, INTERACT_COIN),
    OR_INT(oFlags, (OBJ_FLAG_ACTIVE_FROM_AFAR | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SET_OBJ_PHYSICS(/*Wall hitbox radius*/ 30, /*Gravity*/ -400, /*Bounciness*/ -70, /*Drag strength*/ 1000, /*Friction*/ 1000, /*Buoyancy*/ 200, /*Unused*/ 0, 0),
    BILLBOARD(),
    CALL_NATIVE(bhv_init_room),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_coin_inside_boo_loop),
        ADD_INT(oAnimState, 1),
    END_LOOP(),
};

const BehaviorScript bhvCoinFormationSpawn[] = {
    BEGIN(OBJ_LIST_LEVEL),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BILLBOARD(),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_coin_formation_spawn_loop),
    END_LOOP(),
};

const BehaviorScript bhvCoinFormation[] = {
    BEGIN(OBJ_LIST_SPAWNER),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    CALL_NATIVE(bhv_coin_formation_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_coin_formation_loop),
    END_LOOP(),
};

const BehaviorScript bhvOneCoin[] = {
    BEGIN(OBJ_LIST_LEVEL),
    SET_INT(oBhvParams2ndByte, 1),
    GOTO(bhvYellowCoin + 1),
};

const BehaviorScript bhvYellowCoin[] = {
    BEGIN(OBJ_LIST_LEVEL),
    // Yellow coin - common:
    BILLBOARD(),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    CALL_NATIVE(bhv_yellow_coin_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_yellow_coin_loop),
    END_LOOP(),
};

const BehaviorScript bhvTemporaryYellowCoin[] = {
    BEGIN(OBJ_LIST_LEVEL),
    BILLBOARD(),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    CALL_NATIVE(bhv_yellow_coin_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_temp_coin_loop),
    END_LOOP(),
};

const BehaviorScript bhvThreeCoinsSpawn[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BEGIN_REPEAT(3),
        SPAWN_CHILD(/*Model*/ MODEL_YELLOW_COIN, /*Behavior*/ bhvSingleCoinGetsSpawned),
    END_REPEAT(),
    DEACTIVATE(),
};

const BehaviorScript bhvTenCoinsSpawn[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BEGIN_REPEAT(10),
        SPAWN_CHILD(/*Model*/ MODEL_YELLOW_COIN, /*Behavior*/ bhvSingleCoinGetsSpawned),
    END_REPEAT(),
    DEACTIVATE(),
};

const BehaviorScript bhvSingleCoinGetsSpawned[] = {
    BEGIN(OBJ_LIST_LEVEL),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BILLBOARD(),
    CALL_NATIVE(bhv_spawned_coin_init),
    SET_OBJ_PHYSICS(/*Wall hitbox radius*/ 30, /*Gravity*/ -400, /*Bounciness*/ -70, /*Drag strength*/ 1000, /*Friction*/ 1000, /*Buoyancy*/ 200, /*Unused*/ 0, 0),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_spawned_coin_loop),
        ADD_INT(oAnimState, 1),
    END_LOOP(),
};

const BehaviorScript bhvCoinSparkles[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BILLBOARD(),
    SET_FLOAT(oGraphYOffset, 25),
    SET_INT(oAnimState, -1),
    BEGIN_REPEAT(12),
        ADD_INT(oAnimState, 1),
    END_REPEAT(),
    DEACTIVATE(),
};

const BehaviorScript bhvDoorWarp[] = {
    BEGIN(OBJ_LIST_SURFACE),
    SET_INT(oInteractType, INTERACT_WARP_DOOR),
    GOTO(bhvDoor + 1 + 1),
};

const BehaviorScript bhvDoor[] = {
    BEGIN(OBJ_LIST_SURFACE),
    SET_INT(oInteractType, INTERACT_DOOR),
    // Door - common:
    OR_INT(oFlags, (OBJ_FLAG_ACTIVE_FROM_AFAR | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_ANIMATIONS(oAnimations, door_anime),
    ANIMATE(0),
    LOAD_COLLISION_DATA(door_seg3_collision_0301CE78),
    SET_HITBOX(/*Radius*/ 80, /*Height*/ 100),
    SET_INT(oIntangibleTimer, 0),
    SET_FLOAT(oCollisionDistance, 1000),
    SET_HOME(),
    CALL_NATIVE(bhv_door_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_door_loop),
    END_LOOP(),
};

const BehaviorScript bhvThwomp[] = {
    BEGIN(OBJ_LIST_SURFACE),
    LOAD_COLLISION_DATA(dosun_info2),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    DROP_TO_FLOOR(),
    ADD_FLOAT(oPosY, 2),
    SET_HOME(),
    SCALE(/*Unused*/ 0, /*Field*/ 140),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_grindel_thwomp_loop),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};

const BehaviorScript bhvThwomp2[] = {
    BEGIN(OBJ_LIST_SURFACE),
    LOAD_COLLISION_DATA(dosun_info),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    DROP_TO_FLOOR(),
    ADD_FLOAT(oPosY, 2),
    SCALE(/*Unused*/ 0, /*Field*/ 140),
    SET_HOME(),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_grindel_thwomp_loop),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};

const BehaviorScript bhvTumblingBridgePlatform[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, (OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SET_FLOAT(oCollisionDistance, 300),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_tumbling_bridge_platform_loop),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};

const BehaviorScript bhvTumblingBridge[] = {
    BEGIN(OBJ_LIST_SPAWNER),
    OR_INT(oFlags, (OBJ_FLAG_ACTIVE_FROM_AFAR | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SET_HOME(),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_tumbling_bridge_loop),
    END_LOOP(),
};

const BehaviorScript bhvLLLTumblingBridge[] = {
    BEGIN(OBJ_LIST_SPAWNER),
    OR_INT(oFlags, (OBJ_FLAG_ACTIVE_FROM_AFAR | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SET_HOME(),
    SET_INT(oBhvParams2ndByte, TUMBLING_BRIDGE_BP_LLL),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_tumbling_bridge_loop),
    END_LOOP(),
};

const BehaviorScript bhvFlame[] = {
    BEGIN(OBJ_LIST_LEVEL),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BILLBOARD(),
    SET_HOME(),
    SCALE(/*Unused*/ 0, /*Field*/ 700),
    SET_INTERACT_TYPE(INTERACT_FLAME),
    SET_HITBOX_WITH_OFFSET(/*Radius*/ 50, /*Height*/ 25, /*Downwards offset*/ 25),
    SET_INT(oIntangibleTimer, 0),
    CALL_NATIVE(bhv_init_room),
    BEGIN_LOOP(),
        SET_INT(oInteractStatus, 0),
        ANIMATE_TEXTURE(oAnimState, 2),
    END_LOOP(),
};

const BehaviorScript bhvWaterMist[] = {
    BEGIN(OBJ_LIST_UNIMPORTANT),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BILLBOARD(),
    SET_INT(oOpacity, 254),
    SET_FLOAT(oForwardVel, 20),
    SET_FLOAT(oVelY, -8),
    ADD_FLOAT(oPosY, 62),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_water_mist_loop),
    END_LOOP(),
};

const BehaviorScript bhvBreathParticleSpawner[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BEGIN_REPEAT(8),
        CALL_NATIVE(bhv_water_mist_spawn_loop),
    END_REPEAT(),
    DEACTIVATE(),
};

const BehaviorScript bhvBreakBoxTriangle[] = {
    BEGIN(OBJ_LIST_UNIMPORTANT),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BEGIN_REPEAT(18),
        CALL_NATIVE(cur_obj_rotate_face_angle_using_vel),
        CALL_NATIVE(cur_obj_move_using_fvel_and_gravity),
    END_REPEAT(),
    DEACTIVATE(),
};

const BehaviorScript bhvWaterMist2[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SET_HOME(),
    SET_INT(oFaceAnglePitch, 0xC000),
    SCALE(/*Unused*/ 0, /*Field*/ 2100),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_water_mist_2_loop),
    END_LOOP(),
};

const BehaviorScript bhvUnused0DFC[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    SET_INT(oAnimState, -1),
    SET_FLOAT(oFaceAnglePitch, 0),
    SET_FLOAT(oFaceAngleYaw, 0),
    SET_FLOAT(oFaceAngleRoll, 0),
    BEGIN_REPEAT(6),
        ADD_INT(oAnimState, 1),
    END_REPEAT(),
    DEACTIVATE(),
};

const BehaviorScript bhvMistCircParticleSpawner[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    CALL_NATIVE(bhv_pound_white_puffs_init),
    DELAY(1),
    DEACTIVATE(),
};

const BehaviorScript bhvFlamethrower[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SET_HOME(),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_flamethrower_loop),
    END_LOOP(),
};

const BehaviorScript bhvFlamethrowerFlame[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    SET_INTERACT_TYPE(INTERACT_FLAME),
    SET_HITBOX_WITH_OFFSET(/*Radius*/ 50, /*Height*/ 25, /*Downwards offset*/ 25),
    BILLBOARD(),
    SET_HOME(),
    SET_INT(oIntangibleTimer, 0),
    CALL_NATIVE(bhv_init_room),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_flamethrower_flame_loop),
        ADD_INT(oAnimState, 1),
    END_LOOP(),
};

const BehaviorScript bhvBouncingFireball[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    DISABLE_RENDERING(),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_bouncing_fireball_loop),
    END_LOOP(),
};

const BehaviorScript bhvBouncingFireballFlame[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    SET_INTERACT_TYPE(INTERACT_FLAME),
    SET_FLOAT(oGraphYOffset, 30),
    SET_HITBOX_WITH_OFFSET(/*Radius*/ 50, /*Height*/ 25, /*Downwards offset*/ 25),
    SET_OBJ_PHYSICS(/*Wall hitbox radius*/ 30, /*Gravity*/ -400, /*Bounciness*/ -70, /*Drag strength*/ 1000, /*Friction*/ 1000, /*Buoyancy*/ 200, /*Unused*/ 0, 0),
    BILLBOARD(),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_bouncing_fireball_flame_loop),
        ADD_INT(oAnimState, 1),
    END_LOOP(),
};

const BehaviorScript bhvBowserShockWave[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, (OBJ_FLAG_ACTIVE_FROM_AFAR | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SET_INT(oOpacity, 255),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_bowser_shock_wave_loop),
    END_LOOP(),
};

const BehaviorScript bhvFireParticleSpawner[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BILLBOARD(),
    SET_FLOAT(oGraphYOffset, 70),
    SET_INT(oAnimState, -1),
    BEGIN_LOOP(),
        ADD_INT(oAnimState, 1),
        CALL_NATIVE(bhv_flame_mario_loop),
    END_LOOP(),
};

const BehaviorScript bhvBlackSmokeMario[] = {
    BEGIN(OBJ_LIST_UNIMPORTANT),
    OR_INT(oFlags, (OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_MOVE_XZ_USING_FVEL | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    BILLBOARD(),
    SET_INT(oAnimState, -1),
    SET_FLOAT(oGraphYOffset, 50),
    BEGIN_REPEAT(8),
		ADD_INT(oAnimState, 1),
        CALL_NATIVE(bhv_black_smoke_mario_loop),
        DELAY(1),
        CALL_NATIVE(bhv_black_smoke_mario_loop),
        DELAY(1),
        CALL_NATIVE(bhv_black_smoke_mario_loop),
    END_REPEAT(),
    DEACTIVATE(),
};

const BehaviorScript bhvBlackSmokeBowser[] = {
    BEGIN(OBJ_LIST_UNIMPORTANT),
    OR_INT(oFlags, (OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_MOVE_XZ_USING_FVEL | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    BILLBOARD(),
    SET_FLOAT(oGraphYOffset, 0),
    BEGIN_REPEAT(8),
        CALL_NATIVE(bhv_black_smoke_bowser_loop),
        ANIMATE_TEXTURE(oAnimState, 4),
    END_REPEAT(),
    DEACTIVATE(),
};

const BehaviorScript bhvBlackSmokeUpward[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BEGIN_REPEAT(4),
        CALL_NATIVE(bhv_black_smoke_upward_loop),
    END_REPEAT(),
    DEACTIVATE(),
};

const BehaviorScript bhvBetaFishSplashSpawner[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    DISABLE_RENDERING(),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_beta_fish_splash_spawner_loop),
    END_LOOP(),
};

const BehaviorScript bhvTowerPlatformGroup[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    DISABLE_RENDERING(),
    ADD_FLOAT(oPosY, 300),
    SET_HOME(),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_tower_platform_group_loop),
    END_LOOP(),
};

const BehaviorScript bhvWFSlidingTowerPlatform[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, (OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_COLLISION_DATA(wf_seg7_collision_platform),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_wf_sliding_tower_platform_loop),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};

const BehaviorScript bhvWFElevatorTowerPlatform[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, (OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_COLLISION_DATA(wf_seg7_collision_platform),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_wf_elevator_tower_platform_loop),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};

const BehaviorScript bhvWFSolidTowerPlatform[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, (OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_COLLISION_DATA(wf_seg7_collision_trapezoid),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_wf_solid_tower_platform_loop),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};

const BehaviorScript bhvLeafParticleSpawner[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    CALL_NATIVE(bhv_snow_leaf_particle_spawn_init),
    DELAY(1),
    DEACTIVATE(),
};

const BehaviorScript bhvTreeLeaf[] = {
    BEGIN(OBJ_LIST_UNIMPORTANT),
    OR_INT(oFlags, (OBJ_FLAG_MOVE_XZ_USING_FVEL | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_tree_snow_or_leaf_loop),
    END_LOOP(),
};

const BehaviorScript bhvAnotherTiltingPlatform[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SET_HOME(),
    CALL_NATIVE(bhv_platform_normals_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_tilting_inverted_pyramid_loop),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};

const BehaviorScript bhvPiranhaPlantBubble[] = {
    BEGIN(OBJ_LIST_UNIMPORTANT),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BILLBOARD(),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_piranha_plant_bubble_loop),
    END_LOOP(),
};

const BehaviorScript bhvPiranhaPlantWakingBubbles[] = {
    BEGIN(OBJ_LIST_UNIMPORTANT),
    BILLBOARD(),
    OR_INT(oFlags, (OBJ_FLAG_ACTIVE_FROM_AFAR | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    BEGIN_REPEAT(10),
        CALL_NATIVE(bhv_piranha_plant_waking_bubbles_loop),
    END_REPEAT(),
    DEACTIVATE(),
};

const BehaviorScript bhvHeaveHo[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_HOLDABLE | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_ANIMATIONS(oAnimations, omurobo_anime),
    ANIMATE(0),
    SET_OBJ_PHYSICS(/*Wall hitbox radius*/ 200, /*Gravity*/ -400, /*Bounciness*/ -50, /*Drag strength*/ 1000, /*Friction*/ 1000, /*Buoyancy*/ 600, /*Unused*/ 0, 0),
    SPAWN_OBJ(/*Model*/ MODEL_NONE, /*Behavior*/ bhvHeaveHoThrowMario),
    SET_INT(oInteractType, INTERACT_GRABBABLE),
    SET_INT(oInteractionSubtype, INT_SUBTYPE_NOT_GRABBABLE | INT_SUBTYPE_GRABS_MARIO),
    SET_HITBOX(/*Radius*/ 120, /*Height*/ 100),
    SET_HOME(),
    SET_INT(oIntangibleTimer, 0),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_heave_ho_loop),
    END_LOOP(),
};

const BehaviorScript bhvHeaveHoThrowMario[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, (OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    BILLBOARD(),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_heave_ho_throw_mario_loop),
    END_LOOP(),
};

const BehaviorScript bhvBulletBill[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_MOVE_XZ_USING_FVEL | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SET_HOME(),
    SET_HITBOX_WITH_OFFSET(/*Radius*/ 50, /*Height*/ 50, /*Downwards offset*/ 50),
    SET_INTERACT_TYPE(INTERACT_DAMAGE),
    SET_INT(oDamageOrCoinValue, 3),
    SCALE(/*Unused*/ 0, /*Field*/ 40),
    SET_INT(oIntangibleTimer, 0),
    SET_OBJ_PHYSICS(/*Wall hitbox radius*/ 30, /*Gravity*/ 0, /*Bounciness*/ 0, /*Drag strength*/ 0, /*Friction*/ 0, /*Buoyancy*/ 0, /*Unused*/ 0, 0),
    CALL_NATIVE(bhv_bullet_bill_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_bullet_bill_loop),
    END_LOOP(),
};

const BehaviorScript bhvWhitePuffSmoke[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BILLBOARD(),
    ADD_FLOAT(oPosY, -100),
    CALL_NATIVE(bhv_white_puff_smoke_init),
    SET_INT(oAnimState, -1),
    BEGIN_REPEAT(10),
        ADD_INT(oAnimState, 1),
    END_REPEAT(),
    DEACTIVATE(),
};

const BehaviorScript bhvBowserTailAnchor[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    SET_HITBOX_WITH_OFFSET(/*Radius*/ 100, /*Height*/ 50, /*Downwards offset*/ -50),
    SET_INT(oIntangibleTimer, 0),
    DISABLE_RENDERING(),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_bowser_tail_anchor_loop),
    END_LOOP(),
};

const BehaviorScript bhvBowser[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_HOLDABLE | OBJ_FLAG_ACTIVE_FROM_AFAR | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SET_INT(oInteractType, INTERACT_GRABBABLE),
    SET_HITBOX(/*Radius*/ 400, /*Height*/ 400),
    DROP_TO_FLOOR(),
    SET_HOME(),
    LOAD_ANIMATIONS(oAnimations, bowser_seg6_anims_06057690),
    SPAWN_CHILD(/*Model*/ MODEL_NONE, /*Behavior*/ bhvBowserBodyAnchor),
    SPAWN_CHILD(/*Model*/ MODEL_BOWSER_BOMB_CHILD_OBJ, /*Behavior*/ bhvBowserFlameSpawn),
    SPAWN_OBJ(/*Model*/ MODEL_NONE, /*Behavior*/ bhvBowserTailAnchor),
    // Beta leftover that spawn 50 coins when Bowser is defeated
    SET_INT(oNumLootCoins, 50),
    SET_OBJ_PHYSICS(/*Wall hitbox radius*/ 0, /*Gravity*/ -400, /*Bounciness*/ -75, /*Drag strength*/ 800, /*Friction*/ 1000, /*Buoyancy*/ 200, /*Unused*/ 0, 0),
    SET_HOME(),
    CALL_NATIVE(bhv_bowser_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_bowser_loop),
    END_LOOP(),
};

const BehaviorScript bhvBowserBodyAnchor[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    SET_HITBOX(/*Radius*/ 100, /*Height*/ 300),
    SET_INTERACT_TYPE(INTERACT_DAMAGE),
    SET_INT(oInteractionSubtype, INT_SUBTYPE_BIG_KNOCKBACK),
    DISABLE_RENDERING(),
    SET_INT(oDamageOrCoinValue, 2),
    SET_INT(oIntangibleTimer, 0),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_bowser_body_anchor_loop),
    END_LOOP(),
};

const BehaviorScript bhvBowserFlameSpawn[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    SET_MODEL(MODEL_NONE),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_bowser_flame_spawn_loop),
    END_LOOP(),
};

const BehaviorScript bhvBlueBowserFlame[] = {
    BEGIN(OBJ_LIST_LEVEL),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    SET_INTERACT_TYPE(INTERACT_FLAME),
    BILLBOARD(),
    SET_OBJ_PHYSICS(/*Wall hitbox radius*/ 0, /*Gravity*/ -400, /*Bounciness*/ -70, /*Drag strength*/ 1000, /*Friction*/ 1000, /*Buoyancy*/ 200, /*Unused*/ 0, 0),
    CALL_NATIVE(bhv_blue_bowser_flame_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_blue_bowser_flame_loop),
        ANIMATE_TEXTURE(oAnimState, 2),
    END_LOOP(),
};

const BehaviorScript bhvFlameFloatingLanding[] = {
    BEGIN(OBJ_LIST_LEVEL),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    SET_INTERACT_TYPE(INTERACT_FLAME),
    BILLBOARD(),
    SET_OBJ_PHYSICS(/*Wall hitbox radius*/ 0, /*Gravity*/ -400, /*Bounciness*/ -70, /*Drag strength*/ 1000, /*Friction*/ 1000, /*Buoyancy*/ 200, /*Unused*/ 0, 0),
    CALL_NATIVE(bhv_flame_floating_landing_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_flame_floating_landing_loop),
        ANIMATE_TEXTURE(oAnimState, 2),
    END_LOOP(),
};

const BehaviorScript bhvBlueFlamesGroup[] = {
    BEGIN(OBJ_LIST_LEVEL),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SET_INTERACT_TYPE(INTERACT_FLAME),
    BILLBOARD(),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_blue_flames_group_loop),
    END_LOOP(),
};

const BehaviorScript bhvFlameBouncing[] = {
    BEGIN(OBJ_LIST_LEVEL),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SET_INTERACT_TYPE(INTERACT_FLAME),
    BILLBOARD(),
    CALL_NATIVE(bhv_flame_bouncing_init),
    SET_OBJ_PHYSICS(/*Wall hitbox radius*/ 0, /*Gravity*/ -400, /*Bounciness*/ -70, /*Drag strength*/ 0, /*Friction*/ 0, /*Buoyancy*/ 200, /*Unused*/ 0, 0),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_flame_bouncing_loop),
        ANIMATE_TEXTURE(oAnimState, 2),
    END_LOOP(),
};

const BehaviorScript bhvFlameMovingForwardGrowing[] = {
    BEGIN(OBJ_LIST_LEVEL),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    SET_INTERACT_TYPE(INTERACT_FLAME),
    BILLBOARD(),
    CALL_NATIVE(bhv_flame_moving_forward_growing_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_flame_moving_forward_growing_loop),
        ANIMATE_TEXTURE(oAnimState, 2),
    END_LOOP(),
};

const BehaviorScript bhvFlameBowser[] = {
    BEGIN(OBJ_LIST_LEVEL),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    SET_INTERACT_TYPE(INTERACT_FLAME),
    BILLBOARD(),
    CALL_NATIVE(bhv_flame_bowser_init),
    SET_OBJ_PHYSICS(/*Wall hitbox radius*/ 0, /*Gravity*/ -400, /*Bounciness*/ -70, /*Drag strength*/ 1000, /*Friction*/ 1000, /*Buoyancy*/ 200, /*Unused*/ 0, 0),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_flame_bowser_loop),
        ANIMATE_TEXTURE(oAnimState, 2),
    END_LOOP(),
};

const BehaviorScript bhvFlameLargeBurningOut[] = {
    BEGIN(OBJ_LIST_LEVEL),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    SET_INTERACT_TYPE(INTERACT_FLAME),
    BILLBOARD(),
    CALL_NATIVE(bhv_flame_large_burning_out_init),
    SET_OBJ_PHYSICS(/*Wall hitbox radius*/ 0, /*Gravity*/ -400, /*Bounciness*/ -70, /*Drag strength*/ 1000, /*Friction*/ 1000, /*Buoyancy*/ 200, /*Unused*/ 0, 0),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_flame_bowser_loop),
        ANIMATE_TEXTURE(oAnimState, 2),
    END_LOOP(),
};

const BehaviorScript bhvBlueFish[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, (OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SET_HOME(),
    LOAD_ANIMATIONS(oAnimations, blue_fish_seg3_anims_0301C2B0),
    ANIMATE(0),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_blue_fish_movement_loop),
    END_LOOP(),
};

const BehaviorScript bhvLLLRotatingHexagonalPlatform[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, (OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SET_FLOAT(oDrawingDistance, 20000),
    SET_FLOAT(oCollisionDistance, 20000),
    LOAD_COLLISION_DATA(lll_seg7_collision_hexagonal_platform),
    SET_HOME(),
    BEGIN_LOOP(),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};

const BehaviorScript bhvLLLRotatingBlockWithFireBars[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_COLLISION_DATA(lll_seg7_collision_rotating_fire_bars),
    SET_FLOAT(oCollisionDistance, 4000),
    SET_FLOAT(oDrawingDistance, 6000),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_lll_rotating_block_fire_bars_loop),
    END_LOOP(),
};

const BehaviorScript bhvLLLRotatingHexFlame[] = {
    BEGIN(OBJ_LIST_LEVEL),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SET_INTERACT_TYPE(INTERACT_FLAME),
    SET_HITBOX_WITH_OFFSET(/*Radius*/ 50, /*Height*/ 100, /*Downwards offset*/ 50),
    SET_INT(oIntangibleTimer, 0),
    BILLBOARD(),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_lll_rotating_hex_flame_loop),
        ADD_INT(oAnimState, 1),
    END_LOOP(),
};

const BehaviorScript bhvLLLWoodPiece[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_COLLISION_DATA(lll_seg7_collision_wood_piece),
    SET_HOME(),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_lll_wood_piece_loop),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};

const BehaviorScript bhvLLLFloatingWoodBridge[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SET_MODEL(MODEL_NONE),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_lll_floating_wood_bridge_loop),
    END_LOOP(),
};

const BehaviorScript bhvVolcanoFlames[] = {
    BEGIN(OBJ_LIST_UNIMPORTANT),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    BILLBOARD(),
    BEGIN_LOOP(),
        ADD_INT(oAnimState, 1),
        CALL_NATIVE(bhv_volcano_flames_loop),
    END_LOOP(),
};

const BehaviorScript bhvLLLRotatingHexagonalRing[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_COLLISION_DATA(lll_seg7_collision_rotating_platform),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_lll_rotating_hexagonal_ring_loop),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};

const BehaviorScript bhvLLLSinkingRectangularPlatform[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_COLLISION_DATA(lll_seg7_collision_slow_tilting_platform),
    SET_FLOAT(oCollisionDistance, 2000),
    SET_HOME(),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_lll_sinking_rectangular_platform_loop),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};

const BehaviorScript bhvLLLSinkingSquarePlatforms[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_COLLISION_DATA(lll_seg7_collision_sinking_pyramids),
    ADD_FLOAT(oPosY, 5),
    SET_FLOAT(oCollisionDistance, 2000),
    SET_HOME(),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_lll_sinking_square_platforms_loop),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};

const BehaviorScript bhvLLLTiltingInvertedPyramid[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_COLLISION_DATA(lll_seg7_collision_inverted_pyramid),
    ADD_FLOAT(oPosY, 5),
    SET_HOME(),
    CALL_NATIVE(bhv_platform_normals_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_tilting_inverted_pyramid_loop),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};

const BehaviorScript bhvPiranhaPlant[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_ANIMATIONS(oAnimations, pakun_anime),
    ANIMATE(0),
    SET_INTERACT_TYPE(INTERACT_DAMAGE),
    SET_HITBOX(/*Radius*/ 100, /*Height*/ 200),
    SET_HURTBOX(/*Radius*/ 50, /*Height*/ 200),
    SET_INT(oIntangibleTimer, 0),
    SET_INT(oDamageOrCoinValue, 3),
    SET_INT(oNumLootCoins, 1),
    SPAWN_CHILD(/*Model*/ MODEL_BUBBLE, /*Behavior*/ bhvPiranhaPlantBubble),
    SET_FLOAT(oDrawingDistance, 2000),
    SET_HOME(),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_piranha_plant_loop),
    END_LOOP(),
};

const BehaviorScript bhvLLLHexagonalMesh[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    LOAD_COLLISION_DATA(lll_hexagonal_mesh_seg3_collision_0301CECC),
    BEGIN_LOOP(),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};

const BehaviorScript bhvLLLBowserPuzzlePiece[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_COLLISION_DATA(lll_seg7_collision_puzzle_piece),
    SET_HOME(),
    SET_FLOAT(oCollisionDistance, 3000),
    SET_FLOAT(oDrawingDistance, 5000),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_lll_bowser_puzzle_piece_loop),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};

const BehaviorScript bhvLLLBowserPuzzle[] = {
    BEGIN(OBJ_LIST_SPAWNER),
    DISABLE_RENDERING(),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    ADD_FLOAT(oPosZ, -50),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_lll_bowser_puzzle_loop),
    END_LOOP(),
};

const BehaviorScript bhvTuxiesMother[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_COLLISION_DATA(penguin_seg5_collision_05008B88),
    LOAD_ANIMATIONS(oAnimations, ping_anime),
    ANIMATE(3),
    SET_OBJ_PHYSICS(/*Wall hitbox radius*/ 0, /*Gravity*/ -400, /*Bounciness*/ -50, /*Drag strength*/ 0, /*Friction*/ 0, /*Buoyancy*/ 0, /*Unused*/ 0, 0),
    SET_HOME(),
    SET_FLOAT(oDrawingDistance, 20000),
    SET_INT(oIntangibleTimer, 0),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_tuxies_mother_loop),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};

const BehaviorScript bhvPenguinBaby[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    BREAK(),
};

const BehaviorScript bhvUnused20E0[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    BREAK(),
};

const BehaviorScript bhvSmallPenguin[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_HOLDABLE | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    DROP_TO_FLOOR(),
    LOAD_ANIMATIONS(oAnimations, ping_anime),
    ANIMATE(0),
    SET_OBJ_PHYSICS(/*Wall hitbox radius*/ 90, /*Gravity*/ -400, /*Bounciness*/ -50, /*Drag strength*/ 0, /*Friction*/ 0, /*Buoyancy*/ 200, /*Unused*/ 0, 0),
    SET_INT(oInteractType, INTERACT_GRABBABLE),
    SET_INT(oInteractionSubtype, INT_SUBTYPE_HOLDABLE_NPC),
    SET_INT(oIntangibleTimer, 0),
    SET_HITBOX(/*Radius*/ 40, /*Height*/ 40),
    SET_HOME(),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_small_penguin_loop),
    END_LOOP(),
};

const BehaviorScript bhvManyBlueFishSpawner[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    SET_INT(oBhvParams2ndByte, FISH_SPAWNER_BP_MANY_BLUE),
    GOTO(bhvFishSpawner + 1),
};

const BehaviorScript bhvFewBlueFishSpawner[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    SET_INT(oBhvParams2ndByte, FISH_SPAWNER_BP_FEW_BLUE),
    GOTO(bhvFishSpawner + 1),
};

const BehaviorScript bhvFishSpawner[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    // Fish Spawner - common:
    DISABLE_RENDERING(),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_fish_spawner_loop),
    END_LOOP(),
};

const BehaviorScript bhvFish[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SET_HOME(),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_fish_loop),
    END_LOOP(),
};

const BehaviorScript bhvChirpChirp[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    SET_INT(oBirdChirpChirpUnkF4, 1),
    GOTO(bhvChirpChirpUnused),
};

const BehaviorScript bhvChirpChirpUnused[] = {
    DISABLE_RENDERING(),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_bub_spawner_loop),
    END_LOOP(),
};

const BehaviorScript bhvBub[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_ANIMATIONS(oAnimations, bub_seg6_anims_06012354),
    ANIMATE(0),
    SET_HITBOX_WITH_OFFSET(/*Radius*/ 20, /*Height*/ 10, /*Downwards offset*/ 10),
    SET_INTERACT_TYPE(INTERACT_DAMAGE),
    SET_INT(oDamageOrCoinValue, 1),
    SET_HOME(),
    SET_INT(oIntangibleTimer, 0),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_bub_loop),
    END_LOOP(),
};

const BehaviorScript bhvSoundSpawner[] = {
    BEGIN(OBJ_LIST_UNIMPORTANT),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    DELAY(3),
    CALL_NATIVE(bhv_sound_spawner_init),
    DELAY(30),
    DEACTIVATE(),
};

const BehaviorScript bhvBowsersSub[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, (OBJ_FLAG_ACTIVE_FROM_AFAR | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SET_FLOAT(oDrawingDistance, 20000),
    SET_FLOAT(oCollisionDistance, 20000),
    LOAD_COLLISION_DATA(water_land_submarine_collision),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_bowsers_sub_loop),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};

const BehaviorScript bhvSushiShark[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_ANIMATIONS(oAnimations, RCP_sharkAnime),
    SPAWN_OBJ(/*Model*/ MODEL_NONE, /*Behavior*/ bhvSushiSharkCollisionChild),
    SET_HITBOX_WITH_OFFSET(/*Radius*/ 100, /*Height*/ 50, /*Downwards offset*/ 50),
    SET_INTERACT_TYPE(INTERACT_DAMAGE),
    SET_INT(oDamageOrCoinValue, 3),
    SET_HOME(),
    ANIMATE(0),
    SET_INT(oIntangibleTimer, 0),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_sushi_shark_loop),
    END_LOOP(),
};

const BehaviorScript bhvSushiSharkCollisionChild[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    DISABLE_RENDERING(),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_sushi_shark_collision_loop),
    END_LOOP(),
};

const BehaviorScript bhvMistParticleSpawner[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    PARENT_BIT_CLEAR(oActiveParticleFlags, ACTIVE_PARTICLE_DUST),
    DISABLE_RENDERING(),
    SPAWN_CHILD(/*Model*/ MODEL_SMOKE, /*Behavior*/ bhvWhitePuff2),
    DELAY(1),
    DEACTIVATE(),
};

const BehaviorScript bhvWhitePuff1[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BILLBOARD(),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_white_puff_1_loop),
    END_LOOP(),
};

const BehaviorScript bhvWhitePuff2[] = {
    BEGIN(OBJ_LIST_UNIMPORTANT),
    OR_INT(oFlags, (OBJ_FLAG_MOVE_XZ_USING_FVEL | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    BILLBOARD(),
    SET_INT(oAnimState, -1),
    BEGIN_REPEAT(10),
        ADD_INT(oAnimState, 1),
    END_REPEAT(),
    DEACTIVATE(),
};

const BehaviorScript bhvWhitePuffSmoke2[] = {
    BEGIN(OBJ_LIST_UNIMPORTANT),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BILLBOARD(),
    SET_INT(oAnimState, -1),
    BEGIN_REPEAT(7),
        CALL_NATIVE(cur_obj_move_using_fvel_and_gravity),
        ADD_INT(oAnimState, 1),
    END_REPEAT(),
    DEACTIVATE(),
};

const BehaviorScript bhvAnimatedTexture[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    SET_OBJ_PHYSICS(/*Wall hitbox radius*/ 30, /*Gravity*/ -400, /*Bounciness*/ -70, /*Drag strength*/ 1000, /*Friction*/ 1000, /*Buoyancy*/ 200, /*Unused*/ 0, 0),
    BILLBOARD(),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_animated_texture_loop),
        ADD_INT(oAnimState, 1),
        ANIMATE_TEXTURE(oAnimState, 2),
    END_LOOP(),
};

const BehaviorScript bhvCourtyardBooTriplet[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    DISABLE_RENDERING(),
    CALL_NATIVE(bhv_courtyard_boo_triplet_init),
    DEACTIVATE(),
};

const BehaviorScript bhvBoo[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    SET_INT(oBhvParams2ndByte, BOO_BP_GENERIC),
    GOTO(bhvGhostHuntBoo + 1),
};

const BehaviorScript bhvGhostHuntBoo[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    // Boo - common:
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SET_INT(oIntangibleTimer, 0),
    SET_HOME(),
    SET_INT(oDamageOrCoinValue, 2),
    SET_HITBOX(/*Radius*/ 140, /*Height*/ 80),
    SET_HURTBOX(/*Radius*/ 40, /*Height*/ 60),
    SET_FLOAT(oGraphYOffset, 30),
    CALL_NATIVE(bhv_init_room),
    SPAWN_CHILD(/*Model*/ MODEL_YELLOW_COIN, /*Behavior*/ bhvCoinInsideBoo),
    SET_OBJ_PHYSICS(/*Wall hitbox radius*/ 30, /*Gravity*/ 0, /*Bounciness*/ -50, /*Drag strength*/ 1000, /*Friction*/ 1000, /*Buoyancy*/ 200, /*Unused*/ 0, 0),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_boo_loop),
    END_LOOP(),
};

const BehaviorScript bhvInsideCannon[] = {
    BREAK(),
};

// What is this?
UNUSED static const BehaviorScript unused_1[] = {
    BREAK(),
    BREAK(),
    BREAK(),
    BREAK(),
};

const BehaviorScript bhvStaticObject[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BREAK(),
};

const BehaviorScript bhvCastleFloorTrap[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    DISABLE_RENDERING(),
    CALL_NATIVE(bhv_castle_floor_trap_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_castle_floor_trap_loop),
    END_LOOP(),
};

const BehaviorScript bhvFloorTrapInCastle[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, (OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_COLLISION_DATA(inside_castle_seg7_collision_floor_trap),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_floor_trap_in_castle_loop),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};

const BehaviorScript bhvTree[] = {
    BEGIN(OBJ_LIST_POLELIKE),
    BILLBOARD(),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    SET_INT(oInteractType, INTERACT_POLE),
    SET_HITBOX(/*Radius*/ 80, /*Height*/ 500),
    SET_INT(oIntangibleTimer, 0),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_pole_base_loop),
    END_LOOP(),
};

const BehaviorScript bhvSparkle[] = {
    BEGIN(OBJ_LIST_UNIMPORTANT),
    BILLBOARD(),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    SET_INT(oAnimState, -1),
    BEGIN_REPEAT(9),
        ADD_INT(oAnimState, 1),
    END_REPEAT(),
    DEACTIVATE(),
};

const BehaviorScript bhvSparkleSpawn[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    BILLBOARD(),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_sparkle_spawn_loop),
    END_LOOP(),
};

const BehaviorScript bhvSparkleParticleSpawner[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    PARENT_BIT_CLEAR(oActiveParticleFlags, ACTIVE_PARTICLE_SPARKLES),
    BEGIN(OBJ_LIST_UNIMPORTANT),
    BILLBOARD(),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    SET_FLOAT(oGraphYOffset, 25),
    SET_RANDOM_FLOAT(oMarioParticleFlags, /*Minimum*/ -50, /*Range*/ 100),
    SUM_FLOAT(/*Dest*/ oPosX, /*Value 1*/ oPosX, /*Value 2*/ oMarioParticleFlags),
    SET_RANDOM_FLOAT(oMarioParticleFlags, /*Minimum*/ -50, /*Range*/ 100),
    SUM_FLOAT(/*Dest*/ oPosZ, /*Value 1*/ oPosZ, /*Value 2*/ oMarioParticleFlags),
    SET_RANDOM_FLOAT(oMarioParticleFlags, /*Minimum*/ -50, /*Range*/ 100),
    SUM_FLOAT(/*Dest*/ oPosY, /*Value 1*/ oPosY, /*Value 2*/ oMarioParticleFlags),
    SET_INT(oAnimState, -1),
    BEGIN_REPEAT(12),
        ADD_INT(oAnimState, 1),
    END_REPEAT(),
    DEACTIVATE(),
};

const BehaviorScript bhvSmallWhomp[] = {
    BEGIN(OBJ_LIST_SURFACE),
    SET_INT(oNumLootCoins, 5),
    // Whomp - common:
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_ANIMATIONS(oAnimations, wallman_anime),
    LOAD_COLLISION_DATA(wallman_info),
    ANIMATE(0),
    SET_OBJ_PHYSICS(/*Wall hitbox radius*/ 0, /*Gravity*/ -400, /*Bounciness*/ -50, /*Drag strength*/ 0, /*Friction*/ 0, /*Buoyancy*/ 200, /*Unused*/ 0, 0),
    SET_HOME(),
    SET_FLOAT(oDrawingDistance, 3500),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_whomp_loop),
    END_LOOP(),
};

// The large splash Mario makes when he jumps into a pool of water.
const BehaviorScript bhvWaterSplash[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BILLBOARD(),
    SET_INT(oAnimState, -1),
    BEGIN_REPEAT(3),
        ADD_INT(oAnimState, 1),
        CALL_NATIVE(bhv_water_splash_spawn_droplets),
        DELAY(1),
        CALL_NATIVE(bhv_water_splash_spawn_droplets),
    END_REPEAT(),
    BEGIN_REPEAT(5),
        ADD_INT(oAnimState, 1),
        DELAY(1),
    END_REPEAT(),
    PARENT_BIT_CLEAR(oActiveParticleFlags, ACTIVE_PARTICLE_WATER_SPLASH),
    DEACTIVATE(),
};

// Droplets of water that spawn as a result of various water splashes.
const BehaviorScript bhvWaterDroplet[] = {
    BEGIN(OBJ_LIST_UNIMPORTANT),
    OR_INT(oFlags, (OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_MOVE_XZ_USING_FVEL | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    BILLBOARD(),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_water_droplet_loop),
    END_LOOP(),
};

// Small splashes that are seen when a water droplet lands back into the water.
const BehaviorScript bhvWaterDropletSplash[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    CALL_NATIVE(bhv_water_droplet_splash_init),
    ADD_FLOAT(oPosY, 5),
    SET_INT(oAnimState, -1),
    BEGIN_REPEAT(6),
        ADD_INT(oAnimState, 1),
    END_REPEAT(),
    DEACTIVATE(),
};

// The splash created when an air bubble hits the surface of the water.
const BehaviorScript bhvBubbleSplash[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    SET_FLOAT(oFaceAnglePitch, 0),
    SET_FLOAT(oFaceAngleYaw, 0),
    SET_FLOAT(oFaceAngleRoll, 0),
    SET_INT(oAnimState, -1),
    CALL_NATIVE(bhv_bubble_splash_init),
    BEGIN_REPEAT(6),
        ADD_INT(oAnimState, 1),
    END_REPEAT(),
    DEACTIVATE(),
};

// The water wave surrounding Mario when he is idle in a pool of water.
const BehaviorScript bhvIdleWaterWave[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    SET_FLOAT(oFaceAnglePitch, 0),
    SET_FLOAT(oFaceAngleYaw, 0),
    SET_FLOAT(oFaceAngleRoll, 0),
    SET_INT(oAnimState, -1),
    ADD_INT(oAnimState, 1),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_idle_water_wave_loop),
        ADD_INT(oAnimState, 1),
        BEGIN_REPEAT(6),
            CALL_NATIVE(bhv_idle_water_wave_loop),
        END_REPEAT(),
        CALL_NATIVE(bhv_idle_water_wave_loop),
    END_LOOP(),
};

// Water splashes similar to the splashes created by water droplets, but are created by other objects.
// Unlike water droplet splashes, they are unimportant objects.
const BehaviorScript bhvObjectWaterSplash[] = {
    BEGIN(OBJ_LIST_UNIMPORTANT),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    SET_FLOAT(oFaceAnglePitch, 0),
    SET_FLOAT(oFaceAngleYaw, 0),
    SET_FLOAT(oFaceAngleRoll, 0),
    SET_INT(oAnimState, -1),
    BEGIN_REPEAT(6),
        ADD_INT(oAnimState, 1),
    END_REPEAT(),
    DEACTIVATE(),
};

// Waves that are generated when running in shallow water.
const BehaviorScript bhvShallowWaterWave[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    DISABLE_RENDERING(),
    BEGIN_REPEAT(5),
        SPAWN_WATER_DROPLET(&gShallowWaterWaveDropletParams),
    END_REPEAT_CONTINUE(),
    DELAY(1),
    PARENT_BIT_CLEAR(oActiveParticleFlags, ACTIVE_PARTICLE_SHALLOW_WATER_WAVE),
    DEACTIVATE(),
};

// A small water splash that occurs when jumping in and out of shallow water.
// Unlike the larger water splash it has no visible model of its own.
// It has a 1 in 256 chance of spawning the fish particle easter egg.
const BehaviorScript bhvShallowWaterSplash[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    DISABLE_RENDERING(),
    BEGIN_REPEAT(18),
        SPAWN_WATER_DROPLET(&gShallowWaterSplashDropletParams),
    END_REPEAT_CONTINUE(),
    DELAY(1),
    PARENT_BIT_CLEAR(oActiveParticleFlags, ACTIVE_PARTICLE_SHALLOW_WATER_SPLASH),
    DEACTIVATE(),
};

// Waves created by other objects along the water's surface, specifically the koopa shell and Sushi.
// Unlike Mario's waves, they are unimportant objects.
const BehaviorScript bhvObjectWaveTrail[] = {
    BEGIN(OBJ_LIST_UNIMPORTANT),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    GOTO(bhvWaveTrail + 1 + 1 + 2), // Wave trail - common
};

// The waves created by Mario while he is swimming.
const BehaviorScript bhvWaveTrail[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    PARENT_BIT_CLEAR(oActiveParticleFlags, ACTIVE_PARTICLE_WAVE_TRAIL),
    // Wave trail - common:
    SET_FLOAT(oFaceAnglePitch, 0),
    SET_FLOAT(oFaceAngleYaw, 0),
    SET_FLOAT(oFaceAngleRoll, 0),
    SET_INT(oAnimState, -1),
    BEGIN_REPEAT(8),
        ADD_INT(oAnimState, 1),
        CALL_NATIVE(bhv_wave_trail_shrink),
        DELAY(1),
        CALL_NATIVE(bhv_wave_trail_shrink),
    END_REPEAT(),
    DEACTIVATE(),
};

const BehaviorScript bhvYellowBall[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BILLBOARD(),
    BREAK(),
};

UNUSED static const u64 behavior_data_unused_0 = 0;
const BehaviorScript bhvMario[] = {
    BEGIN(OBJ_LIST_PLAYER),
    SET_INT(oIntangibleTimer, 0),
    OR_INT(oFlags, OBJ_FLAG_0100),
    OR_INT(oUnk94, 0x0001),
    SET_HITBOX(/*Radius*/ 37, /*Height*/ 160),
    BEGIN_LOOP(),
        CALL_NATIVE(try_print_debug_mario_level_info),
        CALL_NATIVE(bhv_mario_update),
        CALL_NATIVE(try_do_mario_debug_object_spawn),
    END_LOOP(),
};

const BehaviorScript bhvInstantActiveWarp[] = {
    BREAK(),
};

const BehaviorScript bhvAirborneWarp[] = {
    BREAK(),
};

const BehaviorScript bhvHardAirKnockBackWarp[] = {
    BREAK(),
};

const BehaviorScript bhvSpinAirborneCircleWarp[] = {
    BREAK(),
};

const BehaviorScript bhvDeathWarp[] = {
    BREAK(),
};

const BehaviorScript bhvSpinAirborneWarp[] = {
    BREAK(),
};

const BehaviorScript bhvFlyingWarp[] = {
    BREAK(),
};

const BehaviorScript bhvPaintingStarCollectWarp[] = {
    BREAK(),
};

const BehaviorScript bhvPaintingDeathWarp[] = {
    BREAK(),
};

const BehaviorScript bhvAirborneDeathWarp[] = {
    BREAK(),
};

const BehaviorScript bhvAirborneStarCollectWarp[] = {
    BREAK(),
};

const BehaviorScript bhvLaunchStarCollectWarp[] = {
    BREAK(),
};

const BehaviorScript bhvLaunchDeathWarp[] = {
    BREAK(),
};

const BehaviorScript bhvSwimmingWarp[] = {
    BREAK(),
};

UNUSED static const u64 behavior_data_unused_1 = 0;
const BehaviorScript bhvRandomAnimatedTexture[] = {
    BEGIN(OBJ_LIST_LEVEL),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    SET_FLOAT(oGraphYOffset, -16),
    BILLBOARD(),
    SET_INT(oAnimState, -1),
    BEGIN_LOOP(),
        ADD_INT(oAnimState, 1),
    END_LOOP(),
};

const BehaviorScript bhvMovingYellowCoin[] = {
    BEGIN(OBJ_LIST_LEVEL),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BILLBOARD(),
    SET_HITBOX(/*Radius*/ 100, /*Height*/ 64),
    SET_INT(oInteractType, INTERACT_COIN),
    SET_INT(oIntangibleTimer, 0),
    SET_INT(oAnimState, -1),
    CALL_NATIVE(bhv_moving_yellow_coin_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_moving_yellow_coin_loop),
        ADD_INT(oAnimState, 1),
    END_LOOP(),
};

const BehaviorScript bhvMovingBlueCoin[] = {
    BEGIN(OBJ_LIST_LEVEL),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BILLBOARD(),
    SET_INT(oIntangibleTimer, 0),
    SET_INT(oAnimState, -1),
    CALL_NATIVE(bhv_moving_blue_coin_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_moving_blue_coin_loop),
        ADD_INT(oAnimState, 1),
    END_LOOP(),
};

const BehaviorScript bhvBlueCoinSliding[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    BILLBOARD(),
    SET_INT(oIntangibleTimer, 0),
    SET_INT(oAnimState, -1),
    CALL_NATIVE(bhv_blue_coin_sliding_jumping_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_blue_coin_sliding_loop),
        ADD_INT(oAnimState, 1),
    END_LOOP(),
};

const BehaviorScript bhvBlueCoinJumping[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    BILLBOARD(),
    SET_INT(oIntangibleTimer, 0),
    SET_INT(oAnimState, -1),
    CALL_NATIVE(bhv_blue_coin_sliding_jumping_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_blue_coin_jumping_loop),
        ADD_INT(oAnimState, 1),
    END_LOOP(),
};

const BehaviorScript bhvJetStream[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_jet_stream_loop),
    END_LOOP(),
};

const BehaviorScript bhvHoot[] = {
    BEGIN(OBJ_LIST_POLELIKE),
    OR_INT(oFlags, (OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_ANIMATIONS(oAnimations, hoot_seg5_anims_05005768),
    SET_INT(oInteractType, INTERACT_HOOT),
    SET_HITBOX(/*Radius*/ 75, /*Height*/ 75),
    CALL_NATIVE(bhv_hoot_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_hoot_loop),
    END_LOOP(),
};

const BehaviorScript bhvBetaHoldableObject[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, (OBJ_FLAG_HOLDABLE | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SET_INT(oInteractType, INTERACT_GRABBABLE),
    DROP_TO_FLOOR(),
    SET_HITBOX(/*Radius*/ 40, /*Height*/ 50),
    CALL_NATIVE(bhv_beta_holdable_object_init),
    BEGIN_LOOP(),
        SET_INT(oIntangibleTimer, 0),
        CALL_NATIVE(bhv_beta_holdable_object_loop),
    END_LOOP(),
};

const BehaviorScript bhvCarrySomething1[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    BREAK(),
};

const BehaviorScript bhvCarrySomething2[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    BREAK(),
};

const BehaviorScript bhvCarrySomething3[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    BREAK(),
};

const BehaviorScript bhvCarrySomething4[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    BREAK(),
};

const BehaviorScript bhvCarrySomething5[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    BREAK(),
};

const BehaviorScript bhvCarrySomething6[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    BREAK(),
};

const BehaviorScript bhvObjectBubble[] = {
    BEGIN(OBJ_LIST_UNIMPORTANT),
    OR_INT(oFlags, (OBJ_FLAG_MOVE_Y_WITH_TERMINAL_VEL | OBJ_FLAG_MOVE_XZ_USING_FVEL | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    BILLBOARD(),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    SET_INT(oAnimState, -1),
    CALL_NATIVE(bhv_object_bubble_init),
    SET_RANDOM_FLOAT(oVelY, /*Minimum*/ 3, /*Range*/ 6),
    SET_INT_RAND_RSHIFT(oMoveAngleYaw, /*Minimum*/ 0, /*Right shift*/ 0),
    DELAY(1),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_object_bubble_loop),
    END_LOOP(),
};

const BehaviorScript bhvObjectWaterWave[] = {
    BEGIN(OBJ_LIST_UNIMPORTANT),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    SET_FLOAT(oFaceAnglePitch, 0),
    SET_FLOAT(oFaceAngleYaw, 0),
    SET_FLOAT(oFaceAngleRoll, 0),
    SET_INT(oAnimState, -1),
    CALL_NATIVE(bhv_object_water_wave_init),
    ADD_INT(oAnimState, 1),
    DELAY(6),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_object_water_wave_loop),
        ADD_INT(oAnimState, 1),
    BEGIN_REPEAT(6),
        CALL_NATIVE(bhv_object_water_wave_loop),
    END_REPEAT(),
    END_LOOP(),
};

const BehaviorScript bhvExplosion[] = {
    BEGIN(OBJ_LIST_DESTRUCTIVE),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    BILLBOARD(),
    SET_INTERACT_TYPE(INTERACT_DAMAGE),
    SET_INT(oDamageOrCoinValue, 2),
    SET_INT(oIntangibleTimer, 0),
    SET_HITBOX_WITH_OFFSET(/*Radius*/ 150, /*Height*/ 150, /*Downwards offset*/ 150),
    SET_INT(oAnimState, -1),
    CALL_NATIVE(bhv_explosion_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_explosion_loop),
        ADD_INT(oAnimState, 1),
    END_LOOP(),
};

const BehaviorScript bhvBobombBullyDeathSmoke[] = {
    BEGIN(OBJ_LIST_UNIMPORTANT),
    OR_INT(oFlags, (OBJ_FLAG_MOVE_Y_WITH_TERMINAL_VEL | OBJ_FLAG_MOVE_XZ_USING_FVEL | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    BILLBOARD(),
    SET_INT(oAnimState, -1),
    CALL_NATIVE(bhv_bobomb_bully_death_smoke_init),
    DELAY(1),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_dust_smoke_loop),
        ADD_INT(oAnimState, 1),
    END_LOOP(),
};

const BehaviorScript bhvSmoke[] = {
    BEGIN(OBJ_LIST_UNIMPORTANT),
    OR_INT(oFlags, (OBJ_FLAG_MOVE_Y_WITH_TERMINAL_VEL | OBJ_FLAG_MOVE_XZ_USING_FVEL | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    BILLBOARD(),
    SET_INT(oAnimState, -1),
    DELAY(1),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_dust_smoke_loop),
        ADD_INT(oAnimState, 1),
    END_LOOP(),
};

const BehaviorScript bhvBobombExplosionBubble[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    BILLBOARD(),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    CALL_NATIVE(bhv_bobomb_explosion_bubble_init),
    ADD_RANDOM_FLOAT(oPosX, /*Minimum*/ -50, /*Range*/ 100),
    ADD_RANDOM_FLOAT(oPosY, /*Minimum*/ -50, /*Range*/ 100),
    ADD_RANDOM_FLOAT(oPosZ, /*Minimum*/ -50, /*Range*/ 100),
    CALL(bhvBobombExplosionBubble3600),
    DELAY(1),
    BEGIN_LOOP(),
        CALL(bhvBobombExplosionBubble3600),
        CALL_NATIVE(bhv_bobomb_explosion_bubble_loop),
    END_LOOP(),
};

const BehaviorScript bhvBobombExplosionBubble3600[] = {
    ADD_RANDOM_FLOAT(oPosX, /*Minimum*/ -2, /*Range*/ 4),
    ADD_RANDOM_FLOAT(oPosZ, /*Minimum*/ -2, /*Range*/ 4),
    RETURN(),
};

const BehaviorScript bhvRespawner[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_respawner_loop),
    END_LOOP(),
};

const BehaviorScript bhvSmallBully[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, (OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_ANIMATIONS(oAnimations, otos_anime),
    DROP_TO_FLOOR(),
    SET_HOME(),
    CALL_NATIVE(bhv_small_bully_init),
    BEGIN_LOOP(),
        SET_INT(oIntangibleTimer, 0),
        CALL_NATIVE(bhv_bully_loop),
    END_LOOP(),
};

const BehaviorScript bhvBigBullyWithMinions[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, (OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_ANIMATIONS(oAnimations, otos_anime),
    SET_HOME(),
    CALL_NATIVE(bhv_big_bully_init),
    CALL_NATIVE(bhv_big_bully_with_minions_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_big_bully_with_minions_loop),
    END_LOOP(),
};

const BehaviorScript bhvJetStreamRingSpawner[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    HIDE(),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_jet_stream_ring_spawner_loop),
    END_LOOP(),
};

const BehaviorScript bhvJetStreamWaterRing[] = {
    BEGIN(OBJ_LIST_LEVEL),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    LOAD_ANIMATIONS(oAnimations, water_ring_seg6_anims_06013F7C),
    SET_HITBOX_WITH_OFFSET(/*Radius*/ 75, /*Height*/ 20, /*Downwards offset*/ 20),
    SET_INTERACT_TYPE(INTERACT_WATER_RING),
    SET_INT(oDamageOrCoinValue, 2),
    SET_INT(oIntangibleTimer, 0),
    CALL_NATIVE(bhv_jet_stream_water_ring_init),
    BEGIN_LOOP(),
        SET_INT(oIntangibleTimer, 0),
        CALL_NATIVE(bhv_jet_stream_water_ring_loop),
    END_LOOP(),
};

const BehaviorScript bhvBowserBomb[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    SET_INT(oIntangibleTimer, 0),
    SET_HITBOX_WITH_OFFSET(/*Radius*/ 40, /*Height*/ 40, /*Downwards offset*/ 40),
    DELAY(1),
    BEGIN_LOOP(),
        SET_INT(oIntangibleTimer, 0),
        CALL_NATIVE(bhv_bowser_bomb_loop),
    END_LOOP(),
};

const BehaviorScript bhvBowserBombExplosion[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BILLBOARD(),
    SET_FLOAT(oGraphYOffset, -288),
    SET_INT(oAnimState, -1),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_bowser_bomb_explosion_loop),
    END_LOOP(),
};

const BehaviorScript bhvBowserBombSmoke[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BILLBOARD(),
    SET_FLOAT(oGraphYOffset, -288),
    SET_INT(oAnimState, -1),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_bowser_bomb_smoke_loop),
    END_LOOP(),
};

const BehaviorScript bhvCelebrationStar[] = {
    BEGIN(OBJ_LIST_LEVEL),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BILLBOARD(),
    CALL_NATIVE(bhv_celebration_star_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_celebration_star_loop),
    END_LOOP(),
};

const BehaviorScript bhvCelebrationStarSparkle[] = {
    BEGIN(OBJ_LIST_UNIMPORTANT),
    BILLBOARD(),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    SET_FLOAT(oGraphYOffset, 25),
    SET_INT(oAnimState, -1),
    BEGIN_LOOP(),
        ADD_INT(oAnimState, 1),
        CALL_NATIVE(bhv_celebration_star_sparkle_loop),
    END_LOOP(),
};

const BehaviorScript bhvStarDust[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    BILLBOARD(),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    SET_INT(oAnimState, -1),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_star_dust),
        ANIMATE_TEXTURE(oAnimState, 2),
    END_LOOP(),
};

const BehaviorScript bhvLLLDrawbridgeSpawner[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    HIDE(),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_lll_drawbridge_spawner_loop),
    END_LOOP(),
};

const BehaviorScript bhvLLLDrawbridge[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, (OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_COLLISION_DATA(lll_seg7_collision_drawbridge),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_lll_drawbridge_loop),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};

const BehaviorScript bhvSmallBomp[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, (OBJ_FLAG_MOVE_XZ_USING_FVEL | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_COLLISION_DATA(wf_seg7_collision_small_bomp),
    CALL_NATIVE(bhv_small_bomp_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_small_bomp_loop),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};

const BehaviorScript bhvLargeBomp[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, (OBJ_FLAG_MOVE_XZ_USING_FVEL | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_COLLISION_DATA(wf_seg7_collision_large_bomp),
    CALL_NATIVE(bhv_large_bomp_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_large_bomp_loop),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};

const BehaviorScript bhvWFSlidingPlatform[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, (OBJ_FLAG_MOVE_XZ_USING_FVEL | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_COLLISION_DATA(wf_seg7_collision_sliding_brick_platform),
    CALL_NATIVE(bhv_wf_sliding_platform_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_wf_sliding_platform_loop),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};

const BehaviorScript bhvBirdsSoundLoop[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_birds_sound_loop),
    END_LOOP(),
};

const BehaviorScript bhvAmbientSounds[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    CALL_NATIVE(bhv_ambient_sounds_init),
    BEGIN_LOOP(),
    END_LOOP(),
};

const BehaviorScript bhvSandSoundLoop[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_sand_sound_loop),
    END_LOOP(),
};

const BehaviorScript bhvStar[] = {
    BEGIN(OBJ_LIST_LEVEL),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BILLBOARD(),
    CALL_NATIVE(bhv_init_room),
    CALL_NATIVE(bhv_collect_star_init),
    SCALE(/*Unused*/ 0, /*Field*/ 150),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_collect_star_loop),
        ANIMATE_TEXTURE(oAnimState, 2),
    END_LOOP(),
};

const BehaviorScript bhvSmallPiranhaFlame[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    BILLBOARD(),
    SET_OBJ_PHYSICS(/*Wall hitbox radius*/ 30, /*Gravity*/ 0, /*Bounciness*/ -50, /*Drag strength*/ 1000, /*Friction*/ 1000, /*Buoyancy*/ 200, /*Unused*/ 0, 0),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_small_piranha_flame_loop),
        ADD_INT(oAnimState, 1),
    END_LOOP(),
};

const BehaviorScript bhvFlyguyFlame[] = {
    BEGIN(OBJ_LIST_UNIMPORTANT),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BILLBOARD(),
    SET_OBJ_PHYSICS(/*Wall hitbox radius*/ 0, /*Gravity*/ 200, /*Bounciness*/ 0, /*Drag strength*/ 1000, /*Friction*/ 1000, /*Buoyancy*/ 200, /*Unused*/ 0, 0),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_fly_guy_flame_loop),
        ADD_INT(oAnimState, 1),
    END_LOOP(),
};

const BehaviorScript bhvBird[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    LOAD_ANIMATIONS(oAnimations, birds_seg5_anims_050009E8),
    ANIMATE(0),
    HIDE(),
    SCALE(/*Unused*/ 0, /*Field*/ 70),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_bird_update),
    END_LOOP(),
};

const BehaviorScript bhvTTCRotatingSolid[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SET_HOME(),
    SET_FLOAT(oCollisionDistance, 450),
    CALL_NATIVE(bhv_ttc_rotating_solid_init),
    SET_INT(oTTCRotatingSolidNumTurns, 1),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_ttc_rotating_solid_update),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};

const BehaviorScript bhvTTCPendulum[] = {
    BEGIN(OBJ_LIST_SURFACE),
    LOAD_COLLISION_DATA(ttc_seg7_collision_clock_pendulum),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SET_FLOAT(oCollisionDistance, 1500),
    CALL_NATIVE(bhv_ttc_pendulum_init),
    SET_FLOAT(oTTCPendulumAccelDir, 1),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_ttc_pendulum_update),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};

const BehaviorScript bhvTTCTreadmill[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SET_FLOAT(oCollisionDistance, 750),
    CALL_NATIVE(bhv_ttc_treadmill_init),
    DELAY(1),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_ttc_treadmill_update),
        CALL_NATIVE(cur_obj_compute_vel_xz),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};

const BehaviorScript bhvTTCMovingBar[] = {
    BEGIN(OBJ_LIST_SURFACE),
    LOAD_COLLISION_DATA(ttc_seg7_collision_sliding_surface),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SET_HOME(),
    SET_FLOAT(oCollisionDistance, 550),
    CALL_NATIVE(bhv_ttc_moving_bar_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_ttc_moving_bar_update),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};

const BehaviorScript bhvTTCCog[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SET_FLOAT(oCollisionDistance, 400),
    CALL_NATIVE(bhv_ttc_cog_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_ttc_cog_update),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};

const BehaviorScript bhvTTCPitBlock[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SET_HOME(),
    SET_FLOAT(oCollisionDistance, 350),
    CALL_NATIVE(bhv_ttc_pit_block_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_ttc_pit_block_update),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};

const BehaviorScript bhvTTCElevator[] = {
    BEGIN(OBJ_LIST_SURFACE),
    LOAD_COLLISION_DATA(ttc_seg7_collision_clock_platform),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SET_HOME(),
    SET_FLOAT(oCollisionDistance, 400),
    CALL_NATIVE(bhv_ttc_elevator_init),
    SET_FLOAT(oTTCElevatorDir, 1),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_ttc_elevator_update),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};

const BehaviorScript bhvTTC2DRotator[] = {
    BEGIN(OBJ_LIST_SURFACE),
    LOAD_COLLISION_DATA(ttc_seg7_collision_clock_main_rotation),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    SET_FLOAT(oCollisionDistance, 1800),
    CALL_NATIVE(bhv_ttc_2d_rotator_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_ttc_2d_rotator_update),
    END_LOOP(),
};

const BehaviorScript bhvTTCSpinner[] = {
    BEGIN(OBJ_LIST_SURFACE),
    LOAD_COLLISION_DATA(ttc_seg7_collision_rotating_clock_platform2),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    SET_FLOAT(oCollisionDistance, 450),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_ttc_spinner_update),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
};
