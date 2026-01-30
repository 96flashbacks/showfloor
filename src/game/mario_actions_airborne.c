#include <PR/ultratypes.h>

#include "sm64.h"
#include "area.h"
#include "audio/external.h"
#include "camera.h"
#include "engine/graph_node.h"
#include "engine/math_util.h"
#include "game_init.h"
#include "interaction.h"
#include "level_update.h"
#include "mario.h"
#include "mario_step.h"
#include "save_file.h"

void play_flip_sounds(struct MarioState *m, s16 frame1, s16 frame2, s16 frame3) {
    s32 animFrame = m->marioObj->header.gfx.animInfo.animFrame;
    if (animFrame == frame1 || animFrame == frame2 || animFrame == frame3) {
        play_sound(SOUND_ACTION_SPIN, m->marioObj->header.gfx.cameraToObject);
    }
}

void play_far_fall_sound(struct MarioState *m) {
    u32 action = m->action;
    if (!(action & ACT_FLAG_INVULNERABLE) && action != ACT_TWIRLING && action != ACT_FLYING
        && !(m->flags & MARIO_UNKNOWN_18)) {
        if (m->peakHeight - m->pos[1] > 5150.0f) {
            play_sound(SOUND_MARIO_WAAAOOOW, m->marioObj->header.gfx.cameraToObject);
            m->flags |= MARIO_UNKNOWN_18;
        }
    }
}

s32 lava_boost_on_wall(struct MarioState *m) {
    m->faceAngle[1] = atan2s(m->wall->normal.z, m->wall->normal.x);

    if (m->forwardVel < 24.0f) {
        m->forwardVel = 24.0f;
    }

    m->hurtCounter += 12;

    play_sound(SOUND_MARIO_WAAAOOOW, m->marioObj->header.gfx.cameraToObject);
    update_mario_sound_and_camera(m);
    return drop_and_set_mario_action(m, ACT_LAVA_BOOST, 1);
}

s32 check_fall_damage(struct MarioState *m, u32 hardFallAction) {
    f32 fallHeight;
    f32 damageHeight;

    fallHeight = m->peakHeight - m->pos[1];

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma GCC diagnostic ignored "-Wtautological-constant-out-of-range-compare"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wtype-limits"
#endif

    damageHeight = 900.0f;

#pragma GCC diagnostic pop

    if (m->action != ACT_TWIRLING && m->floor->type != SURFACE_BURNING) {
        if (m->vel[1] < -55.0f) {
            if (fallHeight > 1800.0f) {
                m->hurtCounter += 16;
                return drop_and_set_mario_action(m, hardFallAction, 4);
            } else if (fallHeight > damageHeight && !mario_floor_is_slippery(m)) {
                m->hurtCounter += 8;
                m->squishTimer = 30;
            }
        }
    }

    return FALSE;
}

s32 check_dive_in_air(struct MarioState *m) {
    if (m->input & INPUT_B_PRESSED) {
        return set_mario_action(m, ACT_DIVE, 0);
    }
    return FALSE;
}

s32 check_horizontal_wind(struct MarioState *m) {
    struct Surface *floor;
    f32 speed;
    s16 pushAngle;

    floor = m->floor;

    if (floor->type == SURFACE_HORIZONTAL_WIND) {
        pushAngle = floor->force << 8;

        m->slideVelX += 1.2f * sins(pushAngle);
        m->slideVelZ += 1.2f * coss(pushAngle);

        speed = sqrtf(m->slideVelX * m->slideVelX + m->slideVelZ * m->slideVelZ);

        if (speed > 48.0f) {
            m->slideVelX = m->slideVelX * 48.0f / speed;
            m->slideVelZ = m->slideVelZ * 48.0f / speed;
            speed = 32.0f; //! This was meant to be 48?
        } else if (speed > 32.0f) {
            speed = 32.0f;
        }

        m->vel[0] = m->slideVelX;
        m->vel[2] = m->slideVelZ;
        m->slideYaw = atan2s(m->slideVelZ, m->slideVelX);
        m->forwardVel = speed * coss(m->faceAngle[1] - m->slideYaw);

        play_sound(SOUND_ENV_WIND2, m->marioObj->header.gfx.cameraToObject);
        return TRUE;
    }

    return FALSE;
}

void update_air_with_turn(struct MarioState *m) {
    s16 intendedDYaw;
    f32 intendedMag;

    if (!check_horizontal_wind(m)) {
        m->forwardVel = approach_f32(m->forwardVel, 0.0f, 0.35f, 0.35f);

        if (m->input & INPUT_NONZERO_ANALOG) {
            intendedDYaw = m->intendedYaw - m->faceAngle[1];
            intendedMag = m->intendedMag / 32.0f;

            m->forwardVel += 1.5f * coss(intendedDYaw) * intendedMag;
            m->faceAngle[1] += 512.0f * sins(intendedDYaw) * intendedMag;
        }

        //! Uncapped air speed. Net positive when moving forward.
        if (m->forwardVel > 32.0f) {
            m->forwardVel -= 1.0f;
        }
        if (m->forwardVel < -16.0f) {
            m->forwardVel += 2.0f;
        }

        m->vel[0] = m->slideVelX = m->forwardVel * sins(m->faceAngle[1]);
        m->vel[2] = m->slideVelZ = m->forwardVel * coss(m->faceAngle[1]);
    }
}

void update_air_without_turn(struct MarioState *m) {
    f32 sidewaysSpeed = 0.0f;
    s16 intendedDYaw;
    f32 intendedMag;

    if (!check_horizontal_wind(m)) { // same thing
        m->forwardVel = approach_f32(m->forwardVel, 0.0f, 0.35f, 0.35f);

        if (m->input & INPUT_NONZERO_ANALOG) {
            intendedDYaw = m->intendedYaw - m->faceAngle[1];
            intendedMag = m->intendedMag / 32.0f;

            m->forwardVel += intendedMag * coss(intendedDYaw) * 1.5f;
            sidewaysSpeed = intendedMag * sins(intendedDYaw) * 10.0f;
        }

        //! Uncapped air speed. Net positive when moving forward.
        if (m->forwardVel > 32.0f) {
            m->forwardVel -= 1.0f;
        }
        if (m->forwardVel < -16.0f) {
            m->forwardVel += 2.0f;
        }

        m->slideVelX = m->forwardVel * sins(m->faceAngle[1]);
        m->slideVelZ = m->forwardVel * coss(m->faceAngle[1]);

        m->slideVelX += sidewaysSpeed * sins(m->faceAngle[1] + 0x4000);
        m->slideVelZ += sidewaysSpeed * coss(m->faceAngle[1] + 0x4000);

        m->vel[0] = m->slideVelX;
        m->vel[2] = m->slideVelZ;
    }
}

void update_lava_boost_or_twirling(struct MarioState *m) {
    s16 intendedDYaw;
    f32 intendedMag;

    if (m->input & INPUT_NONZERO_ANALOG) {
        intendedDYaw = m->intendedYaw - m->faceAngle[1];
        intendedMag = m->intendedMag / 32.0f;

        m->forwardVel += coss(intendedDYaw) * intendedMag;
        m->faceAngle[1] += sins(intendedDYaw) * intendedMag * 1024.0f;

        if (m->forwardVel < 0.0f) {
            m->faceAngle[1] += 0x8000;
            m->forwardVel *= -1.0f;
        }

        if (m->forwardVel > 32.0f) {
            m->forwardVel -= 2.0f;
        }
    }

    m->vel[0] = m->slideVelX = m->forwardVel * sins(m->faceAngle[1]);
    m->vel[2] = m->slideVelZ = m->forwardVel * coss(m->faceAngle[1]);
}

void update_flying_yaw(struct MarioState *m) {
    s16 targetYawVel = -(s16) (m->controller->stickX * (m->forwardVel / 4.0f));

    if (targetYawVel > 0) {
        if (m->angleVel[1] < 0) {
            m->angleVel[1] += 0x40;
            if (m->angleVel[1] > 0x10) {
                m->angleVel[1] = 0x10;
            }
        } else {
            m->angleVel[1] = approach_s32(m->angleVel[1], targetYawVel, 0x10, 0x20);
        }
    } else if (targetYawVel < 0) {
        if (m->angleVel[1] > 0) {
            m->angleVel[1] -= 0x40;
            if (m->angleVel[1] < -0x10) {
                m->angleVel[1] = -0x10;
            }
        } else {
            m->angleVel[1] = approach_s32(m->angleVel[1], targetYawVel, 0x20, 0x10);
        }
    } else {
        m->angleVel[1] = approach_s32(m->angleVel[1], 0, 0x40, 0x40);
    }

    m->faceAngle[1] += m->angleVel[1];
    m->faceAngle[2] = 20 * -m->angleVel[1];
}

void update_flying_pitch(struct MarioState *m) {
    s16 targetPitchVel = -(s16) (m->controller->stickY * (m->forwardVel / 5.0f));

    if (targetPitchVel > 0) {
        if (m->angleVel[0] < 0) {
            m->angleVel[0] += 0x40;
            if (m->angleVel[0] > 0x20) {
                m->angleVel[0] = 0x20;
            }
        } else {
            m->angleVel[0] = approach_s32(m->angleVel[0], targetPitchVel, 0x20, 0x40);
        }
    } else if (targetPitchVel < 0) {
        if (m->angleVel[0] > 0) {
            m->angleVel[0] -= 0x40;
            if (m->angleVel[0] < -0x20) {
                m->angleVel[0] = -0x20;
            }
        } else {
            m->angleVel[0] = approach_s32(m->angleVel[0], targetPitchVel, 0x40, 0x20);
        }
    } else {
        m->angleVel[0] = approach_s32(m->angleVel[0], 0, 0x40, 0x40);
    }
}

void update_flying(struct MarioState *m) {
    UNUSED u8 filler[4];

    update_flying_pitch(m);
    update_flying_yaw(m);

    m->forwardVel -= 2.0f * ((f32) m->faceAngle[0] / 0x4000) + 0.1f;
    m->forwardVel -= 0.5f * (1.0f - coss(m->angleVel[1]));

    if (m->forwardVel < 0.0f) {
        m->forwardVel = 0.0f;
    }

    if (m->forwardVel > 16.0f) {
        m->faceAngle[0] += (m->forwardVel - 32.0f) * 6.0f;
    } else if (m->forwardVel > 4.0f) {
        m->faceAngle[0] += (m->forwardVel - 32.0f) * 10.0f;
    } else {
        m->faceAngle[0] -= 0x400;
    }

    m->faceAngle[0] += m->angleVel[0];

    if (m->faceAngle[0] > 0x2AAA) {
        m->faceAngle[0] = 0x2AAA;
    }
    if (m->faceAngle[0] < -0x2AAA) {
        m->faceAngle[0] = -0x2AAA;
    }

    m->vel[0] = m->forwardVel * coss(m->faceAngle[0]) * sins(m->faceAngle[1]);
    m->vel[1] = m->forwardVel * sins(m->faceAngle[0]);
    m->vel[2] = m->forwardVel * coss(m->faceAngle[0]) * coss(m->faceAngle[1]);

    m->slideVelX = m->vel[0];
    m->slideVelZ = m->vel[2];
}

u32 common_air_action_step(struct MarioState *m, u32 landAction, s32 animation, u32 stepArg) {
    u32 stepResult;

    update_air_without_turn(m);

    m->actionTimer++;

    stepResult = perform_air_step(m, stepArg);
    switch (stepResult) {
        case AIR_STEP_NONE:
            set_mario_animation(m, animation);
            break;

        case AIR_STEP_LANDED:
            if (!check_fall_damage(m, ACT_HARD_BACKWARD_GROUND_KB)) {
                set_mario_action(m, landAction, 0);
            }
            break;

        case AIR_STEP_HIT_WALL:
            set_mario_animation(m, animation);

            if (m->forwardVel > 16.0f) {
                mario_bonk_reflection(m, FALSE);
                m->faceAngle[1] += 0x8000;

                if (m->wall != NULL) {
                    set_mario_action(m, ACT_AIR_HIT_WALL, 0);
                } else {
                    if (m->vel[1] > 0.0f) {
                        m->vel[1] = 0.0f;
                    }

                    //! Hands-free holding. Bonking while no wall is referenced
                    // sets Mario's action to a non-holding action without
                    // dropping the object, causing the hands-free holding
                    // glitch. This can be achieved using an exposed ceiling,
                    // out of bounds, grazing the bottom of a wall while
                    // falling such that the final quarter step does not find a
                    // wall collision, or by rising into the top of a wall such
                    // that the final quarter step detects a ledge, but you are
                    // not able to ledge grab it.
                    if (m->forwardVel >= 38.0f) {
                        set_mario_action(m, ACT_BACKWARD_AIR_KB, 0);
                    } else {
                        if (m->forwardVel > 8.0f) {
                            mario_set_forward_vel(m, -8.0f);
                        }
                        return set_mario_action(m, ACT_AIR_HIT_WALL, 0);
                    }
                }
            } else {
                mario_set_forward_vel(m, 0.0f);
            }
            break;

        case AIR_STEP_GRABBED_LEDGE:
            set_mario_animation(m, MARIO_ANIM_IDLE_ON_LEDGE);
            drop_and_set_mario_action(m, ACT_LEDGE_GRAB, 0);
            break;

        case AIR_STEP_GRABBED_CEILING:
            set_mario_action(m, ACT_START_HANGING, 0);
            break;

        case AIR_STEP_HIT_LAVA_WALL:
            lava_boost_on_wall(m);
            break;
    }

    return stepResult;
}

s32 act_jump(struct MarioState *m) {
    if (check_dive_in_air(m)) {
        return TRUE;
    }

    if (m->input & INPUT_Z_PRESSED) {
        return set_mario_action(m, ACT_GROUND_POUND, 0);
    }

    play_mario_sound(m, SOUND_ACTION_TERRAIN_JUMP, 0);
    common_air_action_step(m, ACT_JUMP_LAND, MARIO_ANIM_SINGLE_JUMP,
                           AIR_STEP_CHECK_LEDGE_GRAB | AIR_STEP_CHECK_HANG);
    return FALSE;
}

s32 act_double_jump(struct MarioState *m) {
    s32 animation = (m->vel[1] >= 0.0f) ? MARIO_ANIM_DOUBLE_JUMP_RISE : MARIO_ANIM_DOUBLE_JUMP_FALL;

    if (check_dive_in_air(m)) {
        return TRUE;
    }

    if (m->input & INPUT_Z_PRESSED) {
        return set_mario_action(m, ACT_GROUND_POUND, 0);
    }

    play_mario_sound(m, SOUND_ACTION_TERRAIN_JUMP, SOUND_MARIO_HAUGH);
    common_air_action_step(m, ACT_DOUBLE_JUMP_LAND, animation,
                           AIR_STEP_CHECK_LEDGE_GRAB | AIR_STEP_CHECK_HANG);
    return FALSE;
}

s32 act_freefall(struct MarioState *m) {
    s32 animation;

    if (m->input & INPUT_B_PRESSED) {
        return set_mario_action(m, ACT_DIVE, 0);
    }

    if (m->input & INPUT_Z_PRESSED) {
        return set_mario_action(m, ACT_GROUND_POUND, 0);
    }

    switch (m->actionArg) {
        case 0:
            animation = MARIO_ANIM_GENERAL_FALL;
            break;
        case 1:
            animation = MARIO_ANIM_FALL_FROM_SLIDE;
            break;
    }

    common_air_action_step(m, ACT_FREEFALL_LAND, animation, AIR_STEP_CHECK_LEDGE_GRAB);
    return FALSE;
}

s32 act_hold_jump(struct MarioState *m) {
    if (m->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT) {
        return drop_and_set_mario_action(m, ACT_FREEFALL, 0);
    }

    if ((m->input & INPUT_B_PRESSED) && !(m->heldObj->oInteractionSubtype & INT_SUBTYPE_HOLDABLE_NPC)) {
        return set_mario_action(m, ACT_AIR_THROW, 0);
    }

    if (m->input & INPUT_Z_PRESSED) {
        return drop_and_set_mario_action(m, ACT_GROUND_POUND, 0);
    }

    play_mario_sound(m, SOUND_ACTION_TERRAIN_JUMP, 0);
    common_air_action_step(m, ACT_HOLD_JUMP_LAND, MARIO_ANIM_JUMP_WITH_LIGHT_OBJ,
                           AIR_STEP_CHECK_LEDGE_GRAB);
    return FALSE;
}

s32 act_hold_freefall(struct MarioState *m) {
    s32 animation;
    if (m->actionArg == 0) {
        animation = MARIO_ANIM_FALL_WITH_LIGHT_OBJ;
    } else {
        animation = MARIO_ANIM_FALL_FROM_SLIDING_WITH_LIGHT_OBJ;
    }

    if (m->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT) {
        return drop_and_set_mario_action(m, ACT_FREEFALL, 0);
    }

    if ((m->input & INPUT_B_PRESSED) && !(m->heldObj->oInteractionSubtype & INT_SUBTYPE_HOLDABLE_NPC)) {
        return set_mario_action(m, ACT_AIR_THROW, 0);
    }

    if (m->input & INPUT_Z_PRESSED) {
        return drop_and_set_mario_action(m, ACT_GROUND_POUND, 0);
    }

    common_air_action_step(m, ACT_HOLD_FREEFALL_LAND, animation, AIR_STEP_CHECK_LEDGE_GRAB);
    return FALSE;
}

s32 act_side_flip(struct MarioState *m) {
    if (m->input & INPUT_B_PRESSED) {
        return set_mario_action(m, ACT_DIVE, 0);
    }

    if (m->input & INPUT_Z_PRESSED) {
        return set_mario_action(m, ACT_GROUND_POUND, 0);
    }

    play_mario_sound(m, SOUND_ACTION_TERRAIN_JUMP, 0);

    if (common_air_action_step(m, ACT_SIDE_FLIP_LAND, MARIO_ANIM_SLIDEFLIP, AIR_STEP_CHECK_LEDGE_GRAB)
        != AIR_STEP_GRABBED_LEDGE) {
        m->marioObj->header.gfx.angle[1] += 0x8000;
    }

    // This must be one line to match on -O2
    // clang-format off
    if (m->marioObj->header.gfx.animInfo.animFrame == 6) play_sound(SOUND_ACTION_SIDE_FLIP_UNK, m->marioObj->header.gfx.cameraToObject);
    // clang-format on
    return FALSE;
}

s32 act_wall_kick_air(struct MarioState *m) {
    if (m->input & INPUT_B_PRESSED) {
        return set_mario_action(m, ACT_DIVE, 0);
    }

    if (m->input & INPUT_Z_PRESSED) {
        return set_mario_action(m, ACT_GROUND_POUND, 0);
    }

    play_mario_jump_sound(m);
    common_air_action_step(m, ACT_JUMP_LAND, MARIO_ANIM_SLIDEJUMP, AIR_STEP_CHECK_LEDGE_GRAB);
    return FALSE;
}

s32 act_twirling(struct MarioState *m) {
    s16 startTwirlYaw = m->twirlYaw;
    s16 yawVelTarget;

    if (m->input & INPUT_A_DOWN) {
        yawVelTarget = 0x2000;
    } else {
        yawVelTarget = 0x1800;
    }

    m->angleVel[1] = approach_s32(m->angleVel[1], yawVelTarget, 350, 512);
    m->twirlYaw += m->angleVel[1];

    switch (m->actionArg) {
        case 0:
            set_mario_animation(m, MARIO_ANIM_DOUBLE_JUMP_RISE);
            play_mario_sound(m, SOUND_ACTION_TERRAIN_JUMP, SOUND_MARIO_YAHOO);
            if (m->vel[1] < 0.0f) {
                m->actionArg = 1;
            }
            break;

        case 1:
            set_mario_animation(m, MARIO_ANIM_START_TWIRL);
            if (is_anim_past_end(m)) {
                m->actionArg = 2;
            }
            break;

        case 2:
            set_mario_animation(m, MARIO_ANIM_TWIRL);
            break;
    }

    if (startTwirlYaw > m->twirlYaw) {
        play_sound(SOUND_ACTION_TWIRL, m->marioObj->header.gfx.cameraToObject);
    }

    update_lava_boost_or_twirling(m);

    switch (perform_air_step(m, 0)) {
        case AIR_STEP_LANDED:
            set_mario_action(m, ACT_TWIRL_LAND, 0);
            break;

        case AIR_STEP_HIT_WALL:
            mario_bonk_reflection(m, FALSE);
            break;

        case AIR_STEP_HIT_LAVA_WALL:
            lava_boost_on_wall(m);
            break;
    }
    m->marioObj->header.gfx.angle[1] += m->twirlYaw;
    return FALSE;
}

s32 act_dive(struct MarioState *m) {
    if (m->actionArg == 0) {
        play_mario_sound(m, SOUND_ACTION_SPIN, SOUND_MARIO_HAUGH);
    } else {
        play_mario_sound(m, SOUND_ACTION_TERRAIN_JUMP, 0);
    }

    set_mario_animation(m, MARIO_ANIM_DIVE);
    if (mario_check_object_grab(m)) {
        mario_grab_used_object(m);
        m->marioBodyState->grabPos = GRAB_POS_LIGHT_OBJ;
        if (m->action != ACT_DIVE) {
            return TRUE;
        }
    }

    update_air_without_turn(m);

    switch (perform_air_step(m, 0)) {
        case AIR_STEP_NONE:
            if (m->vel[1] < 0.0f && m->faceAngle[0] > -0x2AAA) {
                m->faceAngle[0] -= 0x200;
                if (m->faceAngle[0] < -0x2AAA) {
                    m->faceAngle[0] = -0x2AAA;
                }
            }
            m->marioObj->header.gfx.angle[0] = -m->faceAngle[0];
            break;

        case AIR_STEP_LANDED:
            if (!check_fall_damage(m, ACT_HARD_FORWARD_GROUND_KB)) {
                if (m->heldObj == NULL) {
                    set_mario_action(m, ACT_DIVE_SLIDE, 0);
                } else {
                    set_mario_action(m, ACT_DIVE_PICKING_UP, 0);
                }
            }
            m->faceAngle[0] = 0;
            break;

        case AIR_STEP_HIT_WALL:
            mario_bonk_reflection(m, TRUE);
            m->faceAngle[0] = 0;

            if (m->vel[1] > 0.0f) {
                m->vel[1] = 0.0f;
            }

            drop_and_set_mario_action(m, ACT_BACKWARD_AIR_KB, 0);
            break;

        case AIR_STEP_HIT_LAVA_WALL:
            lava_boost_on_wall(m);
            break;
    }

    return FALSE;
}

s32 act_air_throw(struct MarioState *m) {
    if (++(m->actionTimer) == 4) {
        mario_throw_held_object(m);
    }

    play_sound_if_no_flag(m, SOUND_MARIO_WAH2, MARIO_MARIO_SOUND_PLAYED);
    set_mario_animation(m, MARIO_ANIM_THROW_LIGHT_OBJECT);
    update_air_without_turn(m);

    switch (perform_air_step(m, 0)) {
        case AIR_STEP_LANDED:
            if (!check_fall_damage(m, ACT_HARD_BACKWARD_GROUND_KB)) {
                m->action = ACT_AIR_THROW_LAND;
            }
            break;

        case AIR_STEP_HIT_WALL:
            mario_set_forward_vel(m, 0.0f);
            break;

        case AIR_STEP_HIT_LAVA_WALL:
            lava_boost_on_wall(m);
            break;
    }

    return FALSE;
}

s32 act_water_jump(struct MarioState *m) {
    if (m->forwardVel < 15.0f) {
        mario_set_forward_vel(m, 15.0f);
    }

    play_mario_sound(m, SOUND_ACTION_UNKNOWN432, 0);
    set_mario_animation(m, MARIO_ANIM_SINGLE_JUMP);

    switch (perform_air_step(m, AIR_STEP_CHECK_LEDGE_GRAB)) {
        case AIR_STEP_LANDED:
            set_mario_action(m, ACT_JUMP_LAND, 0);
            set_camera_mode(m->area->camera, m->area->camera->defMode, 1);
            break;

        case AIR_STEP_HIT_WALL:
            mario_set_forward_vel(m, 15.0f);
            break;

        case AIR_STEP_GRABBED_LEDGE:
            set_mario_action(m, ACT_LEDGE_GRAB, 0);
            set_camera_mode(m->area->camera, m->area->camera->defMode, 1);
            break;

        case AIR_STEP_HIT_LAVA_WALL:
            lava_boost_on_wall(m);
            break;
    }

    return FALSE;
}

s32 act_hold_water_jump(struct MarioState *m) {
    if (m->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT) {
        return drop_and_set_mario_action(m, ACT_FREEFALL, 0);
    }

    if (m->forwardVel < 15.0f) {
        mario_set_forward_vel(m, 15.0f);
    }

    play_mario_sound(m, SOUND_ACTION_UNKNOWN432, 0);
    set_mario_animation(m, MARIO_ANIM_JUMP_WITH_LIGHT_OBJ);

    switch (perform_air_step(m, 0)) {
        case AIR_STEP_LANDED:
            set_mario_action(m, ACT_HOLD_JUMP_LAND, 0);
            set_camera_mode(m->area->camera, m->area->camera->defMode, 1);
            break;

        case AIR_STEP_HIT_WALL:
            mario_set_forward_vel(m, 15.0f);
            break;

        case AIR_STEP_HIT_LAVA_WALL:
            lava_boost_on_wall(m);
            break;
    }

    return FALSE;
}

s32 act_steep_jump(struct MarioState *m) {
    if (m->input & INPUT_B_PRESSED) {
        return set_mario_action(m, ACT_DIVE, 0);
    }

    play_mario_sound(m, SOUND_ACTION_TERRAIN_JUMP, 0);
    mario_set_forward_vel(m, 0.98f * m->forwardVel);

    switch (perform_air_step(m, 0)) {
        case AIR_STEP_LANDED:
            if (!check_fall_damage(m, ACT_HARD_BACKWARD_GROUND_KB)) {
                m->faceAngle[0] = 0;
                set_mario_action(m, m->forwardVel < 0.0f ? ACT_BEGIN_SLIDING : ACT_JUMP_LAND, 0);
            }
            break;

        case AIR_STEP_HIT_WALL:
            mario_set_forward_vel(m, 0.0f);
            break;

        case AIR_STEP_HIT_LAVA_WALL:
            lava_boost_on_wall(m);
            break;
    }

    set_mario_animation(m, MARIO_ANIM_SINGLE_JUMP);
    m->marioObj->header.gfx.angle[1] = m->marioObj->oMarioSteepJumpYaw;
    return FALSE;
}

s32 act_ground_pound(struct MarioState *m) {
    u32 stepResult;
    f32 yOffset;

    if (m->actionState == 0) {
        if (m->vel[1] > 0.0f) {
            m->vel[1] = 0.0f;
        }

        if (m->actionTimer < 10) {
            yOffset = 20 - 2 * m->actionTimer;
            if (m->pos[1] + yOffset + 160.0f < m->ceilHeight) {
                m->pos[1] += yOffset;
                m->peakHeight = m->pos[1];
                vec3f_copy(m->marioObj->header.gfx.pos, m->pos);
            }
        }

        stepResult = perform_air_step(m, 0);
        mario_set_forward_vel(m, 0.0f);
        set_mario_animation(m, m->actionArg == 0 ? MARIO_ANIM_START_GROUND_POUND
                                                 : MARIO_ANIM_TRIPLE_JUMP_GROUND_POUND);

        if (m->actionTimer == 0) {
            play_sound(SOUND_ACTION_THROW, m->marioObj->header.gfx.cameraToObject);
            play_sound(SOUND_MARIO_HAUGH, m->marioObj->header.gfx.cameraToObject);
        }

        m->actionTimer += 1;

        if (m->actionTimer >= m->marioObj->header.gfx.animInfo.curAnim->loopEnd + 4
            || stepResult == AIR_STEP_LANDED) {
            m->actionState = 1;
        }
    } else {
        set_mario_animation(m, MARIO_ANIM_GROUND_POUND);

        stepResult = perform_air_step(m, 0);
        if (stepResult == AIR_STEP_LANDED) {
            play_sound(SOUND_ACTION_UNK3C, m->marioObj->header.gfx.cameraToObject);
            if (!check_fall_damage(m, ACT_HARD_BACKWARD_GROUND_KB)) {
                set_mario_action(m, ACT_GROUND_POUND_LAND, 0);
            }
            set_camera_shake_from_hit(SHAKE_GROUND_POUND);
        } else if (stepResult == AIR_STEP_HIT_WALL) {
            mario_set_forward_vel(m, -16.0f);
            if (m->vel[1] > 0.0f) {
                m->vel[1] = 0.0f;
            }

            set_mario_action(m, ACT_BACKWARD_AIR_KB, 0);
        }
    }

    return FALSE;
}

s32 act_burning_jump(struct MarioState *m) {
    play_mario_sound(m, SOUND_ACTION_TERRAIN_JUMP, m->actionArg == 0 ? 0 : -1);
    mario_set_forward_vel(m, m->forwardVel);

    if (perform_air_step(m, 0) == AIR_STEP_LANDED) {
        play_mario_landing_sound(m, SOUND_ACTION_TERRAIN_LANDING);
        set_mario_action(m, ACT_BURNING_GROUND, 0);
    }

    set_mario_animation(m, m->actionArg == 0 ? MARIO_ANIM_SINGLE_JUMP : MARIO_ANIM_FIRE_LAVA_BURN);
    m->particleFlags |= PARTICLE_FIRE;
    play_sound(SOUND_MOVING_LAVA_BURN, m->marioObj->header.gfx.cameraToObject);

    m->marioObj->oMarioBurnTimer += 3;

    m->health -= 9;
    if (m->health < 0x100) {
        m->health = 0xFF;
    }

    return FALSE;
}

s32 act_burning_fall(struct MarioState *m) {
    mario_set_forward_vel(m, m->forwardVel);

    if (perform_air_step(m, 0) == AIR_STEP_LANDED) {
        play_mario_landing_sound(m, SOUND_ACTION_TERRAIN_LANDING);
        set_mario_action(m, ACT_BURNING_GROUND, 0);
    }

    set_mario_animation(m, MARIO_ANIM_GENERAL_FALL);
    m->particleFlags |= PARTICLE_FIRE;
    m->marioObj->oMarioBurnTimer += 3;

    m->health -= 10;
    if (m->health < 0x100) {
        m->health = 0xFF;
    }

    return FALSE;
}

u32 common_air_knockback_step(struct MarioState *m, u32 landAction, u32 hardFallAction, s32 animation,
                              f32 speed) {
    u32 stepResult;

    if (m->forwardVel > 0 && m->actionTimer++ == 0 && m->actionArg == 2) {
        mario_set_forward_vel(m, -speed);
    } else {
        mario_set_forward_vel(m, speed);
    }

    stepResult = perform_air_step(m, 0);
    switch (stepResult) {
        case AIR_STEP_NONE:
            set_mario_animation(m, animation);
            break;

        case AIR_STEP_LANDED:
            if (!check_fall_damage(m, hardFallAction)) {
                set_mario_action(m, landAction, m->actionArg);
                if (speed > 0 && m->actionArg == 2)
                    mario_set_forward_vel(m, 0);
            }
            break;

        case AIR_STEP_HIT_WALL:
            set_mario_animation(m, MARIO_ANIM_BACKWARD_AIR_KB);
            mario_bonk_reflection(m, FALSE);

            if (m->vel[1] > 0.0f) {
                m->vel[1] = 0.0f;
            }

            mario_set_forward_vel(m, -speed);
            break;

        case AIR_STEP_HIT_LAVA_WALL:
            lava_boost_on_wall(m);
            break;
    }

    return stepResult;
}

s32 check_wall_kick(struct MarioState *m) {
    if ((m->input & INPUT_A_PRESSED) && m->wallKickTimer != 0 && m->prevAction == ACT_AIR_HIT_WALL) {
        m->faceAngle[1] += 0x8000;
        return set_mario_action(m, ACT_WALL_KICK_AIR, 0);
    }

    return FALSE;
}

s32 act_backward_air_kb(struct MarioState *m) {
    if (check_wall_kick(m)) {
        return TRUE;
    }

    play_sound_if_no_flag(m, SOUND_MARIO_HAUGH, MARIO_MARIO_SOUND_PLAYED);

    common_air_knockback_step(m, ACT_BACKWARD_GROUND_KB, ACT_HARD_BACKWARD_GROUND_KB, 0x0002, -16.0f);
    return FALSE;
}

s32 act_forward_air_kb(struct MarioState *m) {
    if (check_wall_kick(m)) {
        return TRUE;
    }

    if (m->forwardVel < 0)
        play_sound_if_no_flag(m, SOUND_MARIO_HAUGH, MARIO_MARIO_SOUND_PLAYED);

    common_air_knockback_step(m, ACT_FORWARD_GROUND_KB, ACT_HARD_FORWARD_GROUND_KB, 0x002D, 16.0f);
    return FALSE;
}

s32 act_hard_backward_air_kb(struct MarioState *m) {
    play_sound_if_no_flag(m, SOUND_MARIO_HAUGH, MARIO_MARIO_SOUND_PLAYED);
    common_air_knockback_step(m, ACT_HARD_BACKWARD_GROUND_KB, ACT_HARD_BACKWARD_GROUND_KB, 0x0002,
                              -16.0f);
    return FALSE;
}

s32 act_hard_forward_air_kb(struct MarioState *m) {
    play_sound_if_no_flag(m, SOUND_MARIO_HAUGH, MARIO_MARIO_SOUND_PLAYED);
    common_air_knockback_step(m, ACT_HARD_FORWARD_GROUND_KB, ACT_HARD_FORWARD_GROUND_KB, 0x002D, 16.0f);
    return FALSE;
}

s32 act_thrown_backward(struct MarioState *m) {
    u32 landAction;
    if (m->actionArg != 0) {
        landAction = ACT_HARD_BACKWARD_GROUND_KB;
    } else {
        landAction = ACT_BACKWARD_GROUND_KB;
    }

    play_sound_if_no_flag(m, SOUND_MARIO_WAAAOOOW, MARIO_MARIO_SOUND_PLAYED);

    common_air_knockback_step(m, landAction, ACT_HARD_BACKWARD_GROUND_KB, 0x0002, m->forwardVel);

    m->forwardVel *= 0.98f;
    return FALSE;
}

s32 act_thrown_forward(struct MarioState *m) {
    s16 pitch;

    u32 landAction;
    if (m->actionArg != 0) {
        landAction = ACT_HARD_FORWARD_GROUND_KB;
    } else {
        landAction = ACT_FORWARD_GROUND_KB;
    }

    play_sound_if_no_flag(m, SOUND_MARIO_WAAAOOOW, MARIO_MARIO_SOUND_PLAYED);

    if (common_air_knockback_step(m, landAction, ACT_HARD_FORWARD_GROUND_KB, 0x002D, m->forwardVel)
        == AIR_STEP_NONE) {
        pitch = atan2s(m->forwardVel, -m->vel[1]);
        if (pitch > 0x1800) {
            pitch = 0x1800;
        }

        m->marioObj->header.gfx.angle[0] = pitch + 0x1800;
    }

    m->forwardVel *= 0.98f;
    return FALSE;
}

s32 act_soft_bonk(struct MarioState *m) {
    if (check_wall_kick(m)) {
        return TRUE;
    }

    play_sound_if_no_flag(m, SOUND_MARIO_HAUGH, MARIO_MARIO_SOUND_PLAYED);

    common_air_knockback_step(m, ACT_FREEFALL_LAND, ACT_HARD_BACKWARD_GROUND_KB, 0x0056, m->forwardVel);
    return FALSE;
}

s32 act_air_hit_wall(struct MarioState *m) {
    if (m->heldObj != NULL) {
        mario_drop_held_object(m);
    }

    if (++(m->actionTimer) < 2) {
        if (m->input & INPUT_A_PRESSED) {
            m->vel[1] = 52.0f;
            m->faceAngle[1] += 0x8000;
            return set_mario_action(m, ACT_WALL_KICK_AIR, 0);
        }
    } else if (m->forwardVel >= 38.0f) {
        m->wallKickTimer = 5;
        if (m->vel[1] > 0.0f) {
            m->vel[1] = 0.0f;
        }

        return set_mario_action(m, ACT_BACKWARD_AIR_KB, 0);
    } else {
        m->wallKickTimer = 5;
        if (m->vel[1] > 0.0f) {
            m->vel[1] = 0.0f;
        }

        if (m->forwardVel > 8.0f) {
            mario_set_forward_vel(m, -8.0f);
        }
        return set_mario_action(m, ACT_SOFT_BONK, 0);
    }

    set_mario_animation(m, MARIO_ANIM_START_WALLKICK);
    return FALSE;
}

s32 act_forward_rollout(struct MarioState *m) {
    if (m->actionState == 0) {
        if (m->intendedMag > 0) {
            m->vel[1] = 30.0f;
            m->actionState = 1;
        } else {
            m->vel[1] = 15.0f;
            m->actionState = 1;
        }
    }

    update_air_without_turn(m);

    switch (perform_air_step(m, 0)) {
        case AIR_STEP_NONE:
            if (m->actionState == 1) {
                if (set_mario_animation(m, MARIO_ANIM_FORWARD_SPINNING) == 4) {
                    play_sound(SOUND_ACTION_SPIN, m->marioObj->header.gfx.cameraToObject);
                }
            } else {
                set_mario_animation(m, MARIO_ANIM_GENERAL_FALL);
            }
            break;

        case AIR_STEP_LANDED:
            set_mario_action(m, ACT_FREEFALL_LAND_STOP, 0);
            play_mario_landing_sound(m, SOUND_ACTION_TERRAIN_LANDING);
            break;

        case AIR_STEP_HIT_WALL:
            mario_set_forward_vel(m, 0.0f);
            break;

        case AIR_STEP_HIT_LAVA_WALL:
            lava_boost_on_wall(m);
            break;
    }

    if (m->actionState == 1 && is_anim_past_end(m)) {
        m->actionState = 2;
    }
    return FALSE;
}

s32 act_backward_rollout(struct MarioState *m) {
    if (m->actionState == 0) {
        if (m->intendedMag > 0) {
            m->vel[1] = 30.0f;
            m->actionState = 1;
        } else {
            m->vel[1] = 15.0f;
            m->actionState = 1;
        }
    }

    update_air_without_turn(m);

    switch (perform_air_step(m, 0)) {
        case AIR_STEP_NONE:
            if (m->actionState == 1) {
                if (set_mario_animation(m, MARIO_ANIM_BACKWARD_SPINNING) == 4) {
                    play_sound(SOUND_ACTION_SPIN, m->marioObj->header.gfx.cameraToObject);
                }
            } else {
                set_mario_animation(m, MARIO_ANIM_GENERAL_FALL);
            }
            break;

        case AIR_STEP_LANDED:
            set_mario_action(m, ACT_FREEFALL_LAND_STOP, 0);
            play_mario_landing_sound(m, SOUND_ACTION_TERRAIN_LANDING);
            break;

        case AIR_STEP_HIT_WALL:
            mario_set_forward_vel(m, 0.0f);
            break;

        case AIR_STEP_HIT_LAVA_WALL:
            lava_boost_on_wall(m);
            break;
    }

    if (m->actionState == 1 && m->marioObj->header.gfx.animInfo.animFrame == 2) {
        m->actionState = 2;
    }
    return FALSE;
}

s32 act_butt_slide_air(struct MarioState *m) {
    if (++(m->actionTimer) > 30 && m->pos[1] - m->floorHeight > 500.0f) {
        return set_mario_action(m, ACT_FREEFALL, 1);
    }

    update_air_with_turn(m);

    switch (perform_air_step(m, 0)) {
        case AIR_STEP_LANDED:
            if (m->actionState == 0 && m->vel[1] < 0.0f && m->floor->normal.y >= 0.9848077f) {
                m->vel[1] = -m->vel[1] / 2.0f;
                m->actionState = 1;
            } else {
                set_mario_action(m, ACT_BUTT_SLIDE, 0);
            }
            play_sound_and_spawn_particles(m, SOUND_ACTION_TERRAIN_STEP, 0);
            break;

        case AIR_STEP_HIT_WALL:
            if (m->vel[1] > 0.0f) {
                m->vel[1] = 0.0f;
            }

            set_mario_action(m, ACT_BACKWARD_AIR_KB, 0);
            break;

        case AIR_STEP_HIT_LAVA_WALL:
            lava_boost_on_wall(m);
            break;
    }

    set_mario_animation(m, MARIO_ANIM_SLIDE_MOTIONLESS);
    return FALSE;
}

s32 act_hold_butt_slide_air(struct MarioState *m) {
    if (m->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT) {
        return drop_and_set_mario_action(m, ACT_HOLD_FREEFALL, 1);
    }

    if (++m->actionTimer > 30 && m->pos[1] - m->floorHeight > 500.0f) {
        return set_mario_action(m, ACT_HOLD_FREEFALL, 1);
    }

    update_air_with_turn(m);

    switch (perform_air_step(m, 0)) {
        case AIR_STEP_LANDED:
            if (m->actionState == 0 && m->vel[1] < 0.0f && m->floor->normal.y >= 0.9848077f) {
                m->vel[1] = -m->vel[1] / 2.0f;
                m->actionState = 1;
            } else {
                set_mario_action(m, ACT_HOLD_BUTT_SLIDE, 0);
            }
            play_mario_landing_sound(m, SOUND_ACTION_TERRAIN_LANDING);
            break;

        case AIR_STEP_HIT_WALL:
            if (m->vel[1] > 0.0f) {
                m->vel[1] = 0.0f;
            }

            mario_drop_held_object(m);
            set_mario_action(m, ACT_BACKWARD_AIR_KB, 0);
            break;

        case AIR_STEP_HIT_LAVA_WALL:
            lava_boost_on_wall(m);
            break;
    }

    set_mario_animation(m, MARIO_ANIM_SLIDING_ON_BOTTOM_WITH_LIGHT_OBJ);
    return FALSE;
}

s32 act_lava_boost(struct MarioState *m) {
    play_sound_if_no_flag(m, SOUND_MARIO_WAAAOOOW, MARIO_MARIO_SOUND_PLAYED);

    if (!(m->input & INPUT_NONZERO_ANALOG)) {
        m->forwardVel = approach_f32(m->forwardVel, 0.0f, 0.35f, 0.35f);
    }

    update_lava_boost_or_twirling(m);

    switch (perform_air_step(m, 0)) {
        case AIR_STEP_LANDED:
            if (m->floor->type == SURFACE_BURNING) {
                m->actionState = 0;
                m->hurtCounter += 12;
                m->vel[1] = 84.0f;
                play_sound(SOUND_MARIO_WAAAOOOW, m->marioObj->header.gfx.cameraToObject);
            } else {
                play_mario_heavy_landing_sound(m, SOUND_ACTION_TERRAIN_BODY_HIT_GROUND);
                if (m->actionState < 2 && m->vel[1] < 0.0f) {
                    m->vel[1] = -m->vel[1] * 0.55f;
                    mario_set_forward_vel(m, m->forwardVel * 0.5f);
                    m->actionState++;
                } else {
                    set_mario_action(m, ACT_LAVA_BOOST_LAND, 0);
                }
            }
            break;

        case AIR_STEP_HIT_WALL:
            mario_bonk_reflection(m, FALSE);
            break;

        case AIR_STEP_HIT_LAVA_WALL:
            lava_boost_on_wall(m);
            break;
    }

    set_mario_animation(m, MARIO_ANIM_FIRE_LAVA_BURN);
    if ((m->area->terrainType & TERRAIN_MASK) != TERRAIN_SNOW && m->vel[1] > 0.0f) {
        m->particleFlags |= PARTICLE_FIRE;
        if (m->actionState == 0) {
            play_sound(SOUND_MOVING_LAVA_BURN, m->marioObj->header.gfx.cameraToObject);
        }
    }

    if (m->health < 0x100) {
        level_trigger_warp(m, WARP_OP_DEATH);
    }

    m->marioBodyState->eyeState = MARIO_EYES_DEAD;

    return FALSE;
}

s32 act_shot_from_cannon(struct MarioState *m) {
    if (m->area->camera->mode != CAMERA_MODE_BEHIND_MARIO) {
        m->statusForCamera->cameraEvent = CAM_EVENT_SHOT_FROM_CANNON;
    }

    mario_set_forward_vel(m, m->forwardVel);

    m->actionTimer++;

    play_sound_if_no_flag(m, SOUND_MARIO_YAHOO, MARIO_MARIO_SOUND_PLAYED);

    switch (perform_air_step(m, 0)) {
        case AIR_STEP_NONE:
            set_mario_animation(m, MARIO_ANIM_AIRBORNE_ON_STOMACH);
            m->faceAngle[0] = atan2s(m->forwardVel, m->vel[1]);
            m->marioObj->header.gfx.angle[0] = -m->faceAngle[0];
            break;

        case AIR_STEP_LANDED:
            set_mario_action(m, ACT_DIVE_SLIDE, 0);
            m->faceAngle[0] = 0;
            set_camera_mode(m->area->camera, m->area->camera->defMode, 1);
            break;

        case AIR_STEP_HIT_WALL:
            mario_set_forward_vel(m, -16.0f);

            m->faceAngle[0] = 0;
            if (m->vel[1] > 0.0f) {
                m->vel[1] = 0.0f;
            }

            set_mario_action(m, ACT_BACKWARD_AIR_KB, 0);
            set_camera_mode(m->area->camera, m->area->camera->defMode, 1);
            break;

        case AIR_STEP_HIT_LAVA_WALL:
            lava_boost_on_wall(m);
            break;
    }

    m->vel[1] -= 1.15f;

    if (m->actionTimer == 45) {
        set_mario_action(m, ACT_FLYING, 0);
    }

    if ((m->forwardVel -= 0.05) < 10.0f) {
        mario_set_forward_vel(m, 10.0f);
    }

    return FALSE;
}

s32 act_flying(struct MarioState *m) {
    s16 startPitch = m->faceAngle[0];

    if (m->input & INPUT_Z_PRESSED) {
        if (m->area->camera->mode == CAMERA_MODE_BEHIND_MARIO) {
            set_camera_mode(m->area->camera, m->area->camera->defMode, 1);
        }
        return set_mario_action(m, ACT_GROUND_POUND, 1);
    }

    if (m->area->camera->mode != CAMERA_MODE_BEHIND_MARIO) {
        set_camera_mode(m->area->camera, CAMERA_MODE_BEHIND_MARIO, 1);
    }

    if (m->actionState == 0) {
        if (m->actionArg == 0) {
            set_mario_animation(m, MARIO_ANIM_FLY_FROM_CANNON);
        } else {
            set_mario_animation(m, MARIO_ANIM_FORWARD_SPINNING_FLIP);
            if (m->marioObj->header.gfx.animInfo.animFrame == 1) {
                play_sound(SOUND_ACTION_SPIN, m->marioObj->header.gfx.cameraToObject);
            }
        }

        if (is_anim_at_end(m)) {
            if (m->actionArg == 2) {
                load_level_init_text(0);
                m->actionArg = 1;
            }

            set_mario_animation(m, MARIO_ANIM_WING_CAP_FLY);
            m->actionState = 1;
        }
    }

    update_flying(m);

    switch (perform_air_step(m, 0)) {
        case AIR_STEP_NONE:
            m->marioObj->header.gfx.angle[0] = -m->faceAngle[0];
            m->marioObj->header.gfx.angle[2] = m->faceAngle[2];
            m->actionTimer = 0;
            break;

        case AIR_STEP_LANDED:
            set_mario_action(m, ACT_DIVE_SLIDE, 0);

            set_mario_animation(m, MARIO_ANIM_DIVE);
            set_anim_to_frame(m, 7);

            m->faceAngle[0] = 0;
            set_camera_mode(m->area->camera, m->area->camera->defMode, 1);
            break;

        case AIR_STEP_HIT_WALL:
            if (m->wall != NULL) {
                mario_set_forward_vel(m, -16.0f);
                m->faceAngle[0] = 0;

                if (m->vel[1] > 0.0f) {
                    m->vel[1] = 0.0f;
                }
                set_mario_action(m, ACT_BACKWARD_AIR_KB, 0);
                set_camera_mode(m->area->camera, m->area->camera->defMode, 1);
            } else {

                if (m->actionTimer == 30) {
                    m->actionTimer = 0;
                }

                m->faceAngle[0] -= 0x200;
                if (m->faceAngle[0] < -0x2AAA) {
                    m->faceAngle[0] = -0x2AAA;
                }

                m->marioObj->header.gfx.angle[0] = -m->faceAngle[0];
                m->marioObj->header.gfx.angle[2] = m->faceAngle[2];
            }
            break;

        case AIR_STEP_HIT_LAVA_WALL:
            lava_boost_on_wall(m);
            break;
    }

    if (m->faceAngle[0] > 0x800 && m->forwardVel >= 48.0f) {
        m->particleFlags |= PARTICLE_DUST;
    }

    if (startPitch <= 0 && m->faceAngle[0] > 0 && m->forwardVel >= 48.0f) {
    }
    return FALSE;
}

s32 act_riding_hoot(struct MarioState *m) {
    if (!(m->input & INPUT_A_DOWN) || (m->marioObj->oInteractStatus & INT_STATUS_MARIO_UNK7)) {
        m->usedObj->oInteractStatus = 0;
        m->usedObj->oHootMarioReleaseTime = gGlobalTimer;

        play_sound_if_no_flag(m, SOUND_MARIO_HAUGH, MARIO_MARIO_SOUND_PLAYED);

        return set_mario_action(m, ACT_FREEFALL, 0);
    }

    m->pos[0] = m->usedObj->oPosX;
    m->pos[1] = m->usedObj->oPosY - 92.5f;
    m->pos[2] = m->usedObj->oPosZ;

    m->faceAngle[1] = 0x4000 - m->usedObj->oMoveAngleYaw;

    if (m->actionState == 0) {
        set_mario_animation(m, MARIO_ANIM_HANG_ON_CEILING);
        if (is_anim_at_end(m)) {
            set_mario_animation(m, MARIO_ANIM_HANG_ON_OWL);
            m->actionState = 1;
        }
    }

    vec3f_set(m->vel, 0.0f, 0.0f, 0.0f);
    vec3f_set(m->marioObj->header.gfx.pos, m->pos[0], m->pos[1], m->pos[2]);
    vec3s_set(m->marioObj->header.gfx.angle, 0, 0x4000 - m->faceAngle[1], 0);
    return FALSE;
}

s32 act_top_of_pole_jump(struct MarioState *m) {
    play_mario_jump_sound(m);
    common_air_action_step(m, ACT_FREEFALL_LAND, MARIO_ANIM_HANDSTAND_JUMP, AIR_STEP_CHECK_LEDGE_GRAB);
    return FALSE;
}

s32 act_vertical_wind(struct MarioState *m) {
    s16 intendedDYaw = m->intendedYaw - m->faceAngle[1];
    f32 intendedMag = m->intendedMag / 32.0f;

    play_sound_if_no_flag(m, SOUND_MARIO_HERE_WE_GO, MARIO_MARIO_SOUND_PLAYED);
    if (m->actionState == 0) {
        set_mario_animation(m, MARIO_ANIM_FORWARD_SPINNING_FLIP);
        if (m->marioObj->header.gfx.animInfo.animFrame == 1) {
            play_sound(SOUND_ACTION_SPIN, m->marioObj->header.gfx.cameraToObject);
        }

        if (is_anim_past_end(m)) {
            m->actionState = 1;
        }
    } else {
        set_mario_animation(m, MARIO_ANIM_AIRBORNE_ON_STOMACH);
    }

    update_air_without_turn(m);

    switch (perform_air_step(m, 0)) {
        case AIR_STEP_LANDED:
            set_mario_action(m, ACT_DIVE_SLIDE, 0);
            break;

        case AIR_STEP_HIT_WALL:
            mario_set_forward_vel(m, -16.0f);
            break;
    }

    m->marioObj->header.gfx.angle[0] = (s16) (6144.0f * intendedMag * coss(intendedDYaw));
    m->marioObj->header.gfx.angle[2] = (s16) (-4096.0f * intendedMag * sins(intendedDYaw));
    return FALSE;
}

s32 check_common_airborne_cancels(struct MarioState *m) {
    if (m->pos[1] < m->waterLevel - 100) {
        return set_water_plunge_action(m);
    }

    if (m->input & INPUT_SQUISHED) {
        return drop_and_set_mario_action(m, ACT_SQUISHED, 0);
    }

    if (m->floor->type == SURFACE_VERTICAL_WIND && (m->action & ACT_FLAG_ALLOW_VERTICAL_WIND_ACTION)) {
        return drop_and_set_mario_action(m, ACT_VERTICAL_WIND, 0);
    }

    return FALSE;
}

s32 mario_execute_airborne_action(struct MarioState *m) {
    u32 cancel;

    if (check_common_airborne_cancels(m)) {
        return TRUE;
    }

    play_far_fall_sound(m);

    /* clang-format off */
    switch (m->action) {
        case ACT_JUMP:                 cancel = act_jump(m);                 break;
        case ACT_DOUBLE_JUMP:          cancel = act_double_jump(m);          break;
        case ACT_FREEFALL:             cancel = act_freefall(m);             break;
        case ACT_HOLD_JUMP:            cancel = act_hold_jump(m);            break;
        case ACT_HOLD_FREEFALL:        cancel = act_hold_freefall(m);        break;
        case ACT_SIDE_FLIP:            cancel = act_side_flip(m);            break;
        case ACT_WALL_KICK_AIR:        cancel = act_wall_kick_air(m);        break;
        case ACT_TWIRLING:             cancel = act_twirling(m);             break;
        case ACT_WATER_JUMP:           cancel = act_water_jump(m);           break;
        case ACT_HOLD_WATER_JUMP:      cancel = act_hold_water_jump(m);      break;
        case ACT_STEEP_JUMP:           cancel = act_steep_jump(m);           break;
        case ACT_BURNING_JUMP:         cancel = act_burning_jump(m);         break;
        case ACT_BURNING_FALL:         cancel = act_burning_fall(m);         break;
        case ACT_DIVE:                 cancel = act_dive(m);                 break;
        case ACT_AIR_THROW:            cancel = act_air_throw(m);            break;
        case ACT_BACKWARD_AIR_KB:      cancel = act_backward_air_kb(m);      break;
        case ACT_FORWARD_AIR_KB:       cancel = act_forward_air_kb(m);       break;
        case ACT_HARD_FORWARD_AIR_KB:  cancel = act_hard_forward_air_kb(m);  break;
        case ACT_HARD_BACKWARD_AIR_KB: cancel = act_hard_backward_air_kb(m); break;
        case ACT_SOFT_BONK:            cancel = act_soft_bonk(m);            break;
        case ACT_AIR_HIT_WALL:         cancel = act_air_hit_wall(m);         break;
        case ACT_FORWARD_ROLLOUT:      cancel = act_forward_rollout(m);      break;
        case ACT_SHOT_FROM_CANNON:     cancel = act_shot_from_cannon(m);     break;
        case ACT_BUTT_SLIDE_AIR:       cancel = act_butt_slide_air(m);       break;
        case ACT_HOLD_BUTT_SLIDE_AIR:  cancel = act_hold_butt_slide_air(m);  break;
        case ACT_LAVA_BOOST:           cancel = act_lava_boost(m);           break;
        case ACT_BACKWARD_ROLLOUT:     cancel = act_backward_rollout(m);     break;
        case ACT_GROUND_POUND:         cancel = act_ground_pound(m);         break;
        case ACT_THROWN_FORWARD:       cancel = act_thrown_forward(m);       break;
        case ACT_THROWN_BACKWARD:      cancel = act_thrown_backward(m);      break;
        case ACT_FLYING:               cancel = act_flying(m);               break;
        case ACT_RIDING_HOOT:          cancel = act_riding_hoot(m);          break;
        case ACT_TOP_OF_POLE_JUMP:     cancel = act_top_of_pole_jump(m);     break;
        case ACT_VERTICAL_WIND:        cancel = act_vertical_wind(m);        break;
    }
    /* clang-format on */

    return cancel;
}
