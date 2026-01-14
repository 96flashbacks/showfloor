
/**
 * Behavior for bhvKoopa and bhvKoopaRaceEndpoint.
 * bhvKoopa includes normal, unshelled, tiny, and Koopa the Quick.
 * When the race begins, koopa the quick sets his parent to bhvKoopaRaceEndpoint
 * which assists in determining the state of the race. It is positioned at the
 * flag.
 */

/**
 * Hitbox for koopa - this is used for every form except Koopa the Quick, which
 * uses a hardcoded soft hitbox.
 */
static struct ObjectHitbox sKoopaHitbox = {
    /* interactType:      */ INTERACT_KOOPA,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 0,
    /* health:            */ 0,
    /* numLootCoins:      */ -1,
    /* radius:            */ 60,
    /* height:            */ 40,
    /* hurtboxRadius:     */ 40,
    /* hurtboxHeight:     */ 30,
};

/**
 * Attack handlers for regular sized shelled koopa.
 */
static u8 sKoopaShelledAttackHandlers[] = {
    /* ATTACK_PUNCH:                 */ ATTACK_HANDLER_KNOCKBACK,
    /* ATTACK_KICK_OR_TRIP:          */ ATTACK_HANDLER_KNOCKBACK,
    /* ATTACK_FROM_ABOVE:            */ ATTACK_HANDLER_SQUISHED,
    /* ATTACK_GROUND_POUND_OR_TWIRL: */ ATTACK_HANDLER_SQUISHED,
    /* ATTACK_FAST_ATTACK:           */ ATTACK_HANDLER_KNOCKBACK,
    /* ATTACK_FROM_BELOW:            */ ATTACK_HANDLER_KNOCKBACK,
};

/**
 * Initialization function.
 */
void bhv_koopa_init(void) {
    if ((o->oKoopaMovementType = o->oBhvParams2ndByte) == KOOPA_BP_TINY) {
        // Tiny koopa in THI
        o->oKoopaMovementType = KOOPA_BP_NORMAL;
        o->oKoopaAgility = 1.6f / 3.0f;
        o->oDrawingDistance = 1500.0f;
        cur_obj_scale(0.8f);
        o->oGravity = -6.4f / 3.0f;
    } else if (o->oKoopaMovementType >= KOOPA_BP_KOOPA_THE_QUICK_BASE) {
        // Koopa the Quick. Race index is 0 for BoB and 1 for THI
        o->oKoopaTheQuickRaceIndex = o->oKoopaMovementType - KOOPA_BP_KOOPA_THE_QUICK_BASE;
        o->oKoopaAgility = 4.0f;
        cur_obj_scale(3.0f);
    } else {
        o->oKoopaAgility = 1.0f;
    }
}

/**
 * Play the appropriate footstep sound on the two provided animation frames.
 */
static void koopa_play_footstep_sound(s8 animFrame1, s8 animFrame2) {
    s32 sound;

    if (o->header.gfx.scale[0] > 1.5f) {
        sound = SOUND_OBJ_KOOPA_THE_QUICK_WALK;
    } else {
        sound = SOUND_OBJ_KOOPA_WALK;
    }

    cur_obj_play_sound_at_anim_range(animFrame1, animFrame2, sound);
}

/**
 * If mario is close to koopa, and koopa is facing toward mario, then begin
 * running away.
 */
static s32 koopa_check_run_from_mario(void) {
    if (o->oKoopaDistanceToMario < 300.0f
        && abs_angle_diff(o->oKoopaAngleToMario, o->oMoveAngleYaw) < 0x3000) {
        o->oAction = KOOPA_SHELLED_ACT_RUN_FROM_MARIO;
        return TRUE;
    }

    return FALSE;
}

/**
 * Stay still for a while, then change target yaw by 45 degrees and begin
 * walking.
 */
static void koopa_shelled_act_stopped(void) {
    o->oForwardVel = 0.0f;
    if (cur_obj_init_anim_and_check_if_end(7)) {
        o->oAction = KOOPA_SHELLED_ACT_WALK;
        o->oKoopaTargetYaw = o->oMoveAngleYaw + 0x2000 * (s16) random_sign();
    }
}

/**
 * Begin walking, then increment sub-action.
 */
static void koopa_walk_start(void) {
    obj_forward_vel_approach(3.0f * o->oKoopaAgility, 0.3f * o->oKoopaAgility);

    if (cur_obj_init_anim_and_check_if_end(11)) {
        o->oSubAction++;
        o->oKoopaCountdown = random_linear_offset(30, 100);
    }
}

/**
 * Walk until oKoopaCountdown hits zero, then increment sub-action.
 */
static void koopa_walk(void) {
    cur_obj_init_animation_with_sound(9);
    koopa_play_footstep_sound(2, 17);

    if (o->oKoopaCountdown != 0) {
        o->oKoopaCountdown--;
    } else if (cur_obj_check_if_near_animation_end()) {
        o->oSubAction++;
    }
}

/**
 * Stop walking, then enter action KOOPA_SHELLED_ACT_STOPPED.
 */
static void koopa_walk_stop(void) {
    obj_forward_vel_approach(0.0f, 1.0f * o->oKoopaAgility);
    if (cur_obj_init_anim_and_check_if_end(10)) {
        o->oAction = KOOPA_SHELLED_ACT_STOPPED;
    }
}

/**
 * Walk for a while, then come to a stop. During this time, turn toward the
 * target yaw.
 */
static void koopa_shelled_act_walk(void) {
    if (o->oKoopaTurningAwayFromWall) {
        o->oKoopaTurningAwayFromWall = obj_resolve_collisions_and_turn(o->oKoopaTargetYaw, 0x200);
    } else {
        // If far from home, then begin turning toward home
        if (o->oDistanceToMario >= 25000.0f) {
            o->oKoopaTargetYaw = o->oAngleToMario;
        }

        o->oKoopaTurningAwayFromWall = obj_bounce_off_walls_edges_objects(&o->oKoopaTargetYaw);
        cur_obj_rotate_yaw_toward(o->oKoopaTargetYaw, 0x200);
    }

    switch (o->oSubAction) {
        case KOOPA_SHELLED_SUB_ACT_START_WALK:
            koopa_walk_start();
            break;
        case KOOPA_SHELLED_SUB_ACT_WALK:
            koopa_walk();
            break;
        case KOOPA_SHELLED_SUB_ACT_STOP_WALK:
            koopa_walk_stop();
            break;
    }

    koopa_check_run_from_mario();
}

/**
 * Run while turning away from mario. Come to a stop once mario is far enough
 * away.
 */
static void koopa_shelled_act_run_from_mario(void) {
    cur_obj_init_animation_with_sound(1);
    koopa_play_footstep_sound(0, 11);

    // If far from home, run toward it
    if (o->oDistanceToMario >= 25000.0f) {
        o->oAngleToMario += 0x8000;
        o->oDistanceToMario = 0.0f;
    }

    if (o->oTimer > 30 && o->oDistanceToMario > 800.0f) {
        if (obj_forward_vel_approach(0.0f, 1.0f)) {
            o->oAction = KOOPA_SHELLED_ACT_STOPPED;
        }
    } else {
        cur_obj_rotate_yaw_toward(o->oAngleToMario + 0x8000, 0x400);
        obj_forward_vel_approach(17.0f, 1.0f);
    }
}

/**
 * If on the ground, decelerate. Generate dust if moving fast enough.
 */
static void koopa_dive_update_speed(f32 decel) {
    if (o->oMoveFlags & OBJ_MOVE_MASK_ON_GROUND) {
        obj_forward_vel_approach(0.0f, decel);
        if (o->oForwardVel > 5.0f) {
            if (!(o->oTimer % 4)) {
                spawn_object_with_scale(o, MODEL_SMOKE, bhvWhitePuffSmoke2, 1.0f);
            }
        }
    }
}

/**
 * Slide on the ground and then come to a stop.
 */
static void koopa_shelled_act_lying(void) {
    if (o->oForwardVel != 0.0f) {
        if (o->oMoveFlags & OBJ_MOVE_HIT_WALL) {
            o->oMoveAngleYaw = cur_obj_reflect_move_angle_off_wall();
        }

        cur_obj_init_anim_extend(5);
        koopa_dive_update_speed(0.3f);
    } else if (o->oKoopaCountdown != 0) {
        o->oKoopaCountdown--;
        cur_obj_extend_animation_if_at_end();
    } else if (cur_obj_init_anim_and_check_if_end(6)) {
        o->oAction = KOOPA_SHELLED_ACT_STOPPED;
    }
}

/**
 * Attack handler for regular-sized shelled koopa.
 * Lose shell and enter lying action.
 */
void shelled_koopa_attack_handler(s32 attackType) {
    if (o->header.gfx.scale[0] > 0.8f) {
        cur_obj_play_sound_2(SOUND_OBJ_KOOPA_DAMAGE);

        //o->oKoopaMovementType = KOOPA_BP_UNSHELLED;
        //o->oAction = KOOPA_UNSHELLED_ACT_LYING;
        o->oForwardVel = 20.0f;

        // If attacked from the side, get knocked away from mario
        if (attackType != ATTACK_FROM_ABOVE && attackType != ATTACK_GROUND_POUND_OR_TWIRL) {
            o->oMoveAngleYaw = obj_angle_to_object(gMarioObject, o);
        }

        //cur_obj_set_model(MODEL_KOOPA_WITHOUT_SHELL);
        //spawn_object(o, MODEL_KOOPA_SHELL, bhvKoopaShell);

        //! Because bob-ombs/corkboxes come after koopa in processing order,
        //  they can interact with the koopa on the same frame that this
        //  happens. This causes the koopa to die immediately.
        cur_obj_become_intangible();
    } else {
        // Die if tiny koopa
        obj_die_if_health_non_positive();
    }
}

/**
 * Update function for both regular and tiny shelled koopa.
 */
static void koopa_shelled_update(void) {
    cur_obj_update_floor_and_walls();

    switch (o->oAction) {
        case KOOPA_SHELLED_ACT_STOPPED:
            koopa_shelled_act_stopped();
            koopa_check_run_from_mario();
            break;

        case KOOPA_SHELLED_ACT_WALK:
            koopa_shelled_act_walk();
            break;

        case KOOPA_SHELLED_ACT_RUN_FROM_MARIO:
            koopa_shelled_act_run_from_mario();
            break;

        case KOOPA_SHELLED_ACT_LYING:
            koopa_shelled_act_lying();
            break;
    }

    if (o->header.gfx.scale[0] > 0.8f) {
        obj_handle_attacks(&sKoopaHitbox, o->oAction, sKoopaShelledAttackHandlers);
    } else {
        // If tiny koopa, die after attacking mario.
        obj_handle_attacks(&sKoopaHitbox, KOOPA_SHELLED_ACT_DIE, sKoopaShelledAttackHandlers);
        if (o->oAction == KOOPA_SHELLED_ACT_DIE) {
            obj_die_if_health_non_positive();
        }
    }

    cur_obj_move_standard(-78);
}

void bhv_koopa_update(void) {
    // PARTIAL_UPDATE

    o->oDeathSound = SOUND_OBJ_KOOPA_FLYGUY_DEATH;
    if (obj_update_standard_actions(o->oKoopaAgility * 1.5f)) {
        o->oKoopaDistanceToMario = o->oDistanceToMario;
        o->oKoopaAngleToMario = o->oAngleToMario;
        treat_far_home_as_mario(1000.0f);
                koopa_shelled_update();
        }

        o->oAnimState = 1;

    obj_face_yaw_approach(o->oMoveAngleYaw, 0x600);
    }
    