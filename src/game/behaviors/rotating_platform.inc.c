// rotating_platform.inc.c

// No 'rotbg_data' (sRotatingPlatformData), since the rotating platforms that use it didn't exist in the demo

void bhv_wf_rotating_wooden_platform_loop(void) { // s_castle_rotbar (modified)
    if (o->oAction == WF_ROTATING_WOODEN_PLATFORM_ACT_IDLE) {
        o->oAngleVelYaw = 0;
        if (o->oTimer > 60) {
            o->oAction++;
        }
    } else { // WF_ROTATING_WOODEN_PLATFORM_ACT_ROTATING
        o->oAngleVelYaw = 0x100;
        if (o->oTimer > 126) {
            o->oAction = WF_ROTATING_WOODEN_PLATFORM_ACT_IDLE;
        }
        // No sound when rotating
    }
    cur_obj_rotate_face_angle_using_vel();
}

// No 's_castle_rotland' (bhv_rotating_platform_loop), these rotating platforms didn't exist in the demo
