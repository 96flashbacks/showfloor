// blargg.inc.c

/**
 * Leftover behavior from pathunbaba.p, which wasn't included in the final release.
 * It likely still existed in the demo, based on its creation date of '1995.11.20'.
 * The object was abandoned very early, so it barely has any code.
 */

// Animation defines
#define     ANM_unbaba_swim     0
#define     ANM_unbaba_attack   1

// These were never used due to object never being finished
#define unbaba_movemode  o->rawData.asS32[0x1B]
#define unbaba_movecounter o->rawData.asS32[0x1C]

static void blargg_init(void) {
    o->oAction++;
}

static void blargg_swim(void) {
    cur_obj_init_animation_with_sound(ANM_unbaba_swim);
    if (o->oDistanceToMario < 600.0f)	
        o->oAction = 2; // mode_unbaba_attack
}

static void blargg_attack(void) {
    cur_obj_init_animation_with_sound(ANM_unbaba_attack);
    if (cur_obj_check_if_near_animation_end())	
        o->oAction = 1; // mode_unbaba_swim
}

static void (*sBlarggActions[])(void) = {  
   blargg_init,
   blargg_swim,
   blargg_attack,  
};

void bhv_blargg_loop(void) { // s_unbaba
    UNUSED s32 mode;

    cur_obj_scale(2.0f);

    cur_obj_call_action_function(sBlarggActions);
}
