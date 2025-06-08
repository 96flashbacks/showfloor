// bowser_bomb.inc.c

void bhv_bowser_bomb_loop(void) {
    if (obj_check_if_collided_with_object(o, gMarioObject) == TRUE) {
        o->oInteractStatus &= ~INT_STATUS_INTERACTED;
        spawn_object(o, MODEL_EXPLOSION, bhvExplosion);
        o->activeFlags = ACTIVE_FLAG_DEACTIVATED;
    }

    if (o->oInteractStatus & INT_STATUS_HIT_MINE) {
        spawn_object(o, MODEL_BOWSER_FLAMES, bhvBowserBombExplosion);
        create_sound_spawner(SOUND_GENERAL_BOWSER_BOMB_EXPLOSION);
        set_camera_shake_from_point(SHAKE_POS_LARGE, o->oPosX, o->oPosY, o->oPosZ);
        o->activeFlags = ACTIVE_FLAG_DEACTIVATED;
    }

    set_object_visibility(o, 7000);
}

void bhv_bowser_bomb_explosion_loop(void) {
    struct Object *mineSmoke;

    cur_obj_scale((f32) o->oTimer / 14.0f * 9.0 + 1.0);
    if ((o->oTimer % 8 == 0) && (o->oTimer < 20)) {
        mineSmoke = spawn_object(o, MODEL_BOWSER_SMOKE, bhvBowserBombSmoke);
        mineSmoke->oPosX += random_float() * 600.0f - 400.0f;
        mineSmoke->oPosZ += random_float() * 600.0f - 400.0f;
        mineSmoke->oPosY -= 50.0f;
    }

    if (o->oTimer % 2 == 0)
        o->oAnimState++;
    if (o->oTimer == 28)
        o->activeFlags = 0;
}

void bhv_bowser_bomb_smoke_loop(void) {
    if (o->oTimer % 2 == 0)
        o->oAnimState++;

    o->oPosY += o->oVelY;

    if (o->oTimer < 25)
        cur_obj_scale((f32) o->oTimer / 14.0f * 9.0 + 1.0);

    if (o->oTimer == 28)
        o->activeFlags = 0;
}
