// whomp.inc.c

// Removed 'whomp_play_sfx_from_pound_animation'

static void whomp_init(void) { // wallman_init (modified)
    cur_obj_init_animation_with_accel_and_sound(0, 1.0f);
    cur_obj_set_pos_to_home();

    if (o->oDistanceToMario < 500.0f) {
        o->oAction = WHOMP_ACT_PATROL;
    }

    // Removed 'whomp_play_sfx_from_pound_animation'
}

static void whomp_turn(void) { // wallman_walkstart (modified)
    if (o->oSubAction == 0) {
        o->oForwardVel = 0.0f;
        cur_obj_init_animation_with_accel_and_sound(0, 1.0f);
        if (o->oTimer > 31) {
            o->oSubAction++;
        } else {
            o->oMoveAngleYaw += 0x400;
        }
    } else {
        o->oForwardVel = 9.0f; // Matches Game Zero (03:55) better
        if (o->oTimer > 42) {
            o->oAction = WHOMP_ACT_PATROL;
        }
    }

    // Removed 'whomp_play_sfx_from_pound_animation'
}

static void whomp_patrol(void) { // wallman_walk (modified)
    s16 marioAngle = abs_angle_diff(o->oAngleToMario, o->oMoveAngleYaw);
    f32 distWalked = cur_obj_lateral_dist_to_home();

    // No 'patrolDist' variable, the limit is consistent

    cur_obj_init_animation_with_accel_and_sound(0, 1.0f);
    
    // The movement appears to be different in Game Zero (03:55)
    if (cur_obj_check_anim_frame_in_range(0, 23)) {
        o->oForwardVel = 9.0f;
    } else {
        o->oForwardVel = 0;
    }

    if (distWalked > 500.0f) { // different from both final limits (200 and 700)
        o->oAction = WHOMP_ACT_TURN;
    } else if (marioAngle < 0x2000) {
        if (o->oDistanceToMario < 1500.0f) {
            o->oForwardVel = 9.0f;
            cur_obj_init_animation_with_accel_and_sound(0, 3.0f);
        }
        if (o->oDistanceToMario < 300.0f) {
            o->oAction = WHOMP_ACT_PREPARE_JUMP;
        }
    }
}

// No 'wallman_search' (king_whomp_chase)

static void whomp_prepare_jump(void) { // wallman_attack
    o->oForwardVel = 0.0f;
    cur_obj_init_animation_with_accel_and_sound(1, 1.0f);
    if (cur_obj_check_if_near_animation_end()) {
        o->oAction = WHOMP_ACT_JUMP;
    }
}

static void whomp_jump(void) { // wallman_down (modified)
    if (o->oTimer == 0) {
        o->oVelY = 40.0f;
    }

    if (o->oTimer < 8) {
        // Commented out code from 'pathwallman.p' that makes the Whomp shake during the jump
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

static void whomp_land(void) { // wallman_down_end (modified)
    if (o->oSubAction == 0 && o->oMoveFlags & OBJ_MOVE_LANDED) {
        cur_obj_play_sound_2(SOUND_OBJ_THWOMP); // Reused thwomp sound
        cur_obj_shake_screen(SHAKE_POS_SMALL);
        o->oVelY = 0.0f;
        o->oSubAction++;
    }

    if (o->oMoveFlags & OBJ_MOVE_ON_GROUND) {
        o->oAction = WHOMP_ACT_ON_GROUND_GENERAL;
    }
}

// No 'wallman_boss_sleep' (king_whomp_on_ground)

static void whomp_on_ground(void) { // wallman_mini_sleep (modified)
    if (o->oSubAction == 0) {
        if (gMarioObject->platform == o) {
            if (cur_obj_is_mario_ground_pounding_platform()) {
                o->oNumLootCoins = 5;
                obj_spawn_loot_yellow_coins(o, 5, 20.0f);
                o->oAction = WHOMP_ACT_DIE;
            } // Removed coin easter egg if Mario jumps on the Whomp's back
        }
    } else if (!cur_obj_is_mario_on_platform()) {
        o->oSubAction = 0;
    }
}

static void whomp_on_ground_general(void) { // wallman_sleep (modified)
    if (o->oSubAction != 10) {
        o->oForwardVel = 0.0f;
        o->oAngleVelPitch = 0;
        o->oAngleVelYaw = 0;
        o->oAngleVelRoll = 0;

        // No King Whomp handling
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
        o->oAction = WHOMP_ACT_PATROL;
    }
}

static void whomp_die(void) { // wallman_burn (modified)
    // No King Whomp handling
    
    // Removed 'spawn_mist_particles_variable' and 'spawn_triangle_break_particles'
    cur_obj_shake_screen(SHAKE_POS_SMALL);
    create_sound_spawner(SOUND_OBJ_THWOMP);
    obj_mark_for_deletion(o);
}

// No 'wallman_dead_stop' (king_whomp_stop_music)

static void (*sWhompActions[])(void) = { // wallman_modejmp (modified)
    whomp_init, 
    whomp_patrol, 
    whomp_prepare_jump, 
    whomp_jump, 
    whomp_land, 
    whomp_on_ground_general,
    whomp_turn, 
    whomp_die,
};

void bhv_whomp_loop(void) { // s_wallman (modified)
    cur_obj_update_floor_and_walls();
    cur_obj_call_action_function(sWhompActions);
    cur_obj_move_standard(-20);

    // No 'Action != 9' check since action 9 (king_whomp_stop_music) didn't exist yet
    // No cur_obj_hide_if_mario_far_away_y function, as seen in Game Zero (03:55)
    load_object_collision_model();
}
