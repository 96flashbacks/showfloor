
/**
 * Behavior for bhvHana and bhvHanaTripletSpawner,
 * Hanas can either be spawned individually, or spawned by a triplet spawner.
 * The triplet spawner comes before its spawned hanas in processing order.
 */

/**
 * Hitbox for hana.
 */
static struct ObjectHitbox sHanaHitbox = {
    /* interactType:      */ INTERACT_BOUNCE_TOP,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 1,
    /* health:            */ 0,
    /* numLootCoins:      */ 1,
    /* radius:            */ 72,
    /* height:            */ 50,
    /* hurtboxRadius:     */ 42,
    /* hurtboxHeight:     */ 40,
};

/**
 * Properties that vary based on hana size.
 */
struct HanaProperties {
    f32 scale;
    u32 deathSound;
    s16 drawDistance;
    s8 damage;
};

/**
 * Properties for regular, huge, and tiny hanas.
 */
static struct HanaProperties sHanaProperties[] = {
    { 1.5f, SOUND_OBJ_DYING_ENEMY1, 4000, 1 },
    { 3.5f, SOUND_OBJ_DYING_ENEMY1, 4000, 2 },
    { 0.5f, SOUND_OBJ_DYING_ENEMY1, 1500, 0 },
};

/**
 * Attack handlers for hanas.
 */
static u8 sHanaAttackHandlers[][6] = {
    // regular and tiny
    {
        /* ATTACK_PUNCH:                 */ ATTACK_HANDLER_KNOCKBACK,
        /* ATTACK_KICK_OR_TRIP:          */ ATTACK_HANDLER_KNOCKBACK,
        /* ATTACK_FROM_ABOVE:            */ ATTACK_HANDLER_SPECIAL_HUGE_GOOMBA_WEAKLY_ATTACKED,
        /* ATTACK_GROUND_POUND_OR_TWIRL: */ ATTACK_HANDLER_SQUISHED,
        /* ATTACK_FAST_ATTACK:           */ ATTACK_HANDLER_KNOCKBACK,
        /* ATTACK_FROM_BELOW:            */ ATTACK_HANDLER_KNOCKBACK,
    },
};


/**
 * Initialization function for hana.
 */
void bhv_hana_init(void) {
    o->oGoombaSize = o->oBhvParams2ndByte & HANA_BP_SIZE_MASK;

    o->oGoombaScale = sHanaProperties[o->oGoombaSize].scale;
    o->oDeathSound = sHanaProperties[o->oGoombaSize].deathSound;

    obj_set_hitbox(o, &sHanaHitbox);

    o->oDrawingDistance = sHanaProperties[o->oGoombaSize].drawDistance;
    o->oDamageOrCoinValue = sHanaProperties[o->oGoombaSize].damage;

    o->oGravity = -8.0f / 3.0f * o->oGoombaScale;
}

/**
 * Enter the jump action and set initial y velocity.
 */
/*static void hana_begin_jump(void) {
    cur_obj_play_sound_2(SOUND_OBJ_HANA_ALERT);

    o->oAction = HANA_ACT_JUMP;
    o->oForwardVel = 0.0f;
    o->oVelY = 50.0f / 3.0f * o->oGoombaScale;
}*/

/**
 * If spawned by a triplet spawner, mark the flag in the spawner to indicate that
 * this hana died. This prevents it from spawning again when mario leaves and
 * comes back.
 */

/**
 * Walk around randomly occasionally jumping. If mario comes within range,
 * chase him.
 */
static void hana_act_walk(void) {
    treat_far_home_as_mario(1000.0f);

    obj_forward_vel_approach(o->oGoombaRelativeSpeed * o->oGoombaScale, 0.4f);

    // If walking fast enough, play footstep sounds
    /*if (o->oGoombaRelativeSpeed > 4.0f / 3.0f) {
        cur_obj_play_sound_at_anim_range(2, 17, SOUND_OBJ_HANA_WALK);
    }*/

    //! By strategically hitting a wall, steep slope, or another hana, we can
    //  prevent the hana from turning back toward home for a while (hana
    //  chase extension)
    //! It seems theoretically possible to get 2-3 hanas to repeatedly touch
    //  each other and move arbitrarily far from their home, but it's
    //  extremely precise and chaotic in practice, so probably can't be performed
    //  for nontrivial distances
    if (o->oGoombaTurningAwayFromWall) {
        o->oGoombaTurningAwayFromWall = obj_resolve_collisions_and_turn(o->oGoombaTargetYaw, 0x200);
    } else {
        // If far from home, walk toward home.
        if (o->oDistanceToMario >= 25000.0f) {
            o->oGoombaTargetYaw = o->oAngleToMario;
            o->oGoombaWalkTimer = random_linear_offset(20, 30);
        }

        if (!(o->oGoombaTurningAwayFromWall =
                  obj_bounce_off_walls_edges_objects(&o->oGoombaTargetYaw))) {

                o->oGoombaRelativeSpeed = 4.0f;

                if (o->oGoombaWalkTimer != 0) {
                    o->oGoombaWalkTimer--;
                } else {
                    if (random_u16() & 3) {
                        o->oGoombaTargetYaw = obj_random_fixed_turn(0x2000);
                        o->oGoombaWalkTimer = random_linear_offset(100, 100);
                    } else {
                        //hana_begin_jump();
                        o->oGoombaTargetYaw = obj_random_fixed_turn(0x6000);
                    }
                }
            }
        }

        cur_obj_rotate_yaw_toward(o->oGoombaTargetYaw, 0x200);
    }

/**
 * This action occurs when either the hana attacks mario normally, or mario
 * attacks a huge hana with an attack that doesn't kill it.
 */
static void hana_act_attacked_mario(void) {
    if (o->oGoombaSize == HANA_SIZE_TINY) {
        mark_goomba_as_dead();
        o->oNumLootCoins = 0;
        obj_die_if_health_non_positive();
    } else {
        //! This can happen even when the hana is already in the air. It's
        //  hard to chain these in practice
        //hana_begin_jump();
        o->oGoombaTargetYaw = o->oAngleToMario;
        o->oGoombaTurningAwayFromWall = FALSE;
    }
}

/**
 * Move until landing, and rotate toward target yaw.
 */
static void hana_act_jump(void) {
    obj_resolve_object_collisions(NULL);

    //! If we move outside the hana's drawing radius the frame it enters the
    //  jump action, then it will keep its velY, but it will still be counted
    //  as being on the ground.
    //  Next frame, the jump action will think it has already ended because it is
    //  still on the ground.
    //  This puts the hana back in the walk action, but the positive velY will
    //  make it hop into the air. We can then trigger another jump.
    if (o->oMoveFlags & OBJ_MOVE_MASK_ON_GROUND) {
        o->oAction = HANA_ACT_WALK;
    } else {
        cur_obj_rotate_yaw_toward(o->oGoombaTargetYaw, 0x800);
    }
}

/**
 * Attack handler for when mario attacks a huge hana with an attack that
 * doesn't kill it.
 * From the hana's perspective, this is the same as the hana attacking
 * mario.
 */
void huge_hana_weakly_attacked(void) {
    o->oAction = HANA_ACT_ATTACKED_MARIO;
}

/**
 * Update function for hana.
 */
void bhv_hana_update(void) {
    // PARTIAL_UPDATE

    f32 animSpeed;

    if (obj_update_standard_actions(o->oGoombaScale)) {

        cur_obj_scale(o->oGoombaScale);
        cur_obj_update_floor_and_walls();
            animSpeed = 1.0f;
        cur_obj_init_animation_with_accel_and_sound(0, animSpeed);

        switch (o->oAction) {
            case HANA_ACT_WALK:
                hana_act_walk();
                break;
            case HANA_ACT_ATTACKED_MARIO:
                hana_act_attacked_mario();
                break;
            case HANA_ACT_JUMP:
                hana_act_jump();
                break;
        }

        //! @bug Weak attacks on huge hanas in a triplet mark them as dead even if they're not.
        // obj_handle_attacks returns the type of the attack, which is non-zero
        // even for Mario's weak attacks. Thus, if Mario weakly attacks a huge hana
        // without harming it (e.g. by punching it), the hana will be marked as dead
        // and will not respawn if Mario leaves and re-enters the spawner's radius
        // even though the hana isn't actually dead.
        if (obj_handle_attacks(&sHanaHitbox, HANA_ACT_ATTACKED_MARIO,
                               sHanaAttackHandlers[o->oGoombaSize & 1])) {
            mark_goomba_as_dead();
        }

        cur_obj_move_standard(-78);
    } else {
        o->oAnimState = 1;
    }
}
