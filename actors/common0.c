#include <PR/ultratypes.h>
#include <PR/gbi.h>

#include "macros.h"
#include "surface_terrains.h"
#include "types.h"
#include "make_const_nonconst.h"
#include "headers.h"

#define SCALE 0.25

#include "cannon_base/cannon_base.sou"
UNUSED static const u64 binid_3 = 3;

#include "cannon_barrel/cannon_body.sou"
UNUSED static const u64 binid_4 = 4;

#include "test_platform/model.inc.c"
#include "test_platform/collision.inc.c"
UNUSED static const u64 binid_17 = 17;

#include "koopa_shell/nokonoko.sou"
UNUSED static const u64 binid_18 = 18;
