// tuxie.c.inc

// This function is inside 'pathping.p' in the iQue source
s32 set_obj_anim_with_accel_and_sound(s16 f1, s16 f2, s32 sound) { // s_anime_sound
    f32 speed;

    if ((speed = o->header.gfx.animInfo.animAccel / (f32) 0x10000) == 0) {
        speed = 1.0f;
    }

    if (cur_obj_check_anim_frame_in_range(f1, speed) || cur_obj_check_anim_frame_in_range(f2, speed)) {
        cur_obj_play_sound_2(sound);
        return TRUE;
    }

    return FALSE;
}

void play_penguin_walking_sound(s32 walk) { // sound_pingwalk
    s32 sound;

    if (o->oSoundStateID == 0) {
        if (walk == PENGUIN_WALK_BABY) {
            sound = SOUND_OBJ_BABY_PENGUIN_WALK;
        } else { // PENGUIN_WALK_BIG
            sound = SOUND_OBJ_BIG_PENGUIN_WALK;
        }
        set_obj_anim_with_accel_and_sound(1, 11, sound);
    }
}

// Mother penguin code

static void tuxies_mother_act_2(void) { // ping_oya_end (modified)
    f32 objDist;
    UNUSED f32 returnDist;
    struct Object *objBaby = cur_obj_find_nearest_object_with_behavior(bhvSmallPenguin, &objDist);

    if (cur_obj_find_nearby_held_actor(bhvUnused20E0, 1000.0f) != NULL) {
        if (o->oSubAction == 0) {
            cur_obj_init_animation_with_sound(0);
            o->oForwardVel = 10.0f;
            // The mother penguin can chase Mario much farther than the final game
            if (cur_obj_lateral_dist_from_mario_to_home() > 2000.f) {
                o->oSubAction = 1;
            }
            cur_obj_rotate_yaw_toward(o->oAngleToMario, 0x400);
            // This if statement is so the mother penguin doesn't touch the rightmost fences in Snow Slider
            if (o->oPosX > 8000.f) {
                o->oPosX = 8000.f;
            }
        } else {
            o->oForwardVel = 0.0f;
            cur_obj_init_animation_with_sound(3);
            // Distance needs to increase here to work with the earlier higher range
            if (cur_obj_lateral_dist_from_mario_to_home() < 1500.0f) {
                o->oSubAction = 0;
            }
        }
    } else {
        o->oForwardVel = 0.0f;
        cur_obj_init_animation_with_sound(3);
    }

    if (objBaby != NULL && objDist < 300.0f && objBaby->oHeldState != HELD_FREE) {
        o->oAction = 1;
        o->prevObj = objBaby;
    }
}

static void tuxies_mother_act_1(void) { // ping_oya_kodomo_find (modified)
    s32 motherParam;
    s32 babyParam;
    // No dialogID variable

    switch (o->oSubAction) {
        case 0:
            cur_obj_init_animation_with_sound(3);

            if (!cur_obj_is_mario_on_platform()) {
                motherParam = (o->oBhvParams >> 16) & 0xFF;
                babyParam = (o->prevObj->oBhvParams >> 16) & 0xFF;

                // Simpler than the final game, just sets the sub action directly with no dialog
                if (motherParam == babyParam) {
                    o->oSubAction = 1;
                } else {
                    o->oSubAction = 2;
                }

                o->prevObj->oInteractionSubtype |= INT_SUBTYPE_DROP_IMMEDIATELY;
            } else {
                cur_obj_init_animation_with_sound(0);
            }
            break;
        case 1:
            if (o->prevObj->oHeldState == HELD_FREE) {
                //! This line is was almost certainly supposed to be something
                // like o->prevObj->oInteractionSubtype &= ~INT_SUBTYPE_DROP_IMMEDIATELY;
                // however, this code uses the value of o->oInteractionSubtype
                // rather than its offset to rawData. For this object,
                // o->oInteractionSubtype is always 0, so the result is this:
                // o->prevObj->oUnknownUnk88 &= ~INT_SUBTYPE_DROP_IMMEDIATELY
                // which has no effect as o->prevObj->oUnknownUnk88 is always 0
                // or 1, which is not affected by the bitwise AND.
                o->prevObj->OBJECT_FIELD_S32(o->oInteractionSubtype) &= ~INT_SUBTYPE_DROP_IMMEDIATELY;
                obj_set_behavior(o->prevObj, bhvUnused20E0);
                spawn_object_abs_with_rot(o, 0, MODEL_STAR, bhvStar, 7168, -3795, 6141, 0, 0, 0);
                o->oAction = 2;
            }
            break;
        case 2:
            if (o->prevObj->oHeldState == HELD_FREE) {
                //! Same bug as above
                o->prevObj->OBJECT_FIELD_S32(o->oInteractionSubtype) &= ~INT_SUBTYPE_DROP_IMMEDIATELY;
                obj_set_behavior(o->prevObj, bhvPenguinBaby);
                o->oAction = 2;
            }
            break;
    }
}

static void tuxies_mother_act_0(void) { // ping_oya_wait (modified)
    // Only used for the first frame the mother penguin spawns to set the scale
    cur_obj_scale(4.0f);
    cur_obj_init_animation_with_sound(3);

    o->oAction = 2;
}

static void (*sTuxiesMotherActions[])(void) = { // ping_oya_modejmp
    tuxies_mother_act_0,
    tuxies_mother_act_1,
    tuxies_mother_act_2,
};

void bhv_tuxies_mother_loop(void) { // s_ping_oya
    o->activeFlags |= ACTIVE_FLAG_UNK10;
    cur_obj_update_floor_and_walls();
    cur_obj_call_action_function(sTuxiesMotherActions);
    cur_obj_move_standard(-78);
    play_penguin_walking_sound(PENGUIN_WALK_BIG);
    o->oInteractStatus = 0;
}

// Baby penguin code

static void small_penguin_dive_with_mario(void) { // ping_slip_check
    if (mario_is_dive_sliding()) {
        o->oSmallPenguinUnk100 = o->oAction;
        o->oAction = 3;
    }
}

static void small_penguin_act_2(void) { // ping_kodomo_runaway (modified)
    // No 'rflag' variable since that's only used for action 5
    cur_obj_init_animation_with_sound(0);
    o->oForwardVel = o->oSmallPenguinUnk104 + 3.0f;
    cur_obj_rotate_yaw_toward(o->oAngleToMario + 0x8000, o->oSmallPenguinUnk110 + 0x600);
    if (o->oDistanceToMario > o->oSmallPenguinUnk108 + 500.0f)
        o->oAction = 0;
    small_penguin_dive_with_mario();
}

static void small_penguin_act_1(void) { // ping_kodomo_search
    cur_obj_init_animation_with_sound(0);
    o->oForwardVel = o->oSmallPenguinUnk104 + 3.0f;
    cur_obj_rotate_yaw_toward(o->oAngleToMario, o->oSmallPenguinUnk110 + 0x600);
    if (o->oDistanceToMario < o->oSmallPenguinUnk108 + 300.0f) {
        o->oAction = 0;
    }
    if (o->oDistanceToMario > 1100.0f) {
        o->oAction = 0;
    }
    small_penguin_dive_with_mario();
}

static void small_penguin_act_3(void) { // ping_kodomo_slip (modified)
    if (o->oTimer > 5) {
        // No sound when diving, seen in SuperMarioStadium2 (16:28)
        cur_obj_init_animation_with_sound(1);
        if (o->oTimer > 25) {
            if (!mario_is_dive_sliding()) {
                o->oAction = 4;
            }
        }
    }
}

static void small_penguin_act_4(void) { // ping_kodomo_slip_end
    if (o->oTimer > 20) {
        o->oForwardVel = 0.0f;
        cur_obj_init_animation_with_sound(2);
        if (o->oTimer > 40) {
            o->oAction = o->oSmallPenguinUnk100;
        }
    }
}

static void small_penguin_act_0(void) { // ping_kodomo_wait (modified)
    // No 'rflag' variable since that's only used for action 5
    s32 babyParam = (o->oBhvParams >> 16) & 0xFF;

    cur_obj_init_animation_with_sound(3);

    // This check is so the "dud" baby penguins (the ones that don't give a star) are already
    // freed, making the mother penguin chase Mario if one of these penguins is grabbed
    if (babyParam == 2) {
        cur_obj_set_behavior(bhvUnused20E0);
    }

    if (o->oTimer == 0) {
        o->oSmallPenguinUnk110 = (s32) (random_float() * 0x400);
        o->oSmallPenguinUnk108 = random_float() * 100.0f;
        o->oSmallPenguinUnk104 = random_float();
        o->oForwardVel = 0.0f;
    }

    if (o->oDistanceToMario < 1000.0f && o->oDistanceToMario > o->oSmallPenguinUnk108 + 600.0f) {
        o->oAction = 1;
    } else if (o->oDistanceToMario < o->oSmallPenguinUnk108 + 150.0f) { // 150 instead of 300
        o->oAction = 2;
    }

    if (cur_obj_mario_far_away()) {
        cur_obj_set_pos_to_home();
    }
}

// No 'ping_kodomo_oya_loop' (small_penguin_act_5), baby penguins didn't 
// circle around the mother penguin when getting close to her

static void (*sSmallPenguinActions[])(void) = { // ping_kodomo_modejmp (modified)
    small_penguin_act_0,
    small_penguin_act_1,
    small_penguin_act_2,
    small_penguin_act_3,
    small_penguin_act_4
    // No action 5
};

static void small_penguin_free_actions(void) { // s_ping_kodomo_main (modified)
    // Removed check that sets the baby penguin to action 5
    cur_obj_update_floor_and_walls();
    cur_obj_call_action_function(sSmallPenguinActions);
    cur_obj_move_standard(-78);
    play_penguin_walking_sound(PENGUIN_WALK_BABY);
}

void bhv_small_penguin_loop(void) { // s_ping_kodomo (modified)
    switch (o->oHeldState) {
        case HELD_FREE:
            small_penguin_free_actions();
            break;
        case HELD_HELD:
            cur_obj_unrender_set_action_and_anim(0, 0);
            if (cur_obj_has_behavior(bhvPenguinBaby))
                obj_set_behavior(o, bhvSmallPenguin);
            obj_copy_pos(o, gMarioObject);
            // The baby penguin doesn't yell while being held
            break;
        case HELD_THROWN:
            cur_obj_get_thrown_or_placed(0, 0, 0);
            break;
        case HELD_DROPPED:
            cur_obj_get_dropped();
            break;
    }
}

/** Geo switch logic for Tuxie's mother's eyes. Cases 0-4. Interestingly, case
 * 4 is unused, and is the eye state seen in a Shoshinkai 1995 press kit screenshot.
 */
Gfx *geo_switch_tuxie_mother_eyes(s32 run, struct GraphNode *node, UNUSED Mat4 *mtx) { // CtrlPingEye
    if (run == TRUE) {
        struct Object *obj = (struct Object *) gCurGraphNodeObject;
        struct GraphNodeSwitchCase *switchCase = (struct GraphNodeSwitchCase *) node;
        s32 timer;

        switchCase->selectedCase = 0;

        // timer logic for blinking. uses cases 0-2.
        timer = gGlobalTimer % 50;
        if (timer < 43) {
            switchCase->selectedCase = 0;
        } else if (timer < 45) {
            switchCase->selectedCase = 1;
        } else if (timer < 47) {
            switchCase->selectedCase = 2;
        } else {
            switchCase->selectedCase = 1;
        }

        /** make Tuxie's Mother have angry eyes if Mario takes the correct baby
         * after giving it back. The easiest way to check this is to see if she's
         * moving, since she only does when she's chasing Mario.
         */
        if (obj->behavior == segmented_to_virtual(bhvTuxiesMother)) {
            if (obj->oForwardVel > 5.0f) {
                switchCase->selectedCase = 3;
            }
        }
    }

    return NULL;
}
