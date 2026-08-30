#ifndef COMMON1_H
#define COMMON1_H

#include "types.h"
#include "headers.h"

// bub
extern const GeoLayout bub_geo[];
extern const Gfx bub_seg6_dl_06011848[];
extern const Gfx bub_seg6_dl_060118C0[];
extern const Gfx bub_seg6_dl_06011918[];
extern const Gfx bub_seg6_dl_06011968[];
extern const Gfx bub_seg6_dl_06011A50[];
extern const Gfx bub_seg6_dl_06011B28[];
extern const Gfx bub_seg6_dl_06011C58[];
extern const Gfx bub_seg6_dl_06011CF0[];
extern const Gfx bub_seg6_dl_06011DC0[];
extern const Gfx bub_seg6_dl_06011E48[];
extern const Gfx bub_seg6_dl_06011F18[];
extern const Gfx bub_seg6_dl_06011FA0[];
extern const struct Animation *const bub_seg6_anims_06012354[];

// blue_fish
extern const GeoLayout fish_shadow_geo[];
extern const GeoLayout fish_geo[];
extern const Gfx blue_fish_seg3_dl_0301BEC0[];
extern const Gfx blue_fish_seg3_dl_0301BFB8[];
extern const Gfx blue_fish_seg3_dl_0301C0A8[];
extern const Gfx blue_fish_seg3_dl_0301C150[];
extern const struct Animation *const blue_fish_seg3_anims_0301C2B0[];

// butterfly
extern const GeoLayout butterfly_geo[];
extern const Gfx butterfly_seg3_dl_03005408[];
extern const Gfx butterfly_seg3_dl_030054A0[];
extern const struct Animation *const butterfly_seg3_anims_030056B0[];

// coin
extern Hierarchy RCP_HmsItemCoin[];

// door
extern Hierarchy RCP_HmsMainDoor[];
extern Hierarchy RCP_HmsMainroomDoorA[];
extern Hierarchy RCP_HmsMainroomDoorB[];
extern Hierarchy RCP_HmsMainroomDoorC[];
extern Hierarchy RCP_HmsMainroomDoorD[];
extern AnimePtr door_anime[];

// explosion
extern Hierarchy RCP_HmsItembombfire[];

// flame
extern Hierarchy RCP_HmsFireC_Yellow[];
extern Hierarchy RCP_HmsFireC_2_Yellow[];
extern Hierarchy RCP_HmsFireC_Blue[];
extern Hierarchy RCP_HmsFireD_Yellow[];
extern Hierarchy RCP_HmsFireD_Blue[];

// leaves
extern const GeoLayout leaves_geo[];
extern const Gfx leaves_seg3_dl_0301CDE0[];

// power_meter
extern const u8 *const MeterDataPtr[];
extern const Gfx RCP_damegemeter_on[];
extern const Gfx RCP_damegemeter_txt[];
extern const Gfx RCP_damegemeter_off[];

// star
extern Hierarchy RCP_HmsItemStar[];
extern Hierarchy RCP_HmsItemStarDust[];

// transparent_star
extern Hierarchy RCP_HmsDotstar[];

// tree
extern Hierarchy RCP_HmsMainTree[];

// warp_collision
extern const Collision door_seg3_collision_0301CE78[];
extern const Collision lll_hexagonal_mesh_seg3_collision_0301CECC[];

// white_particle
extern const GeoLayout white_particle_geo[];
extern const Gfx white_particle_dl[];

#endif
