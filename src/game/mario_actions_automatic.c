#include <PR/ultratypes.h>

#include "sm64.h"
#include "behavior_data.h"
#include "mario_actions_automatic.h"
#include "audio/external.h"
#include "area.h"
#include "mario.h"
#include "mario_step.h"
#include "engine/math_util.h"
#include "memory.h"
#include "engine/graph_node.h"
#include "save_file.h"
#include "engine/surface_collision.h"
#include "interaction.h"
#include "camera.h"
#include "level_table.h"

#define POLE_NONE 0
#define POLE_TOUCHED_FLOOR 1
#define POLE_FELL_OFF 2

#define HANG_NONE 0
#define HANG_HIT_CEIL_OR_OOB 1
#define HANG_LEFT_CEIL 2

void add_tree_leaf_particles(struct MarioState *m) {
    f32 leafHeight;

    if (m->usedObj->behavior == segmented_to_virtual(bhvTree)) {
        leafHeight = 100.0f;

        if (m->pos[1] - m->floorHeight > leafHeight) {
            m->particleFlags |= PARTICLE_LEAF;
        }
    }
}

void play_climbing_sounds(struct MarioState *m, s32 b) {
    s32 isOnTree = (m->usedObj->behavior == segmented_to_virtual(bhvTree));

    if (b == 1) {
        if (is_anim_past_frame(m, 1)) {
            play_sound(isOnTree ? SOUND_ACTION_CLIMB_UP_TREE : SOUND_ACTION_CLIMB_UP_POLE,
                       m->marioObj->header.gfx.cameraToObject);
        }
    } else {
        play_sound(isOnTree ? SOUND_ACTION_CLIMB_DOWN_TREE : SOUND_MOVING_SLIDE_DOWN_POLE,
                   m->marioObj->header.gfx.cameraToObject);
    }
}

s32 set_pole_position(struct MarioState *m, f32 offsetY) {
    UNUSED u8 filler[12];
    struct Surface *floor;
    struct Surface *ceil;
    f32 floorHeight;
    f32 ceilHeight;
    s32 collided;
    s32 result = POLE_NONE;
    f32 poleTop = m->usedObj->hitboxHeight - 100.0f;
    struct Object *marioObj = m->marioObj;

    if (marioObj->oMarioPolePos > poleTop) {
        marioObj->oMarioPolePos = poleTop;
    }

    m->pos[0] = m->usedObj->oPosX;
    m->pos[2] = m->usedObj->oPosZ;
    m->pos[1] = m->usedObj->oPosY + marioObj->oMarioPolePos + offsetY;

    collided = f32_find_wall_collision(&m->pos[0], &m->pos[1], &m->pos[2], 60.0f, 50.0f);
    collided |= f32_find_wall_collision(&m->pos[0], &m->pos[1], &m->pos[2], 30.0f, 24.0f);

    ceilHeight = vec3f_find_ceil(m->pos, m->pos[1], &ceil);
    if (m->pos[1] > ceilHeight - 160.0f) {
        m->pos[1] = ceilHeight - 160.0f;
        marioObj->oMarioPolePos = m->pos[1] - m->usedObj->oPosY;
    }

    floorHeight = find_floor(m->pos[0], m->pos[1], m->pos[2], &floor);
    if (m->pos[1] < floorHeight) {
        m->pos[1] = floorHeight;
        set_mario_action(m, ACT_IDLE, 0);
        result = POLE_TOUCHED_FLOOR;
    } else if (marioObj->oMarioPolePos < -m->usedObj->hitboxDownOffset) {
        m->pos[1] = m->usedObj->oPosY - m->usedObj->hitboxDownOffset;
        set_mario_action(m, ACT_FREEFALL, 0);
        result = POLE_FELL_OFF;
    } else if (collided) {
        if (m->pos[1] > floorHeight + 20.0f) {
            m->forwardVel = -2.0f;
            set_mario_action(m, ACT_SOFT_BONK, 0);
            result = POLE_FELL_OFF;
        } else {
            set_mario_action(m, ACT_IDLE, 0);
            result = POLE_TOUCHED_FLOOR;
        }
    }

    vec3f_copy(m->marioObj->header.gfx.pos, m->pos);
    vec3s_set(m->marioObj->header.gfx.angle, m->usedObj->oMoveAnglePitch, m->faceAngle[1],
              m->usedObj->oMoveAngleRoll);

    return result;
}

s32 act_holding_pole(struct MarioState *m) {
    struct Object *marioObj = m->marioObj;
    int anim = MARIO_ANIM_IDLE_ON_POLE;

    if (m->input & INPUT_A_PRESSED) {
        add_tree_leaf_particles(m);
		play_mario_jump_sound(m); // play_mario_jump_sound call here to make Mario play a sound only after jumping off a pole
        m->faceAngle[1] += 0x8000;
        return set_mario_action(m, ACT_WALL_KICK_AIR, 0);
    }

    if (m->input & INPUT_Z_PRESSED) {
        add_tree_leaf_particles(m);
        m->forwardVel = -2.0f;
        return set_mario_action(m, ACT_SOFT_BONK, 0);
    }


    if (m->controller->stickY > 16.0f) {
        f32 poleTop = m->usedObj->hitboxHeight - 100.0f;

        if (marioObj->oMarioPolePos < poleTop - 0.4f) {
            return set_mario_action(m, ACT_CLIMBING_POLE, 0);
        }
    }

    if (m->controller->stickY < -16.0f) {
        marioObj->oMarioPoleYawVel -= m->controller->stickY * 2;
        if (marioObj->oMarioPoleYawVel > 0x1000) {
            marioObj->oMarioPoleYawVel = 0x1000;
        }

        m->faceAngle[1] += marioObj->oMarioPoleYawVel;
        marioObj->oMarioPolePos -= marioObj->oMarioPoleYawVel / 0x100;

        if (m->usedObj->behavior == segmented_to_virtual(bhvTree)) {
            //! The Shifting Sand Land palm tree check is done climbing up in
            // add_tree_leaf_particles, but not here, when climbing down.
            if (m->pos[1] - m->floorHeight > 100.0f) {
                m->particleFlags |= PARTICLE_LEAF;
            }
        }
        play_climbing_sounds(m, 2);
        set_sound_moving_speed(SOUND_BANK_MOVING, marioObj->oMarioPoleYawVel / 0x100 * 2);
    } else {
        marioObj->oMarioPoleYawVel = 0;
        m->faceAngle[1] -= m->controller->stickX * 16.0f;
    }

    if (set_pole_position(m, 0.0f) == POLE_NONE) {
        set_mario_animation(m, anim);
    }

    return FALSE;
}

s32 act_climbing_pole(struct MarioState *m) {
    s32 sp24;
    struct Object *marioObj = m->marioObj;
    s16 cameraAngle = m->area->camera->yaw;

    if (m->input & INPUT_A_PRESSED) {
        add_tree_leaf_particles(m);
		play_mario_jump_sound(m); // play_mario_jump_sound call here to make Mario play a sound only after jumping off a pole
        m->faceAngle[1] += 0x8000;
        return set_mario_action(m, ACT_WALL_KICK_AIR, 0);
    }

    if (m->controller->stickY < 8.0f) {
        return set_mario_action(m, ACT_HOLDING_POLE, 0);
    }

    marioObj->oMarioPolePos += m->controller->stickY / 8.0f;
    marioObj->oMarioPoleYawVel = 0;
    m->faceAngle[1] =
        cameraAngle - approach_s32((s16) (cameraAngle - m->faceAngle[1]), 0, 0x400, 0x400);

    if (set_pole_position(m, 0.0f) == POLE_NONE) {
        sp24 = m->controller->stickY / 4.0f * 0x10000;
        set_mario_anim_with_accel(m, MARIO_ANIM_CLIMB_UP_POLE, sp24);
        play_climbing_sounds(m, 1);
    }

    return FALSE;
}

s32 act_grab_pole_slow(struct MarioState *m) {
    play_sound_if_no_flag(m, SOUND_MARIO_WHOA, MARIO_MARIO_SOUND_PLAYED);

    if (set_pole_position(m, 0.0f) == POLE_NONE) {
        set_mario_animation(m, MARIO_ANIM_GRAB_POLE_SHORT);
        if (is_anim_at_end(m)) {
            if ((m->controller->stickY > 16.0f) || (m->controller->stickY < -16.0f)) {
                set_mario_action(m, ACT_HOLDING_POLE, 0);
                m->faceAngle[1] -= m->controller->stickX * 16.0f;
            }
            if (m->input & INPUT_A_PRESSED) {
                add_tree_leaf_particles(m);
                m->faceAngle[1] += 0x8000;
                return set_mario_action(m, ACT_WALL_KICK_AIR, 0);
            }
            if (m->input & INPUT_Z_PRESSED) {
                add_tree_leaf_particles(m);
                m->forwardVel = -2.0f;
                return set_mario_action(m, ACT_SOFT_BONK, 0);
            }
            m->faceAngle[1] -= m->controller->stickX * 16.0f;
        }
    }

    return FALSE;
}

s32 act_grab_pole_fast(struct MarioState *m) {
    struct Object *marioObj = m->marioObj;

    play_sound_if_no_flag(m, SOUND_MARIO_WHOA, MARIO_MARIO_SOUND_PLAYED);
    m->faceAngle[1] += marioObj->oMarioPoleYawVel;
    marioObj->oMarioPoleYawVel = marioObj->oMarioPoleYawVel * 8 / 10;

    if (set_pole_position(m, 0.0f) == POLE_NONE) {
        if (marioObj->oMarioPoleYawVel > 0x800) {
            set_mario_animation(m, MARIO_ANIM_GRAB_POLE_SWING_PART1);
        } else {
            set_mario_animation(m, MARIO_ANIM_GRAB_POLE_SWING_PART2);
            if (is_anim_at_end(m) != 0) {
                marioObj->oMarioPoleYawVel = 0;
                if ((m->controller->stickY > 16.0f) || (m->controller->stickY < -16.0f)) {
                    set_mario_action(m, ACT_HOLDING_POLE, 0);
                    m->faceAngle[1] -= m->controller->stickX * 16.0f;
                }
                if (m->input & INPUT_A_PRESSED) {
                    add_tree_leaf_particles(m);
                    m->faceAngle[1] += 0x8000;
                    return set_mario_action(m, ACT_WALL_KICK_AIR, 0);
                }
                if (m->input & INPUT_Z_PRESSED) {
                    add_tree_leaf_particles(m);
                    m->forwardVel = -2.0f;
                    return set_mario_action(m, ACT_SOFT_BONK, 0);
                }
                m->faceAngle[1] -= m->controller->stickX * 16.0f;
            }
        }
    }

    return FALSE;
}

s32 let_go_of_ledge(struct MarioState *m) {
    f32 floorHeight;
    struct Surface *floor;

    m->vel[1] = 0.0f;
    m->forwardVel = 3.0f;
    m->pos[0] -= 60.0f * sins(m->faceAngle[1]);
    m->pos[2] -= 60.0f * coss(m->faceAngle[1]);
    m->faceAngle[1] += 0x8000;

    floorHeight = find_floor(m->pos[0], m->pos[1], m->pos[2], &floor);
    if (floorHeight < m->pos[1] - 100.0f) {
        m->pos[1] -= 100.0f;
    } else {
        m->pos[1] = floorHeight;
    }

    return set_mario_action(m, ACT_SOFT_BONK, 0);
}

void climb_up_ledge(struct MarioState *m) {
    set_mario_animation(m, MARIO_ANIM_IDLE_HEAD_LEFT);
    m->pos[0] += 14.0f * sins(m->faceAngle[1]);
    m->pos[2] += 14.0f * coss(m->faceAngle[1]);
    vec3f_copy(m->marioObj->header.gfx.pos, m->pos);
}

void update_ledge_climb_camera(struct MarioState *m) {
    f32 sp4;

    if (m->actionTimer < 14) {
        sp4 = m->actionTimer;
    } else {
        sp4 = 14.0f;
    }
    m->statusForCamera->pos[0] = m->pos[0] + sp4 * sins(m->faceAngle[1]);
    m->statusForCamera->pos[2] = m->pos[2] + sp4 * coss(m->faceAngle[1]);
    m->statusForCamera->pos[1] = m->pos[1];
    m->actionTimer++;
    m->flags |= MARIO_LEDGE_CLIMB_CAMERA;
}

void update_ledge_climb(struct MarioState *m, s32 animation, u32 endAction) {
    stop_and_set_height_to_floor(m);

    set_mario_animation(m, animation);
    if (is_anim_at_end(m)) {
        set_mario_action(m, endAction, 0);
        if (endAction == ACT_IDLE) {
            climb_up_ledge(m);
        }
    }
}

s32 act_ledge_grab(struct MarioState *m) {
    f32 heightAboveFloor;
    s16 intendedDYaw = m->intendedYaw - m->faceAngle[1];
    s32 hasSpaceForMario = (m->ceilHeight - m->floorHeight >= 160.0f);

    if (m->actionTimer < 10) {
        m->actionTimer++;
    }

    if (m->floor->normal.y < 0.9063078f) {
        return let_go_of_ledge(m);
    }

    if (m->input & (INPUT_Z_PRESSED | INPUT_OFF_FLOOR)) {
        return let_go_of_ledge(m);
    }

    if ((m->input & INPUT_A_PRESSED) && hasSpaceForMario) {
        return set_mario_action(m, ACT_LEDGE_CLIMB_FAST, 0);
    }

    if (m->input & INPUT_STOMPED) {
        if (m->marioObj->oInteractStatus & INT_STATUS_MARIO_KNOCKBACK_DMG) {
            m->hurtCounter += 12;
        }
        return let_go_of_ledge(m);
    }
    if (m->actionTimer == 10 && (m->input & INPUT_NONZERO_ANALOG)) {
        if (intendedDYaw >= -0x4000 && intendedDYaw <= 0x4000) {
            if (hasSpaceForMario) {
                return set_mario_action(m, ACT_LEDGE_CLIMB_SLOW_1, 0);
            }
        } else {
            return let_go_of_ledge(m);
        }
    }

    heightAboveFloor = m->pos[1] - find_floor_height_relative_polar(m, -0x8000, 30.0f);
    if (hasSpaceForMario && heightAboveFloor < 100.0f) {
        return set_mario_action(m, ACT_LEDGE_CLIMB_FAST, 0);
    }

    if (m->actionArg == 0) {
        play_sound_if_no_flag(m, SOUND_MARIO_WHOA, MARIO_MARIO_SOUND_PLAYED);
    }

    stop_and_set_height_to_floor(m);
    set_mario_animation(m, MARIO_ANIM_IDLE_ON_LEDGE);

    return FALSE;
}

s32 act_ledge_climb_slow(struct MarioState *m) {
    if (m->input & INPUT_OFF_FLOOR) {
        return let_go_of_ledge(m);
    }

    if (m->actionTimer >= 28
        && (m->input
            & (INPUT_NONZERO_ANALOG | INPUT_A_PRESSED | INPUT_OFF_FLOOR | INPUT_ABOVE_SLIDE))) {
        climb_up_ledge(m);
        return check_common_action_exits(m);
    }

    play_sound_if_no_flag(m, SOUND_MARIO_CLIMB, MARIO_MARIO_SOUND_PLAYED);

    update_ledge_climb(m, MARIO_ANIM_SLOW_LEDGE_GRAB, ACT_IDLE);

    update_ledge_climb_camera(m);
    if (m->marioObj->header.gfx.animInfo.animFrame == 17) {
        m->action = ACT_LEDGE_CLIMB_SLOW_2;
    }

    return FALSE;
}

s32 act_ledge_climb_down(struct MarioState *m) {
    if (m->input & INPUT_OFF_FLOOR) {
        return let_go_of_ledge(m);
    }

    play_sound_if_no_flag(m, SOUND_MARIO_WHOA, MARIO_MARIO_SOUND_PLAYED);

    update_ledge_climb(m, MARIO_ANIM_CLIMB_DOWN_LEDGE, ACT_LEDGE_GRAB);
    m->actionArg = 1;

    return FALSE;
}

s32 act_ledge_climb_fast(struct MarioState *m) {
    if (m->input & INPUT_OFF_FLOOR) {
        return let_go_of_ledge(m);
    }

    play_sound_if_no_flag(m, SOUND_MARIO_CLIMB, MARIO_MARIO_SOUND_PLAYED);

    update_ledge_climb(m, MARIO_ANIM_FAST_LEDGE_GRAB, ACT_IDLE);

    if (m->marioObj->header.gfx.animInfo.animFrame == 8) {
        play_mario_landing_sound(m, SOUND_ACTION_TERRAIN_LANDING);
    }
    update_ledge_climb_camera(m);

    return FALSE;
}

s32 act_grabbed(struct MarioState *m) {
    if (m->marioObj->oInteractStatus & INT_STATUS_MARIO_THROWN_BY_OBJ) {
        s32 thrown = (m->marioObj->oInteractStatus & INT_STATUS_MARIO_DROPPED_BY_OBJ) == 0;

        m->faceAngle[1] = m->usedObj->oMoveAngleYaw;
        vec3f_copy(m->pos, m->marioObj->header.gfx.pos);

        return set_mario_action(m, (m->forwardVel >= 0.0f) ? ACT_THROWN_FORWARD : ACT_THROWN_BACKWARD,
                                thrown);
    }

    set_mario_animation(m, MARIO_ANIM_BEING_GRABBED);
    return FALSE;
}

s32 act_in_cannon(struct MarioState *m) {
    struct Object *marioObj = m->marioObj;
    s16 startFacePitch = m->faceAngle[0];
    s16 startFaceYaw = m->faceAngle[1];

    switch (m->actionState) {
        case 0:
            m->marioObj->header.gfx.node.flags &= ~GRAPH_RENDER_ACTIVE;
            m->usedObj->oInteractStatus = INT_STATUS_INTERACTED;

            m->statusForCamera->cameraEvent = CAM_EVENT_CANNON;
            m->statusForCamera->usedObj = m->usedObj;

            vec3f_set(m->vel, 0.0f, 0.0f, 0.0f);

            m->pos[0] = m->usedObj->oPosX;
            m->pos[1] = m->usedObj->oPosY + 350.0f;
            m->pos[2] = m->usedObj->oPosZ;

            m->forwardVel = 0.0f;

            m->actionState = 1;
            break;

        case 1:
            if (m->usedObj->oAction == 1) {
                m->faceAngle[0] = m->usedObj->oMoveAnglePitch;
                m->faceAngle[1] = m->usedObj->oMoveAngleYaw;

                marioObj->oMarioCannonObjectYaw = m->usedObj->oMoveAngleYaw;
                marioObj->oMarioCannonInputYaw = 0;

                m->actionState = 2;
            }
            break;

        case 2:
            m->faceAngle[0] -= (s16) (m->controller->stickY * 10.0f);
            marioObj->oMarioCannonInputYaw -= (s16) (m->controller->stickX * 10.0f);

            if (m->faceAngle[0] > 0x38E3) {
                m->faceAngle[0] = 0x38E3;
            }
            if (m->faceAngle[0] < 0) {
                m->faceAngle[0] = 0;
            }

            if (marioObj->oMarioCannonInputYaw > 0x4000) {
                marioObj->oMarioCannonInputYaw = 0x4000;
            }
            if (marioObj->oMarioCannonInputYaw < -0x4000) {
                marioObj->oMarioCannonInputYaw = -0x4000;
            }

            m->faceAngle[1] = marioObj->oMarioCannonObjectYaw + marioObj->oMarioCannonInputYaw;
            if (m->input & INPUT_A_PRESSED) {
                m->forwardVel = 80.0f * coss(m->faceAngle[0]);

                m->vel[1] = 80.0f * sins(m->faceAngle[0]);

                m->pos[0] += 80.0f * coss(m->faceAngle[0]) * sins(m->faceAngle[1]);
                m->pos[1] += 80.0f * sins(m->faceAngle[0]);
                m->pos[2] += 80.0f * coss(m->faceAngle[0]) * coss(m->faceAngle[1]);

                play_sound(SOUND_OBJ_POUNDING_CANNON, m->marioObj->header.gfx.cameraToObject);

                m->marioObj->header.gfx.node.flags |= GRAPH_RENDER_ACTIVE;

                set_mario_action(m, ACT_SHOT_FROM_CANNON, 0);
                m->usedObj->oAction = 2;
                return FALSE;
            } else {
                if (m->faceAngle[0] != startFacePitch || m->faceAngle[1] != startFaceYaw) {
                    play_sound(SOUND_MOVING_AIM_CANNON, m->marioObj->header.gfx.cameraToObject);
                }
            }
    }

    vec3f_copy(m->marioObj->header.gfx.pos, m->pos);
    vec3s_set(m->marioObj->header.gfx.angle, 0, m->faceAngle[1], 0);
    set_mario_animation(m, MARIO_ANIM_DIVE);

    return FALSE;
}

s32 check_common_automatic_cancels(struct MarioState *m) {
    if (m->pos[1] < m->waterLevel - 100) {
        return set_water_plunge_action(m);
    }

    return FALSE;
}

s32 mario_execute_automatic_action(struct MarioState *m) {
    s32 cancel;

    if (check_common_automatic_cancels(m)) {
        return TRUE;
    }

    /* clang-format off */
    switch (m->action) {
        case ACT_HOLDING_POLE:           cancel = act_holding_pole(m);           break;
        case ACT_GRAB_POLE_SLOW:         cancel = act_grab_pole_slow(m);         break;
        case ACT_GRAB_POLE_FAST:         cancel = act_grab_pole_fast(m);         break;
        case ACT_CLIMBING_POLE:          cancel = act_climbing_pole(m);          break;
        case ACT_LEDGE_GRAB:             cancel = act_ledge_grab(m);             break;
        case ACT_LEDGE_CLIMB_SLOW_1:     cancel = act_ledge_climb_slow(m);       break;
        case ACT_LEDGE_CLIMB_SLOW_2:     cancel = act_ledge_climb_slow(m);       break;
        case ACT_LEDGE_CLIMB_DOWN:       cancel = act_ledge_climb_down(m);       break;
        case ACT_LEDGE_CLIMB_FAST:       cancel = act_ledge_climb_fast(m);       break;
        case ACT_GRABBED:                cancel = act_grabbed(m);                break;
        case ACT_IN_CANNON:              cancel = act_in_cannon(m);              break;
    }
    /* clang-format on */

    return cancel;
}
