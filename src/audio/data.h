#ifndef AUDIO_DATA_H
#define AUDIO_DATA_H

#include <PR/ultratypes.h>

#include "internal.h"
#include "types.h"
#include "load.h"
#include "playback.h"
#include "external.h"
struct SequenceQueueItem {
    u8 seqId;
    u8 priority;
}; // size = 0x2

#define AUDIO_LOCK_UNINITIALIZED 0
#define AUDIO_LOCK_NOT_LOADING 0x76557364
#define AUDIO_LOCK_LOADING 0x19710515

#define NUMAIBUFFERS 3
#define SEQUENCE_ARGS(priority, seqId) ((priority << 8) | seqId)

#define SOUND_MODE_STEREO           0
#define SOUND_MODE_MONO             3
#define SOUND_MODE_HEADSET          1

#define SEQ_PLAYER_LEVEL            0
#define SEQ_PLAYER_ENV              1
#define SEQ_PLAYER_SFX              2

#define MAX_BG_MUSIC_QUEUE_SIZE 6
#define SOUND_BANK_COUNT 10
#define MAX_CHANNELS_PER_SOUND 1
#define MAX_SOUNDS_PER_BANK 1
#define SEQUENCE_NONE 0xFF

extern struct SPTask *gAudioTask;
extern struct SPTask gAudioTasks[2];
extern s32 gAudioLoadLock;//WORRY
extern s32 gAudioFrameCount;
extern s32 gCurrAudioFrameDmaCount; //WORRY
extern s32 gAudioTaskIndex;
extern s32 gCurrAiBufferIndex;
extern u64 *gAudioCmdBuffers[2];

extern struct Note *gNotes;
extern struct SequencePlayer gSequencePlayers[SEQUENCE_PLAYERS];
extern struct SequenceChannel gSequenceChannels[SEQUENCE_CHANNELS];
extern struct SequenceChannelLayer gSequenceLayers[SEQUENCE_LAYERS];
extern struct AudioListItem gLayerFreeList;
extern struct NotePool gNoteFreeLists;
extern OSMesgQueue gCurrAudioFrameDmaQueue;
extern OSMesg gCurrAudioFrameDmaMesgBufs[AUDIO_FRAME_DMA_QUEUE_SIZE];
extern OSIoMesg gCurrAudioFrameDmaIoMesgBufs[AUDIO_FRAME_DMA_QUEUE_SIZE];
extern OSMesgQueue gAudioDmaMesgQueue;
extern OSMesg gAudioDmaMesg;
extern OSIoMesg gAudioDmaIoMesg;
extern struct SharedDma sSampleDmas[0x60];
extern u16 gSampleDmaNumListItems;
extern u16 sSampleDmaListSize1;




extern ALSeqFile *gSeqFileHeader;
extern ALSeqFile *gAlCtlHeader;
extern ALSeqFile *gAlTbl;
extern u8 *gAlBankSets;
 //extern u16 gSequenceCount;
extern struct CtlEntry *gCtlEntries;
extern s32 gAiFrequency;
extern s32 gMaxAudioCmds;
extern s32 gSamplesPerFrameTarget;
extern s32 gMinAiBufferLength;
extern s16 gTempoInternalToExternal;
extern struct SoundAllocPool gAudioSessionPool;
extern struct SoundAllocPool gAudioInitPool;
extern struct SoundAllocPool gNotesAndBuffersPool;
extern struct SoundAllocPool gSeqAndBankPool;
extern struct SoundAllocPool gPersistentCommonPool;
extern struct SoundAllocPool gTemporaryCommonPool;
extern struct SoundMultiPool gSeqLoadedPool;
extern struct SoundMultiPool gBankLoadedPool;
extern struct PoolSplit sPersistentCommonPoolSplit;
extern struct PoolSplit sTemporaryCommonPoolSplit;
extern u8 gSoundDataADSR[]; // sound_data.ctl
extern u8 gSoundDataRaw[];  // sound_data.tbl
extern u8 gMusicData[];     // sequences.s
extern u8 gBankSetsData[];  // bank_sets.s


extern f32 gDefaultSoundArgs[3];
extern u16 gSoundBanksDisabled;
extern u8 sHasStartedFadeOut;
extern u8 gMaxBGMVol; // 0x80: VOL CAP ENABLED
extern u8 sPlayer1CurSeqId;
extern u8 sPlayer1CurSeqIdTargetVolume;

extern u8 sBackgroundMusicQueueSize;
extern struct Sound sSoundRequests[0x100];
extern struct ChannelVolumeScaleFade SeqChannelVelocityFades[3][CHANNELS_MAX];
extern u8 sCurrentSound[SOUND_BANK_COUNT][MAX_CHANNELS_PER_SOUND]; // index into gSoundBanks
extern struct SoundCharacteristics gSoundBanks[SOUND_BANK_COUNT][40];
extern u8 soundBankPitch[SOUND_BANK_COUNT];
extern u8 sPlayer0CurSeqIdTargetVolume;
extern u8 sPlayer0CurSeqId;
extern struct SequenceQueueItem sBackgroundMusicQueue[MAX_BG_MUSIC_QUEUE_SIZE];
extern const u8 sDialogSpeaker[];
extern const s32 sDialogSpeakerVoice[];


extern s16 *gAiBuffers[NUMAIBUFFERS];
extern s16 gAiBufferLengths[NUMAIBUFFERS];
#define AIBUFFER_LEN (0xa0 * 16)

extern u8 gBankLoadStatus[0x40];
extern u8 gSeqLoadStatus[0x100];

extern u8 sCurrentMusicDynamic;
extern u8 sBackgroundMusiorDynamics;
extern u8 sMusicDynamicDelay;
extern u32 gAudioRandom;
extern u8 sProcessedSoundID;
extern u8 sSoundRequestID;
typedef s32 FadeT;
extern u8 sSoundBankUsedListBack[SOUND_BANK_COUNT];
extern u8 sSoundBankFreeListFront[SOUND_BANK_COUNT];

extern const u8 gDefaultShortNoteVelocityTable[16];
extern const u8 gDefaultShortNoteDurationTable[16];
extern u8 sSampleDmaReuseQueue1[256];
extern u8 sSampleDmaReuseQueue2[256];
extern u8 sSampleDmaReuseQueueTail1;
extern u8 sSampleDmaReuseQueueTail2;
extern u8 sSampleDmaReuseQueueHead1;
extern u8 sSampleDmaReuseQueueHead2;




struct PoolSplit {
    u32 wantSeq;
    u32 wantBank;
    u32 wantCustom;
}; // size = 0x10

struct PoolSplit2 {
    u32 wantPersistent;
    u32 wantTemporary;
}; // size = 0x8

extern struct SynthesisReverb gSynthesisReverb;
extern struct SoundAllocPool gAudioSessionPool;
extern struct SoundAllocPool gAudioInitPool;
extern struct SoundAllocPool gNotesAndBuffersPool;
extern struct SoundAllocPool gSeqAndBankPool;
extern struct SoundAllocPool gPersistentCommonPool;
extern struct SoundAllocPool gTemporaryCommonPool;
extern struct SoundMultiPool gSeqLoadedPool;
extern struct SoundMultiPool gBankLoadedPool;
extern struct PoolSplit sPersistentCommonPoolSplit;
extern struct PoolSplit sTemporaryCommonPoolSplit;

#define AUDIO_HEAP_SIZE 0x31200
#define AUDIOINITPOOL_SIZE 0x3000 // SEQ_LEVEL (smart note so you find it teehee) NEEDS TO BE INCREASED WHEN YOU ADD MORE SEQUENCES
#define gMaxSimultaneousNotes 16
#define REVERBBUFFERSIZE 0x1000
#define AUDIOFREQUENCY 32000
#endif // AUDIO_DATA_H
