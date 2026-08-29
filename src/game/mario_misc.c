#include <PR/ultratypes.h>

#include "sm64.h"
#include "camera.h"
#include "engine/graph_node.h"
#include "engine/math_util.h"
#include "game_init.h"
#include "goddard/renderer.h"
#include "level_update.h"
#include "mario_misc.h"
#include "object_helpers.h"
#include "object_list_processor.h"
#include "rendering_graph_node.h"
#include "sound_init.h"

/**
 * The eye texture on succesive frames of Mario's blink animation.
 * He intentionally blinks twice each time.
 */
static s8 gMarioBlinkAnimation[7] = { 1, 2, 1, 0, 1, 2, 1 };

struct MarioBodyState gBodyStates[2]; // 2nd is never accessed in practice, most likely Luigi related

// This whole file is weirdly organized. It has to be the same file due
// to rodata boundaries and function aligns, which means the programmer
// treated this like a "misc" file for vaguely Mario related things
// (Mario head geo, and Mario geo functions)

/**
 * Geo node script that draws Mario's head on the title screen.
 */
Gfx *geo_draw_mario_head_goddard(s32 callContext, struct GraphNode *node, Mat4 *c) {
    Gfx *gfx = NULL;
    s16 sfx = 0;
    struct GraphNodeGenerated *asGenerated = (struct GraphNodeGenerated *) node;
    UNUSED Mat4 *transform = c;

    if (callContext == GEO_CONTEXT_RENDER) {
        if (gPlayer1Controller->controllerData != NULL && !gWarpTransition.isActive) {
            gd_copy_p1_contpad(gPlayer1Controller->controllerData);
        }
        gfx = (Gfx *) PHYSICAL_TO_VIRTUAL(gdm_gettestdl(asGenerated->parameter));
        gGoddardVblankCallback = gd_vblank;
        sfx = gd_sfx_to_play();
        play_menu_sounds(sfx);
    }
    return gfx;
}

/**
 * Determines if Mario is standing or running for the level of detail of his model.
 * If Mario is standing still, he is always high poly. If he is running,
 * his level of detail depends on the distance to the camera.
 */
Gfx *geo_switch_mario_stand_run(s32 callContext, struct GraphNode *node, UNUSED Mat4 *mtx) {
    struct GraphNodeSwitchCase *switchCase = (struct GraphNodeSwitchCase *) node;
    struct MarioBodyState *bodyState = &gBodyStates[switchCase->numCases];

    if (callContext == GEO_CONTEXT_RENDER) {
        // assign result. 0 if moving, 1 if stationary.
        switchCase->selectedCase = ((bodyState->action & ACT_FLAG_STATIONARY) == 0);
    }
    return NULL;
}

/**
 * Geo node script that makes Mario blink
 */
Gfx *geo_switch_mario_eyes(s32 callContext, struct GraphNode *node, UNUSED Mat4 *c) {
    struct GraphNodeSwitchCase *switchCase = (struct GraphNodeSwitchCase *) node;
    struct MarioBodyState *bodyState = &gBodyStates[switchCase->numCases];
    s16 blinkFrame;

    if (callContext == GEO_CONTEXT_RENDER) {
        if (bodyState->eyeState == 0) {
            blinkFrame = ((switchCase->numCases * 32 + gAreaUpdateCounter) >> 1) & 0xF; // 0xF instead of 0x1F
            if (blinkFrame < 7) {
                switchCase->selectedCase = gMarioBlinkAnimation[blinkFrame];
            } else {
                switchCase->selectedCase = 0;
            }
        } else {
            switchCase->selectedCase = bodyState->eyeState - 1;
        }
    }
    return NULL;
}

/**
 * Makes Mario's head rotate with the camera angle when in C-up mode
 */
Gfx *geo_mario_head_rotation(s32 callContext, struct GraphNode *node, UNUSED Mat4 *c) {
    struct GraphNodeGenerated *asGenerated = (struct GraphNodeGenerated *) node;
    struct MarioBodyState *bodyState = &gBodyStates[asGenerated->parameter];
    s32 action = bodyState->action;

    if (callContext == GEO_CONTEXT_RENDER) {
        struct GraphNodeRotation *rotNode = (struct GraphNodeRotation *) node->next;
        struct Camera *camera = gCurGraphNodeCamera->config.camera;

        if (camera->mode == CAMERA_MODE_C_UP) {
            rotNode->rotation[0] = gPlayerCameraState->headRotation[1];
            rotNode->rotation[2] = gPlayerCameraState->headRotation[0];
        } else if (action & ACT_FLAG_WATER_OR_TEXT) {
            rotNode->rotation[0] = bodyState->headAngle[1];
            rotNode->rotation[1] = bodyState->headAngle[2];
            rotNode->rotation[2] = bodyState->headAngle[0];
        } else {
            vec3s_set(bodyState->headAngle, 0, 0, 0);
            vec3s_set(rotNode->rotation, 0, 0, 0);
        }
    }
    return NULL;
}

/**
 * Switch between hand models.
 * Possible options are described in the MarioHandGSCId enum.
 */
Gfx *geo_switch_mario_hand(s32 callContext, struct GraphNode *node, UNUSED Mat4 *c) {
    struct GraphNodeSwitchCase *switchCase = (struct GraphNodeSwitchCase *) node;
    struct MarioBodyState *bodyState = &gBodyStates[0];

    if (callContext == GEO_CONTEXT_RENDER) {
        if (bodyState->handState == MARIO_HAND_FISTS) {
            // switch between fists (0) and open (1)
            switchCase->selectedCase = ((bodyState->action & ACT_FLAG_SWIMMING_OR_FLYING) != 0);
        } else {
            if (switchCase->numCases == 0) {
                switchCase->selectedCase =
                    (bodyState->handState < 5) ? bodyState->handState : MARIO_HAND_OPEN;
            } else {
                switchCase->selectedCase =
                    (bodyState->handState < 2) ? bodyState->handState : MARIO_HAND_FISTS;
            }
        }
    }
    return NULL;
}

/**
 * Geo node that updates the held object node and the HOLP.
 */
Gfx *geo_switch_mario_hand_grab_pos(s32 callContext, struct GraphNode *b, Mat4 *mtx) {
    struct GraphNodeHeldObject *asHeldObj = (struct GraphNodeHeldObject *) b;
    Mat4 *curTransform = mtx;
    struct MarioState *marioState = &gMarioStates[asHeldObj->playerIndex];

    if (callContext == GEO_CONTEXT_RENDER) {
        asHeldObj->objNode = NULL;
        if (marioState->heldObj != NULL) {
            asHeldObj->objNode = marioState->heldObj;
            switch (marioState->marioBodyState->grabPos) {
                case GRAB_POS_LIGHT_OBJ:
                    if (marioState->action & ACT_FLAG_THROWING) {
                        vec3s_set(asHeldObj->translation, 50, 0, 0);
                    } else {
                        vec3s_set(asHeldObj->translation, 50, 0, 110);
                    }
                    break;
                case GRAB_POS_HEAVY_OBJ:
                    vec3s_set(asHeldObj->translation, 145, -173, 180);
                    break;
                case GRAB_POS_BOWSER:
                    vec3s_set(asHeldObj->translation, 80, -180, 1320); // Different offset from the final game
                    break;
            }
        }
    } else if (callContext == GEO_CONTEXT_HELD_OBJ) {
        // ! The HOLP is set here, which is why it only updates when the held object is drawn.
        // This is why it won't update during a pause buffered hitstun or when the camera is very far
        // away.
        get_pos_from_transform_mtx(marioState->marioBodyState->heldObjLastPosition, *curTransform,
                                   *gCurGraphNodeCamera->matrixPtr);
    }
    return NULL;
}
