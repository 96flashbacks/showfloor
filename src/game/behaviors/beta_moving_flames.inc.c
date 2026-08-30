// beta_moving_flames.inc.c

// Unused behavior file for a moving flames hazard. The decomp says it's a Fishing Boo,
// but that doesn't seem to be the case based on the original file names.
// The original file in the iQue source is called 'pathfire.p' and is dated '1995.9.22',
// indicating it existed at the time of the Shoshinkai demo.

void bhv_beta_moving_flames_spawn_loop(void) { // s_sidefire
    o->oDistanceToMario = lateral_dist_between_objects(o, gMarioObject);
    o->oPosY -= 100.0f;

    switch (o->oAction) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
            spawn_object(o, MODEL_RED_FLAME, bhvBetaMovingFlames);
            o->oAction++;
            break;
        case 8:
            break;
        case 9:
            o->oAction++;
            break;
    }
}

void bhv_beta_moving_flames_loop(void) { // s_sidefireball
    cur_obj_scale(5.0f);

    o->oForwardVel = sins(o->oMovingFlameTimer) * 70.0f;
    o->oMovingFlameTimer += 0x800;
}
