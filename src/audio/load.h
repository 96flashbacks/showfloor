#ifndef AUDIO_LOAD_H
#define AUDIO_LOAD_H

#include <PR/ultratypes.h>

#include "internal.h"

#define AUDIO_FRAME_DMA_QUEUE_SIZE 0x40

#define PRELOAD_BANKS 2
#define PRELOAD_SEQUENCE 1
#define IS_SEQUENCE_CHANNEL_VALID(ptr) ((uintptr_t)(ptr) != (uintptr_t)0)

extern struct Note *gNotes;

// Music in SM64 is played using 3 players:
// gSequencePlayers[0] is level background music
// gSequencePlayers[1] is misc music, like the puzzle jingle
// gSequencePlayers[2] is sound
extern struct SequencePlayer gSequencePlayers[SEQUENCE_PLAYERS];
#define ALIGN16(val) (((val) + 0xF) & ~0xF)

struct SharedDma {
    /*0x0*/ u8 *buffer;       // target, points to pre-allocated buffer
    /*0x4*/ uintptr_t source; // device address
    /*0x8*/ u16 sizeUnused;   // set to bufSize, never read
    /*0xA*/ u16 bufSize;      // size of buffer
    /*0xC*/ u8 unused2;       // set to 0, never read
    /*0xD*/ u8 reuseIndex;    // position in sSampleDmaReuseQueue1/2, if ttl == 0
    /*0xE*/ u8 ttl;           // duration after which the DMA can be discarded
};      




#define gAudioUpdatesPerFrame 4


#endif // AUDIO_LOAD_H
