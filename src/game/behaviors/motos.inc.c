// motos.inc.c

/**
 * Behavior from 'pathmotos.p' for Motos, a fully scrapped enemy. It has some small 
 * leftovers in the final game (stubbed out bhv data [bhvStub1D0C in the decomp], 
 * macro_unknown_96 and MODEL_UNKNOWN_58) and likely existed at the time of the demo,
 * based on the bhv file's creation date of '1995.10.17'. It's not fully finished but compared 
 * to other scrapped objects, such as the trampoline or Blargg, it's a lot more complete.
 */

// Animation ID defines
#define ANM_motos_basedata_A    	0 // unused
#define ANM_motos_carry         	1 // unused
#define ANM_motos_carry_run     	2
#define ANM_motos_carry_start   	3
#define ANM_motos_down_recover  	4 // unused
#define ANM_motos_down_stop     	5 // unused
#define ANM_motos_pitch         	6
#define ANM_motos_safe_down     	7 // unused
#define ANM_motos_wait          	8
#define ANM_motos_walk          	9

#define ANM_motos_end          	ANM_motos_walk+1

// This variable is defined in 'pathmotos.p' but never used
#define motos_work o->rawData.asS32[0x1B]

// Identical to 'geo_update_projectile_pos_from_parent', later reused for King Bob-omb and Chuckyas
Gfx *MotosProc1(s32 callContext, UNUSED struct GraphNode *node, Mat4 mtx) {
    if (callContext == GEO_CONTEXT_RENDER) {
        Mat4 modelMtx;
        struct Object *obj = (struct Object *) gCurGraphNodeObject;
        if (obj->prevObj != NULL) {
            create_transformation_from_matrices(modelMtx, mtx, *gCurGraphNodeCamera->matrixPtr);
            obj_update_pos_from_parent_transformation(modelMtx, obj->prevObj);
            obj_set_gfx_pos_from_pos(obj->prevObj);
        }
    }

    return NULL;
}

// Nearly identical to 's_robo_hand' ('bhv_heave_ho_throw_mario_loop' in the decomp)
extern void bhv_motos_hand_loop(void) { // s_motos_hand
    UNUSED struct Object *firep;

    o->oParentRelativePosX = 100.0f;
    o->oParentRelativePosY = 0.0f;
    o->oParentRelativePosZ = 150.0f;

    o->oMoveAngleYaw = o->parentObj->oMoveAngleYaw;

    switch (o->parentObj->oMotosHoldStatus) {
        case 0:
            break;
        case 1:
            obj_set_gfx_pos_at_obj_pos(gMarioObject, o);
            break;
        case 2:
            gMarioObject->oInteractStatus |= INT_STATUS_MARIO_THROWN_BY_OBJ;
            gMarioStates[0].forwardVel = 50.0f;
            o->parentObj->oMotosHoldStatus = 0;
            break;
    }

    // Seems like Motos was planned to throw shells at Mario at some point,
    // doesn't really work with the way the code is set up though
#if 0
    if (o->parentObj->oMotosHoldStatus != 0) {
        o->parentObj->oMotosHoldStatus = 0;
        firep = spawn_object(o, MODEL_GREEN_KOOPA_SHELL, bhvMotosProjectile);
        firep->oForwardVel = 40.0f;
        firep->oVelY = 20.0f;
        firep->oMoveAngleYaw = o->parentObj->oMoveAngleYaw;
    }
#endif
}

// This seems to have been intended as a general object function, based on the
// "Enemy AI Library" comment above it. It's only used here however, 
// and unlike 's_erase_shape' it is actually included in 'pathmotos.p'
static s32 s_ai_pitch(s16 angle,f32 dist) {
    if ((cur_obj_rotate_yaw_toward(o->oAngleToMario, angle)) && (o->oDistanceToMario < dist)) 
        return TRUE;
    else
        return FALSE;
}

static void motos_wait(void) {
    o->oForwardVel = 0;
    o->oVelY = 0;
    cur_obj_init_animation_with_sound(ANM_motos_wait);

    if (o->oDistanceToMario < 500)
        o->oAction = mode_motos_player_search;
}

static void motos_player_search(void) {
    cur_obj_init_animation_with_sound(ANM_motos_walk);

    o->oForwardVel = 2;
    cur_obj_rotate_yaw_toward(o->oAngleToMario, 300);

    if (o->oInteractStatus & INT_STATUS_GRABBED_MARIO) {
        o->oAction = mode_motos_player_carry;
        o->oMotosHoldStatus = 1;
    }
}

static void motos_player_carry(void) {
    cur_obj_init_animation_with_sound(ANM_motos_carry_start);
    
    if (cur_obj_check_if_near_animation_end())	
        o->oAction = mode_motos_player_pitch;
}

static void motos_player_pitch(void) {
    o->oForwardVel = 0;
    
    cur_obj_init_animation_with_sound(ANM_motos_pitch);
    
    if (cur_obj_check_anim_frame(14)) {
        o->oMotosHoldStatus = 2;        /* nageru shyn kan  */
        o->numCollidedObjs = 10;        /* hit time wait!!  */
    }
    
    if (cur_obj_check_if_near_animation_end()) {
        o->oAction = mode_motos_wait;
        o->oInteractStatus &= ~INT_STATUS_GRABBED_MARIO;
    }
}

static void motos_carry_start(void) {
    cur_obj_init_animation_with_sound(ANM_motos_carry_start);
    
    if (cur_obj_check_if_near_animation_end()) {
        if (s_ai_pitch(0x200, 500))
            o->oAction = mode_motos_pitch;
        else
            o->oAction = mode_motos_carry_run;
    }

}

static void motos_carry_run(void) {
    o->oForwardVel = 5;

    cur_obj_init_animation_with_sound(ANM_motos_carry_run);
    
    if (s_ai_pitch(0x200, 500))
        o->oAction = mode_motos_pitch;
    else
        o->oAction = mode_motos_carry_run;
}

static void motos_pitch(void) {
    o->oForwardVel = 0;
    cur_obj_init_animation_with_sound(ANM_motos_pitch);

    if (cur_obj_check_anim_frame(14)) {
        o->oMotosHoldStatus = 0;
    }
    if (cur_obj_check_if_near_animation_end())	
        o->oAction = mode_motos_wait;

}

static void motos_fly(void) {
    cur_obj_init_animation_with_sound(ANM_motos_walk);
    
    if (o->oMoveFlags & OBJ_MOVE_ON_GROUND)
        o->oAction = mode_motos_wait;
}

static void motos_main(void) {
    // It appears as if both of the general obj movement functions were
    // copied from Ukiki, based on the "monky" comments. Weirdly enough 
    // 'pathmonky.p' doesn't have these comments on the equivalent functions
    cur_obj_update_floor_and_walls();   /*  monky bgcheck   */

    // An action function array could've been used here, strange
    switch (o->oAction) {
        case mode_motos_wait:
            motos_wait();
            break;
        case mode_motos_player_search:
            motos_player_search();
            break;
        case mode_motos_player_carry:
            motos_player_carry();
            break;
        case mode_motos_player_pitch:
            motos_player_pitch();
            break;
        case mode_motos_carry_start:
            motos_carry_start();
            break;
        case mode_motos_carry_run:
            motos_carry_run();
            break;
        case mode_motos_pitch:
            motos_pitch();
            break;
        case mode_motos_fly:
            motos_fly();
            break;
        default: 
            rmonpf(("Error objmode motos\n")); 
    }

    cur_obj_move_standard(-78);    /*   monky moving    */
}

// Included in Motos' bhv since it's only used here. 
// There's no code for this function in the iQue source, 
// so this is custom. Based on 'cur_obj_hide_if_mario_far_away_y'
static s32 s_erase_shape(f32 currentDist, f32 maxDist) {
    if (currentDist < maxDist) {
        cur_obj_unhide();
        return FALSE;
    } else {
        cur_obj_hide();
        return TRUE;
    }
}

extern void bhv_motos_loop(void) { // s_motos
    f32 f = 5.0f;
    f32 y = 0.0f;

    cur_obj_scale(2.0f);

    o->oInteractionSubtype |= INT_SUBTYPE_GRABS_MARIO;

    switch (o->oHeldState) {
        case HELD_FREE:
            motos_main();
            break;
        case HELD_HELD:
            cur_obj_unrender_set_action_and_anim(ANM_motos_walk,mode_motos_wait);	
            break;
        case HELD_THROWN:
            cur_obj_get_thrown_or_placed(f, y, mode_motos_fly);
            break;
     //	case HELD_DROPPED:
     //     cur_obj_get_dropped();  
     //     break;
        case HELD_DROPPED:
            cur_obj_get_thrown_or_placed(f, y, mode_motos_fly);
            break;
    }

    //! Missing 'o->oInteractStatus = 0', meaning if Mario gets grabbed by Motos
    //! (which changes the InteractStatus) Motos will become completely intangible

    // This 's_erase_shape' function is only called in 'pathmotos.p' 
    // and isn't included in the final game, thus it has no decomp equivalent
    s_erase_shape(o->oDistanceToMario, 2000.0f);
}
