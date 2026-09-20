// bomp.inc.c

void bhv_small_bomp_init(void) { // s_dossun_bar_init
    o->oFaceAngleYaw -= 0x4000;
    o->oSmallBompInitX = o->oPosX;
    o->oTimer = random_float() * 100.0f;
}

void bhv_small_bomp_loop(void) { // s_dossun_bar_main (modified)
    switch (o->oAction) {
        case BOMP_ACT_WAIT:
            if (o->oTimer > 100) {
                o->oAction = BOMP_ACT_POKE_OUT;
                o->oForwardVel = 30.0f;
            }
            break;

        case BOMP_ACT_POKE_OUT:
            if (o->oPosX > 3450.0f) {
                o->oPosX = 3450.0f;
                o->oForwardVel = 0.0f;
            }

            if (o->oTimer == 15.0) {
                o->oAction = BOMP_ACT_EXTEND;
                o->oForwardVel = 40.0f;
                // No sound when moving
            }
            break;

        case BOMP_ACT_EXTEND:
            if (o->oPosX > 3830.0f) {
                o->oPosX = 3830.0f;
                o->oForwardVel = 0.0f;
            }

            if (o->oTimer == 60) {
                o->oAction = BOMP_ACT_RETRACT;
                o->oForwardVel = 10.0f;
                o->oMoveAngleYaw -= 0x8000;
                // No sound when moving
            }
            break;

        case BOMP_ACT_RETRACT:
            if (o->oPosX < 3330.0f) {
                o->oPosX = 3330.0f;
                o->oForwardVel = 0.0f;
            }

            if (o->oTimer == 90) {
                o->oAction = BOMP_ACT_POKE_OUT;
                o->oForwardVel = 25.0f;
                o->oMoveAngleYaw -= 0x8000;
            }
            break;
    }
}

void bhv_large_bomp_init(void) { // s_dossun_bar2_init
    o->oMoveAngleYaw += 0x4000;
    o->oTimer = random_float() * 100.0f;
}

void bhv_large_bomp_loop(void) { // s_dossun_bar2_main (modified)
    switch (o->oAction) {
        case BOMP_ACT_WAIT:
            if (o->oTimer > 100) {
                o->oAction = BOMP_ACT_POKE_OUT;
                o->oForwardVel = 30.0f;
            }
            break;

        case BOMP_ACT_POKE_OUT:
            if (o->oPosX > 3450.0f) {
                o->oPosX = 3450.0f;
                o->oForwardVel = 0.0f;
            }

            if (o->oTimer == 15.0) {
                o->oAction = BOMP_ACT_EXTEND;
                o->oForwardVel = 10.0f;
                // No sound when moving
            }
            break;

        case BOMP_ACT_EXTEND:
            if (o->oPosX > 3830.0f) {
                o->oPosX = 3830.0f;
                o->oForwardVel = 0.0f;
            }

            if (o->oTimer == 60) {
                o->oAction = BOMP_ACT_RETRACT;
                o->oForwardVel = 10.0f;
                o->oMoveAngleYaw -= 0x8000;
                // No sound when moving
            }
            break;

        case BOMP_ACT_RETRACT:
            if (o->oPosX < 3330.0f) {
                o->oPosX = 3330.0f;
                o->oForwardVel = 0.0f;
            }

            if (o->oTimer == 90) {
                o->oAction = BOMP_ACT_POKE_OUT;
                o->oForwardVel = 25.0f;
                o->oMoveAngleYaw -= 0x8000;
            }
            break;
    }
}
