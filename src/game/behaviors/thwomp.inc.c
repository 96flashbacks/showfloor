// thwomp.inc.c

void thwomp_act_idle_at_bottom(void) { // dosun_wait
    if (o->oTimer == 0) {
        o->oThwompRandomTimer = random_float() * 10.0f + 20.0f; // [20, 29]
    }
    if (o->oTimer > o->oThwompRandomTimer) {
        o->oAction = THWOMP_ACT_RAISE;
    }
}

void thwomp_act_lower(void) { // dosun_down
    o->oVelY += -4.0f;
    o->oPosY += o->oVelY;
    if (o->oPosY < o->oHomeY) {
        o->oPosY = o->oHomeY;
        o->oVelY = 0.0f;
        o->oAction = THWOMP_ACT_LAND;
    }
}

void thwomp_act_land(void) { // dosun_downend (modified)
    if (o->oTimer == 0) {
        if (o->oDistanceToMario < 1500.0f) {
            cur_obj_shake_screen(SHAKE_POS_SMALL);
            cur_obj_play_sound_2(SOUND_OBJ_THWOMP);
        }
    }

    if (o->oTimer <= 10) { 
        if (o->oTimer % 2)
            o->oPosY = o->oHomeY + 6.0f; // Makes the Thwomp shake on the first 10 landing frames
        else
            o->oPosY = o->oHomeY; 
    } else {
        o->oAction = THWOMP_ACT_IDLE_AT_BOTTOM;
    }
}

void thwomp_act_idle_at_top(void) { // dosun_stop
    if (o->oTimer == 0) {
        o->oThwompRandomTimer = random_float() * 30.0f + 10.0f; // [10, 39]
    }
    if (o->oTimer > o->oThwompRandomTimer) {
        o->oAction = THWOMP_ACT_LOWER;
    }
}

void thwomp_act_raise(void) { // dosun_up
    if (o->oTimer > o->oBhvParams2ndByte + 40) {
        o->oAction = THWOMP_ACT_IDLE_AT_TOP;
        o->oPosY += 5.0f;
    } else {
        o->oPosY += 10.0f;
    }
}

void (*sThwompActions[])(void) = { // dosun_modejmp
    thwomp_act_raise,
    thwomp_act_idle_at_top,
    thwomp_act_lower,
    thwomp_act_land,
    thwomp_act_idle_at_bottom,
};

void bhv_thwomp_loop(void) { // s_dosun
    cur_obj_call_action_function(sThwompActions);
}
