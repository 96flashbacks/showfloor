// moving_coin.inc.c

static struct ObjectHitbox sMovingYellowCoinHitbox = {
    /* interactType:      */ INTERACT_COIN,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 1,
    /* health:            */ 0,
    /* numLootCoins:      */ 0,
    /* radius:            */ 32,
    /* height:            */ 64,
    /* hurtboxRadius:     */ 0,
    /* hurtboxHeight:     */ 0,
};

static struct ObjectHitbox sMovingBlueCoinHitbox = {
    /* interactType:      */ INTERACT_COIN,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 1,
    /* health:            */ 0,
    /* numLootCoins:      */ 0,
    /* radius:            */ 32,
    /* height:            */ 64,
    /* hurtboxRadius:     */ 0,
    /* hurtboxHeight:     */ 0,
};

s32 coin_step(s16 *collisionFlagsPtr) {
    *collisionFlagsPtr = object_step();

    obj_check_floor_death(*collisionFlagsPtr, sObjFloor);

    if ((*collisionFlagsPtr & OBJ_COL_FLAG_GROUNDED)
        && !(*collisionFlagsPtr & OBJ_COL_FLAG_NO_Y_VEL)) {
        return TRUE;
    }

    return FALSE;
}

void moving_coin_flicker(void) {
    s16 collisionFlags;

    coin_step(&collisionFlags);
    obj_flicker_and_disappear(o, 0);
}

void coin_collected(void) {
    spawn_object(o, MODEL_SPARKLES, bhvCoinSparkles);
    o->activeFlags = ACTIVE_FLAG_DEACTIVATED;
}

void bhv_moving_coin_init(void) {
    o->oGravity = 3.0f;
    o->oFriction = 1.0f;
    o->oBuoyancy = 1.5f;

    obj_set_hitbox(o, &sMovingYellowCoinHitbox);
}

void bhv_moving_coin_loop(void) {
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

void bhv_slider_coin_init(void) {
    o->oGravity = 5.0f;
    o->oFriction = 1.0f;
    o->oBuoyancy = 1.5f;

    obj_set_hitbox(o, &sMovingBlueCoinHitbox);
}

void bhv_slider_coin_loop(void) {
    s16 collisionFlags;

    switch (o->oAction) {
        case SLD_COIN_ACT_STILL:
            if (is_point_within_radius_of_mario(o->oPosX, o->oPosY, o->oPosZ, 4000)) {
                o->oAction = SLD_COIN_ACT_MOVING;
            }
            break;

        case SLD_COIN_ACT_MOVING:
            collisionFlags = object_step();

            if (collisionFlags & OBJ_COL_FLAG_GROUNDED) {
                o->oForwardVel += 25.0f;
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
