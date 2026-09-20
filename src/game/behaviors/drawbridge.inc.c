// drawbridge.inc.c

void bhv_lll_drawbridge_spawner_loop(void) { // s_motosbridge_main
    struct Object *drawbridge1, *drawbridge2;

    drawbridge1 = spawn_object(o, MODEL_LLL_DRAWBRIDGE_PART, bhvLLLDrawbridge);
    drawbridge1->oMoveAngleYaw = o->oMoveAngleYaw;
    drawbridge1->oPosX += coss(o->oMoveAngleYaw) * 640.0f;
    drawbridge1->oPosZ += sins(o->oMoveAngleYaw) * 640.0f;

    drawbridge2 = spawn_object(o, MODEL_LLL_DRAWBRIDGE_PART, bhvLLLDrawbridge);
    drawbridge2->oMoveAngleYaw = o->oMoveAngleYaw + 0x8000;
    drawbridge2->oPosX += coss(o->oMoveAngleYaw) * -640.0f;
    drawbridge2->oPosZ += sins(o->oMoveAngleYaw) * -640.0f;

    o->activeFlags = ACTIVE_FLAG_DEACTIVATED;
}

void bhv_lll_drawbridge_loop(void) { // s_motos_bridge (modified)
    s32 globalTimer = gGlobalTimer;

    switch (o->oAction) {
        case LLL_DRAWBRIDGE_ACT_LOWER:
            o->oFaceAngleRoll += 0x100;
            break;

        case LLL_DRAWBRIDGE_ACT_RAISE:
            o->oFaceAngleRoll -= 0x100;
            break;
    }

    if ((s16) o->oFaceAngleRoll < -8189) { // Open
        o->oFaceAngleRoll = degree(315);

        //! Because the global timer increments when the game is paused, pausing and unpausing
        //  the game at regular intervals can leave the drawbridge raised indefinitely.
        //  The drawbridge takes longer to move in the demo, after 80 frames instead of 50.
        if (o->oTimer > 80 && (globalTimer & 0x07) == 0) {
            o->oAction = LLL_DRAWBRIDGE_ACT_LOWER;
            // No sound when moving
        }
    }

    if ((s16) o->oFaceAngleRoll >= 0) { // Closed
        o->oFaceAngleRoll = degree(0);

        //! Because the global timer increments when the game is paused, pausing and unpausing
        //  the game at regular intervals can leave the drawbridge lowered indefinitely.
        // The drawbridge takes longer to move in the demo, after 80 frames instead of 50.
        if (o->oTimer > 80 && (globalTimer & 0x07) == 0) {
            o->oAction = LLL_DRAWBRIDGE_ACT_RAISE;
            // No sound when moving
        }
    }
}
