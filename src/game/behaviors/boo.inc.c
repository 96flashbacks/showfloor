// boo.c.inc

static struct ObjectHitbox sBooGivingStarHitbox = {
    /* interactType: */ 0,
    /* downOffset: */ 0,
    /* damageOrCoinValue: */ 3,
    /* health: */ 3,
    /* numLootCoins: */ 0,
    /* radius: */ 140,
    /* height: */ 80,
    /* hurtboxRadius: */ 40,
    /* hurtboxHeight: */ 60,
};

// Relative positions
static s16 sCourtyardBooTripletPositions[][3] = { { 210, 100, 430 }, { 0, 70, 210 }, { -210, 100, -210 } };

static s32 boo_should_be_active(void) {
    if (o->oRoom == -1) {
        if (o->oDistanceToMario < 1500) {
            return TRUE;
        }
    } else {
        if (o->oDistanceToMario < 1500 && (o->oRoom == gMarioCurrentRoom)) {
            return TRUE;
        }
    }

    return FALSE;
}

void bhv_courtyard_boo_triplet_init(void) {
    s32 i;
    struct Object *boo;

    for (i = 0; i < 3; i++) {
        boo = spawn_object_relative(0x01, sCourtyardBooTripletPositions[i][0],
                                    sCourtyardBooTripletPositions[i][1],
                                    sCourtyardBooTripletPositions[i][2], o, MODEL_BOO, bhvGhostHuntBoo);

        boo->oMoveAngleYaw = random_u16();
    }
}

static void boo_approach_target_opacity_and_update_scale(void) {
    f32 scale;

    if (o->oBooTargetOpacity != o->oOpacity) {
        if (o->oBooTargetOpacity > o->oOpacity) {
            o->oOpacity += 20;

            if (o->oBooTargetOpacity < o->oOpacity) {
                o->oOpacity = o->oBooTargetOpacity;
            }
        } else {
            o->oOpacity -= 20;

            if (o->oBooTargetOpacity > o->oOpacity) {
                o->oOpacity = o->oBooTargetOpacity;
            }
        }
    }

    scale = (o->oOpacity / 255.0f * 0.4 + 0.6) * o->oBooBaseScale;
    obj_scale(o, scale); // why no cur_obj_scale? was cur_obj_scale written later?
}

static void boo_oscillate(void) {
    o->oFaceAnglePitch = sins(o->oBooOscillationTimer) * 0x400;

    if (o->oOpacity == 255) {
        o->header.gfx.scale[0] = sins(o->oBooOscillationTimer) * 0.08 + o->oBooBaseScale;
        o->header.gfx.scale[1] = -sins(o->oBooOscillationTimer) * 0.08 + o->oBooBaseScale;
        o->header.gfx.scale[2] = o->header.gfx.scale[0];
        o->oGravity = sins(o->oBooOscillationTimer) * o->oBooBaseScale;

        if ((o->oPosY - o->oGraphYOffset - o->oFloorHeight) < (50 * o->oBooBaseScale)) {
            if (o->oGravity < 0)
                o->oGravity = -o->oGravity;
        }

        o->oBooOscillationTimer += 0x400;
    }
}

static s32 boo_vanish_or_appear(void) {
    s16 relativeAngleToMario = abs_angle_diff(o->oAngleToMario, gMarioObject->oFaceAngleYaw);
    s16 relativeMarioFaceAngle = abs_angle_diff(o->oMoveAngleYaw, gMarioObject->oFaceAngleYaw);
    // magic?
    s16 relativeAngleToMarioThreshhold = 0x5000;
    s16 relativeMarioFaceAngleThreshhold = 0x5000;
    s32 doneAppearing = FALSE;

    o->oVelY = 0.0f;

    if (relativeAngleToMario < relativeAngleToMarioThreshhold
        || relativeMarioFaceAngle < relativeMarioFaceAngleThreshhold) {
        o->oBooTargetOpacity = 255;        

        if (o->oOpacity > 180) {
            doneAppearing = TRUE;
        }
    } else {
        o->oBooTargetOpacity = 40;
    }

    return doneAppearing;
}

static void boo_set_move_yaw_for_during_hit(void) {
    cur_obj_become_intangible();

    o->oFlags &= ~OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW;
    o->oBooMoveYawBeforeHit = (f32) o->oMoveAngleYaw;

    if (coss((s16) o->oMoveAngleYaw - (s16) o->oAngleToMario) < 0.0f) {
        o->oBooMoveYawDuringHit = o->oMoveAngleYaw;
    } else {
        o->oBooMoveYawDuringHit = (s16) (o->oMoveAngleYaw + 0x8000);
    }
}

static void boo_move_during_hit(f32 fVel) {
    // Boos seem to have been supposed to oscillate up then down then back again
    // when hit. However it seems the programmers forgot to scale the cosine,
    // so the Y velocity goes from 1 to -1 and back to 1 over 32 frames.
    // This is such a small change that the Y position only changes by 5 units.
    // It's completely unnoticable in-game.
    s32 oscillationVel = o->oTimer * 0x800 + 0x800;

    o->oForwardVel = fVel;
    o->oVelY = coss(oscillationVel);
    o->oMoveAngleYaw = o->oBooMoveYawDuringHit;
    o->oFaceAngleYaw += sBooHitRotations[o->oTimer];
    o->oFaceAngleRoll += sBooHitRotations[o->oTimer];
}

static void boo_reset_after_hit(void) {
    o->oMoveAngleYaw = o->oBooMoveYawBeforeHit;
    o->oFlags |= OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW;
    o->oInteractStatus = 0;
}

// called iff boo/big boo/cage boo is in action 2, which only occurs if it was non-attack-ly interacted with/bounced on?
static s32 boo_update_after_bounced_on(void) {
    o->oForwardVel = 0.0f;
    o->oVelY = 0.0f;

    if (o->oTimer == 0) {
        boo_set_move_yaw_for_during_hit();
    }

    if (o->oTimer < 32) {
        boo_move_during_hit(sBooHitRotations[o->oTimer] / 5000.0f);
    } else {
        cur_obj_become_tangible();
        boo_reset_after_hit();
        o->oAction = 1;

        return TRUE;
    }

    return FALSE;
}

// called every frame once mario lethally hits the boo until the boo is deleted,
// returns whether death is complete
static s32 boo_update_during_death(void) {
    struct Object *parentBigBoo;

    if (o->oTimer == 0) {
        o->oForwardVel = 40.0f;
        o->oMoveAngleYaw = gMarioObject->oMoveAngleYaw;
        o->oBooDeathStatus = BOO_DEATH_STATUS_DYING;
        o->oFlags &= ~OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW;
    } else {
        if (o->oTimer == 5) {
            o->oBooTargetOpacity = 250;
        }

        if (o->oTimer > 30 || o->oMoveFlags & 0x200) {
            o->oBooDeathStatus = BOO_DEATH_STATUS_DEAD;

            if (o->oBooParentBigBoo != NULL) {
                parentBigBoo = o->oBooParentBigBoo;
                parentBigBoo->oBigBooNumMinionBoosKilled++;
            }

            return TRUE;
        }
    }

    o->oVelY = 5.0f;
    o->oFaceAngleRoll += 0x800;
    o->oFaceAngleYaw += 0x800;

    return FALSE;
}

static s32 boo_get_attack_status(void) {
    s32 attackStatus = BOO_NOT_ATTACKED;

    if (o->oInteractStatus & INT_STATUS_INTERACTED) {
        if (o->oInteractStatus & INT_STATUS_WAS_ATTACKED) {
            cur_obj_become_intangible();

            o->oInteractStatus = 0;

            attackStatus = BOO_ATTACKED;
        } else {
            o->oInteractStatus = 0;

            attackStatus = BOO_BOUNCED_ON;
        }
    }

    return attackStatus;
}

// boo idle/chasing movement?
static void boo_chase_mario(f32 a0, s16 a1) {
    f32 sp1C;
    s16 sp1A;

    if (boo_vanish_or_appear()) {
        o->oInteractType = 0x8000;

        if (cur_obj_lateral_dist_from_mario_to_home() > 1500.0f) {
            sp1A = cur_obj_angle_to_home();
        } else {
            sp1A = o->oAngleToMario;
        }

        cur_obj_rotate_yaw_toward(sp1A, a1);
        o->oVelY = 0.0f;

        if (mario_is_in_air_action() == 0) {
            sp1C = o->oPosY - gMarioObject->oPosY;
            if (a0 < sp1C && sp1C < 500.0f) {
                o->oVelY =
                    increment_velocity_toward_range(o->oPosY, (gMarioObject->oPosY + 60.0f*o->oBooBaseScale), 10.f, 2.0f);
            }
        }

        cur_obj_set_vel_from_mario_vel(10.0f, 0.5f);

        if (o->oForwardVel != 0.0f) {
            boo_oscillate();
        }
    } else {
        o->oInteractType = 0;
        o->oForwardVel = 0.0f;
        o->oVelY = 0.0f;
        o->oGravity = 0.0f;
    }
}

static void boo_act_0(void) {
    cur_obj_set_pos_to_home();
    
    o->oBooBaseScale = 1.0f;
    o->oBooTargetOpacity = 0xFF;

    if (boo_should_be_active())
        o->oAction = 1;
}

static void boo_act_1(void) {
    s32 attackStatus;

    boo_chase_mario(-100.0f, 0x200 + gDebugInfo[DEBUG_PAGE_EFFECTINFO][3]);
    attackStatus = boo_get_attack_status();

    if (o->activeFlags & ACTIVE_FLAG_IN_DIFFERENT_ROOM) {
        o->oAction = 0;
    }

    if (attackStatus == BOO_BOUNCED_ON) {
        o->oAction = 2;
    }

    if (attackStatus == BOO_ATTACKED) {
        o->oAction = 3;
    }
}

static void boo_act_2(void) {
    if (boo_update_after_bounced_on()) {
        o->oAction = 1;
    }
}

static void boo_act_3(void) {
    if (boo_update_during_death()) {
        obj_mark_for_deletion(o);
    }
}

static void (*sBooActions[])(void) = { boo_act_0, boo_act_1, boo_act_2, boo_act_3};

void bhv_boo_loop(void) {
    // PARTIAL_UPDATE

    cur_obj_update_floor_and_walls();
    cur_obj_call_action_function(sBooActions);
    cur_obj_move_standard(-78);
    boo_approach_target_opacity_and_update_scale();
}

void obj_set_secondary_camera_focus(void) {
    gSecondCameraFocus = o;
}

void bhv_animated_texture_loop(void) {
    cur_obj_set_pos_to_home_with_debug();
}
