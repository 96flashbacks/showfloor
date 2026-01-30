#include <PR/ultratypes.h>

#include "prevent_bss_reordering.h"
#include "sm64.h"
#include "area.h"
#include "audio/external.h"
#include "behavior_data.h"
#include "camera.h"
#include "dialog_ids.h"
#include "engine/behavior_script.h"
#include "engine/graph_node.h"
#include "engine/math_util.h"
#include "engine/surface_collision.h"
#include "game_init.h"
#include "gfx_dimensions.h"
#include "ingame_menu.h"
#include "interaction.h"
#include "level_table.h"
#include "level_update.h"
#include "mario.h"
#include "mario_actions_cutscene.h"
#include "mario_actions_moving.h"
#include "mario_step.h"
#include "moving_texture.h"
#include "object_helpers.h"
#include "object_list_processor.h"
#include "save_file.h"
#include "seq_ids.h"
#include "sound_init.h"

// unused
UNUSED static void stub_is_textbox_active(u16 *arg) {
    if (get_dialog_id() == DIALOG_NONE) {
        *arg = 0;
    }
}

/**
 * spawn_obj_at_mario_rel_yaw: Spawns object at Mario with relative yaw.
 * Spawns object with given behavior and model and copies over Mario's position
 * and yaw plus relative yaw.
 */
struct Object *spawn_obj_at_mario_rel_yaw(struct MarioState *m, s32 model,
                                          const BehaviorScript *behavior, s16 relYaw) {
    struct Object *o = spawn_object(m->marioObj, model, behavior);

    o->oFaceAngleYaw = m->faceAngle[1] + relYaw;
    o->oPosX = m->pos[0];
    o->oPosY = m->pos[1];
    o->oPosZ = m->pos[2];

    return o;
}

/**
 * mario_ready_to_speak: Determine if Mario is able to speak to a NPC
 * The following conditions must be met in order for Mario to be considered
 * ready to speak.
 * 1: Mario's action must be in the stationary or moving action groups, or if
 *    not, he must be in the "waiting for dialog" state.
 * 2: Mario mat not be be invulnerable.
 * 3: Mario must not be in first person mode.
 */
s32 mario_ready_to_speak(void) {
    u32 actionGroup = gMarioState->action & ACT_GROUP_MASK;
    s32 isReadyToSpeak = FALSE;

    if ((gMarioState->action == ACT_WAITING_FOR_DIALOG || actionGroup == ACT_GROUP_STATIONARY
         || actionGroup == ACT_GROUP_MOVING)
        && (!(gMarioState->action & (ACT_FLAG_INVULNERABLE))
            && gMarioState->action != ACT_FIRST_PERSON)) {
        isReadyToSpeak = TRUE;
    }

    return isReadyToSpeak;
}

// (can) place Mario in dialog?
// initiate dialog?
// return values:
// 0 = not in dialog
// 1 = starting dialog
// 2 = speaking
s32 set_mario_npc_dialog(s32 actionArg) {
    s32 dialogState = MARIO_DIALOG_STATUS_NONE;

    // in dialog
    if (gMarioState->action == ACT_READING_NPC_DIALOG) {
        if (gMarioState->actionState < 8) {
            dialogState = MARIO_DIALOG_STATUS_START; // starting dialog
        }
        if (gMarioState->actionState == 8) {
            if (actionArg == MARIO_DIALOG_STOP) {
                gMarioState->actionState++; // exit dialog
            } else {
                dialogState = MARIO_DIALOG_STATUS_SPEAK;
            }
        }
    } else if (actionArg != 0 && mario_ready_to_speak()) {
        gMarioState->usedObj = gCurrentObject;
        set_mario_action(gMarioState, ACT_READING_NPC_DIALOG, actionArg);
        dialogState = MARIO_DIALOG_STATUS_START; // starting dialog
    }

    return dialogState;
}

// actionargs:
// 1 : no head turn
// 2 : look up
// 3 : look down
// actionstate values:
// 0 - 7: looking toward npc
// 8: in dialog
// 9 - 22: looking away from npc
// 23: end
s32 act_reading_npc_dialog(struct MarioState *m) {
    s32 headTurnAmount = 0;
    s16 angleToNPC;

    if (m->actionArg == MARIO_DIALOG_LOOK_UP) {
        headTurnAmount = -1024;
    }
    if (m->actionArg == MARIO_DIALOG_LOOK_DOWN) {
        headTurnAmount = 384;
    }

    if (m->actionState < 8) {
        // turn to NPC
        angleToNPC = mario_obj_angle_to_object(m, m->usedObj);
        m->faceAngle[1] =
            angleToNPC - approach_s32((angleToNPC - m->faceAngle[1]) << 16 >> 16, 0, 2048, 2048);
        // turn head to npc
        m->actionTimer += headTurnAmount;
        // set animation
        set_mario_animation(m, m->heldObj == NULL ? MARIO_ANIM_FIRST_PERSON
                                                  : MARIO_ANIM_IDLE_WITH_LIGHT_OBJ);
    } else if (m->actionState >= 9 && m->actionState < 17) {
        // look back from facing NPC
        m->actionTimer -= headTurnAmount;
    } else if (m->actionState == 23) {
        set_mario_action(m, m->heldObj == NULL ? ACT_IDLE : ACT_HOLD_IDLE, 0);
    }
    vec3f_copy(m->marioObj->header.gfx.pos, m->pos);
    vec3s_set(m->marioObj->header.gfx.angle, 0, m->faceAngle[1], 0);
    vec3s_set(m->marioBodyState->headAngle, m->actionTimer, 0, 0);

    if (m->actionState != 8) {
        m->actionState++;
    }

    return FALSE;
}

// puts Mario in a state where he's waiting for (npc) dialog; doesn't do much
s32 act_waiting_for_dialog(struct MarioState *m) {
    set_mario_animation(m,
                        m->heldObj == NULL ? MARIO_ANIM_FIRST_PERSON : MARIO_ANIM_IDLE_WITH_LIGHT_OBJ);
    vec3f_copy(m->marioObj->header.gfx.pos, m->pos);
    vec3s_set(m->marioObj->header.gfx.angle, 0, m->faceAngle[1], 0);
    return FALSE;
}

// makes Mario disappear and triggers warp
s32 act_disappeared(struct MarioState *m) {
    set_mario_animation(m, MARIO_ANIM_A_POSE);
    stop_and_set_height_to_floor(m);
    m->marioObj->header.gfx.node.flags &= ~GRAPH_RENDER_ACTIVE;
    if (m->actionArg) {
        m->actionArg--;
        if ((m->actionArg & 0xFFFF) == 0) {
            level_trigger_warp(m, m->actionArg >> 16);
        }
    }
    return FALSE;
}

s32 act_reading_automatic_dialog(struct MarioState *m) {
    u32 actionArg;

    m->actionState++;
    if (m->actionState == 0) {
        enable_time_stop();
    }
    if (m->actionState < 1) {
        set_mario_animation(m, MARIO_ANIM_IDLE_HEAD_LEFT);
    } else {
        // set mario dialog
        if (m->actionState == 1) {
            actionArg = m->actionArg;
            create_dialog_box(GET_LOW_U16_OF_32(actionArg));
        }
        // wait until dialog is done
        else if (m->actionState == 2) {
            if (gMenuState == 3 && gDialogBoxAngle == 45.0f) {
                disable_time_stop();
                // make mario walk into door after star dialog
                set_mario_action(m, ACT_IDLE, 0);
            } else {
                m->actionState--;
            }
        }
    }

    return FALSE;
}

s32 act_reading_sign(struct MarioState *m) {
    struct Object *marioObj = m->marioObj;

    play_sound_if_no_flag(m, SOUND_ACTION_READ_SIGN, MARIO_ACTION_SOUND_PLAYED);

    switch (m->actionState) {
        // start dialog
        case 0:
            trigger_cutscene_dialog(1);
            enable_time_stop();
            // reading sign
            set_mario_animation(m, MARIO_ANIM_FIRST_PERSON);
            m->actionState = 1;
            // intentional fall through
        // turn toward sign
        case 1:
            m->faceAngle[1] += marioObj->oMarioPoleUnk108 / 11;
            m->pos[0] += marioObj->oMarioReadingSignDPosX / 11.0f;
            m->pos[2] += marioObj->oMarioReadingSignDPosZ / 11.0f;
            // create the text box
            if (m->actionTimer++ == 10) {
                create_dialog_inverted_box(m->usedObj->oBhvParams2ndByte);
                m->actionState = 2;
            }
            break;
        // in dialog
        case 2:
            // dialog finished
            if (gCamera->cutscene == 0) {
                disable_time_stop();
                set_mario_action(m, ACT_IDLE, 0);
            }
            break;
    }

    vec3f_copy(marioObj->header.gfx.pos, m->pos);
    vec3s_set(marioObj->header.gfx.angle, 0, m->faceAngle[1], 0);
    return FALSE;
}

s32 act_debug_free_move(struct MarioState *m) {
    struct Surface *surf;
    f32 floorHeight;
    Vec3f pos;
    f32 speed;
    u32 action;

    // integer immediates, generates convert instructions for some reason
    speed = gPlayer1Controller->buttonDown & B_BUTTON ? 4 : 1;
    if (gPlayer1Controller->buttonDown & L_TRIG) {
        speed = 0.01f;
    }

    set_mario_animation(m, MARIO_ANIM_A_POSE);
    vec3f_copy(pos, m->pos);

    if (gPlayer1Controller->buttonDown & U_JPAD) {
        pos[1] += 16.0f * speed;
    }
    if (gPlayer1Controller->buttonDown & D_JPAD) {
        pos[1] -= 16.0f * speed;
    }

    if (m->intendedMag > 0) {
        pos[0] += 32.0f * speed * sins(m->intendedYaw);
        pos[2] += 32.0f * speed * coss(m->intendedYaw);
    }

    resolve_and_return_wall_collisions(pos, 60.0f, 50.0f);

    floorHeight = find_floor(pos[0], pos[1], pos[2], &surf);
    if (surf != NULL) {
        if (pos[1] < floorHeight) {
            pos[1] = floorHeight;
        }
        vec3f_copy(m->pos, pos);
    }

    m->faceAngle[1] = m->intendedYaw;
    vec3f_copy(m->marioObj->header.gfx.pos, m->pos);
    vec3s_set(m->marioObj->header.gfx.angle, 0, m->faceAngle[1], 0);

    if (gPlayer1Controller->buttonPressed == A_BUTTON) {
        if (m->pos[1] <= m->waterLevel - 100) {
            action = ACT_WATER_IDLE;
        } else {
            action = ACT_IDLE;
        }
        set_mario_action(m, action, 0);
    }

    return FALSE;
}

void general_star_dance_handler(struct MarioState *m) {
    if (m->actionState == 0) {
        switch (++m->actionTimer) {
            case 1:
                spawn_object(m->marioObj, MODEL_STAR, bhvCelebrationStar);
                disable_background_sound();
                play_course_clear();
                break;

            case 42:
                play_sound(SOUND_MARIO_HERE_WE_GO, m->marioObj->header.gfx.cameraToObject);
                break;

            case 80:
                if (!(m->actionArg & 1)) {
                    level_trigger_warp(m, WARP_OP_STAR_EXIT);
                } else {
                    enable_time_stop();
                    create_dialog_box_with_response(gLastCompletedStarNum == 7 ? DIALOG_013
                                                                               : DIALOG_014);
                    m->actionState = 1;
                }
                break;
        }
    }
}

s32 act_star_dance(struct MarioState *m) {
    m->faceAngle[1] = m->area->camera->yaw;
    set_mario_animation(m, m->actionState == 2 ? MARIO_ANIM_RETURN_FROM_STAR_DANCE
                                               : MARIO_ANIM_STAR_DANCE);
    general_star_dance_handler(m);
    if (m->actionState != 2 && m->actionTimer >= 40) {
        m->marioBodyState->handState = MARIO_HAND_PEACE_SIGN;
    }
    stop_and_set_height_to_floor(m);
    return FALSE;
}

s32 act_star_dance_water(struct MarioState *m) {
    m->faceAngle[1] = m->area->camera->yaw;
    set_mario_animation(m, m->actionState == 2 ? MARIO_ANIM_RETURN_FROM_WATER_STAR_DANCE
                                               : MARIO_ANIM_WATER_STAR_DANCE);
    vec3f_copy(m->marioObj->header.gfx.pos, m->pos);
    vec3s_set(m->marioObj->header.gfx.angle, 0, m->faceAngle[1], 0);
    general_star_dance_handler(m);
    if (m->actionState != 2 && m->actionTimer >= 62) {
        m->marioBodyState->handState = MARIO_HAND_PEACE_SIGN;
    }
    return FALSE;
}

s32 act_fall_after_star_grab(struct MarioState *m) {
    if (m->pos[1] < m->waterLevel - 130) {
        play_sound(SOUND_ACTION_UNKNOWN430, m->marioObj->header.gfx.cameraToObject);
        m->particleFlags |= PARTICLE_WATER_SPLASH;
        return set_mario_action(m, ACT_STAR_DANCE_WATER, m->actionArg);
    }
    if (perform_air_step(m, 1) == AIR_STEP_LANDED) {
        play_mario_landing_sound(m, SOUND_ACTION_TERRAIN_LANDING);
        set_mario_action(m, m->actionArg & 1 ? ACT_STAR_DANCE_NO_EXIT : ACT_STAR_DANCE_EXIT,
                         m->actionArg);
    }
    set_mario_animation(m, MARIO_ANIM_GENERAL_FALL);
    return FALSE;
}

s32 common_death_handler(struct MarioState *m, s32 animation, s32 frameToDeathWarp) {
    s32 animFrame = set_mario_animation(m, animation);
    if (animFrame == frameToDeathWarp) {
        level_trigger_warp(m, WARP_OP_DEATH);
    }
    m->marioBodyState->eyeState = MARIO_EYES_DEAD;
    stop_and_set_height_to_floor(m);
    return animFrame;
}

s32 act_standing_death(struct MarioState *m) {
    if (m->input & INPUT_IN_POISON_GAS) {
        return set_mario_action(m, ACT_SUFFOCATION, 0);
    }

    common_death_handler(m, MARIO_ANIM_DYING_FALL_OVER, 80);
    if (m->marioObj->header.gfx.animInfo.animFrame == 77) {
        play_mario_landing_sound(m, SOUND_ACTION_TERRAIN_BODY_HIT_GROUND);
    }
    return FALSE;
}

s32 act_electrocution(struct MarioState *m) {
    common_death_handler(m, MARIO_ANIM_ELECTROCUTION, 43);
    return FALSE;
}

s32 act_suffocation(struct MarioState *m) {
    common_death_handler(m, MARIO_ANIM_SUFFOCATING, 86);
    return FALSE;
}

s32 act_death_on_back(struct MarioState *m) {
    if (common_death_handler(m, MARIO_ANIM_DYING_ON_BACK, 54) == 40) {
        play_mario_heavy_landing_sound(m, SOUND_ACTION_TERRAIN_BODY_HIT_GROUND);
    }
    return FALSE;
}

s32 act_death_on_stomach(struct MarioState *m) {
    if (common_death_handler(m, MARIO_ANIM_DYING_ON_STOMACH, 37) == 37) {
        play_mario_heavy_landing_sound(m, SOUND_ACTION_TERRAIN_BODY_HIT_GROUND);
    }
    return FALSE;
}

s32 act_eaten_by_bubba(struct MarioState *m) {
    set_mario_animation(m, MARIO_ANIM_A_POSE);
    m->marioObj->header.gfx.node.flags &= ~GRAPH_RENDER_ACTIVE;
    m->health = 0xFF;
    if (m->actionTimer++ == 60) {
        level_trigger_warp(m, WARP_OP_DEATH);
    }
    return FALSE;
}

// set animation and forwardVel; when perform_air_step returns AIR_STEP_LANDED,
// set the new action
s32 launch_mario_until_land(struct MarioState *m, s32 endAction, s32 animation, f32 forwardVel) {
    s32 airStepLanded;
    mario_set_forward_vel(m, forwardVel);
    set_mario_animation(m, animation);
    airStepLanded = (perform_air_step(m, 0) == AIR_STEP_LANDED);
    if (airStepLanded) {
        if (m->action == ACT_EXIT_AIRBORNE || m->action == ACT_SPECIAL_EXIT_AIRBORNE) {
            m->faceAngle[1] += 0x7FFF;
            m->forwardVel = 8.0f;
        }

        set_mario_action(m, endAction, 0);
    }
    return airStepLanded;
}

s32 act_unlocking_star_door(struct MarioState *m) {
    switch (m->actionState) {
        case 0:
            m->faceAngle[1] = m->usedObj->oMoveAngleYaw;
            if (m->actionArg & 2) {
                m->faceAngle[1] += 0x8000;
            }
            m->marioObj->oMarioReadingSignDPosX = m->pos[0];
            m->marioObj->oMarioReadingSignDPosZ = m->pos[2];
            set_mario_animation(m, MARIO_ANIM_SUMMON_STAR);
            m->actionState++;
            break;
        case 1:
            /* this will need removed way more cleanly later but I'm quite tired right now */
            break;
        case 2:
            if (m->actionTimer++ == 70) {
                set_mario_animation(m, MARIO_ANIM_RETURN_STAR_APPROACH_DOOR);
                m->actionState++;
            }
            break;
        case 3:
            if (is_anim_at_end(m)) {
                save_file_set_flags(get_door_save_file_flag(m->usedObj));
                set_mario_action(m, ACT_READING_AUTOMATIC_DIALOG, DIALOG_038);
            }
            break;
    }

    m->pos[0] = m->marioObj->oMarioReadingSignDPosX;
    m->pos[2] = m->marioObj->oMarioReadingSignDPosZ;

    update_mario_pos_for_anim(m);
    stop_and_set_height_to_floor(m);

    return FALSE;
}

s32 act_entering_star_door(struct MarioState *m) {
    f32 targetDX;
    f32 targetDZ;
    s16 targetAngle;

    if (m->actionTimer++ == 0) {
        m->interactObj->oInteractStatus = INT_STATUS_UNK16;

        // ~30 degrees / 1/12 rot
        targetAngle = m->usedObj->oMoveAngleYaw + 0x1555;
        if (m->actionArg & 2) {
            targetAngle += 0x5556; // ~120 degrees / 1/3 rot (total 150d / 5/12)
        }

        // targetDX and targetDZ are the offsets to add to Mario's position to
        // have Mario stand 150 units in front of the door

        targetDX = m->usedObj->oPosX + 150.0f * sins(targetAngle) - m->pos[0];
        targetDZ = m->usedObj->oPosZ + 150.0f * coss(targetAngle) - m->pos[2];

        m->marioObj->oMarioReadingSignDPosX = targetDX / 20.0f;
        m->marioObj->oMarioReadingSignDPosZ = targetDZ / 20.0f;

        m->faceAngle[1] = atan2s(targetDZ, targetDX);
    }

    // set Mario's animation
    if (m->actionTimer < 15) {
        set_mario_animation(m, MARIO_ANIM_FIRST_PERSON);
    }

    // go through door? for 20 frames
    else if (m->actionTimer < 35) {
        m->pos[0] += m->marioObj->oMarioReadingSignDPosX;
        m->pos[2] += m->marioObj->oMarioReadingSignDPosZ;

        set_mario_anim_with_accel(m, MARIO_ANIM_WALKING, 0x00028000);
    }

    else {
        m->faceAngle[1] = m->usedObj->oMoveAngleYaw;

        if (m->actionArg & 2) {
            m->faceAngle[1] += 0x8000;
        }

        m->pos[0] += 12.0f * sins(m->faceAngle[1]);
        m->pos[2] += 12.0f * coss(m->faceAngle[1]);

        set_mario_anim_with_accel(m, MARIO_ANIM_WALKING, 0x00028000);
    }

    stop_and_set_height_to_floor(m);

    if (m->actionTimer == 48) {
        set_mario_action(m, ACT_IDLE, 0);
    }

    return FALSE;
}

s32 act_going_through_door(struct MarioState *m) {
    if (m->actionTimer == 0) {
        if (m->actionArg & 1) {
            m->interactObj->oInteractStatus = INT_STATUS_UNK16;
            set_mario_animation(m, MARIO_ANIM_PULL_DOOR_WALK_IN);
        } else {
            m->interactObj->oInteractStatus = INT_STATUS_UNK17;
            set_mario_animation(m, MARIO_ANIM_PUSH_DOOR_WALK_IN);
        }
    }
    m->faceAngle[1] = m->usedObj->oMoveAngleYaw;
    m->pos[0] = m->usedObj->oPosX;
    m->pos[2] = m->usedObj->oPosZ;

    update_mario_pos_for_anim(m);
    stop_and_set_height_to_floor(m);

    if (m->actionArg & 4) {
        if (m->actionTimer == 20) {
            level_trigger_warp(m, WARP_OP_WARP_DOOR);
        }
    } else if (is_anim_at_end(m)) {
        if (m->actionArg & 2) {
            m->faceAngle[1] += 0x8000;
        }
        set_mario_action(m, ACT_IDLE, 0);
    }

    m->actionTimer++;
    return FALSE;
}

s32 act_warp_door_spawn(struct MarioState *m) {
    if (m->actionState == 0) {
        m->actionState = 1;
        if (m->actionArg & 1) {
            m->usedObj->oInteractStatus = 0x00040000;
        } else {
            m->usedObj->oInteractStatus = 0x00080000;
        }
    } else if (m->usedObj->oAction == 0) {
        if (!gMessageHasBeenRead && gCurrLevelNum == LEVEL_CASTLE) {
            gMessageHasBeenRead = TRUE;
            load_level_init_text(0);
        } else {
            set_mario_action(m, ACT_IDLE, 0);
        }
    }
    set_mario_animation(m, MARIO_ANIM_IDLE_HEAD_LEFT);
    stop_and_set_height_to_floor(m);
    return FALSE;
}

s32 act_emerge_from_pipe(struct MarioState *m) {
    struct Object *marioObj = m->marioObj;

    if (m->actionTimer++ < 11) {
        marioObj->header.gfx.node.flags &= ~GRAPH_RENDER_ACTIVE;
        return FALSE;
    }

    marioObj->header.gfx.node.flags |= GRAPH_RENDER_ACTIVE;

    if (gCurrLevelNum == LEVEL_THI) {
        if (gCurrAreaIndex == 2) {
            play_sound_if_no_flag(m, SOUND_MENU_EXIT_PIPE, MARIO_ACTION_SOUND_PLAYED);
        } else {
            play_sound_if_no_flag(m, SOUND_MENU_ENTER_PIPE, MARIO_ACTION_SOUND_PLAYED);
        }
    }

    if (launch_mario_until_land(m, ACT_JUMP_LAND_STOP, MARIO_ANIM_SINGLE_JUMP, 8.0f)) {
        mario_set_forward_vel(m, 0.0f);
        play_mario_landing_sound(m, SOUND_ACTION_TERRAIN_LANDING);
    }
    return FALSE;
}

s32 act_spawn_spin_airborne(struct MarioState *m) {
    // entered water, exit action
    if (m->pos[1] < m->waterLevel - 100) {
        load_level_init_text(0);
        return set_water_plunge_action(m);
    }

    // updates all velocity variables based on m->forwardVel
    mario_set_forward_vel(m, m->forwardVel);

    // if mario lands, launch him up in the air, after he lands again, set his action to his landing
    // action
    switch (perform_air_step(m, 0)) {
        case AIR_STEP_LANDED:
            if (m->actionState++ == 0) {
                m->vel[1] = 36.0f;
            } else {
                set_mario_action(m, ACT_SPAWN_SPIN_LANDING, 0);
            }
            break;

        case AIR_STEP_HIT_WALL:
            mario_bonk_reflection(m, TRUE);
            break;
    }

    // chooses which animation to play depending on his current state
    if (m->actionState == 0 || m->vel[1] > 0.0f) {
        set_mario_animation(m, MARIO_ANIM_FORWARD_SPINNING);
    } else {
        set_mario_animation(m, MARIO_ANIM_GENERAL_FALL);
    }

    spawn_object(m->marioObj, MODEL_SPARKLES, bhvSparkleSpawn);
    m->particleFlags |= PARTICLE_SPARKLES;
    return FALSE;
}

s32 act_spawn_spin_landing(struct MarioState *m) {
    stop_and_set_height_to_floor(m);
    set_mario_animation(m, MARIO_ANIM_GENERAL_LAND);
    if (is_anim_at_end(m)) {
        load_level_init_text(0);
        set_mario_action(m, ACT_IDLE, 0);
    }
    return FALSE;
}

/**
 * act_exit_airborne: Jump out of a level after collecting a Power Star (no
 ** sparkles)
 * Mario always faces a level entrance when he launches out of it, whether he
 * died or he collected a star/key. Because of that, we need him to move away
 * from the painting by setting his speed to -32.0f and have him face away from
 * the painting by adding 0x8000 (180 deg) to his graphics angle. We also set
 * his heal counter to 31 to restore 7.75 units of his health, and enable the
 * particle flag that generates sparkles.
 */
s32 act_exit_airborne(struct MarioState *m) {
    if (15 < m->actionTimer++
        && launch_mario_until_land(m, ACT_EXIT_LAND_SAVE_DIALOG, MARIO_ANIM_GENERAL_FALL, -32.0f)) {
        // heal mario
        m->healCounter = 31;
    }

    if (m->actionTimer == 19) {
        play_sound(SOUND_MARIO_HAUGH, m->marioObj->header.gfx.cameraToObject);
    }

    // rotate him to face away from the entrance
    m->marioObj->header.gfx.angle[1] += 0x8000;
    return FALSE;
}

s32 act_falling_exit_airborne(struct MarioState *m) {
    if (launch_mario_until_land(m, ACT_EXIT_LAND_SAVE_DIALOG, MARIO_ANIM_GENERAL_FALL, 0.0f)) {
        // heal Mario
        m->healCounter = 31;
    }
    // rotate Mario to face away from the entrance
    m->marioObj->header.gfx.angle[1] += 0x8000;
    return FALSE;
}

s32 act_exit_land_save_dialog(struct MarioState *m) {
    perform_ground_step(m);
    mario_set_forward_vel(m, m->forwardVel * 2);
    play_mario_landing_sound_once(m, SOUND_ACTION_TERRAIN_LANDING);
    set_mario_animation(m,
                        m->actionArg == 0 ? MARIO_ANIM_GENERAL_LAND : MARIO_ANIM_LAND_FROM_SINGLE_JUMP);

    if (is_anim_past_frame(m, 11) || is_anim_past_frame(m, 24)) {
        stationary_ground_step(m);
    }

    if (is_anim_past_end(m)) {
        set_mario_action(m, ACT_IDLE, 0);
    }

    return FALSE;
}

s32 act_death_exit(struct MarioState *m) {
    if (m->actionTimer == 15) {
        play_sound(SOUND_MARIO_HAUGH, m->marioObj->header.gfx.cameraToObject);
    }
    if (15 < m->actionTimer++
        && launch_mario_until_land(m, ACT_DEATH_EXIT_LAND, MARIO_ANIM_BACKWARD_AIR_KB, -32.0f)) {
        m->numLives--;
        // restore 7.75 units of health
        m->healCounter = 31;
    }
    // one unit of health
    m->health = 0x0100;
    return FALSE;
}

s32 act_unused_death_exit(struct MarioState *m) {
    if (launch_mario_until_land(m, ACT_FREEFALL_LAND_STOP, MARIO_ANIM_GENERAL_FALL, 0.0f)) {
        play_sound(SOUND_MARIO_ATTACKED, m->marioObj->header.gfx.cameraToObject);
        m->numLives--;
        // restore 7.75 units of health
        m->healCounter = 31;
    }
    // one unit of health
    m->health = 0x0100;
    return FALSE;
}

s32 act_falling_death_exit(struct MarioState *m) {
    if (launch_mario_until_land(m, ACT_DEATH_EXIT_LAND, MARIO_ANIM_GENERAL_FALL, 0.0f)) {
        play_sound(SOUND_MARIO_ATTACKED, m->marioObj->header.gfx.cameraToObject);
        m->numLives--;
        // restore 7.75 units of health
        m->healCounter = 31;
    }
    // one unit of health
    m->health = 0x0100;
    return FALSE;
}

// waits 11 frames before actually executing, also has reduced fvel
s32 act_special_exit_airborne(struct MarioState *m) {
    struct Object *marioObj = m->marioObj;

    play_sound_if_no_flag(m, SOUND_MARIO_YAHOO, MARIO_MARIO_SOUND_PLAYED);

    if (m->actionTimer++ < 11) {
        marioObj->header.gfx.node.flags &= ~GRAPH_RENDER_ACTIVE;
        return FALSE;
    }

    if (launch_mario_until_land(m, ACT_EXIT_LAND_SAVE_DIALOG, MARIO_ANIM_SINGLE_JUMP, -24.0f)) {
        // heal Mario
        m->healCounter = 31;
        m->actionArg = 1;
    }

    // rotate Mario to face away from the entrance
    marioObj->header.gfx.angle[1] += 0x8000;
    // show Mario
    marioObj->header.gfx.node.flags |= GRAPH_RENDER_ACTIVE;

    return FALSE;
}

s32 act_special_death_exit(struct MarioState *m) {
    struct Object *marioObj = m->marioObj;

    if (m->actionTimer++ < 11) {
        marioObj->header.gfx.node.flags &= ~GRAPH_RENDER_ACTIVE;
        return FALSE;
    }

    if (launch_mario_until_land(m, ACT_HARD_BACKWARD_GROUND_KB, MARIO_ANIM_BACKWARD_AIR_KB, -24.0f)) {
        m->numLives--;
        m->healCounter = 31;
    }
    // show Mario
    marioObj->header.gfx.node.flags |= GRAPH_RENDER_ACTIVE;
    // one unit of health
    m->health = 0x0100;

    return FALSE;
}

s32 act_spawn_no_spin_airborne(struct MarioState *m) {
    launch_mario_until_land(m, ACT_SPAWN_NO_SPIN_LANDING, MARIO_ANIM_GENERAL_FALL, 0.0f);
    if (m->pos[1] < m->waterLevel - 100) {
        set_water_plunge_action(m);
    }
    return FALSE;
}

s32 act_spawn_no_spin_landing(struct MarioState *m) {
    play_mario_landing_sound_once(m, SOUND_ACTION_TERRAIN_LANDING);
    set_mario_animation(m, MARIO_ANIM_GENERAL_LAND);
    stop_and_set_height_to_floor(m);
    if (is_anim_at_end(m)) {
        load_level_init_text(0);
        set_mario_action(m, ACT_IDLE, 0);
    }
    return FALSE;
}

s32 act_bbh_enter_spin(struct MarioState *m) {
    f32 floorDist;
    f32 scale;
    f32 cageDX;
    f32 cageDZ;
    f32 cageDist;
    f32 forwardVel;

    cageDX = m->usedObj->oPosX - m->pos[0];
    cageDZ = m->usedObj->oPosZ - m->pos[2];
    cageDist = sqrtf(cageDX * cageDX + cageDZ * cageDZ);

    if (cageDist > 20.0f) {
        forwardVel = 10.0f;
    } else {
        forwardVel = cageDist / 2.0f;
    }
    if (forwardVel < 0.5f) {
        forwardVel = 0.0f;
    }

    switch (m->actionState) {
        case 0:
            floorDist = 512.0f - (m->pos[1] - m->floorHeight);
            m->vel[1] = floorDist > 0 ? sqrtf(4.0f * floorDist + 1.0f) - 1.0f : 2.0f;

            m->actionState = 1;
            m->actionTimer = 100;
            // fallthrough

        case 1:
            m->faceAngle[1] = atan2s(cageDZ, cageDX);
            mario_set_forward_vel(m, forwardVel);

            if (set_mario_animation(m, MARIO_ANIM_FORWARD_SPINNING) == 0) {
                play_sound(SOUND_ACTION_SPIN, m->marioObj->header.gfx.cameraToObject);
            }

            m->flags &= ~MARIO_UNKNOWN_08;
            perform_air_step(m, 0);
            if (m->vel[1] <= 0) {
                m->actionState = 2;
            }
            break;

        case 2:
            // fallthrough
        case 3:
            m->faceAngle[1] = atan2s(cageDZ, cageDX);
            mario_set_forward_vel(m, forwardVel);
            m->flags &= ~MARIO_UNKNOWN_08;
            if (perform_air_step(m, 0) == AIR_STEP_LANDED) {
                level_trigger_warp(m, WARP_OP_UNKNOWN_02);
                m->actionState = 4;
            }
            if (m->actionState == 2) {
                if (m->marioObj->header.gfx.animInfo.animFrame == 0) {
                    m->actionState = 3;
                }
            } else {
                play_sound_if_no_flag(m, SOUND_ACTION_SHRINK_INTO_BBH, MARIO_ACTION_SOUND_PLAYED);
                set_mario_animation(m, MARIO_ANIM_DIVE);
                m->marioObj->header.gfx.angle[0] = atan2s(m->forwardVel, -m->vel[1]);
            }
            m->squishTimer = 0xFF;
            if (m->actionTimer >= 11) {
                m->actionTimer -= 6;
                scale = m->actionTimer / 100.0f;
                vec3f_set(m->marioObj->header.gfx.scale, scale, scale, scale);
            }
            break;

        case 4:
            stop_and_set_height_to_floor(m);
            m->marioObj->header.gfx.node.flags |= GRAPH_RENDER_INVISIBLE;
            break;
    }

    return FALSE;
}

s32 act_bbh_enter_jump(struct MarioState *m) {
    f32 cageDX;
    f32 cageDZ;
    f32 cageDist;

    play_mario_action_sound(m, SOUND_ACTION_TERRAIN_JUMP, 1);
    play_mario_jump_sound(m);

    if (m->actionState == 0) {
        cageDX = m->usedObj->oPosX - m->pos[0];
        cageDZ = m->usedObj->oPosZ - m->pos[2];
        cageDist = sqrtf(cageDX * cageDX + cageDZ * cageDZ);

        m->vel[1] = 60.0f;
        m->faceAngle[1] = atan2s(cageDZ, cageDX);
        mario_set_forward_vel(m, cageDist / 20.0f);

        m->flags &= ~MARIO_UNKNOWN_08;
        m->actionState = 1;
    }

    set_mario_animation(m, MARIO_ANIM_DOUBLE_JUMP_RISE);
    perform_air_step(m, 0);

    if (m->vel[1] <= 0.0f) {
        set_mario_action(m, ACT_BBH_ENTER_SPIN, 0);
    }

    return FALSE;
}

s32 act_teleport_fade_out(struct MarioState *m) {
    play_sound_if_no_flag(m, SOUND_ACTION_TELEPORT, MARIO_ACTION_SOUND_PLAYED);
    set_mario_animation(m, m->prevAction == ACT_CROUCHING ? MARIO_ANIM_CROUCHING
                                                          : MARIO_ANIM_FIRST_PERSON);

    m->flags |= MARIO_TELEPORTING;

    if (m->actionTimer < 32) {
        m->fadeWarpOpacity = (-m->actionTimer << 3) + 0xF8;
    }

    if (m->actionTimer++ == 20) {
        level_trigger_warp(m, WARP_OP_TELEPORT);
    }

    stop_and_set_height_to_floor(m);

    return FALSE;
}

s32 act_teleport_fade_in(struct MarioState *m) {
    play_sound_if_no_flag(m, SOUND_ACTION_TELEPORT, MARIO_ACTION_SOUND_PLAYED);
    set_mario_animation(m, MARIO_ANIM_FIRST_PERSON);

    if (m->actionTimer < 32) {
        m->flags |= MARIO_TELEPORTING;
        m->fadeWarpOpacity = m->actionTimer << 3;
    } else {
        m->flags &= ~MARIO_TELEPORTING;
    }

    if (m->actionTimer++ == 32) {
        if (m->pos[1] < m->waterLevel - 100) {
            // Check if the camera is not underwater.
            if (m->area->camera->mode != CAMERA_MODE_WATER_SURFACE) {
                set_camera_mode(m->area->camera, CAMERA_MODE_WATER_SURFACE, 1);
            }
            set_mario_action(m, ACT_WATER_IDLE, 0);
        } else {
            set_mario_action(m, ACT_IDLE, 0);
        }
    }

    stop_and_set_height_to_floor(m);

    return FALSE;
}

s32 act_squished(struct MarioState *m) {
    UNUSED u8 filler[4];
    f32 squishAmount;
    f32 spaceUnderCeil;
    s16 surfAngle;
    s32 underSteepSurf = FALSE; // seems to be responsible for setting velocity?

    if ((spaceUnderCeil = m->ceilHeight - m->floorHeight) < 0) {
        spaceUnderCeil = 0;
    }

    switch (m->actionState) {
        case 0:
            if (spaceUnderCeil > 160.0f) {
                m->squishTimer = 0;
                return set_mario_action(m, ACT_IDLE, 0);
            }

            m->squishTimer = 0xFF;

            if (spaceUnderCeil >= 10.1f) {
                // Mario becomes a pancake
                squishAmount = spaceUnderCeil / 160.0f;
                vec3f_set(m->marioObj->header.gfx.scale, 2.0f - squishAmount, squishAmount,
                          2.0f - squishAmount);
            } else {
                if (m->invincTimer == 0) {
                    m->hurtCounter += 12;
                    play_sound_if_no_flag(m, SOUND_MARIO_ATTACKED, MARIO_MARIO_SOUND_PLAYED);
                }

                // Both of the 1.8's are really floats, but one of them has to
                // be written as a double for this to match on -O2.
                vec3f_set(m->marioObj->header.gfx.scale, 1.8, 0.05f, 1.8f);
                m->actionState = 1;
            }
            break;
        case 1:
            if (spaceUnderCeil >= 30.0f) {
                m->actionState = 2;
            }
            break;
        case 2:
            m->actionTimer++;
            if (m->actionTimer >= 15) {
                // 1 unit of health
                if (m->health < 0x0100) {
                    level_trigger_warp(m, WARP_OP_DEATH);
                    // woosh, he's gone!
                    set_mario_action(m, ACT_DISAPPEARED, 0);
                } else if (m->hurtCounter == 0) {
                    // un-squish animation
                    m->squishTimer = 30;
                    set_mario_action(m, ACT_IDLE, 0);
                }
            }
            break;
    }

    // steep floor
    if (m->floor != NULL && m->floor->normal.y < 0.5f) {
        surfAngle = atan2s(m->floor->normal.z, m->floor->normal.x);
        underSteepSurf = TRUE;
    }
    // steep ceiling
    if (m->ceil != NULL && -0.5f < m->ceil->normal.y) {
        surfAngle = atan2s(m->ceil->normal.z, m->ceil->normal.x);
        underSteepSurf = TRUE;
    }

    if (underSteepSurf) {
        m->vel[0] = sins(surfAngle) * 10.0f;
        m->vel[2] = coss(surfAngle) * 10.0f;
        m->vel[1] = 0;

        // check if there's no floor 10 units away from the surface
        if (perform_ground_step(m) == GROUND_STEP_LEFT_GROUND) {
            // instant un-squish
            m->squishTimer = 0;
            set_mario_action(m, ACT_IDLE, 0);
            return FALSE;
        }
    }

    // squished for more than 10 seconds, so kill Mario
    if (m->actionArg++ > 300) {
        // 0 units of health
        m->health = 0x00FF;
        m->hurtCounter = 0;
        level_trigger_warp(m, WARP_OP_DEATH);
        // woosh, he's gone!
        set_mario_action(m, ACT_DISAPPEARED, 0);
    }
    stop_and_set_height_to_floor(m);
    set_mario_animation(m, MARIO_ANIM_A_POSE);
    return FALSE;
}

/**
 * advance_cutscene_step: Advances the current step in the current cutscene.
 * Resets action state and action timer, adds 1 to the action arg (responsible
 * for keeping track of what step of the cutscene Mario is in.)
 */
static void advance_cutscene_step(struct MarioState *m) {
    m->actionState = 0;
    m->actionTimer = 0;
    m->actionArg++;
}

static void intro_cutscene_wait_for_dialog(struct MarioState *m) {
    set_mario_animation(m, MARIO_ANIM_FIRST_PERSON);

    if (m->actionTimer++ == 0) {
        m->statusForCamera->cameraEvent = CAM_EVENT_START_INTRO;
    }

    if (m->actionTimer == 65) {
        create_dialog_box(33);
    } else if (m->actionTimer >= 65 && gMenuState < 3 && gDialogBoxAngle < 65.0f) {
        set_anim_to_frame(m, m->marioObj->header.gfx.animInfo.animFrame);
    } else if (m->actionTimer >= 65 && gMenuState == 3) {
        if (gDialogBoxAngle < 30.0f) {
            set_anim_to_frame(m, m->marioObj->header.gfx.animInfo.animFrame);
        } else {
            advance_cutscene_step(m);
        }
    }

    stop_and_set_height_to_floor(m);
}

static void intro_cutscene_set_mario_to_idle(struct MarioState *m) {
    if (gCamera->cutscene == 0) {
        gObjCutsceneDone = TRUE;
        set_camera_mode(m->area->camera, CAMERA_MODE_C_UP, 2);
        set_mario_action(m, ACT_FIRST_PERSON, 1);
    }

    stop_and_set_height_to_floor(m);
}

enum { INTRO_CUTSCENE_WAIT_FOR_DIALOG, INTRO_CUTSCENE_SET_MARIO_TO_IDLE };

static s32 act_intro_cutscene(struct MarioState *m) {
    switch (m->actionArg) {
        case INTRO_CUTSCENE_WAIT_FOR_DIALOG:
            intro_cutscene_wait_for_dialog(m);
            break;
        case INTRO_CUTSCENE_SET_MARIO_TO_IDLE:
            intro_cutscene_set_mario_to_idle(m);
            break;
    }
    return FALSE;
}

s32 mario_execute_cutscene_action(struct MarioState *m) {
    s32 cancel;

    /* clang-format off */
    switch (m->action) {
        case ACT_DISAPPEARED:                cancel = act_disappeared(m);                break;
        case ACT_INTRO_CUTSCENE:             cancel = act_intro_cutscene(m);             break;
        case ACT_STAR_DANCE_EXIT:            cancel = act_star_dance(m);                 break;
        case ACT_STAR_DANCE_NO_EXIT:         cancel = act_star_dance(m);                 break;
        case ACT_STAR_DANCE_WATER:           cancel = act_star_dance_water(m);           break;
        case ACT_FALL_AFTER_STAR_GRAB:       cancel = act_fall_after_star_grab(m);       break;
        case ACT_READING_AUTOMATIC_DIALOG:   cancel = act_reading_automatic_dialog(m);   break;
        case ACT_READING_NPC_DIALOG:         cancel = act_reading_npc_dialog(m);         break;
        case ACT_DEBUG_FREE_MOVE:            cancel = act_debug_free_move(m);            break;
        case ACT_READING_SIGN:               cancel = act_reading_sign(m);               break;
        case ACT_WAITING_FOR_DIALOG:         cancel = act_waiting_for_dialog(m);         break;
        case ACT_STANDING_DEATH:             cancel = act_standing_death(m);             break;
        case ACT_ELECTROCUTION:              cancel = act_electrocution(m);              break;
        case ACT_SUFFOCATION:                cancel = act_suffocation(m);                break;
        case ACT_DEATH_ON_STOMACH:           cancel = act_death_on_stomach(m);           break;
        case ACT_DEATH_ON_BACK:              cancel = act_death_on_back(m);              break;
        case ACT_EATEN_BY_BUBBA:             cancel = act_eaten_by_bubba(m);             break;
        case ACT_PULLING_DOOR:
        case ACT_PUSHING_DOOR:               cancel = act_going_through_door(m);         break;
        case ACT_WARP_DOOR_SPAWN:            cancel = act_warp_door_spawn(m);            break;
        case ACT_EMERGE_FROM_PIPE:           cancel = act_emerge_from_pipe(m);           break;
        case ACT_SPAWN_SPIN_AIRBORNE:        cancel = act_spawn_spin_airborne(m);        break;
        case ACT_SPAWN_SPIN_LANDING:         cancel = act_spawn_spin_landing(m);         break;
        case ACT_EXIT_AIRBORNE:              cancel = act_exit_airborne(m);              break;
        case ACT_EXIT_LAND_SAVE_DIALOG:      cancel = act_exit_land_save_dialog(m);      break;
        case ACT_DEATH_EXIT:                 cancel = act_death_exit(m);                 break;
        case ACT_UNUSED_DEATH_EXIT:          cancel = act_unused_death_exit(m);          break;
        case ACT_FALLING_DEATH_EXIT:         cancel = act_falling_death_exit(m);         break;
        case ACT_SPECIAL_EXIT_AIRBORNE:      cancel = act_special_exit_airborne(m);      break;
        case ACT_SPECIAL_DEATH_EXIT:         cancel = act_special_death_exit(m);         break;
        case ACT_FALLING_EXIT_AIRBORNE:      cancel = act_falling_exit_airborne(m);      break;
        case ACT_UNLOCKING_STAR_DOOR:        cancel = act_unlocking_star_door(m);        break;
        case ACT_ENTERING_STAR_DOOR:         cancel = act_entering_star_door(m);         break;
        case ACT_SPAWN_NO_SPIN_AIRBORNE:     cancel = act_spawn_no_spin_airborne(m);     break;
        case ACT_SPAWN_NO_SPIN_LANDING:      cancel = act_spawn_no_spin_landing(m);      break;
        case ACT_BBH_ENTER_JUMP:             cancel = act_bbh_enter_jump(m);             break;
        case ACT_BBH_ENTER_SPIN:             cancel = act_bbh_enter_spin(m);             break;
        case ACT_TELEPORT_FADE_OUT:          cancel = act_teleport_fade_out(m);          break;
        case ACT_TELEPORT_FADE_IN:           cancel = act_teleport_fade_in(m);           break;
        case ACT_SQUISHED:                   cancel = act_squished(m);                   break;
    }
    /* clang-format on */

#ifdef POWBUILD
    if (!cancel && (m->input & INPUT_IN_WATER)) {
        m->particleFlags |= PARTICLE_IDLE_WATER_WAVE;
    }
#endif

    return cancel;
}
