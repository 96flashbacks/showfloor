// 0x07000000 - 0x07000800
ALIGNED8 static const u8 wf_seg7_texture_07000000[] = {
#include "levels/wf/0.rgba16.inc.c"
};

// 0x07000800 - 0x07001000
ALIGNED8 static const u8 wf_seg7_texture_07000800[] = {
#include "levels/wf/1.rgba16.inc.c"
};

// 0x07001000 - 0x07001800
ALIGNED8 static const u8 wf_seg7_texture_07001000[] = {
#include "levels/wf/2.rgba16.inc.c"
};

// 0x07001800 - 0x07002000
ALIGNED8 static const u8 wf_seg7_texture_07001800[] = {
#include "levels/wf/3.rgba16.inc.c"
};

// 0x07002000 - 0x07002800
ALIGNED8 static const u8 wf_seg7_texture_07002000[] = {
#include "levels/wf/4.rgba16.inc.c"
};

// 0x07002800 - 0x07003800
ALIGNED8 static const u8 wf_seg7_texture_07002800[] = {
#include "levels/wf/5.rgba16.inc.c"
};

// 0x07003800 - 0x07004000
ALIGNED8 static const u8 wf_seg7_texture_07003800[] = {
#include "levels/wf/6.rgba16.inc.c"
};

// 0x07004000 - 0x07004800
ALIGNED8 static const u8 wf_seg7_texture_07004000[] = {
#include "levels/wf/7.rgba16.inc.c"
};

// 0x07004800 - 0x07005800
ALIGNED8 static const u8 wf_seg7_texture_07004800[] = {
#include "levels/wf/8.rgba16.inc.c"
};

// 0x07005800 - 0x07006000
ALIGNED8 static const u8 wf_seg7_texture_07005800[] = {
#include "levels/wf/9.rgba16.inc.c"
};

// 0x07006000 - 0x07006800
ALIGNED8 static const u8 wf_seg7_texture_07006000[] = {
#include "levels/wf/10.rgba16.inc.c"
};

// 0x07006800 - 0x07007000
ALIGNED8 static const u8 wf_seg7_texture_07006800[] = {
#include "levels/wf/11.ia16.inc.c"
};

// 0x07007000 - 0x07007800
ALIGNED8 static const u8 wf_seg7_texture_07007000[] = {
#include "levels/wf/12.rgba16.inc.c"
};

// 0x07007800 - 0x07008000
ALIGNED8 static const u8 wf_seg7_texture_07007800[] = {
#include "levels/wf/13.rgba16.inc.c"
};

// 0x07007E00 - 0x07008600
ALIGNED8 static const u8 wf_seg7_texture_07007E00[] = {
#include "levels/wf/14.rgba16.inc.c"
};

// 0x07008600 - 0x07008E00
ALIGNED8 static const u8 wf_seg7_texture_07008600[] = {
#include "levels/wf/15.rgba16.inc.c"
};

// 0x07008E00 - 0x07009600
ALIGNED8 static const u8 wf_seg7_texture_07008E00[] = {
#include "levels/wf/16.rgba16.inc.c"
};

// 0x07009600 - 0x07009E00
ALIGNED8 static const u8 wf_seg7_texture_07009600[] = {
#include "levels/wf/17.rgba16.inc.c"
};

// 0x07009E00 - 0x0700A600
ALIGNED8 static const u8 wf_seg7_texture_07009E00[] = {
#include "levels/wf/18.rgba16.inc.c"
};

// 0x0700A600 - 0x0700AE00
ALIGNED8 static const u8 wf_seg7_texture_0700A600[] = {
#include "levels/wf/19.rgba16.inc.c"
};

// 0x0700AE00 - 0x0700B600
ALIGNED8 static const u8 wf_seg7_texture_0700AE00[] = {
#include "levels/wf/20.rgba16.inc.c"
};

#ifdef POWBUILD
ALIGNED8 static const u8 wf_seg7_texture_areamap_top[] = {
#include "levels/wf/areamap_mountain_top_pow.rgba16.inc.c"
};

ALIGNED8 static const u8 wf_seg7_texture_areamap_bottom[] = {
#include "levels/wf/areamap_mountain_bottom_pow.rgba16.inc.c"
};
#else
ALIGNED8 static const u8 wf_seg7_texture_areamap_top[] = {
#include "levels/wf/areamap_mountain_top.rgba16.inc.c"
};

ALIGNED8 static const u8 wf_seg7_texture_areamap_bottom[] = {
#include "levels/wf/areamap_mountain_bottom.rgba16.inc.c"
};
#endif
