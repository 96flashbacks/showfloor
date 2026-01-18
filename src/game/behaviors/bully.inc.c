// bully.c.inc

static struct ObjectHitbox sSmallBullyHitbox = {
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

static struct ObjectHitbox sBigBullyHitbox = {
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

void bhv_small_bully_init(void) {
    cur_obj_init_animation(0);

    o->oHomeX = o->oPosX;
    o->oHomeZ = o->oPosZ;
    o->oBhvParams2ndByte = BULLY_BP_SIZE_SMALL;
    o->oGravity = 4.0;
    o->oFriction = 0.91;
    o->oBuoyancy = 1.3;

    obj_set_hitbox(o, &sSmallBullyHitbox);
}

void bhv_big_bully_init(void) {
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

void bully_check_mario_collision(void) {
    if (o->oInteractStatus & INT_STATUS_INTERACTED) {
        o->oInteractStatus &= ~INT_STATUS_INTERACTED;
        o->oAction = BULLY_ACT_KNOCKBACK;
        o->oFlags &= ~0x8; /* bit 3 */
        cur_obj_init_animation(3);
        o->oBullyMarioCollisionAngle = o->oMoveAngleYaw;
    }
}

void bully_act_chase_mario(void) {
    f32 homeX = o->oHomeX;
    f32 posY = o->oPosY;
    f32 homeZ = o->oHomeZ;

    if (o->oTimer < 20) {
        o->oForwardVel = 1.5;
        obj_turn_toward_object(o, gMarioObject, 16, 1280);
    } else if (o->oBhvParams2ndByte == BULLY_BP_SIZE_SMALL) {
        o->oForwardVel = 12.0;
        if (o->oTimer >= 50)
            o->oTimer = 0;
    } else {
        o->oForwardVel = 30.0;
        if (o->oTimer >= 36)
            o->oTimer = 0;
    }

    if (!is_point_within_radius_of_mario(homeX, posY, homeZ, 600)) {
        o->oAction = BULLY_ACT_PATROL;
        cur_obj_init_animation(0);
    }
}

void bully_act_knockback(void) {
    if (o->oForwardVel < 2.0 && (s32) o->oVelY == 0) {
        o->oForwardVel = 1.0;
        o->oBullyKBTimerAndMinionKOCounter++;
        o->oFlags |= 0x8; /* bit 3 */
        o->oMoveAngleYaw = o->oFaceAngleYaw;
        // obj_turn_toward_object(o, gMarioObject, 16, 1280);
    } else
        o->header.gfx.animInfo.animFrame = 0;

    if (o->oBullyKBTimerAndMinionKOCounter == 18) {
        o->oAction = BULLY_ACT_CHASE_MARIO;
        o->oBullyKBTimerAndMinionKOCounter = 0;
        cur_obj_init_animation(1);
    }
}

void bully_act_back_up(void) {
    if (o->oTimer == 0) {
        o->oFlags &= ~0x8; /* bit 3 */
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
        o->oFlags |= 0x8; /* bit 3 */
        o->oAction = BULLY_ACT_PATROL;
    }
}

void bully_backup_check(s16 collisionFlags) {
    if (!(collisionFlags & 0x8) && o->oAction != BULLY_ACT_KNOCKBACK) /* bit 3 */
    {
        o->oPosX = o->oBullyPrevX;
        o->oPosZ = o->oBullyPrevZ;
        o->oAction = BULLY_ACT_BACK_UP;
    }
}

void bully_step(void) {
    s16 collisionFlags = 0;
    collisionFlags = object_step();
    bully_backup_check(collisionFlags);
    obj_check_floor_death(collisionFlags, sObjFloor);
}

void bully_spawn_coin(void) {
    struct Object *coin = spawn_object(o, MODEL_YELLOW_COIN, bhvMovingYellowCoin);
    coin->oForwardVel = 10.0f;
    coin->oVelY = 100.0f;
    coin->oPosY = o->oPosY + 310.0f;
    coin->oMoveAngleYaw = (f32) (o->oBullyMarioCollisionAngle + 0x8000) + random_float() * 1024.0f;
}

void bully_act_level_death(void) {
    if (obj_lava_death() == 1) {
        if (o->oBhvParams2ndByte == BULLY_BP_SIZE_SMALL) {
            if (o->oBullySubtype == BULLY_STYPE_MINION)
                o->parentObj->oBullyKBTimerAndMinionKOCounter++;
            bully_spawn_coin();
        } else {
            spawn_mist_particles();

            spawn_default_star(0, 950.0f, -6800.0f);
            spawn_object_abs_with_rot(o, 0, MODEL_NONE, bhvLLLTumblingBridge, 0, 154, -5631, 0, 0, 0);
        }
    }
}

void bhv_bully_loop(void) {
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

            if (obj_return_home_if_safe(o, o->oHomeX, o->oPosY, o->oHomeZ, 400) == 1) {
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
            cur_obj_scale(0.0f);
            bully_act_level_death();
            break;

        case BULLY_ACT_DEATH_PLANE_DEATH:
            o->activeFlags = 0;
            break;
    }

    set_object_visibility(o, 3000);
}

// sp38 = arg0
// sp3c = arg1
// sp40 = arg2
// sp44 = arg3

void big_bully_spawn_minion(s32 arg0, s32 arg1, s32 arg2, s16 arg3) {
    struct Object *bully =
        spawn_object_abs_with_rot(o, 0, MODEL_BULLY, bhvSmallBully, arg0, arg1, arg2, 0, arg3, 00);
    bully->oBullySubtype = BULLY_STYPE_MINION;
    bully->oBhvParams2ndByte = BULLY_BP_SIZE_SMALL;
}

void bhv_big_bully_with_minions_init(void) {
    big_bully_spawn_minion(2818, 307, -1758, 0);
    big_bully_spawn_minion(3387, 307, -2207, 0);
    big_bully_spawn_minion(3826, 307, -1758, 0);

    o->header.gfx.node.flags |= GRAPH_RENDER_INVISIBLE;

    cur_obj_become_intangible();

    o->oAction = BULLY_ACT_INACTIVE;
}

void big_bully_spawn_star(void) {
    spawn_mist_particles();

    spawn_default_star(3166.0f, 600.0f, -1952.0f);
}

void bhv_big_bully_with_minions_loop(void) {
    o->oBullyPrevX = o->oPosX;
    o->oBullyPrevY = o->oPosY;
    o->oBullyPrevZ = o->oPosZ;

    bully_check_mario_collision();

    switch (o->oAction) {
        case BULLY_ACT_PATROL:
            o->oForwardVel = 5.0;

            if (obj_return_home_if_safe(o, o->oHomeX, o->oPosY, o->oHomeZ, 1000) == 1) {
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

        case BULLY_ACT_INACTIVE:
            if (o->oBullyKBTimerAndMinionKOCounter == 3) {
                big_bully_spawn_star();
                o->activeFlags = 0;
            }
            break;
    }
}
