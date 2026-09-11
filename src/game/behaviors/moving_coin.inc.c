// moving_coin.inc.c

static struct ObjectHitbox sMovingYellowCoinHitbox = { // coin_hit (modified)
    /* interactType:      */ INTERACT_COIN,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 0, // Likely 0 instead of 1 based on 'obakecoin_hit'
    /* health:            */ 0,
    /* numLootCoins:      */ 0,
    /* radius:            */ 32, // Smaller radius (100 in the final game)
    /* height:            */ 64,
    /* hurtboxRadius:     */ 0,
    /* hurtboxHeight:     */ 0,
};

static s32 coin_step(s16 *collisionFlagsPtr) { // CoinBound (modified)
    *collisionFlagsPtr = object_step();

    obj_check_floor_death(*collisionFlagsPtr, sObjFloor);

    if ((*collisionFlagsPtr & OBJ_COL_FLAG_GROUNDED)
        && !(*collisionFlagsPtr & OBJ_COL_FLAG_NO_Y_VEL)) {
        // No sound when bouncing
        return TRUE;
    }

    return FALSE;
}

static void moving_coin_flicker(void) { // RemoveCoin
    s16 collisionFlags;

    coin_step(&collisionFlags);
    obj_flicker_and_disappear(o, 0);
}

static void coin_collected(void) { // RemoveCoinCatch
    spawn_object(o, MODEL_SPARKLES, bhvCoinSparkles);
    o->activeFlags = ACTIVE_FLAG_DEACTIVATED;
}

void bhv_moving_coin_init(void) { // s_move_coin_init
    o->oGravity = 3.0f;
    o->oFriction = 1.0f;
    o->oBuoyancy = 1.5f;

    obj_set_hitbox(o, &sMovingYellowCoinHitbox);
}

void bhv_moving_coin_loop(void) { // s_move_coin_event
    s16 collisionFlags;

    switch (o->oAction) {
        case MOV_YCOIN_ACT_IDLE:
            coin_step(&collisionFlags);

            if (o->oTimer < 10) {
                cur_obj_become_intangible();
            } else {
                cur_obj_become_tangible();
            }

            if (o->oTimer > 300) {
                o->oAction = MOV_YCOIN_ACT_BLINKING;
            }
            break;

        case MOV_YCOIN_ACT_BLINKING:
            moving_coin_flicker();
            break;

        case MOV_YCOIN_ACT_LAVA_DEATH:
            o->activeFlags = ACTIVE_FLAG_DEACTIVATED;
            break;

        case MOV_YCOIN_ACT_DEATH_PLANE_DEATH:
            o->activeFlags = ACTIVE_FLAG_DEACTIVATED;
            break;
    }

    if (o->oInteractStatus & INT_STATUS_INTERACTED) {
        coin_collected();
        o->oInteractStatus = 0;
    }
}

static struct ObjectHitbox sMovingSliderCoinHitbox = { // escapecoin_hit    
    /* interactType:      */ INTERACT_COIN,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 0, // Likely 0 instead of 1 based on 'obakecoin_hit'
    /* health:            */ 0,
    /* numLootCoins:      */ 0,
    /* radius:            */ 32, // Smaller radius (100 in the final game)
    /* height:            */ 64,
    /* hurtboxRadius:     */ 0,
    /* hurtboxHeight:     */ 0,
};

void bhv_slider_coin_init(void) { // s_slider_coin_init
    o->oGravity = 5.0f;
    o->oFriction = 1.0f;
    o->oBuoyancy = 1.5f;

    obj_set_hitbox(o, &sMovingSliderCoinHitbox);
}

void bhv_slider_coin_loop(void) { // s_slider_coin_event (modified)
    s16 collisionFlags;

    switch (o->oAction) {
        case SLD_COIN_ACT_STILL:
            // Slider coins start moving much sooner than the final game
            if (is_point_within_radius_of_mario(o->oPosX, o->oPosY, o->oPosZ, 4000)) {
                o->oAction = SLD_COIN_ACT_MOVING;
            }
            break;

        case SLD_COIN_ACT_MOVING:
            collisionFlags = object_step();

            if (collisionFlags & OBJ_COL_FLAG_GROUNDED) {
                o->oForwardVel += 25.0f;
                // No sound when bouncing
            } else {
                o->oForwardVel *= 0.98;
            }

            if (o->oForwardVel > 75.0) {
                o->oForwardVel = 75.0f;
            }

            obj_flicker_and_disappear(o, 600);
            break;
    }

    if (o->oInteractStatus & INT_STATUS_INTERACTED) {
        coin_collected();
        o->oInteractStatus = 0;
    }
}
