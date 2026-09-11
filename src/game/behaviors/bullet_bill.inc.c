// bullet_bill.inc.c

void bhv_white_puff_smoke_init(void) { // s_killer_smorke
    cur_obj_scale(random_float() * 2 + 2.0);
}

void bhv_bullet_bill_init(void) { // s_killer_init
    o->oBulletBillInitialMoveYaw = o->oMoveAngleYaw;
}

static void bullet_bill_act_0(void) { // killer_init
    cur_obj_become_tangible();
    o->oForwardVel = 0.0f;
    o->oMoveAngleYaw = o->oBulletBillInitialMoveYaw;
    o->oFaceAnglePitch = 0;
    o->oFaceAngleRoll = 0;
    o->oMoveFlags = 0;
    cur_obj_set_pos_to_home();
    o->oAction = 1;
}

static void bullet_bill_act_1(void) { // killer_playerwait
    s16 sp1E = abs_angle_diff(o->oAngleToMario, o->oMoveAngleYaw);
    if (sp1E < 0x2000 && 400.0f < o->oDistanceToMario && o->oDistanceToMario < 1500.0f) {
        o->oAction = 2;
    }
}

static void bullet_bill_act_2(void) { // killer_move (modified)
    if (o->oTimer < 40) {
        o->oForwardVel = 3.0f;
    } else if (o->oTimer < 50) {
        if (o->oTimer % 2) {
            o->oForwardVel = 3.0f;
        } else {
            o->oForwardVel = -3.0f;
        }
    } else {
        if (o->oTimer > 70) {
            cur_obj_update_floor_and_walls();
        }

        spawn_object(o, MODEL_SMOKE, bhvWhitePuffSmoke);
        o->oForwardVel = 30.0f;

        // Bullet Bills didn't follow Mario after being shot, seen in the 1995 B-Roll and Game Zero

        if (o->oTimer == 50) {
            cur_obj_play_sound_2(SOUND_OBJ_POUNDING_CANNON);
            // No screen shake, seen in Ultra 64 Super Mario 64 Beta Preview (RE-UPLOAD) (1:46)
        }

        if (o->oTimer > 150 || o->oMoveFlags & OBJ_MOVE_HIT_WALL) {
            o->oAction = 3;
        }
    }
}

static void bullet_bill_act_3(void) { // killer_end
    o->oAction = 0;
}

static void bullet_bill_act_4(void) { // killer_hit
    if (o->oTimer == 0) {
        o->oForwardVel = -30.0f;
        cur_obj_become_intangible();
    }

    o->oFaceAnglePitch += 0x1000;
    o->oFaceAngleRoll += 0x1000;
    o->oPosY += 20.0f;

    if (o->oTimer > 90) {
        o->oAction = 0;
    }
}

static void (*sBulletBillActions[])(void) = { // killer_modejmp
    bullet_bill_act_0,
    bullet_bill_act_1,
    bullet_bill_act_2,
    bullet_bill_act_3,
    bullet_bill_act_4,
};

void bhv_bullet_bill_loop(void) { // s_killer
    cur_obj_call_action_function(sBulletBillActions);
    if (cur_obj_check_interacted()) {
        o->oAction = 4;
    }
}
