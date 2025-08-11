//! Copt inlining for US/JP. Here be dragons

#include <ultra64.h>
#include <macros.h>

#include "heap.h"
#include "data.h"
#include "load.h"
#include "seqplayer.h"
#include "external.h"
#include "effects.h"

#define PORTAMENTO_IS_SPECIAL(x) ((x).mode & 0x80)
#define PORTAMENTO_MODE(x) ((x).mode & ~0x80)
#define PORTAMENTO_MODE_1 1
#define PORTAMENTO_MODE_2 2
#define PORTAMENTO_MODE_3 3
#define PORTAMENTO_MODE_4 4
#define PORTAMENTO_MODE_5 5



    #define PORTAMENTO_TGT_NOTE portamentoTargetNote
    #define DRUM_INDEX          cmdSemitone
    #define SEMITONE            cmdSemitone
