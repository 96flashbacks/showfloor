// water_splashes_and_waves.inc.c

// No WaterDropletParams according to pathwater.p

// pathwater.p code

// The wf fish splash spawner wasn't in a separate file based on pathwater.p
void bhv_beta_fish_splash_spawner_loop(void) { // s_funsui_main (modified)
    s32 i;
    f32 scale;
    struct Object *obj;
    f32 waterLevel = find_water_level(o->oPosX, o->oPosZ);

     switch (o->oAction) {
        case 0:
            if (o->oDistanceToMario < 200.0f)	
                o->oAction++;
            break;
        case 1:
            if (waterLevel > FLOOR_LOWER_LIMIT_MISC) {
                if (gPrevFrameObjectCount < (OBJECT_POOL_CAPACITY - 28)) {
                    if (gGlobalTimer % 32 == 0)	
                        cur_obj_play_sound_2(SOUND_GENERAL_MOVING_WATER);

                    for (i = 0; i < 2; i++) {	
                        if ((random_u16() & 0x1F) == 0) {
                            obj = spawn_object(o, MODEL_FISH, bhvWaterDroplet);
                            obj_init_animation_with_sound(obj, blue_fish_seg3_anims_0301C2B0, 0);
                        } else if ((random_u16() & 0x3F) == 0) {
                            // Bub can be spawned alongside regular fish, as seen in SuperMarioStadium3 (12:49)
                            obj = spawn_object(o, MODEL_BUB, bhvWaterDroplet);
                            obj_init_animation_with_sound(obj, bub_seg6_anims_06012354, 0);
                        } else {
                            obj = spawn_object(o, MODEL_WHITE_PARTICLE_SMALL, bhvWaterDroplet);
                            scale = random_float() * 1.0f + 0.5f;
                            obj_scale_xyz(obj, scale, scale, scale);
                            obj_set_billboard(obj);
                        }

                        obj->oMoveAngleYaw = random_u16();
                        obj->oPosY = waterLevel;
                        obj->oForwardVel = random_float() * 3.0f + 2.0f;
                        obj->oVelY = random_float() * 20.0f + 20.0f;
                    }
                }
            }

            if (o->oDistanceToMario > 300.0f)	
                o->oAction = 0;
            break;
    }
}

void bhv_shallow_water_splash_init(void) { // s_jumpsplash_init (modified)
    s32 i;
    f32	scale;
    f32	waterLevel;
    struct Object *obj;

    if (gPrevFrameObjectCount < (OBJECT_POOL_CAPACITY - 28)) {
        waterLevel = find_water_level(o->oPosX, o->oPosZ);
        if (waterLevel > FLOOR_LOWER_LIMIT_MISC) {
            for (i = 0; i < 20; i++) {
                // Removed fish easter egg check from pathwater.p
                obj = spawn_object(o, MODEL_WHITE_PARTICLE_SMALL, bhvWaterDroplet);
                scale = random_float() * 1.0f + 0.5f;
                obj_scale_xyz(obj, scale, scale, scale);
                obj_set_billboard(obj);              
                obj->oMoveAngleYaw = random_u16();
                obj->oPosY = waterLevel;
                obj->oForwardVel = random_float() * 3.0f + 2.0f;
                obj->oVelY = random_float() * 20.0f + 20.0f;
            }
        }
    }

    gMarioObject->oActiveParticleFlags &= ~ACTIVE_PARTICLE_SHALLOW_WATER_SPLASH;
}

void bhv_shallow_water_wave_init(void) { // s_smallsplash_init
    s32	i;
    f32	scale;
    f32	waterLevel;
    struct Object *obj;

    if (gPrevFrameObjectCount < (OBJECT_POOL_CAPACITY - 28)) {
        waterLevel = find_water_level(o->oPosX, o->oPosZ);
        if (waterLevel > FLOOR_LOWER_LIMIT_MISC) {
            for (i = 0; i < 5; i++) {
                obj = spawn_object(o, MODEL_WHITE_PARTICLE_SMALL, bhvWaterDroplet);
                obj->oMoveAngleYaw = (gMarioObject->oMoveAngleYaw + 0x8000) + (s32)(random_float() * 0x6000 - 0x3000);
                obj->oPosY = waterLevel;
                obj->oForwardVel = random_float() * 8.0f + 2.0f;
                obj->oVelY = random_float() * 10.0f + 10.0f;
                scale = random_float() * 1.0f + 0.5f;
                obj_scale_xyz(obj, scale, scale, scale);
                obj_set_billboard(obj);
            }
        }
    }

    gMarioObject->oActiveParticleFlags &= ~ACTIVE_PARTICLE_SHALLOW_WATER_WAVE;
}

void bhv_water_droplet_splash_init(void) { // s_smallripple_init
    cur_obj_scale(random_float() + 1.5);
}

void bhv_water_droplet_loop(void) { // s_smalldrop_main
    struct Object *obj;
    f32 waterLevel = find_water_level(o->oPosX, o->oPosZ);

    if (o->oTimer == 0) {
        o->oFaceAngleYaw = random_u16();
    }

    // Apply gravity
    o->oVelY -= 4.0f;
    o->oPosY += o->oVelY;

    // Check if fallen back into the water
    if (o->oVelY < 0.0f) {
        if (waterLevel > o->oPosY) {
            // Create the smaller splash
            obj = spawn_object_at_origin(o, 0, MODEL_SMALL_WATER_SPLASH, bhvWaterDropletSplash);
            obj->oPosX = o->oPosX;
            obj->oPosY = waterLevel + 5;
            obj->oPosZ = o->oPosZ;
            obj_mark_for_deletion(o);
        } else if (o->oTimer > 60) {
            obj_mark_for_deletion(o);
        }
        
    }

    if (waterLevel < FLOOR_LOWER_LIMIT_MISC) {
        obj_mark_for_deletion(o);
    }
}

// Vanilla code

void bhv_water_splash_spawn_droplets(void) { // s_waterdive_main (modified)
    s32	i;
    f32	scale;
    struct Object *obj;
    
    if (o->oTimer == 0) {
        o->oPosY = find_water_level(o->oPosX, o->oPosZ);
    }

    if (o->oPosY > FLOOR_LOWER_LIMIT_MISC) { // Make sure it is not at the default water level
        for (i = 0; i < 3; i++) {
            // Manually spawn the particles like the pathwater.p objects
            obj = spawn_object(o, MODEL_WHITE_PARTICLE_SMALL, bhvWaterDroplet);
            obj->oMoveAngleYaw = random_u16();
            obj->oForwardVel = random_float() * 3.0f + 5.0f;
            obj->oVelY = random_float() * 20.0f + 30.0f;
            scale = random_float() * 1.0f + 0.5f;
            obj_scale_xyz(obj, scale, scale, scale);
            obj_set_billboard(obj);
        }
    }
}

void bhv_bubble_splash_init(void) { // s_dropripple_init
    f32 waterLevel = find_water_level(o->oPosX, o->oPosZ);
    obj_scale_xyz(o, 0.5f, 1.0f, 0.5f);
    o->oPosY = waterLevel + 5.0f;
}

void bhv_idle_water_wave_loop(void) { // s_playerripple_main
    obj_copy_pos(o, gMarioObject);
    o->oPosY = gMarioStates[0].waterLevel + 5;
    if (!(gMarioObject->oMarioParticleFlags & ACTIVE_PARTICLE_IDLE_WATER_WAVE)) {
        gMarioObject->oActiveParticleFlags &= (u16) ~ACTIVE_PARTICLE_IDLE_WATER_WAVE;
        o->activeFlags = ACTIVE_FLAG_DEACTIVATED;
    }
}

void bhv_wave_trail_shrink(void) { // s_wave_main
    f32 waterLevel = find_water_level(o->oPosX, o->oPosZ);
    // Destroy every other water wave to space them out (this is a terrible way of doing it)
    if ((o->oTimer == 0) && (gGlobalTimer & 1)) {
        obj_mark_for_deletion(o);
    }
    o->oPosY = waterLevel + 5.0f;

    if (o->oTimer == 0) {
        o->oWaveTrailSize = o->header.gfx.scale[0];
    }

    if (o->oAnimState > 3) {
        o->oWaveTrailSize = o->oWaveTrailSize - 0.1; // Shrink the wave
        if (o->oWaveTrailSize < 0.0f) {
            o->oWaveTrailSize = 0.0f;
        }
        o->header.gfx.scale[0] = o->oWaveTrailSize;
        o->header.gfx.scale[2] = o->oWaveTrailSize;
    }
}
