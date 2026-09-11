// mr_i.inc.c

// this is actually the MrI particle loop function. piranha
// plant code later on reuses this function.
void bhv_piranha_particle_loop(void) { // s_balloon_gomi
    if (o->oTimer == 0) {
        o->oVelY = 20.0f + 20.0f * random_float();
        o->oForwardVel = 20.0f + 20.0f * random_float();
        o->oMoveAngleYaw = random_u16();
    }
    cur_obj_move_using_fvel_and_gravity();
}

static void mr_i_piranha_particle_act_0(void) { // balloonfire_1
    cur_obj_scale(3.0f);
    o->oForwardVel = 20.0f;
    cur_obj_update_floor_and_walls();

    if (o->oInteractStatus & INT_STATUS_INTERACTED) {
        o->oAction = 1;
    } else if ((o->oTimer > 100) || (o->oMoveFlags & OBJ_MOVE_HIT_WALL)
               || (o->activeFlags & ACTIVE_FLAG_IN_DIFFERENT_ROOM)) {
        obj_mark_for_deletion(o);
    }
}

static void mr_i_piranha_particle_act_1(void) { // balloonfire_2
    s32 i;
    obj_mark_for_deletion(o);
    for (i = 0; i < 10; i++) {
        spawn_object(o, MODEL_PURPLE_MARBLE, bhvPurpleParticle);
    }
}

static void (*sMrIParticleActions[])(void) = { // balloonfire_modejmp
    mr_i_piranha_particle_act_0,
    mr_i_piranha_particle_act_1,
};

void bhv_mr_i_particle_loop(void) { // s_balloon_fire
    cur_obj_call_action_function(sMrIParticleActions);
}

static void spawn_mr_i_particle(void) { // balloon_makefire
    struct Object *particle;
    f32 sp18 = o->header.gfx.scale[1];

    particle = spawn_object(o, MODEL_PURPLE_MARBLE, bhvMrIParticle);
    particle->oPosY += 50.0f * sp18;
    particle->oPosX += sins(o->oMoveAngleYaw) * 90.0f * sp18;
    particle->oPosZ += coss(o->oMoveAngleYaw) * 90.0f * sp18;

    cur_obj_play_sound_2(SOUND_OBJ_MRI_SHOOT);
}

void bhv_mr_i_body_loop(void) { // s_balloon_eye
    obj_copy_pos_and_angle(o, o->parentObj);

    if (!(o->activeFlags & ACTIVE_FLAG_IN_DIFFERENT_ROOM)) {
        obj_copy_scale(o, o->parentObj);
        obj_set_parent_relative_pos(o, 0, 0, o->header.gfx.scale[1] * 100.0f);
        obj_build_transform_from_pos_and_angle(o, 44, 15);
        obj_translate_local(o, 6, 44);
        o->oFaceAnglePitch = o->oMoveAnglePitch;
        o->oGraphYOffset = o->header.gfx.scale[1] * 100.0f;
    }

    if (o->parentObj->oMrIUnk110 != 1) {
        o->oAnimState = -1;
    } else {
        o->oAnimState++;
        if (o->oAnimState == 15) {
            o->parentObj->oMrIUnk110 = 0;
        }
    }

    if (o->parentObj->activeFlags == ACTIVE_FLAG_DEACTIVATED) {
        obj_mark_for_deletion(o);
    }
}

static void mr_i_act_3(void) { // balloon_eyedamage (modified)
    s16 sp36;
    s16 sp34;
    f32 sp30;
    f32 sp2C;
    UNUSED u8 filler[8];
    f32 sp20;
    f32 sp1C;

    // Removed Big Mr. I handling
    sp1C = 1.0f;

    if (o->oMrIUnk100 < 0) {
        sp34 = 0x1000;
    } else {
        sp34 = -0x1000;
    }

    sp2C = (o->oTimer + 1) / 96.0f;

    if (o->oTimer < 64) {
        sp36 = o->oMoveAngleYaw;
        o->oMoveAngleYaw += sp34 * coss(0x4000 * sp2C);

        if (sp36 < 0 && o->oMoveAngleYaw >= 0) {
            cur_obj_play_sound_2(SOUND_OBJ2_MRI_SPINNING);
        }

        o->oMoveAnglePitch = (1.0 - coss(0x4000 * sp2C)) * -0x4000;
        cur_obj_shake_y(4.0f);
    } else if (o->oTimer < 96) {
        sp30 = (f32) (o->oTimer - 63) / 32;
        o->oMoveAngleYaw += sp34 * coss(0x4000 * sp2C);
        o->oMoveAnglePitch = (1.0 - coss(0x4000 * sp2C)) * -0x4000;

        cur_obj_shake_y((s32) ((1.0f - sp30) * 4)); // trucating the f32?
        sp20 = coss(0x4000 * sp30) * 0.4 + 0.6;
        cur_obj_scale(sp20 * sp1C);
    } else if (o->oTimer < 104) {
        // do nothing
    } else if (o->oTimer < 168) {
        if (o->oTimer == 104) {
            cur_obj_become_intangible();
            o->oMrIScale = sp1C * 0.6;
            // Removed Big Mr. I handling
            obj_spawn_loot_yellow_coins(o, 1, 20.0f);
        }

        o->oMrIScale -= 0.2 * sp1C;

        if (o->oMrIScale < 0) {
            o->oMrIScale = 0;
        }

        cur_obj_scale(o->oMrIScale);
    } else {
        obj_mark_for_deletion(o);
    }

    // Commented out code from 'pathbaloon.p' that makes the Mr. I
    // reset if Mario gets over 2000 units away from him while he's dying
    if (o->oDistanceToMario > 2000.0f) {
        o->oAction = 1;
        o->oMoveAngleYaw = 0;
        o->oMoveAnglePitch = 0;
        cur_obj_scale(1.0);
    }
}

static void mr_i_act_2(void) { // balloon_playereye
    s16 sp1E = o->oMoveAngleYaw;
    s16 sp1C;

    if (o->oTimer == 0) {
        o->oMrIUnkF4 = 120;
        o->oMrIUnkFC = 0;
        o->oMrIUnk100 = 0;
        o->oMrIUnk104 = 0;
    }

    obj_turn_toward_object(o, gMarioObject, 0x10, 0x800);
    obj_turn_toward_object(o, gMarioObject, 0x0F, 0x400);

    sp1C = sp1E - (s16) (o->oMoveAngleYaw);

    if (sp1C == 0) {
        o->oMrIUnkFC = 0;
        o->oMrIUnk100 = 0;
    } else if (sp1C > 0) {
        if (o->oMrIUnk100 > 0) {
            o->oMrIUnkFC += sp1C;
        } else {
            o->oMrIUnkFC = 0;
        }
        o->oMrIUnk100 = 1;
    } else {
        if (o->oMrIUnk100 < 0) {
            o->oMrIUnkFC -= sp1C;
        } else {
            o->oMrIUnkFC = 0;
        }
        o->oMrIUnk100 = -1;
    }

    if (o->oMrIUnkFC == 0) {
        o->oMrIUnkF4 = 120;
    }
    if (o->oMrIUnkFC > 65536) {
        o->oAction = 3;
    }

    o->oMrIUnkF4--;

    if (o->oMrIUnkF4 == 0) {
        o->oMrIUnkF4 = 120;
        o->oMrIUnkFC = 0;
    }

    if (o->oMrIUnkFC < 5000) {
        if (o->oMrIUnk104 == o->oMrIUnk108) {
            o->oMrIUnk110 = 1;
        }

        if (o->oMrIUnk104 == o->oMrIUnk108 + 20) {
            spawn_mr_i_particle();
            o->oMrIUnk104 = 0;
            o->oMrIUnk108 = (s32) (random_float() * 50.0f + 50.0f);
        }
        o->oMrIUnk104++;
    } else {
        o->oMrIUnk104 = 0;
        o->oMrIUnk108 = (s32) (random_float() * 50.0f + 50.0f);
    }

    if (o->oDistanceToMario > 800.0f) {
        o->oAction = 1;
    }
}

static void mr_i_act_1(void) { // balloon_roll
    s16 sp1E = obj_angle_to_object(o, gMarioObject);
    s16 sp1C = abs_angle_diff(o->oMoveAngleYaw, sp1E);
    s16 sp1A = abs_angle_diff(o->oMoveAngleYaw, gMarioObject->oFaceAngleYaw);

    if (o->oTimer == 0) {
        cur_obj_become_tangible();
        o->oMoveAnglePitch = 0;
        o->oMrIUnk104 = 30;
        o->oMrIUnk108 = random_float() * 20.0f;
        if (o->oMrIUnk108 & 1) {
            o->oAngleVelYaw = -256;
        } else {
            o->oAngleVelYaw = 256;
        }
    }

    if (sp1C < 1024 && sp1A > 0x4000) {
        if (o->oDistanceToMario < 700.0f) {
            o->oAction = 2;
        } else {
            o->oMrIUnk104++;
        }
    } else {
        o->oMoveAngleYaw += o->oAngleVelYaw;
        o->oMrIUnk104 = 30;
    }

    if (o->oMrIUnk104 == o->oMrIUnk108 + 60) {
        o->oMrIUnk110 = 1;
    }

    if (o->oMrIUnk104 > o->oMrIUnk108 + 80) {
        o->oMrIUnk104 = 0;
        o->oMrIUnk108 = random_float() * 80.0f;
        spawn_mr_i_particle();
    }
}

static void mr_i_act_0(void) { // balloon_init
    o->oMoveAnglePitch = 0;
    o->oMoveAngleYaw = 0;
    o->oMoveAngleRoll = 0;
    cur_obj_scale(o->oBhvParams2ndByte + 1);

    if (o->oTimer == 0) {
        cur_obj_set_pos_to_home();
    }

    if (o->oDistanceToMario < 1500.0f) {
        o->oAction = 1;
    }
}

static void (*sMrIActions[])(void) = { // balloon_modejmp
    mr_i_act_0,
    mr_i_act_1,
    mr_i_act_2,
    mr_i_act_3,
};

static struct ObjectHitbox sMrIHitbox = { // balloon_hit
    /* interactType:      */ INTERACT_DAMAGE,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 2,
    /* health:            */ 2,
    /* numLootCoins:      */ 1,
    /* radius:            */ 80,
    /* height:            */ 150,
    /* hurtboxRadius:     */ 0,
    /* hurtboxHeight:     */ 0,
};

void bhv_mr_i_loop(void) { // s_balloon 
    obj_set_hitbox(o, &sMrIHitbox);
    cur_obj_call_action_function(sMrIActions);

    if (o->oAction != 3) {
        if ((o->oDistanceToMario > 3000.0f) || (o->activeFlags & ACTIVE_FLAG_IN_DIFFERENT_ROOM)) {
            o->oAction = 0;
        }
    }

    o->oInteractStatus = 0;
}
