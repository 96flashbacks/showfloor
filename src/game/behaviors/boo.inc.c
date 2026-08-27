// boo.c.inc

// The behavior here is based on 'pathminiteresa.p', an earlier backup of the boo code

// no obake_speedstop, s_obake_reset and teresa_areaout_check (boo_stop, bhv_boo_init and boo_should_be_stopped)

static s32 boo_should_be_active(void) { // obake_appearcheck
    // 'activationRadius' is always 1500
    
    // No merry go round Boo check
    if (o->oRoom == -1) {
        if (o->oDistanceToMario < 1500)
            return TRUE;
    } else {
        if (o->oDistanceToMario < 1500 && (o->oRoom == gMarioCurrentRoom)) // No 'gMarioCurrentRoom == 0' check
            return TRUE;
    }
    
    return FALSE;
}

// Triplet positions have been edited to match the footage more closely
static s16 sCourtyardBooTripletPositions[][3] = { // teredata
    { 210, 100, 430 },
    { 0, 90, 240 },
    { -210, 120, -210 }
};

void bhv_courtyard_boo_triplet_init(void) { // s_make3teresa
    s32 i;
    struct Object *boo;

    // No OBAKESET_STARCOIN check, courtyard Boos always spawn regardless of star count
    for (i = 0; i < 3; i++) {
        boo = spawn_object_relative(
                BOO_BP_GENERIC, sCourtyardBooTripletPositions[i][0], sCourtyardBooTripletPositions[i][1],
                sCourtyardBooTripletPositions[i][2], o, MODEL_BOO, bhvGhostHuntBoo);
        boo->oMoveAngleYaw = random_u16();
    }

}

static void boo_approach_target_opacity_and_update_scale(void) { // oba_effect
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
    obj_scale(o, scale); // why no cur_obj_scale? was cur_obj_scale written later? (probably)

}

// Missing 'attackStatus' ('ignoreOpacity' in the decomp)
static void boo_oscillate(void) { // oba_animation_scale
    o->oFaceAnglePitch = sins(o->oBooOscillationTimer) * 0x400;

    if (o->oOpacity == 255) {
        o->header.gfx.scale[0] = sins(o->oBooOscillationTimer) * 0.08 + o->oBooBaseScale;
        o->header.gfx.scale[1] = -sins(o->oBooOscillationTimer) * 0.08 + o->oBooBaseScale;
        o->header.gfx.scale[2] = o->header.gfx.scale[0];
        o->oGravity = sins(o->oBooOscillationTimer) * o->oBooBaseScale;

    // This code is commented out in 'pathminiteresa.p' and 'pathteresa.p'.
    // It makes the Boo's oscillation more static and jittery
    if ((o->oPosY - o->oGraphYOffset - o->oFloorHeight) < (50.0f * o->oBooBaseScale)) {
        if (o->oGravity < 0) 
            o->oGravity = -o->oGravity;
    }

        o->oBooOscillationTimer += 0x400;
    }

}

static s32 boo_vanish_or_appear(void) { // teresa_modecheck
    // Checks the difference of o->oAngleToMario to Mario's oFaceAngleYaw instead of the Boo's oMoveAngleYaw
    s16 relativeAngleToMario = abs_angle_diff(o->oAngleToMario,gMarioObject->oFaceAngleYaw);
    s16 relativeMarioFaceAngle = abs_angle_diff(o->oMoveAngleYaw,gMarioObject->oFaceAngleYaw);
    // Different threshholds
    s16 relativeAngleToMarioThreshhold = 0x5000;
    s16 relativeMarioFaceAngleThreshhold = 0x5000;
    s32 doneAppearing = FALSE;

    o->oVelY = 0.0f;

    if (relativeAngleToMario < relativeAngleToMarioThreshhold 
        || relativeMarioFaceAngle < relativeMarioFaceAngleThreshhold) {
        // No 'o->oOpacity == 40' check
        o->oBooTargetOpacity = 255;
        // Missing the laughing sound effect
        if (o->oOpacity > 180) 
            doneAppearing = TRUE;
    } else { // Missing 'o->oOpacity == 255' check
        o->oBooTargetOpacity = 40;
    }

    return doneAppearing;
}

// MT_PULU_TIME is 8 instead of (8*2)
#define MT_ROLL_TIME   32   /* obake to player ga atatta toki!!   */
#define MT_PULU_TIME   8   /* obake to player ga atatta toki!!   */
#define MT_PULU_SPEED   0x400  
#define MT_ROLL_SPEED_Y   3   /* rolling back speed Y      */

// Missing TERESA_DAMAGE_ROLL and TERESA_ATTACK_ROLL defines

// Missing 'attackStatus' ('hurt' in the decomp)
static void boo_set_move_yaw_for_during_hit(void) { // obroll_init
    cur_obj_become_intangible();
    
    o->oFlags &= ~OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW;
    o->oBooMoveYawBeforeHit = o->oMoveAngleYaw;
    
    // No 'attackStatus' check or setting oBooMoveYawDuringHit to Mario's oMoveAngleYaw
    if (coss((s16) o->oMoveAngleYaw - (s16) o->oAngleToMario) < 0.0f) {
        o->oBooMoveYawDuringHit = o->oMoveAngleYaw;
    } else {
        o->oBooMoveYawDuringHit = (s16) (o->oMoveAngleYaw + 0x8000);
    }

}

// Missing 'attackStatus' ('roll' in the decomp)
static void boo_move_during_hit(f32 fVel) { // obroll_roll
    s32 oscillationVel;
    oscillationVel = (o->oTimer + 1) * (0x10000 / MT_ROLL_TIME);

    o->oForwardVel = fVel;  
    o->oVelY = coss(oscillationVel);
    o->oMoveAngleYaw = o->oBooMoveYawDuringHit;
    // No 'attackStatus' check
    o->oFaceAngleYaw += sBooHitRotations[o->oTimer];
    o->oFaceAngleRoll += sBooHitRotations[o->oTimer];
}

static void big_boo_shake_after_hit(void) { // obroll_rollend
    // Oscillate yaw
    s32 oscillationVel = (o->oTimer - MT_ROLL_TIME + 1) * 0x10000 / MT_PULU_TIME;
    o->oFaceAngleYaw += coss(oscillationVel) * MT_PULU_SPEED;
}

static void boo_reset_after_hit(void) { // obroll_exit
    o->oMoveAngleYaw = o->oBooMoveYawBeforeHit;
    o->oFlags |= OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW;
    o->oInteractStatus = 0;
}

// Missing 'obake_winstop' ('boo_update_after_bounced_on' in the decomp)

// This function is only used for Big Boo in the final game, while it's also used for regular Boos in 'pathminiteresa.p',
// So this function is called 'big_boo_update_during_nonlethal_hit' in the decomp
static s32 boo_update_during_nonlethal_hit(void) { // obake_roll
    // No 'obake_speedstop' ('boo_stop' in the decomp), velocities are manually set to 0
    o->oForwardVel = 0;
    o->oVelY = 0;
    // Gravity isn't set to 0 here, it is in the final's 'boo_stop' function

    if (o->oTimer == 0)       
        boo_set_move_yaw_for_during_hit();

    if (o->oTimer <  MT_ROLL_TIME) {
        boo_move_during_hit(sBooHitRotations[o->oTimer] / 5000.0f); // Missing 'speed' variable mutiplier
    } else if (o->oTimer < (MT_ROLL_TIME)+(MT_PULU_TIME)) {
        big_boo_shake_after_hit();
    } else {
        cur_obj_become_tangible();
        boo_reset_after_hit();
        o->oAction = 1; // mode_oba_playersearch

        return TRUE;
    }

    return FALSE;
}

#define MT_REMOVEDEMO_TIME 30

static s32 boo_update_during_death(void) { // obake_hitaway (modified)
    if (o->oTimer == 0) { 
        o->oForwardVel = 40;
        o->oMoveAngleYaw = gMarioObject->oMoveAngleYaw;
        o->obake_flag = 1; // 'obake_flag' is set to 1 instead of 'oBooDeathStatus', based on 'pathobakecoin.p'
        o->oFlags &= ~OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW;
    } else {
        if (o->oTimer == MT_REMOVEDEMO_TIME - 25) {
            // Opacity is set to 240 instead of 0 to make Boos barely fade when dying, like the B-Roll footage
            o->oBooTargetOpacity = 240;
        }

        if (o->oTimer > MT_REMOVEDEMO_TIME || o->oMoveFlags & OBJ_MOVE_HIT_WALL) {
            // Removed 'spawn_mist_particles()';

            // Removed ghost hunt Boo handling
            return TRUE;
        }
    }

    o->oVelY = 5.0f;
    o->oFaceAngleRoll += 0x800;
    o->oFaceAngleYaw += 0x800;

    return FALSE;
}

// Missing 's_check_hitpattern' ('obj_has_attack_type' in the decomp)

static s32 boo_get_attack_status(void) { // oba_hitcheck (modified)
    s32 attackStatus = BOO_NOT_ATTACKED;

    if ((o->oInteractStatus & INT_STATUS_INTERACTED) != 0) {
        // No '!obj_has_attack_type(ATTACK_FROM_ABOVE)' check
        if (o->oInteractStatus & INT_STATUS_WAS_ATTACKED) {
            cur_obj_become_intangible();
            
            o->oInteractStatus = 0;
            
            // Removed laugh sound effect
            
            attackStatus = BOO_ATTACKED;
        } else {
            // Removed bounce sound effect

            o->oInteractStatus = 0;

            attackStatus = BOO_BOUNCED_ON;
        }

    }

    return attackStatus;
}

// Missing 'chasespeed' variable, ('velMultiplier' in the decomp)
static void boo_chase_mario(f32 ymin,s16 anglespeed) { // obake_mainmove
    f32 Ydist;
    s16 targetYaw;

    if (boo_vanish_or_appear()) {
        o->oInteractType = INTERACT_BOUNCE_TOP;

        if (cur_obj_lateral_dist_from_mario_to_home() > 1500.0f) {
            targetYaw = cur_obj_angle_to_home();
        } else {
            targetYaw = o->oAngleToMario;
        }

        cur_obj_rotate_yaw_toward(targetYaw,anglespeed);

        o->oVelY = 0.0f;
        if (!mario_is_in_air_action()) {
            Ydist =  o->oPosY - gMarioObject->oPosY;
            if (ymin < Ydist && Ydist < 500.0f) {
                // Targets Mario's position + 60 * oBooBaseScale instead of just a + 50 increase to Mario's position
                o->oVelY = increment_velocity_toward_range(
                                o->oPosY, (gMarioObject->oPosY + 60.0f * o->oBooBaseScale), 10.0f, 2.0f);
            }
        }
        
        // 10.0f isn't decreased by oBooNegatedAggressiveness, multiplier is a flat 0.5
        cur_obj_set_vel_from_mario_vel(10.0f, 0.5f);

        if (o->oForwardVel != 0.0f) {
            boo_oscillate();
        }
    } else {
        o->oInteractType = 0;
        // No 'boo_stop', though the final game doesn't use it here either
        o->oForwardVel = 0.0f;
        o->oVelY = 0.0f;
        o->oGravity = 0.0f; // Gravity is reset, unlike 'boo_update_during_nonlethal_hit'
    }
}

// Missing OBAKECODE_WAKIDASHI define

static void boo_init(void) { // (modified)
    // No 'OBJECT_GHOST' flag ('ACTIVE_FLAG_MOVE_THROUGH_GRATE' in the decomp)

    // No 'OBAKECODE_WAKIDASHI' ('BOO_BP_MERRY_GO_ROUND' in the decomp) check

    cur_obj_set_pos_to_home();

    // 'oba_startangle' (oBooInitialMoveYaw) doesn't exist so there's no assignment
    // No 'obake_speedstop' ('boo_stop' in the decomp), in this case there's no manual reset

    // Removed big Boo parent assignment
    o->oBooBaseScale = 1.0f;
    o->oBooTargetOpacity = 255;
    if (boo_should_be_active()) 
        // No 'OBAKECODE_WAKIDASHI' ('BOO_BP_MERRY_GO_ROUND' in the decomp) check here either
        o->oAction = 1; // mode_oba_playersearch
}

// Missing oba_datamove

static void boo_playersearch(void) {
    s32 attackStatus;

    // Turning speed is set to '0x200 + effect_p3' instead of 'oBooTurningSpeed + 0x180'
    boo_chase_mario(-100.0f, 0x200 + gDebugInfo[DEBUG_PAGE_EFFECTINFO][3]);
    attackStatus = boo_get_attack_status();

    // Simple flag check instead of 'boo_should_be_stopped()'
    if (o->activeFlags & ACTIVE_FLAG_IN_DIFFERENT_ROOM) {
        o->oAction = 0; // mode_oba_init
    }

    if (attackStatus == BOO_BOUNCED_ON) {
        o->oAction = 2; // mode_oba_roll
    }

    if (attackStatus == BOO_ATTACKED) {
        o->oAction = 3; // mode_oba_hitaway
    }

    // No death sound effect (NA_SE3_MOGURA_HIT, SOUND_OBJ_DYING_ENEMY1)
}

static void boo_roll(void) {
    if (boo_update_during_nonlethal_hit()) {
        o->oAction = 1; // mode_oba_playersearch
    }
}

static void boo_hitaway(void) { // (modified)
    if (boo_update_during_death()) {
        // Removed ghost hunt Boo message
        obj_mark_for_deletion(o);
    }
}

// Removed oba_endmessage (boo_act_4)

static void (*sBooActions[])(void) = { // oba_modejmp (modified)   
    boo_init,
    boo_playersearch,
    boo_roll,
    boo_hitaway,
};

void bhv_boo_loop(void) { // s_oba
    cur_obj_update_floor_and_walls();
    cur_obj_call_action_function(sBooActions);
    cur_obj_move_standard(-78); // -78 instead of 78
    boo_approach_target_opacity_and_update_scale();

    // Missing merry go round check
}

// Removed Big Boo and Boo with cage
