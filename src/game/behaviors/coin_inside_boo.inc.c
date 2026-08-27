// coin_inside_boo.inc.c

// The coin inside Boos seemingly had its own separate file based on 'pathobake.p'

void coin_inside_boo_bound(void) {
    cur_obj_update_floor_and_walls();
    cur_obj_if_hit_wall_bounce_away();

    // 'pathobakecoin.p' has the coin bounce sound implemented which may indicate it was already in,
    // but I'll keep it removed to be safe
    /*if (o->oMoveFlags & OBJ_MOVE_LANDED) { // OBJ_MOVE_BOUNCE in the final game
        cur_obj_play_sound_2(SOUND_GENERAL_COIN_DROP);
    }*/
    
    if (o->oTimer > 90 || (o->oMoveFlags & OBJ_MOVE_LANDED)) {
        // Hitbox is set here in the final game
        cur_obj_become_tangible();
        cur_obj_set_behavior(bhvYellowCoin);
    }

    cur_obj_move_standard(-78); // -78 instead of -30
    // Final has the 'bhv_coin_sparkles_init()' function here

    // No blue coin handling or 'cur_obj_wait_then_blink', the coin doesn't despawn from being idle too long
}

void coin_inside_boo_take(void) {
    s16 playerY;
    f32	playerspeed;
    struct Object *parent = o->parentObj;	/* coin */
    
    obj_copy_pos(o, parent);
    
    // No blue coin handling, they didn't exist when this backup was made

    // Checks for a "obake_flag" that is not used in the final game
    if (parent->obake_flag != 0) {
        parent->obake_flag = 0;
        o->oAction = 1; // mode_obakecoin_bound
        playerY = gMarioObject->oMoveAngleYaw;
        playerspeed = 3.0f;
        o->oVelX = playerspeed * sins(playerY);
        o->oVelZ = playerspeed * coss(playerY);
        o->oVelY = 35.0f;

    }
    
    cur_obj_become_intangible(); // This was moved to before the if statement later
}

// Has it's own hitbox rather than reusing the regular coin hitbox (sYellowCoinHitbox)
struct ObjectHitbox sCoinInsideBooHitbox = { // obakecoin_hit
    /* interactType:      */ INTERACT_COIN,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 0, // 0 instead of 1 here indicates the damageOrCoinValue multiplier in 'interact_coin' didn't exist
    /* health:            */ 0,
    /* numLootCoins:      */ 0,
    /* radius:            */ 32, // 32 instead of 100
    /* height:            */ 64,
    /* hurtboxRadius:     */ 0,
    /* hurtboxHeight:     */ 0,
};

void (*sCoinInsideBooActions[])(void) = { // obakecoin_modejmp
    coin_inside_boo_take,
    coin_inside_boo_bound,
};

void bhv_coin_inside_boo_loop(void) { // s_obake_coin
    // Hitbox is set here instead of 'coin_inside_boo_bound'
    obj_set_hitbox(o, &sCoinInsideBooHitbox);
    
    // Doesn't use 'bhv_coin_sparkles_init()', manually handles the sparkle spawning instead
    if (obj_check_if_collided_with_object(o, gMarioObject)) {
        spawn_object(o, MODEL_SPARKLES, bhvCoinSparkles);
        obj_mark_for_deletion(o);
    }

    cur_obj_call_action_function(sCoinInsideBooActions);
}
