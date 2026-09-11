// bully.c.inc

static struct ObjectHitbox sSmallBullyHitbox = { // otos_hit
    /* interactType:      */ INTERACT_BULLY,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 1,
    /* health:            */ 0,
    /* numLootCoins:      */ 0,
    /* radius:            */ 73,
    /* height:            */ 123,
    /* hurtboxRadius:     */ 63,
    /* hurtboxHeight:     */ 113,
};

static struct ObjectHitbox sBigBullyHitbox = { // oya_otos_hit
    /* interactType:      */ INTERACT_BULLY,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 1,
    /* health:            */ 0,
    /* numLootCoins:      */ 0,
    /* radius:            */ 115,
    /* height:            */ 235,
    /* hurtboxRadius:     */ 105,
    /* hurtboxHeight:     */ 225,
};

void bhv_small_bully_init(void) { // s_otos_init
    cur_obj_init_animation(0);

    o->oHomeX = o->oPosX;
    o->oHomeZ = o->oPosZ;
    o->oBhvParams2ndByte = BULLY_BP_SIZE_SMALL;
    o->oGravity = 4.0;
    // o->oFriction = 0.91 + ((f32)gDebugInfo[DEBUG_PAGE_EFFECTINFO][1]/100);
    o->oFriction = 0.91;
    o->oBuoyancy = 1.3;

    obj_set_hitbox(o, &sSmallBullyHitbox);
}

void bhv_big_bully_init(void) { // s_oya_otos_init
    cur_obj_init_animation(0);

    o->oHomeX = o->oPosX;
    o->oHomeY = o->oPosY;
    o->oHomeZ = o->oPosZ;
    o->oBhvParams2ndByte = BULLY_BP_SIZE_BIG;
    o->oGravity = 5.0;
    o->oFriction = 0.93;
    o->oBuoyancy = 1.3;

    obj_set_hitbox(o, &sBigBullyHitbox);
}

static void bully_check_mario_collision(void) { // OtosPlayerHitEvent (modified)
    if (o->oInteractStatus & INT_STATUS_INTERACTED) {
        // No sound effects
        o->oInteractStatus &= ~INT_STATUS_INTERACTED;
        o->oAction = BULLY_ACT_KNOCKBACK;
        o->oFlags &= ~OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW;
        cur_obj_init_animation(3);
        o->oBullyMarioCollisionAngle = o->oMoveAngleYaw;
    }
}

void bully_act_chase_mario(void) { // OtosPlayerSearchEvent (modified)
    f32 homeX = o->oHomeX;
    f32 posY = o->oPosY;
    f32 homeZ = o->oHomeZ;

    // Initial chasing time is doubled
    if (o->oTimer < 20) {
        o->oForwardVel = 1.5; // Initial chasing speed is halved
        obj_turn_toward_object(o, gMarioObject, 16, 0x500); // Turning speed is halved
    } else if (o->oBhvParams2ndByte == BULLY_BP_SIZE_SMALL) {
        o->oForwardVel = 12.0; // Slower chasing speed
        if (o->oTimer > 50) // Longer timer to compensate for the slower speed
            o->oTimer = 0;
    } else {
        o->oForwardVel = 24.0; // Slower chasing speed
        if (o->oTimer > 50) // Longer timer to compensate for the slower speed
            o->oTimer = 0;
    }

    if (!is_point_within_radius_of_mario(homeX, posY, homeZ, 600)) { // 600 instead of 1000
        o->oAction = BULLY_ACT_PATROL;
        cur_obj_init_animation(0);
    }
}

void bully_act_knockback(void) { // OtosReflectEvent (modified)
    // Bullies need to slow down much more before starting to recover
    if (o->oForwardVel < 2.0 && (s32) o->oVelY == 0) {
        o->oForwardVel = 1.0;
        o->oBullyKBTimer++;
        o->oFlags |= OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW;
        o->oMoveAngleYaw = o->oFaceAngleYaw;
        // Bullies don't turn towards Mario while recovering from being hit
    } else
        o->header.gfx.animInfo.animFrame = 0;

    if (o->oBullyKBTimer == 18) {
        o->oAction = BULLY_ACT_CHASE_MARIO;
        o->oBullyKBTimer = 0;
        cur_obj_init_animation(1);
    }
}

void bully_act_back_up(void) { // OtosU_Turn
    if (o->oTimer == 0) {
        o->oFlags &= ~OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW;
        o->oMoveAngleYaw += 0x8000;
    }

    o->oForwardVel = 5.0;

    //! bully_backup_check() happens after this function, and has the potential to reset
    //  the bully's action to BULLY_ACT_BACK_UP. Because the back up action is only
    //  set to end when the timer EQUALS 15, if this happens on that frame, the bully
    //  will be stuck in BULLY_ACT_BACK_UP forever until Mario hits it or its death
    //  conditions are activated. However because its angle is set to its facing angle,
    //  it will walk forward instead of backing up.

    if (o->oTimer == 15) {
        o->oMoveAngleYaw = o->oFaceAngleYaw;
        o->oFlags |= OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW;
        o->oAction = BULLY_ACT_PATROL;
    }
}

static void bully_backup_check(s16 collisionFlags) { // OtosBgCheck
    if (!(collisionFlags & OBJ_COL_FLAG_NO_Y_VEL) && o->oAction != BULLY_ACT_KNOCKBACK) {
        o->oPosX = o->oBullyPrevX;
        //o->oPosY = o->oBullyPrevY;
        o->oPosZ = o->oBullyPrevZ;
        o->oAction = BULLY_ACT_BACK_UP;
    }
}

// No 'OtosSound' (bully_play_stomping_sound), so Bullies are silent when walking

static void bully_step(void) { // OtosMove (modified)
    s16 collisionFlags = 0;
    collisionFlags = object_step();
    bully_backup_check(collisionFlags);
    // No 'OtosSound' (bully_play_stomping_sound)
    obj_check_floor_death(collisionFlags, sObjFloor);

    // No Chill Bully check
}

static void bully_spawn_coin(void) { // OtosSpitCoin (modified)
    struct Object *coin = spawn_object(o, MODEL_YELLOW_COIN, bhvMovingCoin);
    // No sound
    coin->oForwardVel = 10.0f;
    coin->oVelY = 100.0f;
    coin->oPosY = o->oPosY; // Coin isn't placed 310 units higher since Bullies doesn't sink into the lava
    coin->oMoveAngleYaw = o->oBullyMarioCollisionAngle + 0x8000 + (random_float() * 0x400);
}

static void bully_act_level_death(void) { // OtosMelt (modified)
    if (obj_lava_death() == TRUE) {
        if (o->oBhvParams2ndByte == BULLY_BP_SIZE_SMALL) {
            // Removed Bully minions handling
            bully_spawn_coin();
        } else {
            // No spawn_mist_particles()
            // Removed Chill Bully handling

            // Early star placement that is commented out in the LLL level script and the "star" backup
            spawn_object_abs_with_rot(o, 0, MODEL_STAR, bhvStar, 3850, 323, -5600, 0, 0, 0);
            
        }
    }
}

void bhv_bully_loop(void) { // s_otos_event (modified)
    o->oBullyPrevX = o->oPosX;
    o->oBullyPrevY = o->oPosY;
    o->oBullyPrevZ = o->oPosZ;

    //! Because this function runs no matter what, Mario is able to interrupt the bully's
    //  death action by colliding with it. Since the bully hitbox is tall enough to collide
    //  with Mario even when it is under a lava floor, this can get the bully stuck OOB
    //  if there is nothing under the lava floor.
    bully_check_mario_collision();

    switch (o->oAction) {
        case BULLY_ACT_PATROL:
            o->oForwardVel = 5.0;

            // Smaller chasing radius, 500 instead of 800
            if (obj_return_home_if_safe(o, o->oHomeX, o->oPosY, o->oHomeZ, 500) == 1) {
                o->oAction = BULLY_ACT_CHASE_MARIO;
                cur_obj_init_animation(1);
            }

            bully_step();
            break;

        case BULLY_ACT_CHASE_MARIO:
            bully_act_chase_mario();
            bully_step();
            break;

        case BULLY_ACT_KNOCKBACK:
            bully_act_knockback();
            bully_step();
            break;

        case BULLY_ACT_BACK_UP:
            bully_act_back_up();
            bully_step();
            break;

        case BULLY_ACT_LAVA_DEATH:
            // Bullies seemingly didn't sink in the lava based on Bad Influence
            // so they shrink to not look super awkward while dying
            cur_obj_scale((f32) 1.0 - o->oTimer / 25.0);
            bully_act_level_death();
            break;

        case BULLY_ACT_DEATH_PLANE_DEATH:
            o->activeFlags = ACTIVE_FLAG_DEACTIVATED;
            break;
    }

    set_object_visibility(o, 3000);
}

// It seems as if the Big Bully with the minions didn't exist in the demo, based on him being
// called "big_otos2" and the code for spawning each of the minions (MakeOtos2) being copied from the
// final DDD treasure chest (treasurebox.s), based on the "Make Treasure Box" comment above the function
