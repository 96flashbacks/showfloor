#include <PR/ultratypes.h>

#include "sm64.h"
#include "actors/common0.h"
#include "actors/group17.h"
#include "audio/external.h"
#include "behavior_actions.h"
#include "behavior_data.h"
#include "camera.h"
#include "dialog_ids.h"
#include "engine/behavior_script.h"
#include "engine/math_util.h"
#include "engine/surface_collision.h"
#include "engine/surface_load.h"
#include "game_init.h"
#include "geo_misc.h"
#include "ingame_menu.h"
#include "interaction.h"
#include "level_table.h"
#include "level_update.h"
#include "levels/ccm/header.h"
#include "levels/lll/header.h"
#include "levels/ttc/header.h"
#include "mario.h"
#include "mario_actions_cutscene.h"
#include "memory.h"
#include "obj_behaviors.h"
#include "obj_behaviors_2.h"
#include "object_constants.h"
#include "object_helpers.h"
#include "object_list_processor.h"
#include "platform_displacement.h"
#include "rendering_graph_node.h"
#include "save_file.h"
#include "seq_ids.h"
#include "spawn_sound.h"

#define POS_OP_SAVE_POSITION 0
#define POS_OP_COMPUTE_VELOCITY 1
#define POS_OP_RESTORE_POSITION 2

#define o gCurrentObject

void huge_goomba_weakly_attacked(void);

void wiggler_jumped_on_attack_handler(void) {
    cur_obj_play_sound_2(SOUND_OBJ_WIGGLER_ATTACKED);
    o->oAction = WIGGLER_ACT_JUMPED_ON;
    o->oForwardVel = o->oVelY = 0.0f;
    o->oWigglerSquishSpeed = 0.4f;
}

/* BSS (declared to force order) */
extern s32 sNumActiveFirePiranhaPlants;
extern s32 sNumKilledFirePiranhaPlants;
extern f32 sObjSavedPosX;
extern f32 sObjSavedPosY;
extern f32 sObjSavedPosZ;
extern struct Object *sMasterTreadmill;

/**
 * The treadmill that plays sounds and controls the others on random setting.
 */
struct Object *sMasterTreadmill;

f32 sObjSavedPosX;
f32 sObjSavedPosY;
f32 sObjSavedPosZ;

//! Although having no return value, this function
//! must be u32 to match other functions on -O2.
static BAD_RETURN(u32) obj_perform_position_op(s32 op) {
    switch (op) {
        case POS_OP_SAVE_POSITION:
            sObjSavedPosX = o->oPosX;
            sObjSavedPosY = o->oPosY;
            sObjSavedPosZ = o->oPosZ;
            break;

        case POS_OP_COMPUTE_VELOCITY:
            o->oVelX = o->oPosX - sObjSavedPosX;
            o->oVelY = o->oPosY - sObjSavedPosY;
            o->oVelZ = o->oPosZ - sObjSavedPosZ;
            break;

        case POS_OP_RESTORE_POSITION:
            o->oPosX = sObjSavedPosX;
            o->oPosY = sObjSavedPosY;
            o->oPosZ = sObjSavedPosZ;
            break;
    }
}

static void obj_set_dist_from_home(f32 distFromHome) {
    o->oPosX = o->oHomeX + distFromHome * coss(o->oMoveAngleYaw);
    o->oPosZ = o->oHomeZ + distFromHome * sins(o->oMoveAngleYaw);
}

static void obj_compute_vel_from_move_pitch(f32 speed) {
    o->oForwardVel = speed * coss(o->oMoveAnglePitch);
    o->oVelY = speed * -sins(o->oMoveAnglePitch);
}

static s32 clamp_s16(s16 *value, s16 minimum, s16 maximum) {
    if (*value <= minimum) {
        *value = minimum;
    } else if (*value >= maximum) {
        *value = maximum;
    } else {
        return FALSE;
    }

    return TRUE;
}

static s32 clamp_f32(f32 *value, f32 minimum, f32 maximum) {
    if (*value <= minimum) {
        *value = minimum;
    } else if (*value >= maximum) {
        *value = maximum;
    } else {
        return FALSE;
    }

    return TRUE;
}

static s32 approach_f32_ptr(f32 *px, f32 target, f32 delta) {
    if (*px > target) {
        delta = -delta;
    }

    *px += delta;

    if ((*px - target) * delta >= 0) {
        *px = target;
        return TRUE;
    }
    return FALSE;
}

static s32 cur_obj_play_sound_at_anim_range(s8 arg0, s8 arg1, u32 sound) {
    s32 val04;

    if ((val04 = o->header.gfx.animInfo.animAccel / 0x10000) <= 0) {
        val04 = 1;
    }

    if (cur_obj_check_anim_frame_in_range(arg0, val04) || cur_obj_check_anim_frame_in_range(arg1, val04)) {
        cur_obj_play_sound_2(sound);
        return TRUE;
    }

    return FALSE;
}

static void cur_obj_init_anim_extend(s32 arg0) {
    cur_obj_init_animation_with_sound(arg0);
    cur_obj_extend_animation_if_at_end();
}

static s32 cur_obj_init_anim_and_check_if_end(s32 arg0) {
    cur_obj_init_animation_with_sound(arg0);
    return cur_obj_check_if_near_animation_end();
}

static s32 obj_forward_vel_approach(f32 target, f32 delta) {
    return approach_f32_ptr(&o->oForwardVel, target, delta);
}

static s32 obj_move_pitch_approach(s16 target, s16 delta) {
    o->oMoveAnglePitch = approach_s16_symmetric(o->oMoveAnglePitch, target, delta);

    if ((s16) o->oMoveAnglePitch == target) {
        return TRUE;
    }

    return FALSE;
}

static s32 obj_resolve_object_collisions(s32 *targetYaw) {
    struct Object *otherObject;
    f32 dx;
    f32 dz;
    s16 angle;
    f32 radius;
    f32 otherRadius;
    f32 relativeRadius;
    f32 newCenterX;
    f32 newCenterZ;

    if (o->numCollidedObjs != 0) {
        otherObject = o->collidedObjs[0];
        if (otherObject != gMarioObject) {
            //! If one object moves after collisions are detected and this code
            //  runs, the objects can move toward each other (transport cloning)

            dx = otherObject->oPosX - o->oPosX;
            dz = otherObject->oPosZ - o->oPosZ;
            angle = atan2s(dx, dz); //! This should be atan2s(dz, dx)

            radius = o->hitboxRadius;
            otherRadius = otherObject->hitboxRadius;
            relativeRadius = radius / (radius + otherRadius);

            newCenterX = o->oPosX + dx * relativeRadius;
            newCenterZ = o->oPosZ + dz * relativeRadius;

            o->oPosX = newCenterX - radius * coss(angle);
            o->oPosZ = newCenterZ - radius * sins(angle);

            otherObject->oPosX = newCenterX + otherRadius * coss(angle);
            otherObject->oPosZ = newCenterZ + otherRadius * sins(angle);

            if (targetYaw != NULL && abs_angle_diff(o->oMoveAngleYaw, angle) < 0x4000) {
                // Bounce off object (or it would, if the above atan2s bug
                // were fixed)
                *targetYaw = (s16)(angle - o->oMoveAngleYaw + angle + 0x8000);
                return TRUE;
            }
        }
    }

    return FALSE;
}

static void obj_set_knockback_action(s32 attackType) {
    switch (attackType) {
        case ATTACK_KICK_OR_TRIP:
        case ATTACK_FAST_ATTACK:
            o->oAction = OBJ_ACT_VERTICAL_KNOCKBACK;
            o->oForwardVel = 20.0f;
            o->oVelY = 50.0f;
            break;

        default:
            o->oAction = OBJ_ACT_HORIZONTAL_KNOCKBACK;
            o->oForwardVel = 50.0f;
            o->oVelY = 30.0f;
            break;
    }

    o->oFlags &= ~OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW;
    o->oMoveAngleYaw = obj_angle_to_object(gMarioObject, o);
}

static void obj_set_squished_action(void) {
    cur_obj_play_sound_2(SOUND_OBJ_STOMPED);
    o->oAction = OBJ_ACT_SQUISHED;
}

static void obj_die_if_health_non_positive(void) {
    if (o->oHealth <= 0) {
        if (o->oDeathSound == 0) {
            spawn_mist_particles_with_sound(SOUND_OBJ_DEFAULT_DEATH);
        } else if (o->oDeathSound > 0) {
            spawn_mist_particles_with_sound(o->oDeathSound);
        } else {
            spawn_mist_particles();
        }

        // This doesn't do anything
        obj_spawn_loot_yellow_coins(o, o->oNumLootCoins, 20.0f);

        if (o->oHealth < 0) {
            cur_obj_hide();
            cur_obj_become_intangible();
        } else {
            obj_mark_for_deletion(o);
        }
    }
}

static void obj_act_knockback(UNUSED f32 baseScale) {
    cur_obj_update_floor_and_walls();

    if (o->header.gfx.animInfo.curAnim != NULL) {
        cur_obj_extend_animation_if_at_end();
    }

    //! Dies immediately if above lava
    if ((o->oMoveFlags
         & (OBJ_MOVE_MASK_ON_GROUND | OBJ_MOVE_MASK_IN_WATER | OBJ_MOVE_HIT_WALL | OBJ_MOVE_ABOVE_LAVA))
        || (o->oAction == OBJ_ACT_VERTICAL_KNOCKBACK && o->oTimer >= 9)) {
        obj_die_if_health_non_positive();
    }

    cur_obj_move_standard(-78);
}



static void obj_act_squished(f32 baseScale) {
    f32 targetScaleY = baseScale * 0.3f;

    cur_obj_update_floor_and_walls();

    if (o->header.gfx.animInfo.curAnim != NULL) {
        cur_obj_extend_animation_if_at_end();
    }

    if (approach_f32_ptr(&o->header.gfx.scale[1], targetScaleY, baseScale * 0.14f)) {
        o->header.gfx.scale[0] = o->header.gfx.scale[2] = baseScale * 2.0f - o->header.gfx.scale[1];

        if (o->oTimer > 15) {
            obj_die_if_health_non_positive();
        }
    }

    o->oForwardVel = 0.0f;
    cur_obj_move_standard(-78);
}

static s32 obj_update_standard_actions(f32 scale) {
    if (o->oAction < 100) {
        return TRUE;
    } else {
        cur_obj_become_intangible();

        switch (o->oAction) {
            case OBJ_ACT_HORIZONTAL_KNOCKBACK:
            case OBJ_ACT_VERTICAL_KNOCKBACK:
                obj_act_knockback(scale);
                break;

            case OBJ_ACT_SQUISHED:
                obj_act_squished(scale);
                break;
        }

        return FALSE;
    }
}

static s32 obj_bounce_off_walls_edges_objects(s32 *targetYaw) {
    if (o->oMoveFlags & OBJ_MOVE_HIT_WALL) {
        *targetYaw = cur_obj_reflect_move_angle_off_wall();
    } else if (o->oMoveFlags & OBJ_MOVE_HIT_EDGE) {
        *targetYaw = (s16)(o->oMoveAngleYaw + 0x8000);
    } else if (!obj_resolve_object_collisions(targetYaw)) {
        return FALSE;
    }

    return TRUE;
}

static s32 obj_resolve_collisions_and_turn(s16 targetYaw, s16 turnSpeed) {
    obj_resolve_object_collisions(NULL);

    if (cur_obj_rotate_yaw_toward(targetYaw, turnSpeed)) {
        return FALSE;
    } else {
        return TRUE;
    }
}


static s32 obj_face_yaw_approach(s16 targetYaw, s16 deltaYaw) {
    o->oFaceAngleYaw = approach_s16_symmetric(o->oFaceAngleYaw, targetYaw, deltaYaw);

    if ((s16) o->oFaceAngleYaw == targetYaw) {
        return TRUE;
    }

    return FALSE;
}

static s32 obj_face_roll_approach(s16 targetRoll, s16 deltaRoll) {
    o->oFaceAngleRoll = approach_s16_symmetric(o->oFaceAngleRoll, targetRoll, deltaRoll);

    if ((s16) o->oFaceAngleRoll == targetRoll) {
        return TRUE;
    }

    return FALSE;
}

static void obj_roll_to_match_yaw_turn(s16 targetYaw, s16 maxRoll, s16 rollSpeed) {
    s16 targetRoll = o->oMoveAngleYaw - targetYaw;
    clamp_s16(&targetRoll, -maxRoll, maxRoll);
    obj_face_roll_approach(targetRoll, rollSpeed);
}

static s16 random_linear_offset(s16 base, s16 range) {
    return base + (s16) (range * random_float());
}

static s16 random_mod_offset(s16 base, s16 step, s16 mod) {
    return base + step * (random_u16() % mod);
}

static s16 obj_random_fixed_turn(s16 delta) {
    return o->oMoveAngleYaw + (s16) random_sign() * delta;
}



UNUSED static void obj_unused_die(void) {
    o->oHealth = 0;
    obj_die_if_health_non_positive();
}

static s32 obj_die_if_above_lava_and_health_non_positive(void) {
    if (o->oMoveFlags & OBJ_MOVE_UNDERWATER_ON_GROUND) {
        if (o->oGravity + o->oBuoyancy > 0.0f
            || find_water_level(o->oPosX, o->oPosZ) - o->oPosY < 150.0f) {
            return FALSE;
        }
    } else if (!(o->oMoveFlags & OBJ_MOVE_ABOVE_LAVA)) {
        if (o->oMoveFlags & OBJ_MOVE_ENTERED_WATER) {
            if (o->oWallHitboxRadius < 200.0f) {
                cur_obj_play_sound_2(SOUND_OBJ_DIVING_INTO_WATER);
            } else {
                cur_obj_play_sound_2(SOUND_OBJ_DIVING_IN_WATER);
            }
        }
        return FALSE;
    }

    obj_die_if_health_non_positive();
    return TRUE;
}

static s32 obj_check_attacks(struct ObjectHitbox *hitbox, s32 attackedMarioAction) {
    s32 attackType;

    obj_set_hitbox(o, hitbox);

    //! Dies immediately if above lava
    if (obj_die_if_above_lava_and_health_non_positive()) {
        return 1;
    } else if (o->oInteractStatus & INT_STATUS_INTERACTED) {
        if (o->oInteractStatus & INT_STATUS_ATTACKED_MARIO) {
            if (o->oAction != attackedMarioAction) {
                o->oAction = attackedMarioAction;
                o->oTimer = 0;
            }
        } else {
            attackType = o->oInteractStatus & INT_STATUS_ATTACK_MASK;
            obj_die_if_health_non_positive();
            o->oInteractStatus = 0;
            return attackType;
        }
    }

    o->oInteractStatus = 0;
    return 0;
}

static s32 obj_handle_attacks(struct ObjectHitbox *hitbox, s32 attackedMarioAction,
                              u8 *attackHandlers) {
    s32 attackType;

    obj_set_hitbox(o, hitbox);

    //! Die immediately if above lava
    if (obj_die_if_above_lava_and_health_non_positive()) {
        return 1;
    } else if (o->oInteractStatus & INT_STATUS_INTERACTED) {
        if (o->oInteractStatus & INT_STATUS_ATTACKED_MARIO) {
            if (o->oAction != attackedMarioAction) {
                o->oAction = attackedMarioAction;
                o->oTimer = 0;
            }
        } else {
            attackType = o->oInteractStatus & INT_STATUS_ATTACK_MASK;

            switch (attackHandlers[attackType - 1]) {
                case ATTACK_HANDLER_NOP:
                    break;

                case ATTACK_HANDLER_DIE_IF_HEALTH_NON_POSITIVE:
                    obj_die_if_health_non_positive();
                    break;

                case ATTACK_HANDLER_KNOCKBACK:
                    obj_set_knockback_action(attackType);
                    break;

                case ATTACK_HANDLER_SQUISHED:
                    obj_set_squished_action();
                    break;

                case ATTACK_HANDLER_SPECIAL_KOOPA_LOSE_SHELL:
                    shelled_koopa_attack_handler(attackType);
                    break;

                case ATTACK_HANDLER_SET_SPEED_TO_ZERO:
                    obj_set_speed_to_zero();
                    break;

                case ATTACK_HANDLER_SPECIAL_WIGGLER_JUMPED_ON:
                    wiggler_jumped_on_attack_handler();
                    break;

                case ATTACK_HANDLER_SPECIAL_HUGE_GOOMBA_WEAKLY_ATTACKED:
                    huge_goomba_weakly_attacked();
                    break;

                case ATTACK_HANDLER_SQUISHED_WITH_BLUE_COIN:
                    o->oNumLootCoins = -1;
                    obj_set_squished_action();
                    break;
            }

            o->oInteractStatus = 0;
            return attackType;
        }
    }

    o->oInteractStatus = 0;
    return 0;
}

static void treat_far_home_as_mario(f32 threshold) {
    f32 dx = o->oHomeX - o->oPosX;
    f32 dy = o->oHomeY - o->oPosY;
    f32 dz = o->oHomeZ - o->oPosZ;
    f32 distance = sqrtf(dx * dx + dy * dy + dz * dz);

    if (distance > threshold) {
        o->oAngleToMario = atan2s(dz, dx);
        o->oDistanceToMario = 25000.0f;
    } else {
        dx = o->oHomeX - gMarioObject->oPosX;
        dy = o->oHomeY - gMarioObject->oPosY;
        dz = o->oHomeZ - gMarioObject->oPosZ;
        distance = sqrtf(dx * dx + dy * dy + dz * dz);

        if (distance > threshold) {
            o->oDistanceToMario = 20000.0f;
        }
    }
}

#include "behaviors/koopa.inc.c" // TODO: Text arg field name
#include "behaviors/goomba.inc.c"

/**
 * Used by bowser, fly guy, piranha plant, and fire spitters.
 */
void obj_spit_fire(s16 relativePosX, s16 relativePosY, s16 relativePosZ, f32 scale, s32 model,
                   f32 startSpeed, f32 endSpeed, s16 movePitch) {
    struct Object *obj = spawn_object_relative_with_scale(1, relativePosX, relativePosY, relativePosZ,
                                                          scale, o, model, bhvSmallPiranhaFlame);

    if (obj != NULL) {
        obj->oSmallPiranhaFlameStartSpeed = startSpeed;
        obj->oSmallPiranhaFlameEndSpeed = endSpeed;
        obj->oSmallPiranhaFlameModel = model;
        obj->oMoveAnglePitch = movePitch;
    }
}

struct ObjectHitbox sPiranhaPlantFireHitbox = {
    /* interactType:      */ INTERACT_FLAME,
    /* downOffset:        */ 10,
    /* damageOrCoinValue: */ 0,
    /* health:            */ 0,
    /* numLootCoins:      */ 0,
    /* radius:            */ 10,
    /* height:            */ 20,
    /* hurtboxRadius:     */ 10,
    /* hurtboxHeight:     */ 20,
};

#include "behaviors/flame.inc.c"
#include "behaviors/horizontal_grindel.inc.c"
#include "behaviors/ttc_rotating_solid.inc.c"
#include "behaviors/ttc_pendulum.inc.c"
#include "behaviors/ttc_treadmill.inc.c" // TODO
#include "behaviors/ttc_moving_bar.inc.c"
#include "behaviors/ttc_cog.inc.c"
#include "behaviors/ttc_pit_block.inc.c"
#include "behaviors/ttc_elevator.inc.c"
#include "behaviors/ttc_2d_rotator.inc.c"
#include "behaviors/ttc_spinner.inc.c"

void obj_set_speed_to_zero(void) {
    o->oForwardVel = o->oVelY = 0.0f;
}

#include "behaviors/bird.inc.c"
