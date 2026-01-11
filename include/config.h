#ifndef CONFIG_H
#define CONFIG_H

/**
 * @file config.h
 * A catch-all file for configuring various bugfixes and other settings
 * (maybe eventually) in SM64
 */

// Screen Size Defines
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

// Stack Size Defines
#define IDLE_STACKSIZE 0x800
#define STACKSIZE 0x2000
#define UNUSED_STACKSIZE 0x1400

// What's the point of hav ing a border?
#define BORDER_HEIGHT 2

// POW Build
//#define POWBUILD

// Debug Build
#define DEBUG

#endif // CONFIG_H
