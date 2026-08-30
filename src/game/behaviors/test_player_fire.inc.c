
/**
 * Behavior for bhvTestPlayerFire.
 * Internally called "plfire", it appears to be a
 * simple test object due to its location in 'pathtest.p'.
 */

/**
 * Update function for bhvTestPlayerFire.
 * It continuously updates to be in front of Mario,
 * and attacks all non-Mario objects it touches.
 * It continuously sets its hitbox radius/height
 * based on gDebugInfo[DEBUG_PAGE_EFFECTINFO].
 */
void bhv_test_player_fire_loop(void) { // s_plfire
    // Set the object's position to be 30 units above Mario's feet,
    // and 300 units in front of him.
    cur_obj_set_pos_relative(gMarioObject, 0, 30.0f, 300.0f);

    o->hitboxRadius = gDebugInfo[DEBUG_PAGE_EFFECTINFO][0] + 100;
    o->hitboxHeight = gDebugInfo[DEBUG_PAGE_EFFECTINFO][1] + 300;

    obj_attack_collided_from_other_object(o);
}
