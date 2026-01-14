#ifndef COMMON0_H
#define COMMON0_H

#include "types.h"
#include "headers.h"

// cannon_barrel
extern Hierarchy RCP_HmsCannonBody[];

// cannon_base
extern Hierarchy RCP_HmsCannonBase[];

// carryboy
extern Hierarchy RCP_HmsEnemycarryboy[];
extern Gfx RCP_carry_boy_carry9[];
extern Gfx RCP_carry_boy_carry0[];
extern Gfx RCP_carry_boy_carry1[];
extern Gfx RCP_carry_boy_carry8[];
extern Gfx RCP_carry_boy_carry7[];
extern Gfx RCP_carry_boy_carry5[];
extern Gfx RCP_carry_boy_carry2[];
extern Gfx RCP_carry_boy_carry6[];
extern Gfx RCP_carry_boy_carry3[];
extern Gfx RCP_carry_boy_carry4[];
extern AnimePtr carryboy_anime[];

// bobomb
extern const Hierarchy RCP_HmsEnemybom_basedata[];
extern const Gfx RCP_bom_basedata0[];
extern const Gfx RCP_bom_basedata1[];
extern const Gfx RCP_bom_basedata2[];
extern const Gfx gfx_bom[];
extern const Gfx gfx_bbom[];
extern const Gfx gfx_rbom[];
extern AnimePtr bom_anime[];

// goomba
extern Hierarchy RCP_HmsEnemykuribo[];
extern Gfx kuribo_light_sourcex[];
extern Gfx gfx_kuri_body[];
extern Gfx RCP_kuribo_walk3[];
extern Gfx RCP_kuribo_walk2[];
extern Gfx gfx_kuri_head1[];
extern Gfx gfx_kuri_head2[];
extern Gfx RCP_kuribo_walk0[];
extern Gfx RCP_kuribo_walk1[];
extern AnimePtr kuribo_anime[];
#endif
