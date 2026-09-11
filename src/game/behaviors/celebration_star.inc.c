// celebration_star.inc.c

void bhv_celebration_star_init(void) { // s_V_star_init (modified)
    o->oHomeX = gMarioObject->header.gfx.pos[0];
    o->oPosY = gMarioObject->header.gfx.pos[1] + 30.0f;
    o->oHomeZ = gMarioObject->header.gfx.pos[2];

    o->oMoveAngleYaw = gMarioObject->header.gfx.angle[1] + 0x8000;
    o->oCelebStarDiameterOfRotation = 100;
    
    o->header.gfx.sharedChild = gLoadedGraphNodes[MODEL_STAR];
    cur_obj_scale(0.4f);
    // No pitch and roll reset since the star is 2D
}

static void celeb_star_act_spin_around_mario(void) { // StarRotation (modified)
    o->oPosX = o->oHomeX + sins(o->oMoveAngleYaw) * (f32) (o->oCelebStarDiameterOfRotation / 2);
    o->oPosZ = o->oHomeZ + coss(o->oMoveAngleYaw) * (f32) (o->oCelebStarDiameterOfRotation / 2);
    o->oPosY += 4.0f; // Smaller position increase than the final game (5.0)
    
    o->oAnimState++; // Final game changes the 'oFaceAngleYaw' since it's a 3D model
    o->oMoveAngleYaw += 0x2000;

    if (o->oTimer == 46) { // The star takes 6 additional frames to start facing the camera
        o->oAction = CELEB_STAR_ACT_FACE_CAMERA;
    }

    if (o->oTimer < 40) { // 5 additional frames of rotation around Mario
        spawn_object(o, MODEL_SPARKLES, bhvCelebrationStarSparkle);
        o->oCelebStarDiameterOfRotation++;
    } else {
        o->oCelebStarDiameterOfRotation -= 20;
    }
}

static void celeb_star_act_face_camera(void) { // StarScaleUp (modified)
    if (o->oTimer == 0) {
        // Spawn a sparkle on the first frame but keep the scale used earlier (0.4)
        spawn_object(o, MODEL_SPARKLES, bhvCelebrationStarSparkle);
    } else if (o->oTimer < 10) {
        cur_obj_scale((f32) o->oTimer / 10.0); // Then scale up the star on the next 9 frames
    }

    if (o->oTimer < 10) {
        o->oAnimState++;
    }

    // The star doesn't dissapear after 59 frames, "no exit" stars didn't exist at this point
}

void bhv_celebration_star_loop(void) { // s_V_star_event
    switch (o->oAction) {
        case CELEB_STAR_ACT_SPIN_AROUND_MARIO:
            celeb_star_act_spin_around_mario();
            break;

        case CELEB_STAR_ACT_FACE_CAMERA:
            celeb_star_act_face_camera();
            break;
    }
}

void bhv_celebration_star_sparkle_loop(void) { // s_V_starspark
    o->oPosY -= 15.0f;

    if (o->oTimer == 12) {
        o->activeFlags = ACTIVE_FLAG_DEACTIVATED;
    }
}

// Named 'bhv_star_key_collection_puff_spawner_loop' in the decomp
void bhv_star_dust_loop(void) { // s_V_stardust (modified)
    // Same method of animating on every other frame as 'bhv_bowser_bomb_smoke_loop'
    if ((o->oTimer & 0x01) == 0) {
        o->oAnimState++;
    }
    
    if (o->oTimer == 32) {
        o->activeFlags = ACTIVE_FLAG_DEACTIVATED;
    }
}
