// bobomb.inc.c

static struct ObjectHitbox sBobombHitbox = {
    /* interactType:      */ INTERACT_GRABBABLE,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 0,
    /* health:            */ 0,
    /* numLootCoins:      */ 0,
    /* radius:            */ 65,
    /* height:            */ 113,
    /* hurtboxRadius:     */ 0,
    /* hurtboxHeight:     */ 0,
};

void bhv_bobomb_init(void) {
    o->oGravity = 2.5f;
    o->oFriction = 0.8f;
    o->oBuoyancy = 1.3f;
    o->oInteractionSubtype = INT_SUBTYPE_KICKABLE;
}

void bobomb_spawn_coin(void) {
    if (!((o->oBhvParams >> 8) & 0x01)) {
        obj_spawn_yellow_coins(o, 1);
        o->oBhvParams = 0x100;
        set_object_respawn_info_bits(o, 1);
    }
}

void bobomb_act_explode(void) {
    cur_obj_init_animation(1);
    if (o->oTimer < 5) {
        cur_obj_scale(1.0 + (f32) o->oTimer / 5.0);
    } else {
        struct Object *explosion = spawn_object(o, MODEL_EXPLOSION, bhvExplosion);
        explosion->oGraphYOffset += 100.0f;

        bobomb_spawn_coin();
        create_respawner(MODEL_BLACK_BOBOMB, bhvBobomb, 3000);

        o->activeFlags = ACTIVE_FLAG_DEACTIVATED;
    }
}

void bobomb_check_interactions(void) {
    obj_set_hitbox(o, &sBobombHitbox);

    if (o->oInteractStatus & INT_STATUS_INTERACTED) {
        if (o->oInteractStatus & INT_STATUS_MARIO_KNOCKBACK_DMG) {
            o->oMoveAngleYaw = gMarioObject->header.gfx.angle[1];
            o->oForwardVel = 25.0f;
            o->oVelY = 30.0f;
            o->oAction = BOBOMB_ACT_LAUNCHED;
        }

        if (o->oInteractStatus & INT_STATUS_TOUCHED_BOB_OMB) {
            o->oAction = BOBOMB_ACT_EXPLODE;
        }

        o->oInteractStatus = 0;
    }

    if (obj_attack_collided_from_other_object(o) == TRUE) {
        o->oAction = BOBOMB_ACT_EXPLODE;
    }
}

void bobomb_act_patrol(void) {
    
    UNUSED u8 filler[4];
    UNUSED s16 animFrame = o->header.gfx.animInfo.animFrame;
    s16 collisionFlags;
cur_obj_init_animation(3);
    o->oForwardVel = 5.0f;
    collisionFlags = object_step();

    if ((obj_return_home_if_safe(o, o->oHomeX, o->oHomeY, o->oHomeZ, 400) == TRUE)
        && (obj_check_if_facing_toward_angle(o->oMoveAngleYaw, o->oAngleToMario, 0x2000) == TRUE)) {
        o->oBobombFuseLit = 1;
        o->oAction = BOBOMB_ACT_CHASE_MARIO;
    }

    obj_check_floor_death(collisionFlags, sObjFloor);
}

void bobomb_act_chase_mario(void) {
    UNUSED u8 filler[4];
    s16 animFrame = ++o->header.gfx.animInfo.animFrame;
    s16 collisionFlags;

    o->oForwardVel = 20.0f;
    collisionFlags = object_step();

    //if (animFrame == 5 || animFrame == 16) {
    //    cur_obj_play_sound_2(SOUND_OBJ_BOBOMB_WALK);
    //}

    obj_turn_toward_object(o, gMarioObject, 16, 0x800);
    obj_check_floor_death(collisionFlags, sObjFloor);
}

void bobomb_act_launched(void) {
    s16 collisionFlags = 0;
    collisionFlags = object_step();
    if ((collisionFlags & OBJ_COL_FLAG_GROUNDED) == OBJ_COL_FLAG_GROUNDED) {
        o->oAction = BOBOMB_ACT_EXPLODE;
    }
}

void generic_bobomb_free_loop(void) {
    switch (o->oAction) {
        case BOBOMB_ACT_PATROL:
            bobomb_act_patrol();
            break;

        case BOBOMB_ACT_LAUNCHED:
            bobomb_act_launched();
            break;

        case BOBOMB_ACT_CHASE_MARIO:
            bobomb_act_chase_mario();
            break;

        case BOBOMB_ACT_EXPLODE:
            bobomb_act_explode();
            break;

        case BOBOMB_ACT_LAVA_DEATH:
            if (obj_lava_death() == TRUE) {
                create_respawner(MODEL_BLACK_BOBOMB, bhvBobomb, 3000);
            }
            break;

        case BOBOMB_ACT_DEATH_PLANE_DEATH:
            o->activeFlags = ACTIVE_FLAG_DEACTIVATED;
            create_respawner(MODEL_BLACK_BOBOMB, bhvBobomb, 3000);
            break;
    }

    bobomb_check_interactions();

    if (o->oBobombFuseTimer > 150) {
        o->oAction = 3;
    }
}

void stationary_bobomb_free_loop(void) {
    switch (o->oAction) {
        case BOBOMB_ACT_LAUNCHED:
            bobomb_act_launched();
            break;

        case BOBOMB_ACT_EXPLODE:
            bobomb_act_explode();
            break;

        case BOBOMB_ACT_LAVA_DEATH:
            if (obj_lava_death() == TRUE) {
                create_respawner(MODEL_BLACK_BOBOMB, bhvBobomb, 3000);
            }
            break;

        case BOBOMB_ACT_DEATH_PLANE_DEATH:
            o->activeFlags = ACTIVE_FLAG_DEACTIVATED;
            create_respawner(MODEL_BLACK_BOBOMB, bhvBobomb, 3000);
            break;
    }

    bobomb_check_interactions();

    if (o->oBobombFuseTimer > 150) {
        o->oAction = 3;
    }
}

void bobomb_free_loop(void) {
    if (o->oBhvParams2ndByte == BOBOMB_BP_STYPE_GENERIC) {
        generic_bobomb_free_loop();
    } else {
        stationary_bobomb_free_loop();
    }
}

void bobomb_held_loop(void) {
    o->header.gfx.node.flags |= GRAPH_RENDER_INVISIBLE;
    cur_obj_init_animation(2);
    cur_obj_set_pos_relative(gMarioObject, 0, 60.0f, 100.0);

    o->oBobombFuseLit = 1;
    if (o->oBobombFuseTimer > 150) {
        //! Although the Bob-omb's action is set to explode when the fuse timer expires,
        //  bobomb_act_explode() will not execute until the bob-omb's held state changes.
        //  This allows the Bob-omb to be regrabbed indefinitely.
        gMarioObject->oInteractStatus |= INT_STATUS_MARIO_DROP_OBJECT;
        o->oAction = BOBOMB_ACT_EXPLODE;
    }
}

void bobomb_dropped_loop(void) {
    cur_obj_get_dropped();

    o->header.gfx.node.flags &= ~GRAPH_RENDER_INVISIBLE;

    o->oHeldState = 0;
    o->oAction = BOBOMB_ACT_PATROL;
}

void bobomb_thrown_loop(void) {
    cur_obj_enable_rendering_2();

    o->header.gfx.node.flags &= ~GRAPH_RENDER_INVISIBLE;
    o->oHeldState = 0;
    o->oFlags &= ~OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW;
    o->oForwardVel = 25.0f;
    o->oVelY = 20.0f;
    o->oAction = BOBOMB_ACT_LAUNCHED;
}


void bhv_bobomb_loop(void) {
    s8 dustPeriodMinus1;

    if (is_point_within_radius_of_mario(o->oPosX, o->oPosY, o->oPosZ, 4000)) {
        switch (o->oHeldState) {
            case HELD_FREE:
                bobomb_free_loop();
                break;

            case HELD_HELD:
                bobomb_held_loop();
                break;

            case HELD_THROWN:
                bobomb_thrown_loop();
                break;

            case HELD_DROPPED:
                bobomb_dropped_loop();
                break;
        }

        if (o->oBobombFuseLit == 1) {
            if (o->oBobombFuseTimer > 120) {
                dustPeriodMinus1 = 1;
            } else {
                dustPeriodMinus1 = 7;
            }

            // oBobombFuseTimer % 2 or oBobombFuseTimer % 8
            if (!(dustPeriodMinus1 & o->oBobombFuseTimer)) {
                spawn_object(o, MODEL_SMOKE, bhvBobombFuseSmoke);
            }

            cur_obj_play_sound_1(SOUND_AIR_BOBOMB_LIT_FUSE);

            o->oBobombFuseTimer++;
        }
    }
}

void bhv_bobomb_fuse_smoke_init(void) {
    o->oPosX += (s32)(random_float() * 80.0f) - 40;
    o->oPosY += (s32)(random_float() * 80.0f) + 60;
    o->oPosZ += (s32)(random_float() * 80.0f) - 40;
    cur_obj_scale(1.2f);
}
