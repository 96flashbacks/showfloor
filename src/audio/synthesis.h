#ifndef AUDIO_SYNTHESIS_H
#define AUDIO_SYNTHESIS_H

#include "internal.h"
#include "sm64.h"

#define DEFAULT_LEN_1CH 0x140
#define DEFAULT_LEN_2CH 0x280


#define MAX_UPDATES_PER_FRAME 4

struct ReverbRingBufferItem
{
    s16 numSamplesAfterDownsampling;
    s16 chunkLen; // never read
    s16 *toDownsampleLeft;
    s16 *toDownsampleRight; // data pointed to by left and right are adjacent in memory
    s32 startPos; // start pos in ring buffer
    s16 lengths[2]; // first length in ring buffer (max until end) and second length in ring buffer (from pos 0)
}; // size = 0x14

struct SynthesisReverb
{
    /*0x00, 0x00*/ u8 resampleFlags;
   // /*0x01, 0x01*/ u8 useReverb;
    /*0x02, 0x02*/ u8 framesLeftToIgnore;
    /*0x03, 0x03*/ u8 curFrame;
    /*0x04, 0x08*/ u16 reverbGain;
    /*0x06, 0x0A*/ u16 resampleRate;
    /*0x08, 0x0C*/ s32 nextRingBufferPos;
    /*0x10, 0x14*/ s32 bufSizePerChannel;
    struct
    {
        s16 *left;
        s16 *right;
    } ringBuffer;
    /*0x1C, 0x20*/ s16 *resampleStateLeft;
    /*0x20, 0x24*/ s16 *resampleStateRight;
    /*0x2C, 0x30*/ struct ReverbRingBufferItem items[2][MAX_UPDATES_PER_FRAME];
}; // 0xCC <= size <= 0x100


#endif // AUDIO_SYNTHESIS_H
