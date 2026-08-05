// celebration_star.inc.c

void bhv_celebration_star_init(void) {
    o->oHomeX = gMarioObject->header.gfx.pos[0];
    o->oPosY = gMarioObject->header.gfx.pos[1] + 30.0f;
    o->oHomeZ = gMarioObject->header.gfx.pos[2];
    o->oMoveAngleYaw = gMarioObject->header.gfx.angle[1] + 0x8000;
    o->oCelebStarDiameterOfRotation = 100;
    o->header.gfx.sharedChild = gLoadedGraphNodes[MODEL_STAR];
    cur_obj_scale(0.4f);
}

void celeb_star_act_spin_around_mario(void) {
    o->oPosX = o->oHomeX + sins(o->oMoveAngleYaw) * (f32) (o->oCelebStarDiameterOfRotation / 2);
    o->oPosZ = o->oHomeZ + coss(o->oMoveAngleYaw) * (f32) (o->oCelebStarDiameterOfRotation / 2);
    o->oPosY += 4.0f;
    o->oMoveAngleYaw += 0x2000;
    o->oAnimState++;

    if (o->oTimer > 45)
        o->oAction = CELEB_STAR_ACT_FACE_CAMERA;
    if (o->oTimer < 40) {
        spawn_object(o, MODEL_SPARKLES, bhvCelebrationStarSparkle);
        o->oCelebStarDiameterOfRotation++;
    } else {
        o->oCelebStarDiameterOfRotation -= 20;
    }
}

void celeb_star_act_face_camera(void) {
    if (o->oTimer == 0) {
        spawn_object(o, MODEL_SPARKLES, bhvCelebrationStarSparkle); // spawns the sparkle in the first frame but leaves the scale used earlier (0.4)
    } else if (o->oTimer < 10) {
        cur_obj_scale((f32) o->oTimer / 10.0); // then scales the star on the next 9 frames
    }

    if (o->oTimer < 10) {
        o->oAnimState++;
    }
}

void bhv_celebration_star_loop(void) {
    switch (o->oAction) {
        case CELEB_STAR_ACT_SPIN_AROUND_MARIO:
            celeb_star_act_spin_around_mario();
            break;

        case CELEB_STAR_ACT_FACE_CAMERA:
            celeb_star_act_face_camera();
            break;
    }
}

void bhv_celebration_star_sparkle_loop(void) {
    o->oPosY -= 15.0f;

    if (o->oTimer == 12) {
        o->activeFlags = ACTIVE_FLAG_DEACTIVATED;
    }
}

void bhv_star_dust_loop(void) {
    if (o->oTimer % 2 == 0) {
        o->oAnimState++;
    }
    
    if (o->oTimer == 32) {
        o->activeFlags = ACTIVE_FLAG_DEACTIVATED;
    }
}
