#ifndef INTRO_GEO_H
#define INTRO_GEO_H

#include <PR/ultratypes.h>
#include <PR/gbi.h>

#include "types.h"
#include "engine/graph_node.h"

Gfx *geo_intro_nintendo_logo(s32 sp50, struct GraphNode *sp54, UNUSED void *context);
Gfx *geo_intro_regular_backdrop(s32 sp48, struct GraphNode *sp4c, UNUSED void *context);
Gfx *geo_intro_gameover_backdrop(s32 sp40, struct GraphNode *sp44, UNUSED void *context);

#endif // INTRO_GEO_H
