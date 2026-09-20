// tower_platform.inc.c

// enum from 'pathgoalbar.p'
enum {
	GOALBAR_WAIT,
	GOALBAR_MAKE,
	GOALBAR_DISP,
	GOALBAR_REMOVE
};

void bhv_wf_solid_tower_platform_loop(void) { // s_goalbar_stop
    if (o->parentObj->oAction == GOALBAR_REMOVE) {
        obj_mark_for_deletion(o);
    }
}

// constants from 'pathgoalbar.p'
#define	goalbar_updown_speed		5
#define	goalbar_updown_height		700

void bhv_wf_elevator_tower_platform_loop(void) { // s_goalbar_updown
    switch (o->oAction) {
        case 0:
            if (gMarioObject->platform == o) {
                o->oAction++;
            }
            break;

        case 1:
            cur_obj_play_sound_1(SOUND_ENV_ELEVATOR1);
            if (o->oTimer > goalbar_updown_height/goalbar_updown_speed) {
                o->oAction++;
            } else {
                o->oPosY += goalbar_updown_speed;
            }
            break;

        case 2:
            if (o->oTimer > 60) {
                o->oAction++;
            }
            break;

        case 3:
            cur_obj_play_sound_1(SOUND_ENV_ELEVATOR1);
            if (o->oTimer > goalbar_updown_height/goalbar_updown_speed) {
                o->oAction = 0;
            } else {
                o->oPosY -= goalbar_updown_speed;
            }
            break;
    }

    if (o->parentObj->oAction == GOALBAR_REMOVE) {
        obj_mark_for_deletion(o);
    }
}

void bhv_wf_sliding_tower_platform_loop(void) { // s_goalbar_move
    s32 time = o->oPlatformLength / o->oPlatformSpeed;

    switch (o->oAction) {
        case 0:
            if (o->oTimer > time) {
                o->oAction++;
            }
            o->oForwardVel = -o->oPlatformSpeed;
            break;

        case 1:
            if (o->oTimer > time) {
                o->oAction = 0;
            }
            o->oForwardVel = o->oPlatformSpeed;
            break;
    }

    cur_obj_compute_vel_xz();

    o->oPosX += o->oVelX;
    o->oPosZ += o->oVelZ;

    if (o->parentObj->oAction == GOALBAR_REMOVE) {
        obj_mark_for_deletion(o);
    }
}

static void spawn_and_init_wf_platforms(s16 a, const BehaviorScript *bhv) { // goalbar_makeobj
    s16 yaw;
    struct Object *platform = spawn_object(o, a, bhv);

    yaw = o->oPlatformSpawnerCounter * o->oPlatformSpawnerYawMultiplier + o->oPlatformSpawnerStartingAngle;

    platform->oMoveAngleYaw = yaw;
    platform->oPosX += o->oPlatformSpawnerRadius * sins(yaw);
    platform->oPosY += 100 * o->oPlatformSpawnerCounter;
    platform->oPosZ += o->oPlatformSpawnerRadius * coss(yaw);

    platform->oPlatformLength = o->oPlatformSpawnerLength;
    platform->oPlatformSpeed = o->oPlatformSpawnerSpeed;

    o->oPlatformSpawnerCounter++;
}

void spawn_wf_platform_group(void) { // s_goalbar_make (modified)
    UNUSED s32 ang = 8; // named "ang" in 'pathgoalbar.p'

    o->oPlatformSpawnerCounter = 0;
    o->oPlatformSpawnerStartingAngle = 0;
    o->oPlatformSpawnerYawMultiplier = 0x2000;
    o->oPlatformSpawnerRadius = 704.0f;
    o->oPlatformSpawnerLength = 380.0f;
    o->oPlatformSpawnerSpeed = 3.0f;

    // The solid platforms are trapezoids while the sliding platforms are squares
    spawn_and_init_wf_platforms(MODEL_WF_TOWER_TRAPEZOID_PLATORM, bhvWFSolidTowerPlatform);
    spawn_and_init_wf_platforms(MODEL_WF_TOWER_SQUARE_PLATORM, bhvWFSlidingTowerPlatform);
    spawn_and_init_wf_platforms(MODEL_WF_TOWER_TRAPEZOID_PLATORM, bhvWFSolidTowerPlatform);
    spawn_and_init_wf_platforms(MODEL_WF_TOWER_SQUARE_PLATORM, bhvWFSlidingTowerPlatform);
    spawn_and_init_wf_platforms(MODEL_WF_TOWER_TRAPEZOID_PLATORM, bhvWFSolidTowerPlatform);
    spawn_and_init_wf_platforms(MODEL_WF_TOWER_SQUARE_PLATORM, bhvWFSlidingTowerPlatform);
    spawn_and_init_wf_platforms(MODEL_WF_TOWER_TRAPEZOID_PLATORM, bhvWFSolidTowerPlatform);
    spawn_and_init_wf_platforms(MODEL_WF_TOWER_SQUARE_PLATORM_ELEVATOR, bhvWFElevatorTowerPlatform);
}

void bhv_tower_platform_group_loop(void) { // s_goalbar_main
    f32 marioY = gMarioObject->oPosY;
    o->oDistanceToMario = dist_between_objects(o, gMarioObject);

    switch (o->oAction) {
        case GOALBAR_WAIT:
            if (marioY > (o->oHomeY - 1000.0f)) {
                o->oAction++;
            }
            break;

        case GOALBAR_MAKE:
            spawn_wf_platform_group();
            o->oAction++;
            break;

        case GOALBAR_DISP:
            if (marioY < (o->oHomeY - 1000.0f)) {
                o->oAction++;
            }
            break;

        case GOALBAR_REMOVE:
            o->oAction = 0;
            break;
    }
}
