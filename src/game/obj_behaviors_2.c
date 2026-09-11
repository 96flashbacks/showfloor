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

// This file contains all the objects programmed by Toshio Iwawaki

#define POS_OP_SAVE_POSITION 0
#define POS_OP_COMPUTE_VELOCITY 1
#define POS_OP_RESTORE_POSITION 2

#define o gCurrentObject

/* BSS (declared to force order) */
extern f32 sObjSavedPosX;
extern f32 sObjSavedPosY;
extern f32 sObjSavedPosZ;

f32 sObjSavedPosX;
f32 sObjSavedPosY;
f32 sObjSavedPosZ;

static void obj_compute_vel_from_move_pitch(f32 speed) {
    o->oForwardVel = speed * coss(o->oMoveAnglePitch);
    o->oVelY = speed * -sins(o->oMoveAnglePitch);
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

static s32 obj_move_pitch_approach(s16 target, s16 delta) {
    o->oMoveAnglePitch = approach_s16_symmetric(o->oMoveAnglePitch, target, delta);

    if ((s16) o->oMoveAnglePitch == target) {
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

static s16 random_linear_offset(s16 base, s16 range) {
    return base + (s16) (range * random_float());
}

static void obj_die_if_health_non_positive(void) {
    if (o->oHealth <= 0) {
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

#include "behaviors/triplet_butterfly.inc.c" // pathfly.c
