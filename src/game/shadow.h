#ifndef SHADOW_H
#define SHADOW_H

#include <PR/ultratypes.h>
#include <PR/gbi.h>

/**
 * Shadow types. Shadows are circles, squares, or hardcoded rectangles, and
 * can be composed of either 4 or 9 vertices.
 */
enum ShadowType {
    SHADOW_CIRCLE_9_VERTS = 0,
    SHADOW_CIRCLE_4_VERTS = 1,
    SHADOW_CIRCLE_4_VERTS_FLAT = 2,
    SHADOW_CIRCLE_LUIGI = 98, // The 'shade.c' backup shows ID 98 was for Luigi
    SHADOW_CIRCLE_MARIO = 99
};

/**
 * Flag for if the current shadow is above water or lava.
 */
extern s8 gShadowAboveWaterOrLava;

/**
 * Given the (x, y, z) location of an object, create a shadow below that object
 * with the given initial solidity and "shadowType" (described above).
 */
Gfx *create_shadow_below_xyz(f32 xPos, f32 yPos, f32 zPos, s16 shadowScale, u8 shadowSolidity, s8 shadowType);

#endif // SHADOW_H
