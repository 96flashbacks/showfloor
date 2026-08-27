
/**
 * Behavior for bhvSmallKey and bhvKeyInsideBoo.
 * They were intended to be a key that would be contained in boos
 * and would fall out, like coins do. There is a model, MODEL_SMALL_KEY, that
 * is loaded in script_func_global_10, which contains boo-themed models used in
 * BBH and the castle courtyard. It is used in a macro preset with bhvSmallKey,
 * which is also grouped near other boo/BBH-related macros. 
 * bhvKeyInsideBoo has code similar to bhvSmallKey's for rotation and collection, 
 * and functions correctly when spawned as a child of a boo.
 */

/**
 * Update function for bhvSmallKey.
 * It rotates the key, and deletes it when collected.
 * The code in this function is similar to
 * that found in the bhvKeyInsideBoo code.
 */
void bhv_small_key_loop(void) { // s_key
    // Rotate the key
    o->oFaceAngleRoll += 0x200;
    o->oFaceAngleYaw += 0x200;

    if (obj_check_if_collided_with_object(o, gMarioObject)) {
        // This line was used to activate the haunted bookshelf in BBH.
        // The bookshelf has a commented out line to spawn this key object,
        // and right after checks if oHauntedBookshelfShouldOpen isn't FALSE.
        // Collecting the key spawned by the bookshelf causes it to recede.
        o->parentObj->oHauntedBookshelfShouldOpen = TRUE;

        // Delete the object and spawn sparkles
        obj_mark_for_deletion(o);
        spawn_object(o, MODEL_SPARKLES, bhvCoinSparkles);
    }
}

// For some reason, the action functions for the beta boo key
// are written in reverse order.

/**
 * Continue to make the key fall, and handle collection.
 */
static void key_inside_boo_dropped_loop(void) { // keycoin_bound
    // Apply standard physics to the key
    cur_obj_update_floor_and_walls();
    cur_obj_move_standard(78);

    // Slowly increase the Y offset to make the model aligned correctly.
    // This is spread out over 13 frames so that it's not noticable.
    if (o->oGraphYOffset < 26.0f) {
        o->oGraphYOffset += 2.0f;
    }

    // Transition from rotating in both the yaw and the roll axes
    // to just in the yaw axis. This is done by truncating the key's roll
    // to the nearest multiple of 0x800, then continuously adding 0x800
    // until it reaches a multiple of 0x10000, at which point &-ing with
    // 0xFFFF returns 0 and the key stops rotating in the roll direction.
    if (o->oFaceAngleRoll & 0xFFFF) {
        o->oFaceAngleRoll &= 0xF800;
        o->oFaceAngleRoll += 0x800;
    }

    // Once the key stops bouncing, stop its horizontal movement on the ground.
    if (o->oMoveFlags & OBJ_MOVE_ON_GROUND) {
        o->oVelX = 0.0f;
        o->oVelZ = 0.0f;
    }

    // Rotate the key
    o->oFaceAngleYaw += 0x800;

    // If the key hits the floor or 90 frames have elapsed since it was dropped,
    // become tangible and handle collision.
    if (o->oTimer > 90 || o->oMoveFlags & OBJ_MOVE_LANDED) {
        cur_obj_become_tangible();

        if (obj_check_if_collided_with_object(o, gMarioObject)) {
            // This interaction status is 0x01, the first interaction status flag.
            // It was only used for Hoot in the final game, but it seems it could've
            // treated as a TRUE/FALSE statement or held some special meaning in beta.
            // Earlier, in key_inside_boo_drop (called when the parent boo is killed),
            // o->parentObj is set to the parent boo's parentObj. This means that
            // here, the parentObj is actually the parent of the old parent boo.
            o->parentObj->oInteractStatus = TRUE;

            // Delete the object and spawn sparkles
            obj_mark_for_deletion(o);
            spawn_object(o, MODEL_SPARKLES, bhvCoinSparkles);
        }
    }
}

/**
 * Drop the key. This function is run once, the frame after the boo dies;
 * It immediately sets the action to BETA_BOO_KEY_ACT_DROPPED.
 */
static void key_inside_boo_drop(void) { // keycoin_boundwait
    s16 velocityDirection;
    f32 velocityMagnitude;

    // Update the key to be inside the boo
    struct Object *parent = o->parentObj;
    obj_copy_pos(o, parent);

    // This if statement to only run this code on the first frame
    // is redundant, since it instantly sets the action to BETA_BOO_KEY_ACT_DROPPED
    // which stops this function from running again.
    if (o->oTimer == 0) {
        // Separate from the parent boo
        o->parentObj = parent->parentObj;

        o->oAction = BETA_BOO_KEY_ACT_DROPPED;

        // Make the key move laterally away from Mario at 3 units/frame
        // (as if he transferred kinetic energy to it)
        velocityDirection = gMarioObject->oMoveAngleYaw;
        velocityMagnitude = 3.0f;

        o->oVelX = sins(velocityDirection) * velocityMagnitude;
        o->oVelZ = coss(velocityDirection) * velocityMagnitude;

        // Give it an initial Y velocity of 40 units/frame
        o->oVelY = 40.0f;
    }

    // Rotate the key
    o->oFaceAngleYaw += 0x200;
    o->oFaceAngleRoll += 0x200;
}

/**
 * Update the key to be inside its parent boo, and handle the boo dying.
 */
static void key_inside_boo_inside_boo_loop(void) { // keycoin_take (modified)
    // Update the key to be inside the boo at all times
    struct Object *parent = o->parentObj;
    obj_copy_pos(o, parent);

    // Use a Y offset of 40 to make the key model aligned correctly.
    // (Why didn't they use oGraphYOffset?)
    o->oPosY += 40.0f;

    // If the boo is dying/dead, set the action to BETA_BOO_KEY_ACT_DROPPING.
    if (parent->obake_flag != 0) { // 'obake_flag' check like 'pathobakecoin.p'
        o->oAction = BETA_BOO_KEY_ACT_DROPPING;
    }

    // Rotate the key
    o->oFaceAngleRoll += 0x200;
    o->oFaceAngleYaw += 0x200;
}

static void (*sBetaBooKeyActions[])(void) = { // keycoin_modejmp
    key_inside_boo_inside_boo_loop,
    key_inside_boo_drop,
    key_inside_boo_dropped_loop,
};

/**
 * Update function for bhvKeyInsideBoo.
 */
void bhv_key_inside_boo_loop(void) { // s_teresa_key
    cur_obj_call_action_function(sBetaBooKeyActions);
}
