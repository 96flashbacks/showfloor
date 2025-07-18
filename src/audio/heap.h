#ifndef AUDIO_HEAP_H
#define AUDIO_HEAP_H

#include <PR/ultratypes.h>

#include "internal.h"

#define SOUND_LOAD_STATUS_NOT_LOADED     0
#define SOUND_LOAD_STATUS_IN_PROGRESS    1
#define SOUND_LOAD_STATUS_COMPLETE       2
#define SOUND_LOAD_STATUS_DISCARDABLE    3
#define SOUND_LOAD_STATUS_4              4
#define SOUND_LOAD_STATUS_5              5

#define IS_BANK_LOAD_COMPLETE(bankId) (gBankLoadStatus[bankId] >= SOUND_LOAD_STATUS_COMPLETE)
#define IS_SEQ_LOAD_COMPLETE(seqId) (gSeqLoadStatus[seqId] >= SOUND_LOAD_STATUS_COMPLETE)

struct SoundAllocPool {
    u8 *start;
    u8 *cur;
    u32 size;
    s32 numAllocatedEntries;
}; // size = 0x10

struct SeqOrBankEntry {
    u8 *ptr;
    u32 size;
    s32 id; // seqId or bankId
}; // size = 0xC

struct PersistentPool {
    /*0x00*/ u32 numEntries;
    /*0x04*/ struct SoundAllocPool pool;
    /*0x14*/ struct SeqOrBankEntry entries[32];
}; // size = 0x194

struct TemporaryPool {
    /*EU,   SH*/
    /*0x00, 0x00*/ u32 nextSide;
    /*0x04,     */ struct SoundAllocPool pool;
    /*0x04,        pool.start     */
    /*0x08,        pool.cur       */
    /*0x0C, 0x0C   pool.size      */
    /*0x10, 0x10   pool.numAllocatedEntries */
    /*0x14,     */ struct SeqOrBankEntry entries[2];
    /*0x14, 0x14   entries[0].ptr */
    /*0x18,        entries[0].size*/
    /*0x1C, 0x1E   entries[0].id  */
    /*0x20, 0x20   entries[1].ptr */
    /*0x24,        entries[1].size*/
    /*0x28, 0x2A   entries[1].id  */
}; // size = 0x2C

struct SoundMultiPool {
    /*0x000*/ struct PersistentPool persistent;
    /*0x194*/ struct TemporaryPool temporary;
    /*     */ u32 pad2[4];
}; // size = 0x1D0

struct Unk1Pool {
    struct SoundAllocPool pool;
    struct SeqOrBankEntry entries[32];
};

struct UnkEntry {
    s8 used;
    s8 medium;
    s8 bankId;
    u32 pad;
    u8 *srcAddr;
    u8 *dstAddr;
    u32 size;
};

struct UnkPool {
    /*0x00*/  struct SoundAllocPool pool;
    /*0x10*/  struct UnkEntry entries[64];
    /*0x510*/ s32 numEntries;
    /*0x514*/ u32 unk514;
};

extern u8 gAudioHeap[];
extern s16 gVolume;
extern s8 gReverbDownsampleRate;
extern struct SoundAllocPool gAudioInitPool;
extern struct SoundAllocPool gNotesAndBuffersPool;
extern struct SoundAllocPool gPersistentCommonPool;
extern struct SoundAllocPool gTemporaryCommonPool;
extern struct SoundMultiPool gSeqLoadedPool;
extern struct SoundMultiPool gBankLoadedPool;
extern u8 gBankLoadStatus[64];
extern u8 gSeqLoadStatus[256];
extern volatile u8 gAudioResetStatus;
extern u8 gAudioResetPresetIdToLoad;

void *soundAlloc(struct SoundAllocPool *pool, u32 size);
void *sound_alloc_uninitialized(struct SoundAllocPool *pool, u32 size);
void sound_init_main_pools(s32 sizeForAudioInitPool);
void sound_alloc_pool_init(struct SoundAllocPool *pool, void *memAddr, u32 size);
void *alloc_bank_or_seq(struct SoundMultiPool *arg0, s32 arg1, s32 size, s32 arg3, s32 id);
void *get_bank_or_seq(struct SoundMultiPool *arg0, s32 arg1, s32 id);
void audio_reset_session(struct AudioSessionSettings *preset);
void discard_bank(s32 bankId);

#endif // AUDIO_HEAP_H
