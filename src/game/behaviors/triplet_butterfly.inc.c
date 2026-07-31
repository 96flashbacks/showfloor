// triplet_butterfly.inc.c

static struct ObjectHitbox sTripletButterflyExplodeHitbox = {
    /* interactType:      */ 0,
    /* downOffset:        */ 50,
    /* damageOrCoinValue: */ 2,
    /* health:            */ 1,
    /* numLootCoins:      */ 0,
    /* radius:            */ 100,
    /* height:            */ 50,
    /* hurtboxRadius:     */ 100,
    /* hurtboxHeight:     */ 50,
};

static void triplet_butterfly_act_init(void) {
    s32 butterflySpawnType = o->oBhvParams2ndByte & TRIPLET_BUTTERFLY_BP_SPAWN_TYPE_MASK;
    s32 i;

    if (o->oDistanceToMario < 200.0f) {
        if (butterflySpawnType == TRIPLET_BUTTERFLY_BP_SPAWN_TYPE_SPAWNER) {
            for (i = 1; i <= 2; i++) {
                spawn_object_relative(i, 0, 0, 0, o, MODEL_BUTTERFLY, bhvTripletButterfly);
            }
        }

        o->oAction = TRIPLET_BUTTERFLY_ACT_WANDER;

        o->oTripletButterflyBaseYaw = o->oBhvParams2ndByte * (0x10000 / 3);
        o->oMoveAngleYaw = (s32)(o->oTripletButterflyBaseYaw + random_linear_offset(0, 0x5555));
        o->oTripletButterflySpeed = random_linear_offset(15, 15);

        cur_obj_unhide();
    }
}

static void triplet_butterfly_act_wander(void) {
    if (o->oDistanceToMario > 1500.0f) {
        obj_mark_for_deletion(o);
    } else {
        approach_f32_ptr(&o->oTripletButterflySpeed, 8.0f, 0.5f);
        if (o->oTimer < 60) {
            o->oTripletButterflyTargetYaw = cur_obj_angle_to_home();
        } else {
            o->oTripletButterflyTargetYaw = (s32) o->oTripletButterflyBaseYaw;
        }

        if (o->oHomeY < o->oFloorHeight) {
            o->oHomeY = o->oFloorHeight;
        }

        if (o->oPosY < o->oHomeY + random_linear_offset(50, 50)) {
            o->oTripletButterflyTargetPitch = -0x2000;
        } else {
            o->oTripletButterflyTargetPitch = 0x2000;
        }

        obj_move_pitch_approach(o->oTripletButterflyTargetPitch, 400);
        cur_obj_rotate_yaw_toward(o->oTripletButterflyTargetYaw, random_linear_offset(400, 800));
    }
}

void bhv_triplet_butterfly_update(void) {
    cur_obj_update_floor_and_walls();

    switch (o->oAction) {
        case TRIPLET_BUTTERFLY_ACT_INIT:
            triplet_butterfly_act_init();
            break;
        case TRIPLET_BUTTERFLY_ACT_WANDER:
            triplet_butterfly_act_wander();
            break;
    }

    cur_obj_scale(o->oTripletButterflyScale);
    obj_compute_vel_from_move_pitch(o->oTripletButterflySpeed);
    cur_obj_move_standard(78);
}
