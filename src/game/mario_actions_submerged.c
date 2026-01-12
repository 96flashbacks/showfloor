#include <PR/ultratypes.h>

#include "sm64.h"
#include "level_update.h"
#include "memory.h"
#include "engine/math_util.h"
#include "area.h"
#include "save_file.h"
#include "sound_init.h"
#include "engine/surface_collision.h"
#include "interaction.h"
#include "mario.h"
#include "mario_step.h"
#include "camera.h"
#include "audio/external.h"
#include "behavior_data.h"
#include "level_table.h"

#define MIN_SWIM_STRENGTH 160
#define MIN_SWIM_SPEED 16.0f

static s16 sWasAtSurface = FALSE;
static s16 sSwimStrength = MIN_SWIM_STRENGTH;
static s16 sWaterCurrentSpeeds[] = { 28, 12, 8, 4 };

static s16 sBobTimer;
static s16 sBobIncrement;
static f32 sBobHeight;

/* bunch of junk copied from TPP */

static void set_swimming_at_surface_particles(struct MarioState *m, u32 particleFlag) {
    s16 atSurface = m->pos[1] >= m->waterLevel - 130;

    if (atSurface) {
        m->particleFlags |= particleFlag;
        if (atSurface ^ sWasAtSurface) {
            play_sound(SOUND_ACTION_UNKNOWN431, m->marioObj->header.gfx.cameraToObject);
        }
    }

    sWasAtSurface = atSurface;
}

static s32 swimming_near_surface(struct MarioState *m) {
    return (m->waterLevel - 100) - m->pos[1] < 400.0f;
}

static f32 get_buoyancy(struct MarioState *m) {
    f32 buoyancy = 0.0f;

    if (swimming_near_surface(m)) {
        buoyancy = 1.25f;
        //} else if (!(m->action & ACT_FLAG_MOVING)) {
        // buoyancy = -2.0f;
    }

    return buoyancy;
}

static u32 perform_water_full_step(struct MarioState *m, Vec3f nextPos) {
    struct Surface *wall;
    struct Surface *ceil;
    struct Surface *floor;
    f32 ceilHeight;
    f32 floorHeight;

    wall = resolve_and_return_wall_collisions(nextPos, 10.0f, 110.0f);
    floorHeight = find_floor(nextPos[0], nextPos[1], nextPos[2], &floor);
    ceilHeight = vec3f_find_ceil(nextPos, floorHeight, &ceil);

    if (floor == NULL) {
        return WATER_STEP_CANCELLED;
    }

    if (nextPos[1] >= floorHeight) {
        if (ceilHeight - nextPos[1] >= 160.0f) {
            vec3f_copy(m->pos, nextPos);
            m->floor = floor;
            m->floorHeight = floorHeight;

            if (wall != NULL) {
                return WATER_STEP_HIT_WALL;
            } else {
                return WATER_STEP_NONE;
            }
        }

        if (ceilHeight - floorHeight < 160.0f) {
            return WATER_STEP_CANCELLED;
        }

        //! Water ceiling downwarp
        vec3f_set(m->pos, nextPos[0], ceilHeight - 160.0f, nextPos[2]);
        m->floor = floor;
        m->floorHeight = floorHeight;
        return WATER_STEP_HIT_CEILING;
    } else {
        if (ceilHeight - floorHeight < 160.0f) {
            return WATER_STEP_CANCELLED;
        }

        vec3f_set(m->pos, nextPos[0], floorHeight, nextPos[2]);
        m->floor = floor;
        m->floorHeight = floorHeight;
        return WATER_STEP_HIT_FLOOR;
    }
}

static void apply_water_current(struct MarioState *m, Vec3f step) {
    if (m->floor->type == SURFACE_FLOWING_WATER) {
        s16 currentAngle = m->floor->force << 8;
        f32 currentSpeed = sWaterCurrentSpeeds[m->floor->force >> 8];

        step[0] += currentSpeed * sins(currentAngle);
        step[2] += currentSpeed * coss(currentAngle);
    }
}

static u32 perform_water_step(struct MarioState *m) {
    UNUSED u8 filler[4];
    u32 stepResult;
    Vec3f nextPos;
    Vec3f step;
    struct Object *marioObj = m->marioObj;

    vec3f_copy(step, m->vel);

    if (m->action & ACT_FLAG_SWIMMING) {
        apply_water_current(m, step);
    }

    nextPos[0] = m->pos[0] + step[0];
    nextPos[1] = m->pos[1] + step[1];
    nextPos[2] = m->pos[2] + step[2];

    if (nextPos[1] > m->waterLevel - 100) {
        nextPos[1] = m->waterLevel - 100;
        m->vel[1] = 0.0f;
    }

    stepResult = perform_water_full_step(m, nextPos);

    vec3f_copy(marioObj->header.gfx.pos, m->pos);
    vec3s_set(marioObj->header.gfx.angle, -m->faceAngle[0], m->faceAngle[1], m->faceAngle[2]);

    return stepResult;
}

static void stationary_slow_down(struct MarioState *m) {
    f32 buoyancy = get_buoyancy(m);

    m->angleVel[0] = 0;
    m->angleVel[1] = 0;

    m->forwardVel = approach_f32(m->forwardVel, 0.0f, 1.0f, 1.0f);
    m->vel[1] = approach_f32(m->vel[1], buoyancy, 2.0f, 1.0f);

    m->faceAngle[0] = approach_s32(m->faceAngle[0], 0, 0x200, 0x200);
    m->faceAngle[2] = approach_s32(m->faceAngle[2], 0, 0x100, 0x100);

    m->vel[0] = m->forwardVel * coss(m->faceAngle[0]) * sins(m->faceAngle[1]);
    m->vel[2] = m->forwardVel * coss(m->faceAngle[0]) * coss(m->faceAngle[1]);
}

static void update_swimming_speed(struct MarioState *m, f32 decelThreshold) {
    f32 buoyancy = get_buoyancy(m);
    f32 maxSpeed = 28.0f;

    if (m->action & ACT_FLAG_STATIONARY) {
        m->forwardVel -= 2.0f;
    }

    if (m->forwardVel < 0.0f) {
        m->forwardVel = 0.0f;
    }

    if (m->forwardVel > maxSpeed) {
        m->forwardVel = maxSpeed;
    }

    if (m->forwardVel > decelThreshold) {
        m->forwardVel -= 0.5f;
    }

    m->vel[0] = m->forwardVel * coss(m->faceAngle[0]) * sins(m->faceAngle[1]);
    m->vel[1] = m->forwardVel * sins(m->faceAngle[0]) + buoyancy;
    m->vel[2] = m->forwardVel * coss(m->faceAngle[0]) * coss(m->faceAngle[1]);
}

static void update_swimming_yaw(struct MarioState *m, s32 arg) {
    s16 targetYawVel = -(s16) (10.0f * m->controller->stickX);

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
    // m->faceAngle[2] = -m->angleVel[1] * 8;

    // If we're not floating on the water then let Mario change on his roll axis.
    if (arg == 0) {
        m->faceAngle[2] = -m->angleVel[1] * 8;
    } else {
        m->faceAngle[2] = 0;
    }
}

static void update_swimming_pitch(struct MarioState *m) {
    s16 targetPitch = -(s16) (252.0f * m->controller->stickY);

    s16 pitchVel;
    if (m->faceAngle[0] < 0) {
        pitchVel = 0x100;
    } else {
        pitchVel = 0x200;
    }

    if (m->faceAngle[0] < targetPitch) {
        if ((m->faceAngle[0] += pitchVel) > targetPitch) {
            m->faceAngle[0] = targetPitch;
        }
    } else if (m->faceAngle[0] > targetPitch) {
        if ((m->faceAngle[0] -= pitchVel) < targetPitch) {
            m->faceAngle[0] = targetPitch;
        }
    }
}

static void common_idle_step(struct MarioState *m, s32 animation, s32 arg) {
    s16 targetPitch = -(s16) (252.0f);
    s16 targetSpeed = (s16) (32.0f * m->controller->stickY);

    update_swimming_yaw(m, arg);
    update_swimming_speed(m, MIN_SWIM_SPEED);
    perform_water_step(m);

    if (targetSpeed > 0.0f) {
        m->forwardVel += 2.5f;
    }

    if (m->forwardVel > 12.0f) {
        m->forwardVel = 12.0f;
    }

#ifdef POWBUILD
    if (m->forwardVel > 2.0f) {
        set_swimming_at_surface_particles(m, PARTICLE_WAVE_TRAIL);
    }
#endif

    if (m->faceAngle[0] < targetPitch) {
        if ((m->faceAngle[0] += 0x200) > targetPitch) {
            m->faceAngle[0] = targetPitch;
        }
    } else if (m->faceAngle[0] > targetPitch) {
        if ((m->faceAngle[0] -= 0x200) < targetPitch) {
            m->faceAngle[0] = targetPitch;
        }
    }

    if (m->pos[1] <= m->waterLevel - 130) {
        if (m->actionTimer++ > 15) {
            m->actionTimer = 0;
        }
        if (m->actionTimer == 1) {
            play_sound(SOUND_ACTION_SWIMMING_IDLE, m->marioObj->header.gfx.cameraToObject);
        }
    }

    if (targetSpeed <= 0.0f && m->angleVel[1] == 0.0f) {
        set_mario_animation(m, animation);
    } else if (animation == MARIO_ANIM_SWIM_STOP) {
        set_mario_animation(m, animation);
    } else if (arg == 1 && (targetSpeed > 0.0f || m->angleVel[1] != 0.0f)) {
        set_mario_animation(m, MARIO_ANIM_SWIM_MOVE);
    } else {
        set_mario_anim_with_accel(m, animation, arg);
    }
#ifdef POWBUILD
    set_swimming_at_surface_particles(m, PARTICLE_IDLE_WATER_WAVE);
#endif
}

static s32 act_water_idle(struct MarioState *m) {
    /*if (m->input & INPUT_B_PRESSED) {
        return set_mario_action(m, ACT_WATER_PUNCH, 0);
    }*/

    if (m->input & INPUT_A_PRESSED) {
        return set_mario_action(m, ACT_BREASTSTROKE, 0);
    }

    common_idle_step(m, MARIO_ANIM_SWIM_WAIT, 1);
    return FALSE;
}

static s32 act_hold_water_idle(struct MarioState *m) {
    if (m->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT) {
        return drop_and_set_mario_action(m, ACT_WATER_IDLE, 0);
    }

    if (m->input & INPUT_B_PRESSED) {
        return set_mario_action(m, ACT_WATER_THROW, 0);
    }

    if (m->input & INPUT_A_PRESSED) {
        return set_mario_action(m, ACT_HOLD_BREASTSTROKE, 0);
    }

    common_idle_step(m, MARIO_ANIM_WATER_IDLE_WITH_OBJ, 0);
    return FALSE;
}

static s32 act_water_action_end(struct MarioState *m) {
    /*if (m->input & INPUT_B_PRESSED) {
        return set_mario_action(m, ACT_WATER_PUNCH, 0);
    }*/

    if (m->input & INPUT_A_PRESSED) {
        return set_mario_action(m, ACT_BREASTSTROKE, 0);
    }

    common_idle_step(m, MARIO_ANIM_SWIM_STOP, 1);
    if (is_anim_at_end(m)) {
        set_mario_action(m, ACT_WATER_IDLE, 0);
    }
    return FALSE;
}

static s32 act_hold_water_action_end(struct MarioState *m) {
    if (m->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT) {
        return drop_and_set_mario_action(m, ACT_WATER_IDLE, 0);
    }

    if (m->input & INPUT_B_PRESSED) {
        return set_mario_action(m, ACT_WATER_THROW, 0);
    }

    if (m->input & INPUT_A_PRESSED) {
        return set_mario_action(m, ACT_HOLD_BREASTSTROKE, 0);
    }

    common_idle_step(
        m, m->actionArg == 0 ? MARIO_ANIM_WATER_ACTION_END_WITH_OBJ : MARIO_ANIM_STOP_GRAB_OBJ_WATER,
        0);
    if (is_anim_at_end(m)) {
        set_mario_action(m, ACT_HOLD_WATER_IDLE, 0);
    }
    return FALSE;
}

static void reset_bob_variables(struct MarioState *m) {
    sBobTimer = 0;
    sBobIncrement = 0x800;
    sBobHeight = m->faceAngle[0] / 256.0f + 20.0f;
}

static void common_swimming_step(struct MarioState *m, s16 swimStrength) {
    s16 floorPitch;
    UNUSED struct Object *marioObj = m->marioObj;

    update_swimming_yaw(m, 0);
    update_swimming_pitch(m);
    update_swimming_speed(m, swimStrength / 10.0f);

    switch (perform_water_step(m)) {
        case WATER_STEP_HIT_FLOOR:
            floorPitch = -find_floor_slope(m, -0x8000);
            if (m->faceAngle[0] < floorPitch) {
                m->faceAngle[0] = floorPitch;
            }
            break;

        case WATER_STEP_HIT_CEILING:
            if (m->faceAngle[0] > -0x3000) {
                m->faceAngle[0] -= 0x100;
            }
            break;

        case WATER_STEP_HIT_WALL:
            if (m->controller->stickY == 0.0f) {
                if (m->faceAngle[0] > 0.0f) {
                    m->faceAngle[0] += 0x200;
                    if (m->faceAngle[0] > 0x3F00) {
                        m->faceAngle[0] = 0x3F00;
                    }
                } else {
                    m->faceAngle[0] -= 0x200;
                    if (m->faceAngle[0] < -0x3F00) {
                        m->faceAngle[0] = -0x3F00;
                    }
                }
            }
            break;
    }

    m->marioBodyState->headAngle[0] = approach_s32(m->marioBodyState->headAngle[0], 0, 0x200, 0x200);

#ifdef POWBUILD
    set_swimming_at_surface_particles(m, PARTICLE_WAVE_TRAIL);
#endif
}

static void play_swimming_noise(struct MarioState *m) {
    s16 animFrame = m->marioObj->header.gfx.animInfo.animFrame;

    // This must be one line to match on -O2
    if (animFrame == 0 || animFrame == 12)
        play_sound(SOUND_ACTION_UNKNOWN434, m->marioObj->header.gfx.cameraToObject);
}

static s32 check_water_jump(struct MarioState *m) {
    s32 probe = (s32) (m->pos[1] + 1.5f);

    if (m->input & INPUT_A_PRESSED) {
        if (probe >= m->waterLevel - 100 && m->faceAngle[0] >= 0 && m->controller->stickY < -63.0f) {
            vec3s_set(m->angleVel, 0, 0, 0);

            m->vel[1] = 62.0f;

            if (m->heldObj == NULL) {
                return set_mario_action(m, ACT_WATER_JUMP, 0);
            } else {
                return set_mario_action(m, ACT_HOLD_WATER_JUMP, 0);
            }
        }
    }

    return FALSE;
}

static s32 act_breaststroke(struct MarioState *m) {
    if (m->actionArg == 0) {
        sSwimStrength = MIN_SWIM_STRENGTH;
    }

    /*if (m->input & INPUT_B_PRESSED) {
        return set_mario_action(m, ACT_WATER_PUNCH, 0);
    }*/

    if (++m->actionTimer == 14) {
        return set_mario_action(m, ACT_FLUTTER_KICK, 0);
    }

    if (check_water_jump(m)) {
        return TRUE;
    }

    if (m->actionTimer < 6) {
        m->forwardVel += 0.5f;
    }

    if (m->actionTimer >= 9) {
        m->forwardVel += 1.5f;
    }

    if (m->actionTimer >= 2) {
        if (m->actionTimer < 6 && (m->input & INPUT_A_PRESSED)) {
            m->actionState = 1;
        }

        if (m->actionTimer == 9 && m->actionState == 1) {
            set_anim_to_frame(m, 0);
            m->actionState = 0;
            m->actionTimer = 1;
            sSwimStrength = MIN_SWIM_STRENGTH;
        }
    }

    if (m->actionTimer == 1) {
        play_sound(SOUND_ACTION_SWIM, m->marioObj->header.gfx.cameraToObject);
        reset_bob_variables(m);
    }

    set_mario_animation(m, MARIO_ANIM_SWIM_PART1);
    common_swimming_step(m, sSwimStrength);

    return FALSE;
}

static s32 act_swimming_end(struct MarioState *m) {
    /*if (m->input & INPUT_B_PRESSED) {
        return set_mario_action(m, ACT_WATER_PUNCH, 0);
    }*/

    if (m->actionTimer >= 15) {
        return set_mario_action(m, ACT_WATER_ACTION_END, 0);
    }

    if (check_water_jump(m)) {
        return TRUE;
    }

    if ((m->input & INPUT_A_DOWN) && m->actionTimer >= 7) {
        if (m->actionTimer == 7 && sSwimStrength < 280) {
            sSwimStrength += 10;
        }
        return set_mario_action(m, ACT_BREASTSTROKE, 1);
    }

    if (m->actionTimer >= 7) {
        sSwimStrength = MIN_SWIM_STRENGTH;
    }

    m->actionTimer++;

    m->forwardVel -= 0.25f;
    set_mario_animation(m, MARIO_ANIM_SWIM_PART2);
    common_swimming_step(m, sSwimStrength);

    return FALSE;
}

static s32 act_flutter_kick(struct MarioState *m) {
    /*if (m->input & INPUT_B_PRESSED) {
        return set_mario_action(m, ACT_WATER_PUNCH, 0);
    }*/

    if (!(m->input & INPUT_A_DOWN)) {
        if (m->actionTimer == 0 && sSwimStrength < 280) {
            sSwimStrength += 10;
        }
        return set_mario_action(m, ACT_SWIMMING_END, 0);
    }

    m->forwardVel = approach_f32(m->forwardVel, 12.0f, 0.1f, 0.15f);
    m->actionTimer = 1;
    sSwimStrength = MIN_SWIM_STRENGTH;

    if (m->forwardVel < 14.0f) {
        play_swimming_noise(m);
        set_mario_animation(m, MARIO_ANIM_FLUTTERKICK);
    }

    common_swimming_step(m, sSwimStrength);
    return FALSE;
}

static s32 act_hold_breaststroke(struct MarioState *m) {
    if (m->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT) {
        return drop_and_set_mario_action(m, ACT_WATER_IDLE, 0);
    }

    if (++m->actionTimer == 17) {
        return set_mario_action(m, ACT_HOLD_FLUTTER_KICK, 0);
    }

    if (m->input & INPUT_B_PRESSED) {
        return set_mario_action(m, ACT_WATER_THROW, 0);
    }

    if (check_water_jump(m)) {
        return TRUE;
    }

    if (m->actionTimer < 6) {
        m->forwardVel += 0.5f;
    }

    if (m->actionTimer >= 9) {
        m->forwardVel += 1.5f;
    }

    if (m->actionTimer >= 2) {
        if (m->actionTimer < 6 && (m->input & INPUT_A_PRESSED)) {
            m->actionState = 1;
        }

        if (m->actionTimer == 9 && m->actionState == 1) {
            set_anim_to_frame(m, 0);
            m->actionState = 0;
            m->actionTimer = 1;
        }
    }

    if (m->actionTimer == 1) {
        play_sound(SOUND_ACTION_SWIM, m->marioObj->header.gfx.cameraToObject);
        reset_bob_variables(m);
    }

    set_mario_animation(m, MARIO_ANIM_SWIM_WITH_OBJ_PART1);
    common_swimming_step(m, 0x00A0);
    return FALSE;
}

static s32 act_hold_swimming_end(struct MarioState *m) {
    if (m->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT) {
        return drop_and_set_mario_action(m, ACT_WATER_IDLE, 0);
    }

    if (m->actionTimer >= 15) {
        return set_mario_action(m, ACT_HOLD_WATER_ACTION_END, 0);
    }

    if (m->input & INPUT_B_PRESSED) {
        return set_mario_action(m, ACT_WATER_THROW, 0);
    }

    if (check_water_jump(m)) {
        return TRUE;
    }

    if ((m->input & INPUT_A_DOWN) && m->actionTimer >= 7) {
        return set_mario_action(m, ACT_HOLD_BREASTSTROKE, 0);
    }

    m->actionTimer++;

    m->forwardVel -= 0.25f;
    set_mario_animation(m, MARIO_ANIM_SWIM_WITH_OBJ_PART2);
    common_swimming_step(m, 0x00A0);
    return FALSE;
}

static s32 act_hold_flutter_kick(struct MarioState *m) {
    if (m->marioObj->oInteractStatus & INT_STATUS_MARIO_DROP_OBJECT) {
        return drop_and_set_mario_action(m, ACT_WATER_IDLE, 0);
    }

    if (m->input & INPUT_B_PRESSED) {
        return set_mario_action(m, ACT_WATER_THROW, 0);
    }

    if (!(m->input & INPUT_A_DOWN)) {
        return set_mario_action(m, ACT_HOLD_SWIMMING_END, 0);
    }

    m->forwardVel = approach_f32(m->forwardVel, 12.0f, 0.1f, 0.15f);
    if (m->forwardVel < 14.0f) {
        play_swimming_noise(m);
        set_mario_animation(m, MARIO_ANIM_FLUTTERKICK_WITH_OBJ);
    }
    common_swimming_step(m, 0x00A0);
    return FALSE;
}

static s32 check_water_grab(struct MarioState *m) {
    //! Heave hos have the grabbable interaction type but are not normally
    // grabbable. Since water grabbing doesn't check the appropriate input flag,
    // you can use water grab to pick up heave ho.
    if (m->marioObj->collidedObjInteractTypes & INTERACT_GRABBABLE) {
        struct Object *object = mario_get_collided_object(m, INTERACT_GRABBABLE);
        f32 dx = object->oPosX - m->pos[0];
        f32 dz = object->oPosZ - m->pos[2];
        s16 dAngleToObject = atan2s(dz, dx) - m->faceAngle[1];

        if (dAngleToObject >= -0x2AAA && dAngleToObject <= 0x2AAA) {
            m->usedObj = object;
            mario_grab_used_object(m);
            m->marioBodyState->grabPos = GRAB_POS_LIGHT_OBJ;
            return TRUE;
        }
    }

    return FALSE;
}

static s32 act_water_throw(struct MarioState *m) {
    update_swimming_yaw(m, 0);
    update_swimming_pitch(m);
    update_swimming_speed(m, MIN_SWIM_SPEED);
    perform_water_step(m);

    set_mario_animation(m, MARIO_ANIM_WATER_THROW_OBJ);
    play_sound_if_no_flag(m, SOUND_ACTION_SWIM, MARIO_ACTION_SOUND_PLAYED);

    m->marioBodyState->headAngle[0] = approach_s32(m->marioBodyState->headAngle[0], 0, 0x200, 0x200);

    if (m->actionTimer++ == 5) {
        mario_throw_held_object(m);
    }

    if (is_anim_at_end(m)) {
        set_mario_action(m, ACT_WATER_IDLE, 0);
    }

    return FALSE;
}

static s32 act_water_punch(struct MarioState *m) {
    if (m->forwardVel < 7.0f) {
        m->forwardVel += 1.0f;
    }

    update_swimming_yaw(m, 0);
    update_swimming_pitch(m);
    update_swimming_speed(m, MIN_SWIM_SPEED);
    perform_water_step(m);

    m->marioBodyState->headAngle[0] = approach_s32(m->marioBodyState->headAngle[0], 0, 0x200, 0x200);

    play_sound_if_no_flag(m, SOUND_ACTION_SWIM, MARIO_ACTION_SOUND_PLAYED);

    switch (m->actionState) {
        case 0:
            set_mario_animation(m, MARIO_ANIM_WATER_GRAB_OBJ_PART1);
            if (is_anim_at_end(m)) {
                m->actionState = check_water_grab(m) + 1;
            }
            break;

        case 1:
            set_mario_animation(m, MARIO_ANIM_WATER_GRAB_OBJ_PART2);
            if (is_anim_at_end(m)) {
                set_mario_action(m, ACT_WATER_ACTION_END, 0);
            }
            break;

        case 2:
            set_mario_animation(m, MARIO_ANIM_WATER_PICK_UP_OBJ);
            if (is_anim_at_end(m)) {
                set_mario_action(m, ACT_HOLD_WATER_ACTION_END, 1);
            }
            break;
    }

    return FALSE;
}

static void common_water_knockback_step(struct MarioState *m, s32 animation, u32 endAction) {
    set_mario_animation(m, animation);

    m->marioBodyState->headAngle[0] = 0;

    if (is_anim_at_end(m)) {

        set_mario_action(m, m->health >= 0x100 ? endAction : ACT_WATER_DEATH, 0);
    }
}

static s32 act_backward_water_kb(struct MarioState *m) {
    common_water_knockback_step(m, MARIO_ANIM_BACKWARDS_WATER_KB, ACT_WATER_IDLE);
    return FALSE;
}

static s32 act_forward_water_kb(struct MarioState *m) {
    common_water_knockback_step(m, MARIO_ANIM_WATER_FORWARD_KB, ACT_WATER_IDLE);
    return FALSE;
}

static s32 act_water_shocked(struct MarioState *m) {
    play_sound_if_no_flag(m, SOUND_MARIO_WAAAOOOW, MARIO_ACTION_SOUND_PLAYED);
    play_sound(SOUND_MOVING_SHOCKED, m->marioObj->header.gfx.cameraToObject);

    if (set_mario_animation(m, MARIO_ANIM_SHOCKED) == 0) {
        m->actionTimer++;
        m->flags |= MARIO_METAL_SHOCK;
    }

    if (m->actionTimer >= 6) {
        set_mario_action(m, m->health < 0x100 ? ACT_WATER_DEATH : ACT_WATER_IDLE, 0);
    }

    stationary_slow_down(m);
    perform_water_step(m);
    m->marioBodyState->headAngle[0] = 0;
    return FALSE;
}

static s32 act_drowning(struct MarioState *m) {
    switch (m->actionState) {
        case 0:
            set_mario_animation(m, MARIO_ANIM_DROWNING_PART1);
            m->marioBodyState->eyeState = MARIO_EYES_HALF_CLOSED;
            if (is_anim_at_end(m)) {
                m->actionState = 1;
            }
            break;

        case 1:
            set_mario_animation(m, MARIO_ANIM_DROWNING_PART2);
            m->marioBodyState->eyeState = MARIO_EYES_DEAD;
            if (m->marioObj->header.gfx.animInfo.animFrame == 30) {
                level_trigger_warp(m, WARP_OP_DEATH);
            }
            break;
    }

    stationary_slow_down(m);
    perform_water_step(m);

    return FALSE;
}

static s32 act_water_death(struct MarioState *m) {
    stationary_slow_down(m);
    perform_water_step(m);

    m->marioBodyState->eyeState = MARIO_EYES_DEAD;

    set_mario_animation(m, MARIO_ANIM_WATER_DYING);
    if (set_mario_animation(m, MARIO_ANIM_WATER_DYING) == 35) {
        level_trigger_warp(m, WARP_OP_DEATH);
    }

    return FALSE;
}

static s32 act_water_plunge(struct MarioState *m) {
    u32 stepResult;
    s32 stateFlags = m->heldObj != NULL;

    f32 endVSpeed;
    if (swimming_near_surface(m)) {
        endVSpeed = 0.0f;
    } else {
        endVSpeed = -5.0f;
    }

    if ((m->prevAction & ACT_FLAG_DIVING) || (m->input & INPUT_A_DOWN)) {
        stateFlags |= 2;
    }

    m->actionTimer++;

    stationary_slow_down(m);

    stepResult = perform_water_step(m);

    if (m->actionState == 0) {
        play_sound(SOUND_ACTION_UNKNOWN430, m->marioObj->header.gfx.cameraToObject);
#ifdef POWBUILD
        m->particleFlags |= PARTICLE_WATER_SPLASH;
#endif
        m->actionState = 1;
    }

    if (stepResult == WATER_STEP_HIT_FLOOR || m->vel[1] >= endVSpeed || m->actionTimer > 20) {
        switch (stateFlags) {
            case 0:
                set_mario_action(m, ACT_WATER_IDLE, 0);
                break;
            case 1:
                set_mario_action(m, ACT_HOLD_WATER_ACTION_END, 0);
                break;
            case 2:
                set_mario_action(m, ACT_FLUTTER_KICK, 0);
                break;
            case 3:
                set_mario_action(m, ACT_HOLD_FLUTTER_KICK, 0);
                break;
        }
        sBobIncrement = 0;
    }

    switch (stateFlags) {
        case 0:
            set_mario_animation(m, MARIO_ANIM_SWIM_WAIT);
            break;
        case 1:
            set_mario_animation(m, MARIO_ANIM_WATER_ACTION_END_WITH_OBJ);
            break;
        case 2:
            set_mario_animation(m, MARIO_ANIM_FLUTTERKICK);
            break;
        case 3:
            set_mario_animation(m, MARIO_ANIM_FLUTTERKICK_WITH_OBJ);
            break;
        case 4:
            set_mario_animation(m, MARIO_ANIM_GENERAL_FALL);
            break;
        case 5:
            set_mario_animation(m, MARIO_ANIM_FALL_WITH_LIGHT_OBJ);
            break;
    }
#ifdef POWBUILD
    m->particleFlags |= PARTICLE_PLUNGE_BUBBLE;
#endif
    return FALSE;
}

static s32 check_common_submerged_cancels(struct MarioState *m) {
    if (m->pos[1] > m->waterLevel - 80) {
        if (m->waterLevel - 80 > m->floorHeight) {
            m->pos[1] = m->waterLevel - 80;
        } else {
            return transition_submerged_to_walking(m);
        }
    }

    if (m->health < 0x100 && !(m->action & (ACT_FLAG_INTANGIBLE | ACT_FLAG_INVULNERABLE))) {
        set_mario_action(m, ACT_DROWNING, 0);
    }

    return FALSE;
}

s32 mario_execute_submerged_action(struct MarioState *m) {
    s32 cancel;

    if (check_common_submerged_cancels(m)) {
        return TRUE;
    }

    m->marioBodyState->headAngle[1] = 0;
    m->marioBodyState->headAngle[2] = 0;

    /* clang-format off */
    switch (m->action) {
        case ACT_WATER_IDLE:                 cancel = act_water_idle(m);                 break;
        case ACT_HOLD_WATER_IDLE:            cancel = act_hold_water_idle(m);            break;
        case ACT_WATER_ACTION_END:           cancel = act_water_action_end(m);           break;
        case ACT_HOLD_WATER_ACTION_END:      cancel = act_hold_water_action_end(m);      break;
        case ACT_DROWNING:                   cancel = act_drowning(m);                   break;
        case ACT_BACKWARD_WATER_KB:          cancel = act_backward_water_kb(m);          break;
        case ACT_FORWARD_WATER_KB:           cancel = act_forward_water_kb(m);           break;
        case ACT_WATER_DEATH:                cancel = act_water_death(m);                break;
        case ACT_WATER_SHOCKED:              cancel = act_water_shocked(m);              break;
        case ACT_BREASTSTROKE:               cancel = act_breaststroke(m);               break;
        case ACT_SWIMMING_END:               cancel = act_swimming_end(m);               break;
        case ACT_FLUTTER_KICK:               cancel = act_flutter_kick(m);               break;
        case ACT_HOLD_BREASTSTROKE:          cancel = act_hold_breaststroke(m);          break;
        case ACT_HOLD_SWIMMING_END:          cancel = act_hold_swimming_end(m);          break;
        case ACT_HOLD_FLUTTER_KICK:          cancel = act_hold_flutter_kick(m);          break;
        case ACT_WATER_THROW:                cancel = act_water_throw(m);                break;
        case ACT_WATER_PUNCH:                cancel = act_water_punch(m);                break;
        case ACT_WATER_PLUNGE:               cancel = act_water_plunge(m);               break;
    }
    /* clang-format on */

    return cancel;
}
