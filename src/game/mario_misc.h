#ifndef MARIO_MISC_H
#define MARIO_MISC_H

#include <PR/ultratypes.h>

#include "macros.h"
#include "types.h"

extern struct MarioBodyState gBodyStates[2];

Gfx *geo_draw_mario_head_goddard(s32 callContext, struct GraphNode *node, Mat4 *c);
Gfx *geo_switch_mario_stand_run(s32 callContext, struct GraphNode *node, UNUSED Mat4 *mtx);
Gfx *geo_switch_mario_eyes(s32 callContext, struct GraphNode *node, UNUSED Mat4 *c);
Gfx *geo_mario_head_rotation(s32 callContext, struct GraphNode *node, UNUSED Mat4 *c);
Gfx *geo_switch_mario_hand(s32 callContext, struct GraphNode *node, UNUSED Mat4 *c);
Gfx *geo_switch_mario_hand_grab_pos(s32 callContext, struct GraphNode *b, Mat4 *mtx);

#endif // MARIO_MISC_H
