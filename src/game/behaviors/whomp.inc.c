// whomp.inc.c

/* why isn't this equated by default? */
#define WHOMP_ACT_INIT 0
#define WHOMP_ACT_PATROL 1
#define WHOMP_ACT_PREPARE 2
#define WHOMP_ACT_JUMP 3
#define WHOMP_ACT_LAND 4
#define WHOMP_ACT_GENERAL 5
#define WHOMP_ACT_TURN 6
#define WHOMP_ACT_DIE 7

void whomp_play_sfx_from_pound_animation(void) {
    UNUSED s32 animFrame = o->header.gfx.animInfo.animFrame;
    s32 playSound = FALSE;

    if (o->oForwardVel < 5.0f) {
        playSound = cur_obj_check_anim_frame(0);
        playSound |= cur_obj_check_anim_frame(23);
    } else {
        playSound = cur_obj_check_anim_frame_in_range(0, 3);
        playSound |= cur_obj_check_anim_frame_in_range(23, 3);
    }

    if (playSound) {
        cur_obj_play_sound_2(SOUND_OBJ_POUNDING1);
    }
}

void whomp_init(void) {
    cur_obj_init_animation_with_accel_and_sound(0, 1.0f);
    cur_obj_set_pos_to_home();

    if (o->oDistanceToMario < 500.0f) {
        o->oAction = WHOMP_ACT_PATROL;
    }
}

void whomp_turn(void) {
    if (o->oSubAction == 0) {
        o->oForwardVel = 0.0f;
        cur_obj_init_animation_with_accel_and_sound(0, 1.0f);
        if (o->oTimer > 31) {
            o->oSubAction++;
        } else {
            o->oMoveAngleYaw += 0x400;
        }
    } else {
        o->oForwardVel = 3.0f;
        if (o->oTimer > 42) {
            o->oAction = WHOMP_ACT_PATROL;
        }
    }
}

void whomp_patrol(void) {
    s16 marioAngle = abs_angle_diff(o->oAngleToMario, o->oMoveAngleYaw);
    f32 distWalked = cur_obj_lateral_dist_to_home();

    cur_obj_init_animation_with_accel_and_sound(0, 1.0f);
    
    if (cur_obj_check_anim_frame_in_range(0, 23)) {
        o->oForwardVel = 9.0f;
    } else {
        o->oForwardVel = 0;
    }

    if (distWalked > 450.0f) {
        o->oAction = WHOMP_ACT_TURN;
    } else if (marioAngle < 0x2000) {
        if (o->oDistanceToMario < 1500.0f) {
            o->oForwardVel = 9.0f;
            cur_obj_init_animation_with_accel_and_sound(0, 3.0f);
        }
        if (o->oDistanceToMario < 300.0f) {
            o->oAction = WHOMP_ACT_PREPARE;
        }
    }
}

void whomp_prepare_jump(void) {
    o->oForwardVel = 0.0f;
    cur_obj_init_animation_with_accel_and_sound(1, 1.0f);
    if (cur_obj_check_if_near_animation_end()) {
        o->oAction = WHOMP_ACT_JUMP;
    }
}

void whomp_jump(void) {
    if (o->oTimer == 0) {
        o->oVelY = 40.0f;
    }

    if (o->oTimer < 8) {
        if (o->oTimer % 2)
            o->oFaceAnglePitch = 0x200;
        else
            o->oFaceAnglePitch = 0;
    } else {
        o->oAngleVelPitch += 0x100;
        o->oFaceAnglePitch += o->oAngleVelPitch;
        if (o->oFaceAnglePitch > 0x4000) {
            o->oAngleVelPitch = 0;
            o->oFaceAnglePitch = 0x4000;
            o->oAction = WHOMP_ACT_LAND;
        }
    }
}

void whomp_land(void) {
    if (o->oSubAction == 0 && o->oMoveFlags & OBJ_MOVE_LANDED) {
        cur_obj_shake_screen(SHAKE_POS_SMALL);
        create_sound_spawner(SOUND_OBJ_THWOMP);
        o->oVelY = 0.0f;
        o->oSubAction++;
    }

    if (o->oMoveFlags & OBJ_MOVE_ON_GROUND) {
        o->oAction = WHOMP_ACT_GENERAL;
    }
}

void whomp_on_ground(void) {
    if (o->oSubAction == 0) {
        if (gMarioObject->platform == o) {
            if (cur_obj_is_mario_ground_pounding_platform()) {
                o->oNumLootCoins = 3;
                obj_spawn_loot_yellow_coins(o, 3, 20.0f);
                o->oAction = WHOMP_ACT_DIE;
            }
        }
    } else if (!cur_obj_is_mario_on_platform()) {
        o->oSubAction = 0;
    }
}

void whomp_on_ground_general(void) {
    if (o->oSubAction != 10) {
        o->oForwardVel = 0.0f;
        o->oAngleVelPitch = 0;
        o->oAngleVelYaw = 0;
        o->oAngleVelRoll = 0;

        whomp_on_ground();
        if (o->oTimer > 100 || (gMarioState->action == ACT_SQUISHED && o->oTimer > 30)) {
            o->oSubAction = 10;
        }
    } else if (o->oFaceAnglePitch > 0) {
        o->oAngleVelPitch = -0x200;
        o->oFaceAnglePitch += o->oAngleVelPitch;
    } else {
        o->oAngleVelPitch = 0;
        o->oFaceAnglePitch = 0;
        o->oAction = 1;
    }
}

void whomp_die(void) {
    cur_obj_shake_screen(SHAKE_POS_SMALL);
    create_sound_spawner(SOUND_OBJ_THWOMP);
    obj_mark_for_deletion(o);
}

void (*sWhompActions[])(void) = {
    whomp_init, whomp_patrol, whomp_prepare_jump, whomp_jump, whomp_land, whomp_on_ground_general,
    whomp_turn, whomp_die,
};

void bhv_whomp_loop(void) {
    cur_obj_update_floor_and_walls();
    cur_obj_call_action_function(sWhompActions);
    cur_obj_move_standard(-20);
    if (o->oAction != 9) {
        load_object_collision_model();
    }
}
