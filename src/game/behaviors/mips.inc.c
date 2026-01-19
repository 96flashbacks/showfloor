
/**
 * Behavior for MIPS (everyone's favorite yellow rabbit).
 */

/**
 * Initializes MIPS' physics parameters and checks if he should be active,
 * hiding him if necessary.
 */
void bhv_mips_init(void) {
    // Retrieve star flags for Castle Secret Stars on current save file.
        o->oBhvParams2ndByte = MIPS_BP_15_STARS;
        o->oMipsForwardVelocity = 40.0f;

    o->oInteractionSubtype = INT_SUBTYPE_HOLDABLE_NPC;

#ifndef VERSION_JP
    o->oGravity = 15.0f;
#else
    o->oGravity = 2.5f;
#endif
    o->oFriction = 0.89f;
    o->oBuoyancy = 1.2f;

    cur_obj_init_animation(3);
}

/**
 * Helper function that finds the waypoint that is both within 800 units of MIPS
 * and furthest from Mario's current location.
 */


/**
 * Wait until Mario comes close, then resume following our path.
 */
void bhv_mips_act_wait_for_nearby_mario(void) {
    UNUSED s16 collisionFlags = 0;

    o->oForwardVel = 0.0f;
    collisionFlags = object_step();

    // If Mario is within 500 units...

}
/**
 * Continue to follow our path around the basement area.
 */

/**
 * Seems to wait until the current animation is done, then go idle.
 */
void bhv_mips_act_wait_for_animation_done(void) {
    if (cur_obj_check_if_near_animation_end() == TRUE) {
        cur_obj_init_animation(3);
        o->oAction = MIPS_ACT_IDLE;
    }
}

/**
 * Handles MIPS falling down after being thrown.
 */
void bhv_mips_act_fall_down(void) {
    s16 collisionFlags = 0;

    collisionFlags = object_step();
    o->header.gfx.animInfo.animFrame = 0;

    if ((collisionFlags & OBJ_COL_FLAG_GROUNDED) == OBJ_COL_FLAG_GROUNDED) {
        o->oAction = MIPS_ACT_WAIT_FOR_ANIMATION_DONE;

        o->oFlags |= OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW;
        o->oMoveAngleYaw = o->oFaceAngleYaw;

        if (collisionFlags & OBJ_COL_FLAG_UNDERWATER) {
            spawn_object(o, MODEL_NONE, bhvShallowWaterSplash);
        }
    }
}

/**
 * Idle loop, after you catch MIPS and put him down.
 */
void bhv_mips_act_idle(void) {
    UNUSED s16 collisionFlags = 0;

    o->oForwardVel = 0.0f;
    collisionFlags = object_step();

    // Spawn a star if he was just picked up for the first time.
    if (o->oMipsStarStatus == MIPS_STAR_STATUS_SHOULD_SPAWN_STAR) {
        bhv_spawn_star_no_level_exit(STAR_INDEX_ACT_4 + o->oBhvParams2ndByte);
        o->oMipsStarStatus = MIPS_STAR_STATUS_ALREADY_SPAWNED_STAR;
    }
}

/**
 * Handles all the actions MIPS does when he is not held.
 */
void bhv_mips_free(void) {
    switch (o->oAction) {
        case MIPS_ACT_WAIT_FOR_NEARBY_MARIO:
            bhv_mips_act_wait_for_nearby_mario();
            break;

        case MIPS_ACT_WAIT_FOR_ANIMATION_DONE:
            bhv_mips_act_wait_for_animation_done();
            break;

        case MIPS_ACT_FALL_DOWN:
            bhv_mips_act_fall_down();
            break;

        case MIPS_ACT_IDLE:
            bhv_mips_act_idle();
            break;
    }
}

/**
 * Handles MIPS being held by Mario.
 */
void bhv_mips_held(void) {
    s16 dialogID;

    o->header.gfx.node.flags |= GRAPH_RENDER_INVISIBLE;
    cur_obj_init_animation(4); // Held animation.
    cur_obj_set_pos_relative(gMarioObject, 0, 60.0f, 100.0f);
    cur_obj_become_intangible();

    
        
    
}

/**
 * Handles MIPS being dropped by Mario.
 */
void bhv_mips_dropped(void) {
    cur_obj_get_dropped();
    o->header.gfx.node.flags &= ~GRAPH_RENDER_INVISIBLE;
    cur_obj_init_animation(3);
    o->oHeldState = HELD_FREE;
    cur_obj_become_tangible();
    o->oForwardVel = 3.0f;
    o->oAction = MIPS_ACT_IDLE;
}

/**
 * Handles MIPS being thrown by Mario.
 */
void bhv_mips_thrown(void) {
    cur_obj_enable_rendering_2();
    o->header.gfx.node.flags &= ~GRAPH_RENDER_INVISIBLE;
    o->oHeldState = HELD_FREE;
    o->oFlags &= ~OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW;
    cur_obj_init_animation(2);
    cur_obj_become_tangible();
    o->oForwardVel = 25.0f;
    o->oVelY = 20.0f;
    o->oAction = MIPS_ACT_FALL_DOWN;
}

/**
 * MIPS' main loop.
 */
void bhv_mips_loop(void) {
    // Determine what to do based on MIPS' held status.
    switch (o->oHeldState) {
        case HELD_FREE:
            bhv_mips_free();
            break;

        case HELD_HELD:
            bhv_mips_held();
            break;

        case HELD_THROWN:
            bhv_mips_thrown();
            break;

        case HELD_DROPPED:
            bhv_mips_dropped();
            break;
    }
}
