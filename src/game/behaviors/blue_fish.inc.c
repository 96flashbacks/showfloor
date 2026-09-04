
/**
 * @file blue_fish.inc.c
 * Implements behaviour and spawning for bhvBlueFish located in the castle aquarium outside of SA.
 */

/**
 * Determines fish movement.
 */
void bhv_blue_fish_movement_loop(void) {
    f32 randomSwitch;

    switch (o->oAction) {
        // Initial dive phase after spawning
        case BLUE_FISH_ACT_DIVE:
            cur_obj_init_animation_with_accel_and_sound(0, 1.0f);

            // Assigns random values to variables that help determine natural motion.
            if (o->oTimer == 0) {
                o->oBlueFishRandomAngle = random_sign() * 0x800;
                o->oBlueFishRandomVel = random_float() * 2;
                o->oBlueFishRandomTime = (s32) (random_float() * 30) & 0xFE;

                // Adjusts pitch velocity or sets to zero dependant on outcome of randomSwitch.
                randomSwitch = random_float() * 5;
                if (randomSwitch < 2.0f) {
                    o->oAngleVelPitch = random_f32_around_zero(128);
                } else {
                    o->oAngleVelPitch = 0;
                }
            }

            // Set forward velocity and progress oAction to BLUE_FISH_ACT_TURN.
            o->oForwardVel = o->oBlueFishRandomVel + 3.0f;
            if (o->oTimer >= o->oBlueFishRandomTime + 60) {
                o->oAction++;
            }

            // Set pitch velocity
            if (o->oTimer < (o->oBlueFishRandomTime + 60) / 2) {
                o->oFaceAnglePitch += o->oAngleVelPitch;
            } else {
                o->oFaceAnglePitch -= o->oAngleVelPitch;
            }

            // Calculate new Y velocity
            o->oVelY = -sins(o->oFaceAnglePitch) * o->oForwardVel;
            break;

        // Animates and adjusts fish yaw angle.
        case BLUE_FISH_ACT_TURN:
            cur_obj_init_animation_with_accel_and_sound(0, 2.0f);
            o->oMoveAngleYaw = (s32) (o->oBlueFishRandomAngle + o->oMoveAngleYaw);
            if (o->oTimer == 15) {
                o->oAction++;
            }
            break;

        // Animates and adjusts pitch to an upward direction.
        case BLUE_FISH_ACT_ASCEND:
            cur_obj_init_animation_with_accel_and_sound(0, 1.0f);

            // Progresses oAction to BLUE_FISH_ACT_TURN_BACK after elapsed time.
            if (o->oTimer >= o->oBlueFishRandomTime + 60) {
                o->oAction++;
            }

            // Adjusts pitch angle. Direction relies on time not passed.
            if (o->oTimer < (o->oBlueFishRandomTime + 60) / 2) {
                o->oFaceAnglePitch -= o->oAngleVelPitch;
            } else {
                o->oFaceAnglePitch += o->oAngleVelPitch;
            }
            break;

        // Animates and turns fish around
        case BLUE_FISH_ACT_TURN_BACK:
            cur_obj_init_animation_with_accel_and_sound(0, 2.0f);
            o->oMoveAngleYaw = (s32) (o->oBlueFishRandomAngle + o->oMoveAngleYaw);

            // Sets the fish back to the BLUE_FISH_ACT_DIVE phase.
            if (o->oTimer == 15) {
                o->oAction = BLUE_FISH_ACT_DIVE;
            }
            break;
    }

    // Calculates Y velocity and calls physics engine.
    o->oVelY = -sins(o->oFaceAnglePitch) * o->oForwardVel;
    cur_obj_move_using_fvel_and_gravity();

    // Deletes object if the parent has oAction set to BLUE_FISH_ACT_DUPLICATE.
    if (o->parentObj->oAction == BLUE_FISH_ACT_DUPLICATE) {
        obj_mark_for_deletion(o);
    }
}
