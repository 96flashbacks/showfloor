// castle_floor_trap.c.inc

void bhv_floor_trap_in_castle_loop(void) { // s_mainroom_trap_parts
    if (gMarioObject->platform == o) {
        o->parentObj->oInteractStatus |= INT_STATUS_TRAP_TURN;
    }
    o->oFaceAngleRoll = o->parentObj->oFaceAngleRoll;
}

void bhv_castle_floor_trap_init(void) { // s_mainroom_trap_init (modified)
    struct Object *sp2C;
    // Different X offset since the platforms are thinner, and the original floor trap model ID (172)
    sp2C = spawn_object_relative(0, -307, 0, 0, o, MODEL_CASTLE_FLOOR_TRAP, bhvFloorTrapInCastle);
    sp2C = spawn_object_relative(0,  307, 0, 0, o, MODEL_CASTLE_FLOOR_TRAP, bhvFloorTrapInCastle);
    sp2C->oMoveAngleYaw += 0x8000;
}

void bhv_castle_floor_trap_open_detect(void) { // trap_wait
    if (gMarioStates[0].action == ACT_SPECIAL_EXIT_AIRBORNE
        || gMarioStates[0].action == ACT_SPECIAL_DEATH_EXIT) {
        o->oAction = 4; // rotates trapdoor so it looks always open
    } else {
        o->oAngleVelRoll = 0x400;
        if (o->oInteractStatus & INT_STATUS_TRAP_TURN) {
            o->oAction = 1; // detects interact then opens the trapdoor
        }
    }
}

void bhv_castle_floor_trap_open(void) { // trap_start (modified)
    if (o->oTimer == 0) {
        cur_obj_play_sound_2(SOUND_GENERAL_CASTLE_TRAP_OPEN);
    }

    o->oAngleVelRoll -= 0x100;
    o->oFaceAngleRoll += o->oAngleVelRoll;

    if (o->oFaceAngleRoll < -0x4000) {
        o->oFaceAngleRoll = -0x4000;
        // No close detection check here
    }
}

void bhv_castle_floor_trap_close_detect(void) { // trap_stop
    if (o->oDistanceToMario > 1000.0f) {
        o->oAction = 3; // close trapdoor
    }
}

void bhv_castle_floor_trap_close(void) { // trap_retrun
    o->oFaceAngleRoll += 0x400;

    if (o->oFaceAngleRoll > 0) {
        o->oFaceAngleRoll = 0;
        o->oAction = 0; // after closing, reloads open detection
        o->oInteractStatus &= ~INT_STATUS_TRAP_TURN;
    }
}

void bhv_castle_floor_trap_rotate(void) { // trap_comeback
    o->oFaceAngleRoll = -0x3C00;
}

void bhv_castle_floor_trap_loop(void) { // s_mainroom_trap_main
    UNUSED struct Surface *bgdata;
    UNUSED f32 dz;
    UNUSED s32 se;

    switch (o->oAction) {
        case 0:
            bhv_castle_floor_trap_open_detect();
            break;
        case 1:
            bhv_castle_floor_trap_open();
            break;
        case 2:
            bhv_castle_floor_trap_close_detect();
            break;
        case 3:
            bhv_castle_floor_trap_close();
            break;
        case 4:
            bhv_castle_floor_trap_rotate();
            break;
    }
}
