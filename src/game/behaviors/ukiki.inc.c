
/**
 * @file Contains behavior for the ukiki objects.
 *
 * Cap ukiki is the ukiki that steals Mario's cap.
 * Cage ukiki is the ukiki that triggers the cage star.
 */

void ukiki_held_loop(void) {
    o->header.gfx.node.flags |= GRAPH_RENDER_INVISIBLE;
    cur_obj_init_animation(2); // broken
    cur_obj_set_pos_relative(gMarioObject, 0, 60.0f, 100.0f);
    cur_obj_become_intangible();
}



/**
 * Unused copy of geo_update_projectile_pos_from_parent. Perhaps a copy paste mistake.
 */
Gfx *geo_update_projectile_pos_from_parent_copy(s32 run,UNUSED struct GraphNode *node, Mat4 mtx) {
    if (run == TRUE) {
        Mat4 mtx2;
        struct Object *obj = (struct Object *) gCurGraphNodeObject; // TODO: change global type to Object pointer

        if (obj->prevObj != NULL) {
            create_transformation_from_matrices(mtx2, mtx, *gCurGraphNodeCamera->matrixPtr);
            obj_update_pos_from_parent_transformation(mtx2, obj->prevObj);
            obj_set_gfx_pos_from_pos(obj->prevObj);
        }
    }

    return NULL;
}

/**
 * Chooses random idle taunts and loops them a random number of times.
 */
void idle_ukiki_taunt(void) {
    o->oForwardVel = 0.0f;

    if (o->oSubAction == UKIKI_SUB_ACT_TAUNT_NONE) {
        // Subaction is between 1 and 4.
        o->oSubAction = (s32)(random_float() * 4.0f + 1.0f);

        // Counter keeps track of the iterations done, while ToBeDone
        // is the count of iterations to be done (between 2 and 5).
        o->oUkikiTauntCounter = 0;
        o->oUkikiTauntsToBeDone = (s16)(random_float() * 4.0f + 2.0f);
    }

    // Switch goes from 1-4.
    switch (o->oSubAction) {
        case UKIKI_SUB_ACT_TAUNT_ITCH:
            cur_obj_init_animation_with_sound(UKIKI_ANIM_ITCH);

            if (cur_obj_check_if_near_animation_end()) {
                o->oSubAction = UKIKI_SUB_ACT_TAUNT_NONE;
            }
            break;

        case UKIKI_SUB_ACT_TAUNT_SCREECH:
            cur_obj_init_animation_with_sound(UKIKI_ANIM_SCREECH);

            if (cur_obj_check_if_near_animation_end()) {
                o->oUkikiTauntCounter++;
            }

            if (o->oUkikiTauntCounter >= o->oUkikiTauntsToBeDone * 2) {
                o->oSubAction = UKIKI_SUB_ACT_TAUNT_NONE;
            }
            break;

        case UKIKI_SUB_ACT_TAUNT_JUMP_CLAP:
            cur_obj_init_animation_with_sound(UKIKI_ANIM_JUMP_CLAP);

            if (cur_obj_check_if_near_animation_end()) {
                o->oUkikiTauntCounter++;
            }

            if (o->oUkikiTauntCounter >= o->oUkikiTauntsToBeDone) {
                o->oSubAction = UKIKI_SUB_ACT_TAUNT_NONE;
            }
            break;

        case UKIKI_SUB_ACT_TAUNT_HANDSTAND:
            cur_obj_init_animation_with_sound(UKIKI_ANIM_HANDSTAND);

            if (cur_obj_check_if_near_animation_end()) {
                o->oSubAction = UKIKI_SUB_ACT_TAUNT_NONE;
            }
            break;
    }
}

/**
 * Ukiki's general idle action. This is for when ukiki is doing nothing else and
 * standing around.
 */
void ukiki_act_idle(void) {
    idle_ukiki_taunt();

    
        if (o->oDistanceToMario > 700.0f && o->oDistanceToMario < 1000.0f) {
            o->oAction = UKIKI_ACT_RUN;
        } else if (o->oDistanceToMario <= 700.0f && o->oDistanceToMario > 200.0f) {
            if (abs_angle_diff(o->oAngleToMario, o->oMoveAngleYaw) > 0x1000) {
                o->oAction = UKIKI_ACT_TURN_TO_MARIO;
            }
        }


    // Jump away from Mario after stealing his cap.
    if (o->oUkikiTextState == UKIKI_TEXT_STOLE_CAP) {
        o->oMoveAngleYaw = gMarioObject->oMoveAngleYaw + 0x8000;

        if (check_if_moving_over_floor(50.0f, 150.0f)) {
            o->oAction = UKIKI_ACT_JUMP;
        } else {
            o->oMoveAngleYaw = gMarioObject->oMoveAngleYaw + 0x4000;

            if (check_if_moving_over_floor(50.0f, 150.0f)) {
                o->oAction = UKIKI_ACT_JUMP;
            } else {
                o->oMoveAngleYaw = gMarioObject->oMoveAngleYaw - 0x4000;
                if (check_if_moving_over_floor(50.0f, 150.0f)) {
                    o->oAction = UKIKI_ACT_JUMP;
                }
            }
        }

        o->oUkikiTextState = UKIKI_TEXT_HAS_CAP;
    }

    if (o->oBhvParams2ndByte == UKIKI_BP_CAP) {
        if (o->oPosY < -1550.0f) {
            o->oAction = UKIKI_ACT_RETURN_HOME;
        }
    }
}

/**
 * Ukiki attempts to run home, which is often impossible depending on terrain.
 * Only used for the cap ukiki.
 */
void ukiki_act_return_home(void) {
    UNUSED u8 filler[4];

    cur_obj_init_animation_with_sound(UKIKI_ANIM_RUN);
    o->oMoveAngleYaw = cur_obj_angle_to_home();
    o->oForwardVel = 10.0f;

    // If ukiki somehow walked home, go back to the idle action.
    if (o->oPosY > -1550.0f) {
        o->oAction = UKIKI_ACT_IDLE;
    }
}

/**
 * Ukiki has gone somewhere he shouldn't, so wait until Mario
 * leaves then reset position to his home.
 */
void ukiki_act_wait_to_respawn(void) {
    idle_ukiki_taunt();

    if (cur_obj_mario_far_away()) {
        cur_obj_set_pos_to_home_and_stop();
        o->oAction = UKIKI_ACT_IDLE;
    }
}

/**
 * A seemgingly stubbed unused action.
 *
 * Perhaps an early attempt at the UKIKI_SUB_ACT_CAGE_WAIT_FOR_MARIO
 * part of the ukiki_act_go_to_cage action.
 */
void ukiki_act_unused_turn(void) {
    idle_ukiki_taunt();

    if (o->oSubAction == UKIKI_SUB_ACT_TAUNT_JUMP_CLAP) {
        cur_obj_rotate_yaw_toward(o->oAngleToMario, 0x400);
    }
}

/**
 * Turns ukiki to face towards Mario while moving with slow forward velocity.
 */
void ukiki_act_turn_to_mario(void) {
    s32 facingMario;

    // Initialize the action with a random fVel from 2-5.
    if (o->oTimer == 0) {
        o->oForwardVel = random_float() * 3.0f + 2.0f;
    }

    cur_obj_init_animation_with_sound(UKIKI_ANIM_TURN);

    facingMario = cur_obj_rotate_yaw_toward(o->oAngleToMario, 0x800);

    if (facingMario) {
        o->oAction = UKIKI_ACT_IDLE;
    }

    
        if (o->oDistanceToMario > 500.0f) {
            o->oAction = UKIKI_ACT_RUN;

}
}

/**
 * Ukiki either runs away away from Mario or towards him if stealing Mario's cap.
 */
void ukiki_act_run(void) {
    s32 fleeMario = TRUE;
    s16 goalYaw = o->oAngleToMario + 0x8000;



    if (o->oTimer == 0) {
        o->oUkikiChaseFleeRange = random_float() * 100.0f + 350.0f;
    }

    cur_obj_init_animation_with_sound(UKIKI_ANIM_RUN);
    cur_obj_rotate_yaw_toward(goalYaw, 0x800);

    //! @bug (Ukikispeedia) This function sets forward speed to 0.9 * Mario's
    //! forward speed, which means ukiki can move at hyperspeed rates.
    cur_obj_set_vel_from_mario_vel(20.0f, 0.9f);

    if (fleeMario) {
        if (o->oDistanceToMario > o->oUkikiChaseFleeRange) {
            o->oAction = UKIKI_ACT_TURN_TO_MARIO;
        }
    } else if (o->oDistanceToMario < o->oUkikiChaseFleeRange) {
        o->oAction = UKIKI_ACT_TURN_TO_MARIO;
    }

    if (fleeMario && o->oDistanceToMario < 200.0f) {
        if ((o->oMoveFlags & OBJ_MOVE_HIT_WALL)
            && is_mario_moving_fast_or_in_air(10)) {
            o->oAction = UKIKI_ACT_JUMP;
            o->oMoveAngleYaw = o->oWallAngle;
        } else if ((o->oMoveFlags & OBJ_MOVE_HIT_EDGE)
                   && is_mario_moving_fast_or_in_air(10)) {
            o->oAction = UKIKI_ACT_JUMP;
            o->oMoveAngleYaw += 0x8000;
        }
    }
}

/**
 * Jump for a distance, typically used to jump
 * over Mario when after reaching a wall or edge.
 */
void ukiki_act_jump(void) {
    o->oForwardVel = 10.0f;
    cur_obj_become_intangible();

    if (o->oSubAction == 0) {
        if (o->oTimer == 0) {
            cur_obj_set_y_vel_and_animation(random_float() * 10.0f + 45.0f, UKIKI_ANIM_JUMP);
        } else if (o->oMoveFlags & (OBJ_MOVE_MASK_ON_GROUND | OBJ_MOVE_AT_WATER_SURFACE
                                    | OBJ_MOVE_UNDERWATER_ON_GROUND)) {
            o->oSubAction++;
            o->oVelY = 0.0f;
        }
    } else {
        o->oForwardVel = 0.0f;
        cur_obj_init_animation_with_sound(UKIKI_ANIM_LAND);
        cur_obj_become_tangible();

        if (cur_obj_check_if_near_animation_end()) {
            o->oAction = UKIKI_ACT_RUN;
        }
    }
}

/**
 * Waypoints that lead from the top of the mountain to the cage.
 */


/**
 * Travel to the cage, wait for Mario, jump to it, and ride it to
 * our death. Ukiki is a tad suicidal.
 */


/**
 * A struct of Ukiki's sounds based on his current
 * SoundState number.
 */
struct SoundState sUkikiSoundStates[] = {
    {1, 1, 10, SOUND_OBJ_UKIKI_STEP_DEFAULT},
    {0, 0, 0,  NO_SOUND},
    {0, 0, 0,  NO_SOUND},
    {0, 0, 0,  NO_SOUND},
    {1, 0, -1, SOUND_OBJ_UKIKI_CHATTER_SHORT},
    {1, 0, -1, SOUND_OBJ_UKIKI_CHATTER_LONG},
    {0, 0, 0,  NO_SOUND},
    {0, 0, 0,  NO_SOUND},
    {1, 0, -1, SOUND_OBJ_UKIKI_CHATTER_LONG},
    {1, 0, -1, SOUND_OBJ_UKIKI_STEP_LEAVES},
    {1, 0, -1, SOUND_OBJ_UKIKI_CHATTER_IDLE},
    {0, 0, 0,  NO_SOUND},
    {0, 0, 0,  NO_SOUND},
};

/**
 * An array of each of Ukiki's actions. A function is called
 * every frame.
 */
void (*sUkikiActions[])(void) = {
    ukiki_act_idle,
    ukiki_act_run,
    ukiki_act_turn_to_mario,
    ukiki_act_jump,
    ukiki_act_wait_to_respawn,
    ukiki_act_unused_turn,
    ukiki_act_return_home,
};

/**
 * Called via the main behavior function when Ukiki is either nothing
 * being held, dropped, or thrown.
 */
void ukiki_free_loop(void) {
    s32 steepSlopeAngleDegrees;

    cur_obj_update_floor_and_walls();
    cur_obj_call_action_function(sUkikiActions);

    if (o->oAction == UKIKI_ACT_RETURN_HOME) {
        steepSlopeAngleDegrees = -88;
    } else {
        steepSlopeAngleDegrees = -20;
    }

    cur_obj_move_standard(steepSlopeAngleDegrees);

    if (!(o->oMoveFlags & OBJ_MOVE_MASK_IN_WATER)) {
        exec_anim_sound_state(sUkikiSoundStates);
    }
}

/**
 * Unused function for timing ukiki's blinking.
 * Image still present in Ukiki's actor graphics.
 *
 * Possibly unused so AnimState could be used for wearing a cap?
 */
UNUSED static void ukiki_blink_timer(void) {
    if (gGlobalTimer % 50 < 7) {
        o->oAnimState = UKIKI_ANIM_STATE_EYE_CLOSED;
    } else {
        o->oAnimState = UKIKI_ANIM_STATE_DEFAULT;
    }
}

/**

 * The main behavior function for ukiki. Chooses which behavior to use
 * dependent on the held state and whick ukiki it is (cage or cap).
 */
void bhv_ukiki_loop(void) {
    switch (o->oHeldState) {
        case HELD_FREE:
            //! @bug (PARTIAL_UPDATE)
            o->oUkikiTextboxTimer = 0;
            ukiki_free_loop();
            break;

        case HELD_HELD:
            cur_obj_unrender_set_action_and_anim(UKIKI_ANIM_HELD, 0);
            obj_copy_pos(o, gMarioObject);
                ukiki_held_loop();

        case HELD_THROWN:
        case HELD_DROPPED:
            cur_obj_get_dropped();
            break;
    }


        o->oAnimState = UKIKI_ANIM_STATE_DEFAULT;
    

    o->oInteractStatus = 0;

    print_debug_bottom_up("mode   %d\n", o->oAction);
    print_debug_bottom_up("action %d\n", o->oHeldState);
}
