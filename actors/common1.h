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

// dirt
extern const GeoLayout dirt_animation_geo[];
extern const GeoLayout cartoon_star_geo[];
extern const Gfx dirt_seg3_dl_0302BFF8[];
extern const Gfx dirt_seg3_dl_0302C028[];
extern const Gfx dirt_seg3_dl_0302C238[];
extern const Gfx dirt_seg3_dl_0302C298[];
extern const Gfx dirt_seg3_dl_0302C2B8[];
extern const Gfx dirt_seg3_dl_0302C2D8[];
extern const Gfx dirt_seg3_dl_0302C2F8[];
extern const Gfx dirt_seg3_dl_0302C318[];
extern const Gfx dirt_seg3_dl_0302C378[];
extern const Gfx dirt_seg3_dl_0302C3B0[];
extern const Gfx dirt_seg3_dl_0302C3E8[];
extern const Gfx dirt_seg3_dl_0302C420[];
extern const Gfx dirt_seg3_dl_0302C458[];

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
extern const GeoLayout red_flame_shadow_geo[];
extern const GeoLayout red_flame_geo[];
extern const GeoLayout yellow_flame_geo[];
extern const Gfx flame_seg3_dl_0301B320[];
extern const Gfx flame_seg3_dl_0301B3B0[];
extern const Gfx flame_seg3_dl_0301B3C8[];
extern const Gfx flame_seg3_dl_0301B3E0[];
extern const Gfx flame_seg3_dl_0301B3F8[];
extern const Gfx flame_seg3_dl_0301B410[];
extern const Gfx flame_seg3_dl_0301B428[];
extern const Gfx flame_seg3_dl_0301B440[];
extern const Gfx flame_seg3_dl_0301B458[];
extern const Gfx RCP_FireC_Yellow01[];
extern const Gfx RCP_FireC_Yellow02[];
extern const Gfx RCP_FireC_Yellow03[];
extern const Gfx RCP_FireC_Yellow04[];
extern const Gfx RCP_FireC_Yellow05[];
extern const Gfx RCP_FireC_Yellow06[];
extern const Gfx RCP_FireC_Yellow07[];
extern const Gfx RCP_FireC_Yellow08[];
extern const Gfx RCP_FireC_Yellow09[];
extern const Gfx RCP_FireC_Yellow10[];
extern const Gfx RCP_FireC_Yellow11[];
extern const Gfx RCP_FireC_Yellow12[];
extern const Gfx RCP_FireC_Yellow13[];
extern const Gfx RCP_FireC_Yellow14[];
extern const Gfx RCP_FireC_Yellow15[];

// leaves
extern const GeoLayout leaves_geo[];
extern const Gfx leaves_seg3_dl_0301CDE0[];

// mist
extern const GeoLayout mist_geo[];
extern const GeoLayout white_puff_geo[];
extern const Gfx mist_seg3_dl_03000880[];
extern const Gfx mist_seg3_dl_03000920[];

// number
extern const GeoLayout number_geo[];

// power_meter
extern const u8 *const MeterDataPtr[];
extern const Gfx RCP_damegemeter_on[];
extern const Gfx RCP_damegemeter_txt[];
extern const Gfx RCP_damegemeter_off[];

// sand
extern const Gfx sand_seg3_dl_0302BCD0[];

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
