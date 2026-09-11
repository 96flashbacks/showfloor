// flame_mario.inc.c

// No 's_make_smoke' (bhv_black_smoke_upward_loop) or 's_enemy_firesmoke_parts' (bhv_black_smoke_bowser_loop), 
// they are only used after Bowser's flames fizzle out in the final game, but those flames spawn nothing in the demo

void bhv_black_smoke_mario_loop(void) { // s_firesmoke_smoke
    if (o->oTimer == 0) {
        cur_obj_set_pos_relative(gMarioObject, 0, 0, -30.0f);

        o->oForwardVel = random_float() * 2 + 0.5;
        o->oMoveAngleYaw = (gMarioObject->oMoveAngleYaw + 0x7000) + random_float() * 8192.0f;
        o->oVelY = 8.0f;
    }

    o->oMoveAngleYaw += o->oAngleVelYaw;
    o->oPosY += o->oVelY;
}

void bhv_flame_mario_loop(void) { // s_firesmoke (modified)
    cur_obj_scale(2.0f);

    if (o->oTimer != 0) {
        if (o->oTimer & 1) {
            spawn_object(o, MODEL_BURN_SMOKE, bhvBlackSmokeMario);
        }
    }

    gMarioObject->prevObj = o; // weird?
    //obj_set_parent_relative_pos(o, 40, -120, 0);

    // Commented out code from 'pathfiresmoke.p' that manually sets the flame's relative position,
    // functionally identical to using 'obj_set_parent_relative_pos'.
    o->oParentRelativePosX = 40;
    o->oParentRelativePosY = -120;
    o->oParentRelativePosZ = 0;

    if (!(gMarioObject->oMarioParticleFlags & ACTIVE_PARTICLE_FIRE)) {
        o->parentObj->oActiveParticleFlags &= ~ACTIVE_PARTICLE_FIRE;
        obj_mark_for_deletion(o);
        gMarioObject->prevObj = NULL;
    }
}
