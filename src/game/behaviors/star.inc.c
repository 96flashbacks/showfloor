// star.inc.c

/*
 * It's unkown where the "e_star" code would be placed exactly, as there is no
 * "pathstar.p" or something similar, with the closest file being "pathclearstar.p".
 * The behavior here is custom, based on how Yajima's other objects were programmed.
*/

static struct ObjectHitbox sStarHitbox = {
    /* interactType:      */ INTERACT_STAR,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 0,
    /* health:            */ 0,
    /* numLootCoins:      */ 0,
    /* radius:            */ 80,
    /* height:            */ 50,
    /* hurtboxRadius:     */ 0,
    /* hurtboxHeight:     */ 0,
};

// Based on 'check_star_backup' (bhv_spawned_star_init), which was programmed by Yajima
void bhv_star_init(void) {
    s32 starIndex;

    o->oBhvParams = o->parentObj->oBhvParams;

    starIndex = (o->oBhvParams >> 24) & 0xFF;

    if (bit_shift_left(starIndex)
        & save_file_get_star_flags(gCurrSaveFileNum - 1, COURSE_NUM_TO_INDEX(gCurrCourseNum))) {
        cur_obj_set_model(MODEL_TRANSPARENT_STAR);
    }

    obj_set_hitbox(o, &sStarHitbox);
}

void bhv_star_loop(void) {
    if (o->oInteractStatus & INT_STATUS_INTERACTED) {
        // Stars spawned a coin sparkle when collected in the Shoshinkai demo
        spawn_object(o, MODEL_SPARKLES, bhvCoinSparkles);
        obj_mark_for_deletion(o);
    }
}

// It's very likely 'spawn_default_star' or any other star spawning function didn't exist at this point.
// Even the Boo bhv backup from 1996 (pathminiteresa.p), with the 3D star, still manually 
// spawned the star with 's_makeobj_absolute' (spawn_object_abs_with_rot)
