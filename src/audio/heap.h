#ifndef AUDIO_HEAP_H
#define AUDIO_HEAP_H

#include <PR/ultratypes.h>

#include "internal.h"

#define SOUND_LOAD_STATUS_NOT_LOADED     0
#define SOUND_LOAD_STATUS_IN_PROGRESS    1
#define SOUND_LOAD_STATUS_COMPLETE       2
#define SOUND_LOAD_STATUS_DISCARDABLE    3

#define IS_BANK_LOAD_COMPLETE(bankId) (gBankLoadStatus[bankId] >= SOUND_LOAD_STATUS_COMPLETE)
#define IS_SEQ_LOAD_COMPLETE(seqId) (gSeqLoadStatus[seqId] >= SOUND_LOAD_STATUS_COMPLETE)

#define gAudioHeapSize DOUBLE_SIZE_ON_64_BIT(AUDIO_HEAP_SIZE)

struct SoundAllocPool
{
    u8 *start;
    u8 *cur;
    u32 size;
}; // size = 0x10

struct SeqOrBankEntry {
    u8 *ptr;
    u32 size;
    s32 id; // seqId or bankId
}; // size = 0xC

struct PersistentPool
{
    /*0x00*/ u32 numEntries;
    /*0x04*/ struct SoundAllocPool pool;
    /*0x14*/ struct SeqOrBankEntry entries[32];
}; // size = 0x194

struct TemporaryPool
{
    /*0x00*/ u32 nextSide;
    /*0x04*/ struct SoundAllocPool pool;
    /*0x14*/ struct SeqOrBankEntry entries[2];
}; // size = 0x2C

struct SoundMultiPool
{
    /*0x000*/ struct PersistentPool persistent;
    /*0x194*/ struct TemporaryPool temporary;
}; // size = 0x1D0

extern u8 gAudioHeap[];
#define gVolume 0x7FFF
#define gReverbDownsampleRate 1
extern volatile u8 gAudioResetStatus;
extern u8 gAudioResetPresetIdToLoad;
#endif // AUDIO_HEAP_H
