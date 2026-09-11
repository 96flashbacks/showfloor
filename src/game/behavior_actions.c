#include <PR/ultratypes.h>

#include "types.h"
#include "actors/common1.h"
#include "actors/group12.h"
#include "actors/group13.h"
#include "area.h"
#include "audio/external.h"
#include "behavior_actions.h"
#include "behavior_data.h"
#include "camera.h"
#include "debug.h"
#include "dialog_ids.h"
#include "engine/behavior_script.h"
#include "engine/graph_node.h"
#include "engine/math_util.h"
#include "engine/surface_collision.h"
#include "engine/surface_load.h"
#include "game_init.h"
#include "ingame_menu.h"
#include "interaction.h"
#include "level_misc_macros.h"
#include "level_table.h"
#include "level_update.h"
#include "levels/castle_inside/header.h"
#include "main.h"
#include "mario.h"
#include "mario_actions_cutscene.h"
#include "mario_step.h"
#include "obj_behaviors.h"
#include "obj_behaviors_2.h"
#include "object_constants.h"
#include "object_helpers.h"
#include "object_list_processor.h"
#include "paintings.h"
#include "platform_displacement.h"
#include "rendering_graph_node.h"
#include "save_file.h"
#include "seq_ids.h"
#include "sm64.h"
#include "spawn_object.h"
#include "spawn_sound.h"

// This file contains all the objects programmed by Hajime Yajima

#define o gCurrentObject

// Boo Roll
static s16 sBooHitRotations[] = { // rotdata
    6047, 5664, 5292, 4934, 4587, 4254, 3933, 3624, 3329, 3046, 2775, 2517, 2271, 2039, 1818, 1611,
    1416, 1233, 1063, 906,  761,  629,  509,  402,  308,  226,  157,  100,  56,   25,   4,    0,
};

// Yajima iQue source equivalents in the comments
#include "behaviors/mr_i.inc.c" // pathballoon.p
#include "behaviors/pole.inc.c" // pathbar.p
#include "behaviors/beta_chest.inc.c" // pathbox.p
#include "behaviors/water_objs.inc.c" // pathbubble.p and pathbubblejet.p
#include "behaviors/cannon.inc.c" // pathcannon.p
// There's 'pathcastlebg.p' but it only has bhv data (bhvUnused05A8)
#include "behaviors/rotating_platform.inc.c" // pathcastleobj.p
#include "behaviors/warp.inc.c" // pathchimney.p
#include "behaviors/star.inc.c" // Exact placement of 2D star is unknown, the placement here is based on 'pathclearstar.p'
#include "behaviors/coin.inc.c" // pathcoin.p and pathcoinspark.p
#include "behaviors/door.inc.c" // pathdoor.p
#include "behaviors/thwomp.inc.c" // pathdosun.p
#include "behaviors/tumbling_bridge.inc.c" // pathdownbar.p
#include "behaviors/motos_projectile.inc.c" // pathenemyfire.p
#include "behaviors/beta_moving_flames.inc.c" // pathfire.p
#include "behaviors/flamethrower.inc.c" // pathfirebar.p
#include "behaviors/bouncing_fireball.inc.c" // pathfirebar.p
#include "behaviors/flame_mario.inc.c" // pathfiresmoke.p
#include "behaviors/tower_platform.inc.c" // pathgoalbar.p
#include "behaviors/tree_particles.inc.c" // pathgrass.p
#include "behaviors/piranha_bubbles.inc.c" // pathhanapoo.p

// not sure what this is doing here. not in a behavior file. (inside 'pathhitmario.p' in the iQue source)
Gfx *geo_move_mario_part_from_parent(s32 run, UNUSED struct GraphNode *node, Mat4 mtx) { // MarioProc1
    if (run == TRUE) {
        Mat4 sp20;
        struct Object *obj = (struct Object *) gCurGraphNodeObject;
        if (obj == gMarioObject && obj->prevObj != NULL) {
            create_transformation_from_matrices(sp20, mtx, *gCurGraphNodeCamera->matrixPtr);
            obj_update_pos_from_parent_transformation(sp20, obj->prevObj);
            obj_set_gfx_pos_from_pos(obj->prevObj);
        }
    }

    return NULL;
}

// Technically 'pathhopper.p' is called here but it's completely empty
#include "behaviors/beta_trampoline.inc.c" // pathjumpbg.p
#include "behaviors/beta_boo_key.inc.c" // pathkey.p
#include "behaviors/bullet_bill.inc.c" // pathkiller.p
#include "behaviors/bowser.inc.c" // pathkopa.p
#include "behaviors/bowser_flame.inc.c" // pathkopafire.p
#include "behaviors/blue_fish.inc.c" // pathloopfish.p
#include "behaviors/motos.inc.c" // pathmotos.p (not in the final game)
#include "behaviors/lll_octagonal_moving_mesh.inc.c" // pathmotosobj.p
#include "behaviors/lll_rotating_hex_flame.inc.c" // pathmotosobj.p
#include "behaviors/lll_floating_wood_piece.inc.c" // pathmotosobj.p
#include "behaviors/lll_volcano_flames.inc.c" // pathmotosobj.p
#include "behaviors/lll_hexagonal_ring.inc.c" // pathmotosobj.p
#include "behaviors/lll_sinking_rectangle.inc.c" // pathmotosobj.p
#include "behaviors/tilting_inverted_pyramid.inc.c" // pathmovebg.p
#include "behaviors/boo.inc.c" // pathobake.p (moved to 'pathteresa.p' in the final game)
#include "behaviors/coin_inside_boo.inc.c" // pathobakecoin.p (moved to 'pathcoin.p' in the final game)
#include "behaviors/piranha_plant.inc.c" // pathpakun.p
// There's 'pathpatch.p' but it just has bhv data (bhvLLLHexagonalMesh)
#include "behaviors/bowser_puzzle_piece.inc.c" // pathpazzule.p
#include "behaviors/tuxie.inc.c" // pathping.p
#include "behaviors/fish.inc.c" // pathpirania.p
#include "behaviors/bub.inc.c" // pathpuku.p
#include "behaviors/sound_spawner.inc.c" // pathremovesound.p
// pathsensuikan.p (DDD submarine bhv file) has functions in the final game, seemingly only had bhv data in the demo
#include "behaviors/sushi.inc.c" // pathshark.p
#include "behaviors/white_puff.inc.c" // pathsmoke.p
#include "behaviors/test_player_fire.inc.c" // pathtest.p
#include "behaviors/castle_floor_trap.inc.c" // pathtrap.p
#include "behaviors/pole_base.inc.c" // pathtree.p
#include "behaviors/sparkle_spawn.inc.c" // pathtwinkle.p
#include "behaviors/blargg.inc.c" // pathunbaba.p (not in the final game)
#include "behaviors/whomp.inc.c" // pathwallman.p
#include "behaviors/water_splashes_and_waves.inc.c" // pathwater.p, pathwaterdive.p, pathwaterripple.p and pathwaterwave.p
// There's 'pathyellowball.p' here but it just has bhv data (bhvYellowBall)
