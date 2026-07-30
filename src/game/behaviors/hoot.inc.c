// hoot.c.inc

void bhv_hoot_init(void) {
    cur_obj_init_animation(0);

    o->oHomeX = o->oPosX + 800.0f;
    o->oHomeY = o->oPosY - 150.0f;
    o->oHomeZ = o->oPosZ + 300.0f;
    o->header.gfx.node.flags |= GRAPH_RENDER_INVISIBLE;

    cur_obj_become_intangible();
}

// sp28 = arg0
// sp2c = arg1

f32 hoot_find_next_floor(struct FloorGeometry **arg0, f32 arg1) {
    f32 sp24 = arg1 * sins(o->oMoveAngleYaw) + o->oPosX;
    UNUSED f32 sp20 = o->oPosY;
    f32 sp1c = arg1 * coss(o->oMoveAngleYaw) + o->oPosZ;
    f32 floorY = find_floor_height_and_data(sp24, 10000.0f, sp1c, arg0);

    return floorY;
}

void hoot_floor_bounce(void) {
    struct FloorGeometry *sp1c;
    f32 floorY;

    floorY = hoot_find_next_floor(&sp1c, 375.0f);
    if (floorY + 75.0f > o->oPosY)
        o->oMoveAnglePitch -= 3640.8888;

    floorY = hoot_find_next_floor(&sp1c, 200.0f);
    if (floorY + 125.0f > o->oPosY)
        o->oMoveAnglePitch -= 7281.7776;

    floorY = hoot_find_next_floor(&sp1c, 0);
    if (floorY + 125.0f > o->oPosY)
        o->oPosY = floorY + 125.0f;
    if (o->oMoveAnglePitch < -21845.3328)
        o->oMoveAnglePitch = -21845;
}

// sp30 = fastOscY
// sp34 = speed

void hoot_free_step(s16 fastOscY, s32 speed) {
    struct FloorGeometry *sp2c;
    s16 yaw = o->oMoveAngleYaw;
    s16 pitch = o->oMoveAnglePitch;
    s16 sp26 = o->header.gfx.animInfo.animFrame;
    f32 xPrev = o->oPosX;
    f32 zPrev = o->oPosZ;
    f32 hSpeed;

    o->oVelY = sins(pitch) * speed;
    hSpeed = coss(pitch) * speed;
    o->oVelX = sins(yaw) * hSpeed;
    o->oVelZ = coss(yaw) * hSpeed;

    o->oPosX += o->oVelX;
    if (fastOscY == 0)
        o->oPosY -= o->oVelY + coss((s32) (sp26 * 3276.8)) * 50.0f / 4;
    else
        o->oPosY -= o->oVelY + coss((s32) (sp26 * 6553.6)) * 50.0f / 4;
    o->oPosZ += o->oVelZ;

    find_floor_height_and_data(o->oPosX, o->oPosY, o->oPosZ, &sp2c);
    if (sp2c == NULL) {
        o->oPosX = xPrev;
        o->oPosZ = zPrev;
    }

    if (sp26 == 0)
        cur_obj_play_sound_2(SOUND_GENERAL_SWISH_WATER);
}

void hoot_player_set_yaw(void) {
    s16 stickX = gPlayer3Controller->rawStickX;
    s16 stickY = gPlayer3Controller->rawStickY;
    UNUSED s16 pitch = o->oMoveAnglePitch;

    if (stickX < 10 && stickX >= -9)
        stickX = 0;
    if (stickY < 10 && stickY >= -9)
        stickY = 0;

    o->oMoveAngleYaw -= 5 * stickX;
    o->oMoveAnglePitch -= 5 * stickY;

    if (o->oMoveAnglePitch >= 0x2800) {
        o->oMoveAnglePitch = 0x2800;
    } else if (o->oMoveAnglePitch <= -0x2800) {
        o->oMoveAnglePitch = -0x2800;
    }

    if (stickY == 0)
        o->oMoveAnglePitch = 0;
}

// sp28 = speed
// sp2c = xPrev
// sp30 = zPrev

void hoot_carry_step(s32 speed, UNUSED f32 xPrev, UNUSED f32 zPrev) {
    s16 yaw = o->oMoveAngleYaw;
    s16 pitch = o->oMoveAnglePitch;
    s16 sp22 = o->header.gfx.animInfo.animFrame;
    s16 stickX = gPlayer3Controller->rawStickX;
    s16 stickY = gPlayer3Controller->rawStickY;
    s32 spd;
    f32 hSpeed;

    if (stickY < 0)
        stickY = -stickY;
    if (stickY > 80)
        stickY = 80;

    if (stickX < 0)
        stickX = -stickX;
    if (stickX > 80)
        stickX = 80;

    spd = (speed - (stickY / 4)) + (stickX / 4);

    if (spd > speed)
        spd = speed;

    o->oVelY = sins(pitch) * speed;
    hSpeed = coss(pitch) * spd;
    o->oVelX = sins(yaw) * hSpeed;
    o->oVelZ = coss(yaw) * hSpeed;

    o->oPosX += o->oVelX;
    o->oPosY -= o->oVelY + coss((s32) (sp22 * 6553.6)) * 50.0f / 4;
    o->oPosZ += o->oVelZ;

    if (sp22 == 0)
        cur_obj_play_sound_2(SOUND_GENERAL_SWISH_WATER);
}

// sp48 = xPrev
// sp4c = yPrev
// sp50 = zPrev

void hoot_surface_collision(f32 xPrev, UNUSED f32 yPrev, f32 zPrev) {
    struct FloorGeometry *sp44;
    struct WallCollisionData hitbox;
    f32 floorY;

    hitbox.x = o->oPosX;
    hitbox.y = o->oPosY;
    hitbox.z = o->oPosZ;
    hitbox.offsetY = 10.0;
    hitbox.radius = 50.0;

    if (find_wall_collisions(&hitbox) != 0) {
        o->oPosX = xPrev;
        o->oPosZ = zPrev;
    }

    floorY = find_floor_height_and_data(o->oPosX, o->oPosY, o->oPosZ, &sp44);
    if (sp44 == NULL) {
        o->oPosX = xPrev;
        o->oPosZ = zPrev;
        return;
    }

    if (absf_2(o->oPosX) > 8000.0f)
        o->oPosX = xPrev;
    if (absf_2(o->oPosZ) > 8000.0f)
        o->oPosZ = zPrev;
    if (floorY + 125.0f > o->oPosY)
        o->oPosY = floorY + 125.0f;
}

void hoot_action_loop(void) { // ajsfhkjadsf
    f32 xPrev = o->oPosX;
    f32 yPrev = o->oPosY;
    f32 zPrev = o->oPosZ;

    switch (o->oAction) {
        case HOOT_ACT_CARRY:
            hoot_player_set_yaw();

            if (o->oPosY > 7000.0f) {
                gMarioObject->oInteractStatus |= INT_STATUS_MARIO_DROP_FROM_HOOT;
            }

            hoot_carry_step(20, xPrev, zPrev);
            break;
    }

    hoot_surface_collision(xPrev, yPrev, zPrev);
}

void hoot_turn_to_home(void) {
    f32 homeDistX = o->oHomeX - o->oPosX;
    f32 homeDistY = o->oHomeY - o->oPosY;
    f32 homeDistZ = o->oHomeZ - o->oPosZ;
    s16 hAngleToHome = atan2s(homeDistZ, homeDistX);
    s16 vAngleToHome = atan2s(sqrtf(homeDistX * homeDistX + homeDistZ * homeDistZ), -homeDistY);

    o->oMoveAngleYaw = approach_s16_symmetric(o->oMoveAngleYaw, hAngleToHome, 0x140);
    o->oMoveAnglePitch = approach_s16_symmetric(o->oMoveAnglePitch, vAngleToHome, 0x140);
}

void hoot_awake_loop(void) {
    if (o->oInteractStatus == TRUE) { //! Note: Not a flag, treated as a TRUE/FALSE statement
        hoot_action_loop();
        cur_obj_init_animation(1);
    } else {
        cur_obj_init_animation(0);

        hoot_turn_to_home();
        hoot_floor_bounce();
        hoot_free_step(0, 10);

        o->oAction = 0;
        o->oTimer = 0;
    }

    set_object_visibility(o, 2000);
}

void bhv_hoot_loop(void) {
    switch (o->oHootAvailability) {
        case HOOT_AVAIL_ASLEEP_IN_TREE:
            if (is_point_within_radius_of_mario(o->oPosX, o->oPosY, o->oPosZ, 50)) {
                o->header.gfx.node.flags &= ~GRAPH_RENDER_INVISIBLE;
                o->oHootAvailability = HOOT_AVAIL_WANTS_TO_TALK;
            }
            break;

        case HOOT_AVAIL_WANTS_TO_TALK:
            hoot_awake_loop();

            if ((gMarioState->action & ACT_GROUP_MASK) == ACT_GROUP_STATIONARY || 
                (gMarioState->action & ACT_GROUP_MASK) == ACT_GROUP_MOVING) {
                level_set_transition(-1, NULL);
                create_dialog_box(MESS_TORI);
                o->oHootAvailability = HOOT_AVAIL_READY_TO_FLY;
            }
            break;

        case HOOT_AVAIL_READY_TO_FLY:
            hoot_awake_loop();
            cur_obj_become_tangible();
            break;
    }
}
