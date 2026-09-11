// coin.inc.c

struct ObjectHitbox sYellowCoinHitbox = { // coin_hit (modified)
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

static s32 bhv_coin_sparkles_init(void) { // coin_hitcheck
    if (o->oInteractStatus & INT_STATUS_INTERACTED
        && !(o->oInteractStatus & INT_STATUS_TOUCHED_BOB_OMB)) {
        spawn_object(o, MODEL_SPARKLES, bhvCoinSparkles);
        obj_mark_for_deletion(o);
        return TRUE;
    }

    o->oInteractStatus = 0;

    return FALSE;
}

void bhv_yellow_coin_init(void) { // s_coin_init
    cur_obj_set_behavior(bhvYellowCoin);
    obj_set_hitbox(o, &sYellowCoinHitbox);
    bhv_init_room();
    cur_obj_update_floor_height();

    // "no shadow" variant of the coin doesn't exist in the demo

    if (o->oFloorHeight < FLOOR_LOWER_LIMIT_MISC) {
        obj_mark_for_deletion(o);
    }
}

void bhv_yellow_coin_loop(void) { // s_coin
    bhv_coin_sparkles_init();
    o->oAnimState++;
}

void bhv_spawned_coin_init(void) { // s_player_coin_init
    o->oVelY = random_float() * 10.0f + 30.0f + o->oCoinBaseVelY;
    o->oForwardVel = random_float() * 10.0f;
    o->oMoveAngleYaw = random_u16();

    cur_obj_set_behavior(bhvYellowCoin);
    obj_set_hitbox(o, &sYellowCoinHitbox);
    cur_obj_become_intangible();
}

void bhv_spawned_coin_loop(void) { // s_player_coin
    struct Surface *floor;

    cur_obj_update_floor_and_walls();
    cur_obj_if_hit_wall_bounce_away();
    cur_obj_move_standard(-62);

    if ((floor = o->oFloor) != NULL) {
        if (o->oMoveFlags & OBJ_MOVE_ON_GROUND) {
            o->oSubAction = 1;
        }
        if (o->oSubAction == 1) {
            o->oBounciness = 0;
            if (floor->normal.y < 0.9) {
                s16 sp1A = atan2s(floor->normal.z, floor->normal.x);
                cur_obj_rotate_yaw_toward(sp1A, 0x400);
            }
        }
    }

    if (o->oVelY < 0.0f) {
        cur_obj_become_tangible();
    }

    if (o->oMoveFlags & OBJ_MOVE_LANDED) {
        if (o->oMoveFlags & OBJ_MOVE_ABOVE_LAVA) {
            obj_mark_for_deletion(o);
        }
    }

    // Removed 'cur_obj_wait_then_blink()' based on 'pathobakecoin.p' 

    bhv_coin_sparkles_init();
}

// The coin inside Boos seemingly had its own separate file based on 'pathobakecoin.p'

// The coin sparkles didn't have their 2 functions (s_coinspark, s_coinspark_make) in the demo.
// They only spawned once and had no random 30 unit position offset
