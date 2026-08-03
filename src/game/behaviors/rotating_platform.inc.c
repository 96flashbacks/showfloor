// rotating_platform.inc.c

#include "levels/wf/header.h"

void bhv_wf_rotating_wooden_platform_loop(void) {
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
    }
    cur_obj_rotate_face_angle_using_vel();
}

void bhv_rotating_platform_loop(void) {
    s8 bhvParams1stByte = o->oBhvParams >> 24;
    if (o->oTimer == 0) {
        obj_set_collision_data(o, wf_seg7_collision_rotating_platform);
        o->oCollisionDistance = 2000;
        cur_obj_scale(100 * 0.01f);
    }
    o->oAngleVelYaw = bhvParams1stByte << 4;
    o->oFaceAngleYaw += o->oAngleVelYaw;
}
