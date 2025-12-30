/**
 * Behavior for bhvPiranhaPlant.
 * This controls Piranha Plants, which alternate between sleeping, attacking,
 * and dying, primarily depending on Mario's proximity and interaction state.
 */

/**
 * Reset the Piranha Plant back to a sleeping animation, no matter what state
 * it was in previously, and make it intangible. If Mario is close, transition
 * directly to the sleeping state.
 */
void piranha_plant_act_idle(void) {
    cur_obj_become_intangible();
    cur_obj_init_animation_with_sound(8);

    if (o->oDistanceToMario < 1200.0f) {
        o->oAction = PIRANHA_PLANT_ACT_SLEEPING;
    }
}

/**
 * Check if the player has interacted with the Piranha Plant. If the Piranha
 * Plant was attacked, move it to the dying state. If the player interacted
 * with it through some other means (e.g. by running into it), move it to the
 * woken up state.
 *
 * @return 1 if the player interacted with the Piranha Plant, 0 otherwise
 */
s32 piranha_plant_check_interactions(void) {
    s32 i;
    s32 interacted = 1;
    if (o->oInteractStatus & INT_STATUS_INTERACTED) {
        func_80321080(50);
        if (o->oInteractStatus & INT_STATUS_WAS_ATTACKED) {
            cur_obj_play_sound_2(SOUND_OBJ2_PIRANHA_PLANT_DYING);

            // Spawn 20 intangible purple particles that quickly dissipate.
            for (i = 0; i < 20; i++) {
                spawn_object(o, MODEL_BUBBLE, bhvPurpleParticle);
            }
            o->oAction = PIRANHA_PLANT_ACT_ATTACKED;
        } else {
            o->oAction = PIRANHA_PLANT_ACT_WOKEN_UP;
        }
        o->oInteractStatus = 0;
    } else {
        interacted = 0;
    }
    return interacted;
}

/**
 * Make the Piranha Plant sleep. If Mario moves too quickly, move the Piranha
 * Plant to the woken up state. Otherwise, play the lullaby if Mario is close
 * enough. If the player interacts with the Piranha Plant, it will act according
 * to piranha_plant_check_interactions().
 */
void piranha_plant_act_sleeping(void) {
    cur_obj_become_tangible();
    o->oInteractType = INTERACT_BOUNCE_TOP;

    cur_obj_init_animation_with_sound(8);

    cur_obj_set_hitbox_radius_and_height(100.0f, 200.0f);
    cur_obj_set_hurtbox_radius_and_height(100.0f, 100.0f);

    if (o->oDistanceToMario < 400.0f) {
        if (mario_moving_fast_enough_to_make_piranha_plant_bite()) {
            o->oAction = PIRANHA_PLANT_ACT_WOKEN_UP;
        }
    }

    piranha_plant_check_interactions();
}

/**
 * Make the Piranha Plant wake up and stop the lullaby. After a few frames, move
 * to the biting state.
 */
void piranha_plant_act_woken_up(void) {
    if (o->oTimer == 0)
        func_80321080(50);

    if (piranha_plant_check_interactions() == 0)
        if (o->oTimer > 10)
            o->oAction = PIRANHA_PLANT_ACT_BITING;
}

/**
 * Make the Piranha Plant play a falling-over animation and move to the dying
 * state.
 */
void piranha_plant_attacked(void) {
    cur_obj_become_intangible();
    cur_obj_init_animation_with_sound(2);
    o->oInteractStatus = 0;
    if (cur_obj_check_if_near_animation_end())
        o->oAction = PIRANHA_PLANT_ACT_SHRINK_AND_DIE;
}

/**
 * Make the Piranha Plant play a sound of defeat, shrink, and then spawn a
 * blue coin. Then, move it to the waiting state.
 */
void piranha_plant_act_shrink_and_die(void) {
    if (o->oTimer == 0) {
        cur_obj_play_sound_2(SOUND_OBJ_DYING_ENEMY1);
        o->oPiranhaPlantScale = 1.0f;
    }

    /**
     * Note that this if-statement occurs unconditionally after the above if-
     * statement. Since the Piranha Plant's scale is 1.0f by default, perhaps
     * this was intentional. However, it is equally plausible that the
     * programmers meant to type `else if`.
     */
    if (o->oPiranhaPlantScale > 0.0f) {
        // Shrink by 0.04 per frame.
        o->oPiranhaPlantScale = o->oPiranhaPlantScale - 0.04;
    } else {
        o->oPiranhaPlantScale = 0.0f;
        obj_spawn_loot_yellow_coins(o, 1, 20.0f);
        o->oAction = PIRANHA_PLANT_ACT_WAIT_TO_RESPAWN;
    }

    cur_obj_scale(o->oPiranhaPlantScale);
}

/**
 * Wait for Mario to move far away, then respawn the Piranha Plant.
 */
void piranha_plant_act_wait_to_respawn(void) {
    if (o->oDistanceToMario > 1200.0f) {
        o->oAction = PIRANHA_PLANT_ACT_RESPAWN;
    }
}

/**
 * Set the Piranha Plant to the sleeping animation and unshrink it. When fully-
 * grown, set it to the idle state.
 */
void piranha_plant_act_respawn(void) {
    cur_obj_init_animation_with_sound(8);
    if (o->oTimer == 0) {
        o->oPiranhaPlantScale = 0.3f;
    }

    /**
     * This state only occurs after PIRANHA_PLANT_ACT_WAIT_TO_RESPAWN, which
     * in turn only occurs after PIRANHA_PLANT_ACT_SHRINK_AND_DIE. The latter
     * sets the Piranha Plant's scale to 0, therefore the Piranha Plant will
     * grow from the ground unconditionally when in this state.
     */
    if (o->oPiranhaPlantScale < 1.0) {
        // Grow by 0.02 per frame.
        o->oPiranhaPlantScale += 0.02;
    } else {
        o->oPiranhaPlantScale = 1.0f;
        o->oAction = PIRANHA_PLANT_ACT_IDLE;
    }
    cur_obj_scale(o->oPiranhaPlantScale);
}

/**
 * The frames of the Piranha Plant's biting animation on which to play a bite
 * sound.
 */
static s8 sPiranhaPlantBiteSoundFrames[] = { 12, 28, 50, 64, -1 };

/**
 * Make the Piranha Plant bite in the direction of the player. If the player
 * moves far away, move it to the stopped biting state. If the player is wearing
 * the Metal Cap and touches the Piranha Plant while it is attacking, the
 * Piranha Plant will move to the attacked state.
 */
void piranha_plant_act_biting(void) {
    s32 frame = o->header.gfx.animInfo.animFrame;

    cur_obj_become_tangible();

    o->oInteractType = INTERACT_DAMAGE;

    cur_obj_init_animation_with_sound(0);

    cur_obj_set_hitbox_radius_and_height(100.0f, 100.0f);
    cur_obj_set_hurtbox_radius_and_height(100.0f, 100.0f);

    // Play a bite sound effect on certain frames.
    if (is_item_in_array(frame, sPiranhaPlantBiteSoundFrames)) {
        cur_obj_play_sound_2(SOUND_OBJ2_PIRANHA_PLANT_BITE);
    }

    // Move to face the player.
    o->oMoveAngleYaw = approach_s16_symmetric(o->oMoveAngleYaw, o->oAngleToMario, 0x400);

    if (o->oDistanceToMario > 615.0f) {
        o->header.gfx.animInfo.animFrame = 0;
        o->oAction = PIRANHA_PLANT_ACT_STOPPED_BITING;
    }
}

/**
 * Check whether the player is moving fast enough to cause the Piranha Plant to
 * start biting.
 *
 * This is called from both the "stopped biting" state and the "sleeping" state.
 */
s32 mario_moving_fast_enough_to_make_piranha_plant_bite(void) {
    if (gMarioStates->forwardVel > 10.0f)
        return 1;
    return 0;
}

/**
 * Make the Piranha Plant quickly nod to indicate that it has stopped biting.
 * If the player has drawn closer during this short animation, make the Piranha
 * Plant start biting again. Otherwise, make it go back to sleep.
 */
void piranha_plant_act_stopped_biting(void) {
    cur_obj_become_intangible();
    cur_obj_init_animation(0);

    if (cur_obj_check_if_near_animation_end())
        o->oAction = PIRANHA_PLANT_ACT_SLEEPING;

    /**
     * Note that this state only occurs initially when the player goes further
     * than 500.0f units from the Piranha Plant while it is biting. This if-
     * statement activates only when the player has drawn within 400.0f units
     * of the Piranha Plant during the short time the Piranha Plant's nod
     * animation plays.
     */
    if (o->oDistanceToMario < 400.0f)
        if (mario_moving_fast_enough_to_make_piranha_plant_bite())
            o->oAction = PIRANHA_PLANT_ACT_BITING;
}

/**
 * Table of functions corresponding to the actions the Piranha Plant can take.
 */
void (*TablePiranhaPlantActions[])(void) = {
    piranha_plant_act_idle,            // PIRANHA_PLANT_ACT_IDLE,
    piranha_plant_act_sleeping,        // PIRANHA_PLANT_ACT_SLEEPING,
    piranha_plant_act_biting,          // PIRANHA_PLANT_ACT_BITING,
    piranha_plant_act_woken_up,        // PIRANHA_PLANT_ACT_WOKEN_UP,
    piranha_plant_act_stopped_biting,  // PIRANHA_PLANT_ACT_STOPPED_BITING,
    piranha_plant_attacked,            // PIRANHA_PLANT_ATTACKED,
    piranha_plant_act_shrink_and_die,  // PIRANHA_PLANT_ACT_SHRINK_AND_DIE,
    piranha_plant_act_wait_to_respawn, // PIRANHA_PLANT_ACT_WAIT_TO_RESPAWN,
    piranha_plant_act_respawn          // PIRANHA_PLANT_ACT_RESPAWN
};

/**
 * Main loop for bhvPiranhaPlant.
 */
void bhv_piranha_plant_loop(void) {
    cur_obj_call_action_function(TablePiranhaPlantActions);

    // In WF, hide all Piranha Plants once high enough up.
    if (gCurrLevelNum == LEVEL_WF) {
        if (gMarioObject->oPosY > 3400.0f)
            cur_obj_hide();
        else
            cur_obj_unhide();
    }
    o->oInteractStatus = 0;
}
