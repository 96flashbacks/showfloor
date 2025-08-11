#include <ultra64.h>
#include "sm64.h"
#include "heap.h"      //done
#include "load.h"      //done
#include "data.h"      //done
#include "external.h"  //done
#include "playback.h"  //done
#include "synthesis.h" //done
#include "effects.h"   //done
#include "game/level_update.h"
#include "objupdate/object_list_processor.h"
#include "game/camera.h"
#include "seq_ids.h"
#include "dialog_ids.h"
#include "game/OPT_FOR_SIZE.h"
#include "seq_channel_layer_process_script.h"

Vec3f gGlobalSoundSource = { 0.0f, 0.0f, 0.0f };

#ifdef VERSION_EU
#define EU_FLOAT(x) x##f
#else
#define EU_FLOAT(x) x
#endif

s32 sGameLoopTicked = 0;

struct SynthesisReverb gSynthesisReverb;

// Dialog sounds
// The US difference is the sound for DIALOG_037 ("I win! You lose! Ha ha ha ha!
// You're no slouch, but I'm a better sledder! Better luck next time!"), spoken
// by Koopa instead of the penguin in JP.

#define UKIKI 0
#define TUXIE 1
#define BOWS1 2 // Bowser Intro / Doors Laugh
#define KOOPA 3
#define KBOMB 4
#define BOO 5
#define BOMB 6
#define BOWS2 7 // Bowser Battle Laugh
#define GRUNT 8
#define WIGLR 9
#define YOSHI 10
#define CRYSH 11
#define PAMSH 12
#define DORRIE 13
#define SHYGUY 14
#define _ 0xFF

#ifdef VERSION_JP
#define DIFF KOOPA
#else
#define DIFF TUXIE
#endif

const u8 sDialogSpeaker[] = {
    //       0      1      2      3      4      5      6      7      8      9
    /* 0*/ _,     GRUNT, GRUNT, GRUNT, GRUNT,  KOOPA,  KOOPA,  KOOPA,  _,      KOOPA,
    /* 1*/ _,     _,     _,     _,     _,      _,      _,      _,      _,      _,
    /* 2*/ _,     GRUNT, TUXIE, TUXIE, TUXIE,  SHYGUY, SHYGUY, GRUNT,  GRUNT,  _,
    /* 3*/ _,     _,     _,     _,     _,      _,      _,      _,      _,      _,
    /* 4*/ _,     KOOPA, _,     _,     _,      _,      _,      _,      _,      _,
    /* 5*/ _,     _,     _,     _,     _,      TUXIE,  TUXIE,  TUXIE,  TUXIE,  TUXIE,
    /* 6*/ _,     _,     _,     _,     _,      _,      _,      _,      _,      _,
    /* 7*/ _,     _,     TUXIE, TUXIE, TUXIE,  _,      _,      TUXIE,  _,      UKIKI,
    /* 8*/ TUXIE, TUXIE, TUXIE, _,     _,      _,      _,      _,      _,      TUXIE,
    /* 9*/ BOWS2, TUXIE, BOWS2, TUXIE, TUXIE,  _,      _,      _,      BOO,    BOO,
    /*10*/ _,     _,     _,     _,     _,      _,      GRUNT,  BOO,    BOO,    _,
    /*11*/ _,     _,     _,     _,     DORRIE, PAMSH,  CRYSH,  PAMSH,  CRYSH,  _,
    /*12*/ CRYSH, YOSHI, _,     CRYSH, CRYSH,  CRYSH,  CRYSH,  _,      PAMSH,  CRYSH,
    /*13*/ _,     _,     TUXIE, _,     _,      _,      _,      _,      _,      _,
    /*14*/ _,     _,     _,     _,     _,      _,      _,      _,      _,      _,
    /*15*/ _,     _,     _,     _,     _,      _,      _,      GRUNT,  _,      _,
    /*16*/ _,     _,     _,     _,     _,      _,      SHYGUY, SHYGUY, SHYGUY, SHYGUY,
    /*17*/ YOSHI, YOSHI, YOSHI, YOSHI, YOSHI,  YOSHI,  YOSHI,  YOSHI,  YOSHI,  YOSHI,
    /*18*/ YOSHI, YOSHI, YOSHI, YOSHI, YOSHI,  YOSHI,  PAMSH,  PAMSH,  PAMSH,  PAMSH,
    /*19*/ PAMSH, CRYSH, CRYSH, CRYSH, CRYSH,  CRYSH,  YOSHI,  YOSHI,  YOSHI,  YOSHI,
    /*20*/ YOSHI, YOSHI, YOSHI, YOSHI, YOSHI,  YOSHI,  YOSHI,  YOSHI,  YOSHI,  YOSHI,
    /*21*/ YOSHI, YOSHI, YOSHI, YOSHI, YOSHI,  YOSHI,  YOSHI,  YOSHI,  YOSHI,  YOSHI,
    /*22*/ YOSHI, YOSHI, YOSHI, YOSHI, YOSHI,  YOSHI,  YOSHI,  YOSHI,  YOSHI,  YOSHI,
    /*23*/ YOSHI, YOSHI, YOSHI, YOSHI, YOSHI,  YOSHI,  YOSHI,  YOSHI,  YOSHI,  YOSHI,
    /*24*/ _,     _,     _,     _,     _,      _,      _,      _,      _,      _,
    /*25*/ _,     _,     _,     _,     _,      _,      _,      _,      _,      _,
    /*26*/ _,     _,     _,     _,     _,      _,      _,      _,      _,      CRYSH,
    /*27*/ _,     _,     _,     _,     _,      _,      _,      _,      _,      _,
    /*28*/ _,     _,     _,     _,     _,      _,      _,      _,      _,      _
};
#undef _
STATIC_ASSERT(ARRAY_COUNT(sDialogSpeaker) == DIALOG_COUNT,
              "change this array if you are adding dialogs");

const s32 sDialogSpeakerVoice[] = {
    SOUND_OBJ_UKIKI_CHATTER_LONG,      SOUND_OBJ_BIG_PENGUIN_YELL,
    SOUND_OBJ_BOWSER_INTRO_LAUGH,      SOUND_OBJ_KOOPA_TALK,
    SOUND_OBJ_KING_BOBOMB_TALK,        SOUND_OBJ_BOO_LAUGH_LONG,
    SOUND_OBJ_BOBOMB_BUDDY_TALK,       SOUND_OBJ_BOWSER_LAUGH,
    SOUND_OBJ2_BOSS_DIALOG_GRUNT,      SOUND_OBJ_WIGGLER_TALK,
    SOUND_GENERAL_YOSHI_TALK,          SOUND_GENERAL_YOSHI_WAAH,
    SOUND_GENERAL_YOSHI_PAM,           SOUND_OBJ_DORRIE,
    SOUND_OBJ_WIGGLER_ATTACKED_SHYGUY, NO_SOUND,
};

u8 sProcessedSoundID = 0;
u8 sSoundRequestID = 0;

// Music dynamic tables. A dynamic describes which volumes to apply to which
// channels of a sequence (I think?), and different parts of a level can have
// different dynamics. Each table below specifies first the sequence to apply
// the dynamics to, then a bunch of conditions for when each dynamic applies
// (e.g. "only if Mario's X position is between 100 and 300"), and finally a
// fallback dynamic. Due to the encoding of the tables, the conditions must
// come in the same order as the macros.
// TODO: dynamic isn't a great term for this, it means other things in music...

#define MARIO_X_GE 0
#define MARIO_Y_GE 1
#define MARIO_Z_GE 2
#define MARIO_X_LT 3
#define MARIO_Y_LT 4
#define MARIO_Z_LT 5
#define MARIO_IS_IN_AREA 6
#define MARIO_IS_IN_ROOM 7

s16 sDynNone[] = { SEQ_SOUND_PLAYER, 0 };

u8 sCurrentMusicDynamic = 0xff;
u8 sBackgroundMusiorDynamics = SEQUENCE_NONE;

#define STUB_LEVEL(_0, _1, _2, _3, _4, _5, _6, leveldyn, _8) leveldyn,
#define DEFINE_LEVEL(_0, _1, _2, _3, _4, _5, _6, _7, _8, leveldyn, _10) leveldyn,

#define _ sDynNone
s16 *sLevelDynamics[LEVEL_COUNT] = {
    _, // LEVEL_NONE
#include "levels/level_defines.h"
};
#undef _
#undef STUB_LEVEL
#undef DEFINE_LEVEL

struct MusicDynamic {
    /*0x0*/ s16 bits1;
    /*0x2*/ u16 volScale1;
    /*0x4*/ s16 dur1;
    /*0x6*/ s16 bits2;
    /*0x8*/ u16 volScale2;
    /*0xA*/ s16 dur2;
}; // size = 0xC
/*
const struct MusicDynamic sMusicDynamics[8] = {
    { 0x0000, 127, 100, 0x0e43, 0, 100 }, // SEQ_LEVEL_WATER
    { 0x0003, 127, 100, 0x0e40, 0, 100 }, // SEQ_LEVEL_WATER
    { 0x0e43, 127, 200, 0x0000, 0, 200 }, // SEQ_LEVEL_WATER
    { 0x02ff, 127, 100, 0x0100, 0, 100 }, // SEQ_LEVEL_UNDERGROUND
    { 0x03f7, 127, 100, 0x0008, 0, 100 }, // SEQ_LEVEL_UNDERGROUND
    { 0x0070, 127, 10, 0x0000, 0, 100 },  // SEQ_LEVEL_SPOOKY
    { 0x0000, 127, 100, 0x0070, 0, 10 },  // SEQ_LEVEL_SPOOKY
    { 0xffff, 127, 100, 0x0000, 0, 100 }, // any (unused)
};*/
/*
#define STUB_LEVEL(_0, _1, _2, _3, echo1, echo2, echo3, _7, _8) { echo1, echo2, echo3 },
#define DEFINE_LEVEL(_0, _1, _2, _3, _4, _5, echo1, echo2, echo3, _9, _10) { echo1, echo2, echo3 },

u8 gAreaEchoLevel[LEVEL_COUNT][3] = {
    { 0x00, 0x00, 0x00 }, // LEVEL_NONE
#include "levels/level_defines.h"
};
#undef STUB_LEVEL
#undef DEFINE_LEVEL
*/

struct Note *gNotes;
struct SequencePlayer gSequencePlayers[SEQUENCE_PLAYERS];
struct SequenceChannel gSequenceChannels[SEQUENCE_CHANNELS];
struct SequenceChannelLayer gSequenceLayers[SEQUENCE_LAYERS];
struct AudioListItem gLayerFreeList;
struct NotePool gNoteFreeLists;
OSMesgQueue gCurrAudioFrameDmaQueue;
OSMesg gCurrAudioFrameDmaMesgBufs[AUDIO_FRAME_DMA_QUEUE_SIZE];
OSIoMesg gCurrAudioFrameDmaIoMesgBufs[AUDIO_FRAME_DMA_QUEUE_SIZE];
OSMesgQueue gAudioDmaMesgQueue;
OSMesg gAudioDmaMesg;
OSIoMesg gAudioDmaIoMesg;
struct SharedDma sSampleDmas[0x60];
u16 gSampleDmaNumListItems;
u16 sSampleDmaListSize1;
u8 sSampleDmaReuseQueue1[256];
u8 sSampleDmaReuseQueue2[256];
u8 sSampleDmaReuseQueueTail1;
u8 sSampleDmaReuseQueueTail2;
u8 sSampleDmaReuseQueueHead1;
u8 sSampleDmaReuseQueueHead2;

// bss correct up to here

ALSeqFile *gSeqFileHeader;
ALSeqFile *gAlCtlHeader;
ALSeqFile *gAlTbl;
u8 *gAlBankSets;
// u16 gSequenceCount;
struct CtlEntry *gCtlEntries;
s32 gAiFrequency;
s32 gMaxAudioCmds;
s32 gSamplesPerFrameTarget;
s32 gMinAiBufferLength;
s16 gTempoInternalToExternal;

struct SoundAllocPool gAudioSessionPool;
struct SoundAllocPool gAudioInitPool;
struct SoundAllocPool gNotesAndBuffersPool;
struct SoundAllocPool gSeqAndBankPool;
struct SoundAllocPool gPersistentCommonPool;
struct SoundAllocPool gTemporaryCommonPool;
struct SoundMultiPool gSeqLoadedPool;
struct SoundMultiPool gBankLoadedPool;
struct PoolSplit sPersistentCommonPoolSplit;
struct PoolSplit sTemporaryCommonPoolSplit;

u8 gBankLoadStatus[0x40];
u8 gSeqLoadStatus[0x100];
u8 gAreaEchoLevel[LEVEL_COUNT][8] = {
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // LEVEL_NONE
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // LEVEL_NONE
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // LEVEL_NONE
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // LEVEL_NONE
    { 0x00, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00 }, // LEVEL_BBH
    { 0x00, 0x30, 0x50, 0x30, 0x00, 0x00, 0x00, 0x00 }, // LEVEL_CCM
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // LEVEL_NONE
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // LEVEL_NONE
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // LEVEL_NONE
    { 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // LEVEL_BOB
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // LEVEL_NONE
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // LEVEL_NONE
    { 0x20, 0x30, 0x5f, 0x5f, 0x5f, 0x5f, 0x30, 0x5f }, // LEVEL_JRB
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // LEVEL_NONE
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // LEVEL_NONE
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // LEVEL_NONE
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // LEVEL_NONE
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // LEVEL_NONE
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // LEVEL_NONE
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // LEVEL_NONE
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // LEVEL_NONE
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // LEVEL_NONE
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // LEVEL_NONE
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // LEVEL_NONE
    { 0x00, 0x30, 0x40, 0x5f, 0x30, 0x00, 0x00, 0x00 }, // LEVEL_WF
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // LEVEL_NONE
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // LEVEL_NONE
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // LEVEL_NONE
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // LEVEL_NONE
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // LEVEL_NONE
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // LEVEL_NONE
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // LEVEL_NONE
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // LEVEL_NONE
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // LEVEL_NONE
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // LEVEL_NONE
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // LEVEL_NONE
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // LEVEL_NONE
};
/*
#define STUB_LEVEL(_0, _1, _2, volume, _4, _5, _6, _7, _8) volume,
#define DEFINE_LEVEL(_0, _1, _2, _3, _4, volume, _6, _7, _8, _9, _10) volume,

u16 D_80332028[LEVEL_COUNT] = {
    20000, // LEVEL_NONE
#include "levels/level_defines.h"
};

#undef STUB_LEVEL
#undef DEFINE_LEVEL*/

#define AUDIO_MAX_DISTANCE US_FLOAT(22000.0)
// Format:
// - frequency
// - max number of simultaneous notes
// - reverb downsample rate (makes the ring buffer be downsampled to save memory)
// - reverb window size (ring buffer size, length affects reverb delay)
// - reverb gain (0 = min reverb, 32767 = max reverb, 32769 to 65535 = louder and louder...)
// - volume
// - memory used for persistent sequences
// - memory used for persistent banks
// - memory used for temporary sequences
// - memory used for temporary banks
#define PERSISTENTBANKMEMREQ 0x8D00
const struct AudioSessionSettings gAudioSessionPresets[18] = {
    { 32000, 16, 1, 0x1000, 0x2FFF, 0x7FFF, 0x5A00, PERSISTENTBANKMEMREQ, 0x4400, 0x2A00 },
    { 32000, 16, 1, 0x1000, 0x47FF, 0x7FFF, 0x3A00, PERSISTENTBANKMEMREQ, 0x4400, 0x2A00 },
    { 32000, 16, 1, 0x1000, 0x2FFF, 0x7FFF, 0x3A00, PERSISTENTBANKMEMREQ, 0x4400, 0x2A00 },
    { 32000, 16, 1, 0x1000, 0x3FFF, 0x7FFF, 0x3A00, PERSISTENTBANKMEMREQ, 0x4400, 0x2A00 },
    { 32000, 16, 1, 0x1000, 0x4FFF, 0x7FFF, 0x3A00, PERSISTENTBANKMEMREQ, 0x4400, 0x2A00 },
    { 32000, 16, 1, 0x1000, 0x2FFF, 0x7FFF, 0x4000, PERSISTENTBANKMEMREQ, 0x3F00, 0x2A00 },
    { 32000, 16, 1, 0x1000, 0x47FF, 0x7FFF, 0x4100, PERSISTENTBANKMEMREQ, 0x4400, 0x2A80 },
    { 32000, 20, 1, 0x1000, 0x37FF, 0x7FFF, 0x34C0, PERSISTENTBANKMEMREQ, 0x4000, 0x1B00 },
    { 32000, 16, 1, 0x1000, 0x2FFF, 0x7FFF, 0x2500, PERSISTENTBANKMEMREQ, 0x7400, 0x2400 },
    { 32000, 16, 1, 0x1000, 0x3FFF, 0x7FFF, 0x2500, PERSISTENTBANKMEMREQ, 0x7400, 0x2400 },
    { 32000, 16, 1, 0x1000, 0x2FFF, 0x7FFF, 0x2500, PERSISTENTBANKMEMREQ, 0x7400, 0x2400 },
    { 32000, 16, 1, 0x1000, 0x3FFF, 0x7FFF, 0x2500, PERSISTENTBANKMEMREQ, 0x7400, 0x2400 },
    { 32000, 16, 1, 0x1000, 0x4FFF, 0x7FFF, 0x2500, PERSISTENTBANKMEMREQ, 0x7400, 0x2400 },
    { 32000, 16, 1, 0x1000, 0x2FFF, 0x7FFF, 0x2500, PERSISTENTBANKMEMREQ, 0x7400, 0x2400 },
    { 32000, 16, 1, 0x1000, 0x2FFF, 0x7FFF, 0x2500, PERSISTENTBANKMEMREQ, 0x7400, 0x2400 },
    { 32000, 16, 1, 0x1000, 0x2FFF, 0x7FFF, 0x2500, PERSISTENTBANKMEMREQ, 0x7400, 0x2400 },
    { 32000, 16, 1, 0x1000, 0x2FFF, 0x7FFF, 0x2500, PERSISTENTBANKMEMREQ, 0x7400, 0x2400 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};
const u16 gAudioCosineTable[128] = {
    0x7FFF, 32764, 32757, 32744, 32727, 32704, 32677, 32644, 32607, 32564, 32517, 32464, 32407,
    32344,  32277, 32205, 32127, 32045, 31958, 31866, 31770, 31668, 31561, 31450, 31334, 31213,
    31087,  30957, 30822, 30682, 30537, 30388, 30234, 30075, 29912, 29744, 29572, 29395, 29214,
    29028,  28838, 28643, 28444, 28241, 28033, 27821, 27605, 27385, 27160, 26931, 26698, 26461,
    26220,  25975, 25726, 25473, 25216, 24956, 24691, 24423, 24151, 23875, 23596, 23313, 23026,
    22736,  22442, 22145, 21845, 21541, 21234, 20924, 20610, 20294, 19974, 19651, 19325, 18997,
    18665,  18331, 17993, 17653, 17310, 16965, 16617, 16266, 15913, 15558, 15200, 14840, 14477,
    14113,  13746, 13377, 13006, 12633, 12258, 11881, 11503, 11122, 10740, 10357, 9971,  9584,
    9196,   8806,  8415,  8023,  7630,  7235,  6839,  6442,  6044,  5646,  5246,  4845,  4444,
    4042,   3640,  3237,  2833,  2429,  2025,  1620,  1216,  810,   405,   0,
};

// Transforms a pitch scale factor in -127..127 into a frequency scale factor
// between -1 and +1 octave.
// gPitchBendFrequencyScale[k] = 0.5 * 2^(k/127)
const f32 gPitchBendFrequencyScale[255] = {
    0.5f,      0.502736f, 0.505488f, 0.508254f, 0.511036f, 0.513833f, 0.516645f, 0.519472f, 0.522315f,
    0.525174f, 0.528048f, 0.530938f, 0.533843f, 0.536765f, 0.539702f, 0.542656f, 0.545626f, 0.548612f,
    0.551614f, 0.554633f, 0.557669f, 0.560721f, 0.563789f, 0.566875f, 0.569977f, 0.573097f, 0.576233f,
    0.579387f, 0.582558f, 0.585746f, 0.588951f, 0.592175f, 0.595415f, 0.598674f, 0.601950f, 0.605245f,
    0.608557f, 0.611888f, 0.615236f, 0.618603f, 0.621989f, 0.625393f, 0.628815f, 0.632257f, 0.635717f,
    0.639196f, 0.642694f, 0.646212f, 0.649748f, 0.653304f, 0.656880f, 0.660475f, 0.664089f, 0.667724f,
    0.671378f, 0.675052f, 0.678747f, 0.682461f, 0.686196f, 0.689952f, 0.693727f, 0.697524f, 0.701341f,
    0.705180f, 0.709039f, 0.712919f, 0.716821f, 0.720744f, 0.724689f, 0.728655f, 0.732642f, 0.736652f,
    0.740684f, 0.744737f, 0.748813f, 0.752911f, 0.757031f, 0.761175f, 0.765340f, 0.769529f, 0.773740f,
    0.777975f, 0.782232f, 0.786513f, 0.790818f, 0.795146f, 0.799497f, 0.803873f, 0.808272f, 0.812696f,
    0.817144f, 0.821616f, 0.826112f, 0.830633f, 0.835179f, 0.839750f, 0.844346f, 0.848966f, 0.853613f,
    0.858284f, 0.862982f, 0.867704f, 0.872453f, 0.877228f, 0.882029f, 0.886856f, 0.891709f, 0.896590f,
    0.901496f, 0.906430f, 0.911391f, 0.916379f, 0.921394f, 0.926436f, 0.931507f, 0.936604f, 0.941730f,
    0.946884f, 0.952066f, 0.957277f, 0.962516f, 0.967783f, 0.973080f, 0.978405f, 0.983760f, 0.989144f,
    0.994557f, 1.0f,      1.005473f, 1.010975f, 1.016508f, 1.022071f, 1.027665f, 1.033289f, 1.038944f,
    1.044630f, 1.050347f, 1.056095f, 1.061875f, 1.067687f, 1.073530f, 1.079405f, 1.085312f, 1.091252f,
    1.097224f, 1.103229f, 1.109267f, 1.115337f, 1.121441f, 1.127579f, 1.133750f, 1.139955f, 1.146193f,
    1.152466f, 1.158773f, 1.165115f, 1.171491f, 1.177903f, 1.184349f, 1.190831f, 1.197348f, 1.203901f,
    1.210489f, 1.217114f, 1.223775f, 1.230473f, 1.237207f, 1.243978f, 1.250786f, 1.257631f, 1.264514f,
    1.271434f, 1.278392f, 1.285389f, 1.292423f, 1.299497f, 1.306608f, 1.313759f, 1.320949f, 1.328178f,
    1.335447f, 1.342756f, 1.350104f, 1.357493f, 1.364922f, 1.372392f, 1.379903f, 1.387455f, 1.395048f,
    1.402683f, 1.410360f, 1.418078f, 1.425839f, 1.433642f, 1.441488f, 1.449377f, 1.457309f, 1.465285f,
    1.473304f, 1.481367f, 1.489474f, 1.497626f, 1.505822f, 1.514063f, 1.522349f, 1.530681f, 1.539058f,
    1.547481f, 1.555950f, 1.564465f, 1.573027f, 1.581636f, 1.590292f, 1.598995f, 1.607746f, 1.616545f,
    1.625392f, 1.634287f, 1.643231f, 1.652224f, 1.661266f, 1.670358f, 1.679500f, 1.688691f, 1.697933f,
    1.707225f, 1.716569f, 1.725963f, 1.735409f, 1.744906f, 1.754456f, 1.764058f, 1.773712f, 1.783419f,
    1.793179f, 1.802993f, 1.812860f, 1.822782f, 1.832757f, 1.842788f, 1.852873f, 1.863013f, 1.873209f,
    1.883461f, 1.893768f, 1.904132f, 1.914553f, 1.925031f, 1.935567f, 1.946159f, 1.956810f, 1.967520f,
    1.978287f, 1.989114f, 2.0f
};

// Frequencies for notes using the standard twelve-tone equal temperament scale.
// For indices 0..116, gNoteFrequencies[k] = 2^((k-39)/12).
// For indices 117..128, gNoteFrequencies[k] = 0.5 * 2^((k-39)/12).
// The 39 in the formula refers to piano key 40 (middle C, at 256 Hz) being
// the reference frequency, which is assigned value 1.
// clang-format off
const f32 gNoteFrequencies[128] = {
    0.105112f,  0.111362f,  0.117984f,  0.125f, 0.132433f, 0.140308f,  0.148651f,  0.15749f,  0.166855f, 0.176777f, 0.187288f,  0.198425f,
    0.210224f,  0.222725f,  0.235969f,  0.25f,  0.264866f, 0.280616f,  0.297302f,  0.31498f,  0.33371f,  0.353553f, 0.374577f,  0.39685f,
    0.420448f,  0.445449f,  0.471937f,  0.5f,   0.529732f, 0.561231f,  0.594604f,  0.629961f, 0.66742f,  0.707107f, 0.749154f,  0.793701f,
    0.840897f,  0.890899f,  0.943875f,  1.0f,   1.059463f, 1.122462f,  1.189207f,  1.259921f, 1.33484f,  1.414214f, 1.498307f,  1.587401f,
    1.681793f,  1.781798f,  1.887749f,  2.0f,   2.118926f, 2.244924f,  2.378414f,  2.519842f, 2.66968f,  2.828428f, 2.996615f,  3.174803f,
    3.363586f,  3.563596f,  3.775498f,  4.0f,   4.237853f, 4.489849f,  4.756829f,  5.039685f, 5.33936f,  5.656855f, 5.993229f,  6.349606f,
    6.727173f,  7.127192f,  7.550996f,  8.0f,   8.475705f, 8.979697f,  9.513658f,  10.07937f, 10.67872f, 11.31371f, 11.986459f, 12.699211f,
    13.454346f, 14.254383f, 15.101993f, 16.0f,  16.95141f, 17.959394f, 19.027315f, 20.15874f, 21.35744f, 22.62742f, 23.972918f, 25.398422f,
    26.908691f, 28.508766f, 30.203985f, 32.0f,  33.90282f, 35.91879f,  38.05463f,  40.31748f, 42.71488f, 45.25484f, 47.945835f, 50.796844f,
    53.817383f, 57.017532f, 60.40797f,  64.0f,  67.80564f, 71.83758f,  76.10926f,  80.63496f, 85.42976f, 45.25484f, 47.945835f, 50.796844f,
    53.817383f, 57.017532f, 60.40797f,  64.0f,  67.80564f, 71.83758f,  76.10926f,  80.63496f
};
// clang-format on

// goes up by ~12 at each step for the first 4 values (starting from 0), then by ~6
const u8 gDefaultShortNoteVelocityTable[16] = {
    12, 25, 38, 51, 57, 64, 71, 76, 83, 89, 96, 102, 109, 115, 121, 127,
};

// goes down by 26 at each step for the first 4 values (starting from 255), then by ~12
const u8 gDefaultShortNoteDurationTable[16] = {
    229, 203, 177, 151, 139, 126, 113, 100, 87, 74, 61, 48, 36, 23, 10, 0,
};

const s8 gVibratoCurve[16] = { 0, 8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96, 104, 112, 120 };

const struct AdsrEnvelope gDefaultEnvelope[] = {
    { BSWAP16(4), BSWAP16(32000) },    // go from 0 to 32000 over the course of 16ms
    { BSWAP16(1000), BSWAP16(32000) }, // stay there for 4.16 seconds
    { BSWAP16(ADSR_HANG), 0 }          // then continue staying there
};
/*
const s16 sSineWave[0x40] = {
    0,      3211,   6392,   9511,   12539,   15446,  18204,  20787,  23169,  25329,  27244,
    28897,  30272,  31356,  32137,  32609,   0x7FFF, 32609,  32137,  31356,  30272,  28897,
    27244,  25329,  23169,  20787,  18204,   15446,  12539,  9511,   6392,   3211,   0,
    -3211,  -6392,  -9511,  -12539, -15446,  -18204, -20787, -23169, -25329, -27244, -28897,
    -30272, -31356, -32137, -32609, -0x7FFF, -32609, -32137, -31356, -30272, -28897, -27244,
    -25329, -23169, -20787, -18204, -15446,  -12539, -9511,  -6392,  -3211,
};

const s16 sSquareWave[0x40] = {
    0,       0,       0,       0,       0,       0,       0,       0,       0,       0,       0,
    0,       0,       0,       0,       0,       0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,
    0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0x7FFF,  0,
    0,       0,       0,       0,       0,       0,       0,       0,       0,       0,       0,
    0,       0,       0,       0,       -0x7FFF, -0x7FFF, -0x7FFF, -0x7FFF, -0x7FFF, -0x7FFF, -0x7FFF,
    -0x7FFF, -0x7FFF, -0x7FFF, -0x7FFF, -0x7FFF, -0x7FFF, -0x7FFF, -0x7FFF, -0x7FFF,
};
const s16 sTriangleWave[0x40] = {
    0,       0x7FF,   0xFFF,   0x17FF,  0x1FFF,  0x27FF,  0x2FFF,  0x37FF,  0x3FFF,  0x47FF,  0x4FFF,
    0x57FF,  0x5FFF,  0x67FF,  0x6FFF,  0x77FF,  0x7FFF,  0x77FF,  0x6FFF,  0x67FF,  0x5FFF,  0x57FF,
    0x4FFF,  0x47FF,  0x3FFF,  0x37FF,  0x2FFF,  0x27FF,  0x1FFF,  0x17FF,  0xFFF,   0x7FF,   0,
    -0x7FF,  -0xFFF,  -0x17FF, -0x1FFF, -10239,  -0x2FFF, -0x37FF, -0x3FFF, -0x47FF, -0x4FFF, -22527,
    -24575,  -26623,  -28671,  -30719,  -0x7FFF, -30719,  -28671,  -26623,  -24575,  -22527,  -0x4FFF,
    -0x47FF, -0x3FFF, -0x37FF, -0x2FFF, -0x27FF, -0x1FFF, -0x17FF, -0xFFF,  -0x7FF,
};

const s16 sSawtoothWave[0x40] = {
    0,       1023,   2047,    3071,   4095,    5119,   6143,    7167,   8191,    9215,   10239,
    11263,   0x2FFF, 13311,   0x37FF, 15359,   0x3FFF, 17407,   0x47FF, 19455,   0x4FFF, 21503,
    22527,   23551,  24575,   25599,  26623,   27647,  28671,   29695,  30719,   31743,  -0x7FFF,
    -31743,  -30719, -29695,  -28671, -27647,  -26623, -25599,  -24575, -23551,  -22527, -21503,
    -0x4FFF, -19455, -0x47FF, -17407, -0x3FFF, -15359, -0x37FF, -13311, -0x2FFF, -11263, -10239,
    -9215,   -8191,  -7167,   -6143,  -5119,   -4095,  -3071,   -2047,  -1023,
};
const s16 *gWaveSamples[4] = { sSawtoothWave, sTriangleWave, sSineWave, sSquareWave };*/

//const u16 gHeadsetPanQuantization[10] = { 0x40, 0x30, 0x20, 0x10, 0, 0, 0, 0, 0, 0 };

// Linearly interpolated between
// f(0/4 * 127) = 1/sqrt(2)
// f(1/4 * 127) = 1
// f(2/4 * 127) = 1/sqrt(2)
// f(3/4 * 127) = 0
// f(4/4 * 127) = 1/sqrt(8)
const f32 gStereoPanVolume[128] = {
    0.707f,    0.716228f, 0.725457f, 0.734685f, 0.743913f, 0.753142f, 0.76237f,  0.771598f, 0.780827f,
    0.790055f, 0.799283f, 0.808512f, 0.81774f,  0.826968f, 0.836197f, 0.845425f, 0.854654f, 0.863882f,
    0.87311f,  0.882339f, 0.891567f, 0.900795f, 0.910024f, 0.919252f, 0.92848f,  0.937709f, 0.946937f,
    0.956165f, 0.965394f, 0.974622f, 0.98385f,  0.993079f, 0.997693f, 0.988465f, 0.979236f, 0.970008f,
    0.960779f, 0.951551f, 0.942323f, 0.933095f, 0.923866f, 0.914638f, 0.905409f, 0.896181f, 0.886953f,
    0.877724f, 0.868496f, 0.859268f, 0.850039f, 0.840811f, 0.831583f, 0.822354f, 0.813126f, 0.803898f,
    0.794669f, 0.785441f, 0.776213f, 0.766984f, 0.757756f, 0.748528f, 0.739299f, 0.730071f, 0.720843f,
    0.711614f, 0.695866f, 0.673598f, 0.651331f, 0.629063f, 0.606795f, 0.584528f, 0.56226f,  0.539992f,
    0.517724f, 0.495457f, 0.473189f, 0.450921f, 0.428654f, 0.406386f, 0.384118f, 0.36185f,  0.339583f,
    0.317315f, 0.295047f, 0.27278f,  0.250512f, 0.228244f, 0.205976f, 0.183709f, 0.161441f, 0.139173f,
    0.116905f, 0.094638f, 0.07237f,  0.050102f, 0.027835f, 0.005567f, 0.00835f,  0.019484f, 0.030618f,
    0.041752f, 0.052886f, 0.06402f,  0.075154f, 0.086287f, 0.097421f, 0.108555f, 0.119689f, 0.130823f,
    0.141957f, 0.153091f, 0.164224f, 0.175358f, 0.186492f, 0.197626f, 0.20876f,  0.219894f, 0.231028f,
    0.242161f, 0.253295f, 0.264429f, 0.275563f, 0.286697f, 0.297831f, 0.308965f, 0.320098f, 0.331232f,
    0.342366f, 0.3535f
};

// gDefaultVolume[k] = cos(pi/2 * k / 127)
const f32 gDefaultPanVolume[128] = {
    1.0f,      0.999924f, 0.999694f, 0.999312f, 0.998776f, 0.998088f, 0.997248f, 0.996254f, 0.995109f,
    0.993811f, 0.992361f, 0.990759f, 0.989006f, 0.987101f, 0.985045f, 0.982839f, 0.980482f, 0.977976f,
    0.97532f,  0.972514f, 0.96956f,  0.966457f, 0.963207f, 0.959809f, 0.956265f, 0.952574f, 0.948737f,
    0.944755f, 0.940629f, 0.936359f, 0.931946f, 0.92739f,  0.922692f, 0.917853f, 0.912873f, 0.907754f,
    0.902497f, 0.897101f, 0.891567f, 0.885898f, 0.880093f, 0.874153f, 0.868079f, 0.861873f, 0.855535f,
    0.849066f, 0.842467f, 0.835739f, 0.828884f, 0.821901f, 0.814793f, 0.807561f, 0.800204f, 0.792725f,
    0.785125f, 0.777405f, 0.769566f, 0.76161f,  0.753536f, 0.745348f, 0.737045f, 0.72863f,  0.720103f,
    0.711466f, 0.70272f,  0.693867f, 0.684908f, 0.675843f, 0.666676f, 0.657406f, 0.648036f, 0.638567f,
    0.629f,    0.619337f, 0.609579f, 0.599728f, 0.589785f, 0.579752f, 0.56963f,  0.559421f, 0.549126f,
    0.538748f, 0.528287f, 0.517745f, 0.507124f, 0.496425f, 0.485651f, 0.474802f, 0.46388f,  0.452888f,
    0.441826f, 0.430697f, 0.419502f, 0.408243f, 0.396921f, 0.385538f, 0.374097f, 0.362598f, 0.351044f,
    0.339436f, 0.327776f, 0.316066f, 0.304308f, 0.292503f, 0.280653f, 0.268761f, 0.256827f, 0.244854f,
    0.232844f, 0.220798f, 0.208718f, 0.196606f, 0.184465f, 0.172295f, 0.160098f, 0.147877f, 0.135634f,
    0.12337f,  0.111087f, 0.098786f, 0.086471f, 0.074143f, 0.061803f, 0.049454f, 0.037097f, 0.024734f,
    0.012368f, 0.0f
};

// gVolRampingLhs136[k] = 2^16 * max(1, (256*k)^(1/17)
const f32 gVolRampingLhs136[128] = {
    65536.0f,    90811.555f,  94590.766f,  96873.96f,   98527.26f,   99829.06f,   100905.47f,
    101824.61f,  102627.57f,  103341.086f, 103983.55f,  104568.164f, 105104.75f,  105600.8f,
    106062.14f,  106493.46f,  106898.52f,  107280.414f, 107641.73f,  107984.62f,  108310.93f,
    108622.23f,  108919.875f, 109205.055f, 109478.8f,   109742.0f,   109995.48f,  110239.94f,
    110476.02f,  110704.305f, 110925.3f,   111139.45f,  111347.21f,  111548.945f, 111745.0f,
    111935.7f,   112121.35f,  112302.2f,   112478.51f,  112650.51f,  112818.4f,   112982.38f,
    113142.66f,  113299.37f,  113452.69f,  113602.766f, 113749.734f, 113893.73f,  114034.87f,
    114173.26f,  114309.02f,  114442.26f,  114573.055f, 114701.5f,   114827.69f,  114951.695f,
    115073.6f,   115193.47f,  115311.375f, 115427.39f,  115541.56f,  115653.96f,  115764.63f,
    115873.64f,  115981.04f,  116086.86f,  116191.164f, 116293.99f,  116395.38f,  116495.38f,
    116594.02f,  116691.34f,  116787.39f,  116882.19f,  116975.77f,  117068.17f,  117159.414f,
    117249.54f,  117338.57f,  117426.53f,  117513.45f,  117599.35f,  117684.266f, 117768.2f,
    117851.195f, 117933.266f, 118014.44f,  118094.72f,  118174.14f,  118252.71f,  118330.46f,
    118407.4f,   118483.55f,  118558.914f, 118633.53f,  118707.4f,   118780.54f,  118852.97f,
    118924.695f, 118995.74f,  119066.11f,  119135.82f,  119204.88f,  119273.31f,  119341.125f,
    119408.32f,  119474.92f,  119540.93f,  119606.36f,  119671.22f,  119735.52f,  119799.28f,
    119862.5f,   119925.195f, 119987.36f,  120049.02f,  120110.18f,  120170.84f,  120231.016f,
    120290.71f,  120349.945f, 120408.7f,   120467.016f, 120524.875f, 120582.3f,   120639.28f,
    120695.84f,  120751.984f
};

// gVolRampingRhs136[k] = 1 / max(1, (256*k)^(1/17))
const f32 gVolRampingRhs136[128] = {
    1.0f,      0.72167f,  0.692837f, 0.676508f, 0.665156f, 0.656482f, 0.649479f, 0.643616f, 0.638581f,
    0.634172f, 0.630254f, 0.62673f,  0.62353f,  0.620601f, 0.617902f, 0.615399f, 0.613067f, 0.610885f,
    0.608835f, 0.606901f, 0.605073f, 0.603339f, 0.60169f,  0.600119f, 0.598618f, 0.597183f, 0.595806f,
    0.594485f, 0.593215f, 0.591991f, 0.590812f, 0.589674f, 0.588573f, 0.587509f, 0.586478f, 0.585479f,
    0.58451f,  0.583568f, 0.582654f, 0.581764f, 0.580898f, 0.580055f, 0.579233f, 0.578432f, 0.57765f,
    0.576887f, 0.576142f, 0.575414f, 0.574701f, 0.574005f, 0.573323f, 0.572656f, 0.572002f, 0.571361f,
    0.570733f, 0.570118f, 0.569514f, 0.568921f, 0.568339f, 0.567768f, 0.567207f, 0.566656f, 0.566114f,
    0.565582f, 0.565058f, 0.564543f, 0.564036f, 0.563537f, 0.563046f, 0.562563f, 0.562087f, 0.561618f,
    0.561156f, 0.560701f, 0.560253f, 0.559811f, 0.559375f, 0.558945f, 0.558521f, 0.558102f, 0.557689f,
    0.557282f, 0.55688f,  0.556483f, 0.556091f, 0.555704f, 0.555322f, 0.554944f, 0.554571f, 0.554203f,
    0.553839f, 0.553479f, 0.553123f, 0.552772f, 0.552424f, 0.55208f,  0.55174f,  0.551404f, 0.551071f,
    0.550742f, 0.550417f, 0.550095f, 0.549776f, 0.549461f, 0.549148f, 0.548839f, 0.548534f, 0.548231f,
    0.547931f, 0.547634f, 0.54734f,  0.547048f, 0.54676f,  0.546474f, 0.546191f, 0.54591f,  0.545632f,
    0.545357f, 0.545084f, 0.544813f, 0.544545f, 0.54428f,  0.544016f, 0.543755f, 0.543496f, 0.543239f,
    0.542985f, 0.542732f
};

// gVolRampingLhs144[k] = 2^16 * max(1, (256*k)^(1/18))
const f32 gVolRampingLhs144[128] = {
    65536.0f,    89180.734f,  92681.9f,    94793.33f,   96320.52f,   97522.02f,  98514.84f,
    99362.14f,   100101.99f,  100759.16f,  101350.664f, 101888.74f,  102382.46f, 102838.75f,
    103263.016f, 103659.58f,  104031.914f, 104382.89f,  104714.88f,  105029.89f, 105329.61f,
    105615.5f,   105888.81f,  106150.63f,  106401.914f, 106643.49f,  106876.12f, 107100.44f,
    107317.05f,  107526.47f,  107729.17f,  107925.6f,   108116.125f, 108301.12f, 108480.88f,
    108655.72f,  108825.91f,  108991.68f,  109153.28f,  109310.914f, 109464.77f, 109615.04f,
    109761.88f,  109905.46f,  110045.92f,  110183.41f,  110318.02f,  110449.91f, 110579.17f,
    110705.914f, 110830.234f, 110952.234f, 111071.99f,  111189.59f,  111305.12f, 111418.64f,
    111530.23f,  111639.95f,  111747.875f, 111854.05f,  111958.54f,  112061.4f,  112162.67f,
    112262.42f,  112360.68f,  112457.51f,  112552.93f,  112647.0f,   112739.76f, 112831.23f,
    112921.46f,  113010.484f, 113098.33f,  113185.02f,  113270.61f,  113355.11f, 113438.555f,
    113520.97f,  113602.375f, 113682.805f, 113762.27f,  113840.81f,  113918.44f, 113995.18f,
    114071.055f, 114146.08f,  114220.266f, 114293.65f,  114366.24f,  114438.06f, 114509.12f,
    114579.44f,  114649.02f,  114717.91f,  114786.086f, 114853.586f, 114920.42f, 114986.6f,
    115052.14f,  115117.055f, 115181.34f,  115245.04f,  115308.13f,  115370.65f, 115432.59f,
    115493.98f,  115554.81f,  115615.11f,  115674.875f, 115734.12f,  115792.85f, 115851.08f,
    115908.82f,  115966.07f,  116022.85f,  116079.16f,  116135.01f,  116190.4f,  116245.35f,
    116299.87f,  116353.945f, 116407.6f,   116460.84f,  116513.67f,  116566.09f, 116618.125f,
    116669.76f,  116721.01f
};

// gVolRampingRhs144[k] = 1 / max(1, (256*k)^(1/18))
const f32 gVolRampingRhs144[128] = {
    1.0f,      0.734867f, 0.707107f, 0.691357f, 0.680395f, 0.672012f, 0.66524f,  0.659567f, 0.654692f,
    0.650422f, 0.646626f, 0.643211f, 0.64011f,  0.637269f, 0.634651f, 0.632223f, 0.629961f, 0.627842f,
    0.625852f, 0.623975f, 0.622199f, 0.620515f, 0.618913f, 0.617387f, 0.615929f, 0.614533f, 0.613196f,
    0.611912f, 0.610677f, 0.609487f, 0.60834f,  0.607233f, 0.606163f, 0.605128f, 0.604125f, 0.603153f,
    0.60221f,  0.601294f, 0.600403f, 0.599538f, 0.598695f, 0.597874f, 0.597074f, 0.596294f, 0.595533f,
    0.59479f,  0.594064f, 0.593355f, 0.592661f, 0.591983f, 0.591319f, 0.590669f, 0.590032f, 0.589408f,
    0.588796f, 0.588196f, 0.587608f, 0.58703f,  0.586463f, 0.585906f, 0.58536f,  0.584822f, 0.584294f,
    0.583775f, 0.583265f, 0.582762f, 0.582268f, 0.581782f, 0.581303f, 0.580832f, 0.580368f, 0.579911f,
    0.57946f,  0.579017f, 0.578579f, 0.578148f, 0.577722f, 0.577303f, 0.576889f, 0.576481f, 0.576078f,
    0.575681f, 0.575289f, 0.574902f, 0.574519f, 0.574142f, 0.573769f, 0.5734f,   0.573036f, 0.572677f,
    0.572321f, 0.57197f,  0.571623f, 0.57128f,  0.57094f,  0.570605f, 0.570273f, 0.569945f, 0.56962f,
    0.569299f, 0.568981f, 0.568667f, 0.568355f, 0.568047f, 0.567743f, 0.567441f, 0.567142f, 0.566846f,
    0.566553f, 0.566263f, 0.565976f, 0.565692f, 0.56541f,  0.565131f, 0.564854f, 0.56458f,  0.564309f,
    0.56404f,  0.563773f, 0.563509f, 0.563247f, 0.562987f, 0.56273f,  0.562475f, 0.562222f, 0.561971f,
    0.561722f, 0.561476f
};

// gVolRampingLhs128[k] = 2^16 * max(1, (256*k)^(1/16))
const f32 gVolRampingLhs128[128] = {
    65536.0f,    92681.9f,    96785.28f,   99269.31f,   101070.33f,  102489.78f,  103664.336f,
    104667.914f, 105545.09f,  106324.92f,  107027.39f,  107666.84f,  108253.95f,  108796.87f,
    109301.95f,  109774.29f,  110217.98f,  110636.39f,  111032.33f,  111408.164f, 111765.9f,
    112107.234f, 112433.66f,  112746.46f,  113046.766f, 113335.555f, 113613.72f,  113882.02f,
    114141.164f, 114391.77f,  114634.414f, 114869.58f,  115097.74f,  115319.31f,  115534.68f,
    115744.19f,  115948.16f,  116146.875f, 116340.625f, 116529.66f,  116714.195f, 116894.46f,
    117070.64f,  117242.945f, 117411.52f,  117576.55f,  117738.17f,  117896.54f,  118051.77f,
    118204.0f,   118353.35f,  118499.92f,  118643.83f,  118785.16f,  118924.01f,  119060.47f,
    119194.625f, 119326.555f, 119456.336f, 119584.03f,  119709.71f,  119833.445f, 119955.29f,
    120075.31f,  120193.555f, 120310.08f,  120424.94f,  120538.17f,  120649.836f, 120759.97f,
    120868.62f,  120975.82f,  121081.62f,  121186.05f,  121289.14f,  121390.94f,  121491.47f,
    121590.766f, 121688.87f,  121785.79f,  121881.57f,  121976.24f,  122069.82f,  122162.33f,
    122253.805f, 122344.266f, 122433.73f,  122522.23f,  122609.77f,  122696.4f,   122782.11f,
    122866.93f,  122950.89f,  123033.99f,  123116.26f,  123197.72f,  123278.37f,  123358.24f,
    123437.34f,  123515.69f,  123593.3f,   123670.19f,  123746.36f,  123821.84f,  123896.63f,
    123970.76f,  124044.23f,  124117.04f,  124189.23f,  124260.78f,  124331.73f,  124402.07f,
    124471.83f,  124540.99f,  124609.59f,  124677.63f,  124745.12f,  124812.055f, 124878.47f,
    124944.34f,  125009.71f,  125074.57f,  125138.92f,  125202.79f,  125266.164f, 125329.06f,
    125391.5f,   125453.47f
};

// gVolRampingRhs128[k] = 1 / max(1, (256*k)^(1/16))
const f32 gVolRampingRhs128[128] = {
    1.0f,      0.707107f, 0.677128f, 0.660184f, 0.64842f,  0.639439f, 0.632194f, 0.626133f, 0.620929f,
    0.616375f, 0.612329f, 0.608693f, 0.605391f, 0.60237f,  0.599587f, 0.597007f, 0.594604f, 0.592355f,
    0.590243f, 0.588251f, 0.586369f, 0.584583f, 0.582886f, 0.581269f, 0.579725f, 0.578247f, 0.576832f,
    0.575473f, 0.574166f, 0.572908f, 0.571696f, 0.570525f, 0.569394f, 0.5683f,   0.567241f, 0.566214f,
    0.565218f, 0.564251f, 0.563311f, 0.562398f, 0.561508f, 0.560642f, 0.559799f, 0.558976f, 0.558173f,
    0.55739f,  0.556625f, 0.555877f, 0.555146f, 0.554431f, 0.553732f, 0.553047f, 0.552376f, 0.551719f,
    0.551075f, 0.550443f, 0.549823f, 0.549216f, 0.548619f, 0.548033f, 0.547458f, 0.546892f, 0.546337f,
    0.545791f, 0.545254f, 0.544726f, 0.544206f, 0.543695f, 0.543192f, 0.542696f, 0.542209f, 0.541728f,
    0.541255f, 0.540788f, 0.540329f, 0.539876f, 0.539429f, 0.538988f, 0.538554f, 0.538125f, 0.537702f,
    0.537285f, 0.536873f, 0.536467f, 0.536065f, 0.535669f, 0.535277f, 0.534891f, 0.534509f, 0.534131f,
    0.533759f, 0.53339f,  0.533026f, 0.532666f, 0.53231f,  0.531958f, 0.53161f,  0.531266f, 0.530925f,
    0.530588f, 0.530255f, 0.529926f, 0.529599f, 0.529277f, 0.528957f, 0.528641f, 0.528328f, 0.528018f,
    0.527711f, 0.527407f, 0.527106f, 0.526808f, 0.526513f, 0.52622f,  0.525931f, 0.525644f, 0.525359f,
    0.525077f, 0.524798f, 0.524522f, 0.524247f, 0.523975f, 0.523706f, 0.523439f, 0.523174f, 0.522911f,
    0.522651f, 0.522393f
};


// .bss

//u64 *gAudioCmd;

struct SPTask *gAudioTask;
struct SPTask gAudioTasks[2];
s32 gAudioLoadLock = AUDIO_LOCK_UNINITIALIZED;//WORRY
s32 gAudioFrameCount;
s32 gCurrAudioFrameDmaCount; //WORRY
s32 gAudioTaskIndex;
s32 gCurrAiBufferIndex;
u64 *gAudioCmdBuffers[2];

s16 *gAiBuffers[NUMAIBUFFERS];
s16 gAiBufferLengths[NUMAIBUFFERS];

u32 gAudioRandom;

u8 sPlayer0CurSeqId = SEQUENCE_NONE;
u8 sMusicDynamicDelay = 0;
u8 sSoundBankUsedListBack[SOUND_BANK_COUNT] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0
}; // pointers to head of list
u8 sSoundBankFreeListFront[SOUND_BANK_COUNT] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1
}; // pointers to head of list
// u8 sMaxChannelsForSoundBank[SOUND_BANK_COUNT] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };


f32 gDefaultSoundArgs[3] = { 0.0f, 0.0f, 0.0f };
u16 gSoundBanksDisabled = 0;
u8 sHasStartedFadeOut = FALSE;
u8 gMaxBGMVol = 0; // 0x80: VOL CAP ENABLED
u8 sPlayer1CurSeqId = 0;
u8 sPlayer1CurSeqIdTargetVolume = 0;

u8 sBackgroundMusicQueueSize = 0;

// bss
s16 *gCurrAiBuffer;
struct Sound sSoundRequests[0x100];
// Curiously, this has size 3, despite SEQUENCE_PLAYERS == 4 on EU
struct ChannelVolumeScaleFade SeqChannelVelocityFades[3][CHANNELS_MAX];
// u8 sUsedChannelsForSoundBank[SOUND_BANK_COUNT];
u8 sCurrentSound[SOUND_BANK_COUNT][MAX_CHANNELS_PER_SOUND]; // index into gSoundBanks
// list item memory for sSoundBankUsedListBack and sSoundBankFreeListFront
struct SoundCharacteristics gSoundBanks[SOUND_BANK_COUNT][40];
u8 soundBankPitch[SOUND_BANK_COUNT];
u8 sPlayer0CurSeqIdTargetVolume;
struct SequenceQueueItem sBackgroundMusicQueue[MAX_BG_MUSIC_QUEUE_SIZE];






// Local functions that could be static but are defined in/called from GLOBAL_ASM blocks,
// or not part of the large block of static functions.

// returns fade volume or 0xff for background music


/**
 * Fade out a sequence player
 */

#include <ultra64.h>

#include "synthesis.h"
#include "heap.h"
#include "data.h"
#include "load.h"
#include "seqplayer.h"
#include "external.h"
#include <PR/ultratypes.h>
#include "effects.h"
#include "external.h"
#include "playback.h"
#include "sm64.h"
#include "game/OPT_FOR_SIZE.h"

#define PORTAMENTO_IS_SPECIAL(x) ((x).mode & 0x80)
#define PORTAMENTO_MODE(x) ((x).mode & ~0x80)
#define PORTAMENTO_MODE_1 1
#define PORTAMENTO_MODE_2 2
#define PORTAMENTO_MODE_3 3
#define PORTAMENTO_MODE_4 4
#define PORTAMENTO_MODE_5 5

#define DMEM_ADDR_TEMP 0x0
#define DMEM_ADDR_UNCOMPRESSED_NOTE 0x180
#define DMEM_ADDR_ADPCM_RESAMPLED 0x20
#define DMEM_ADDR_ADPCM_RESAMPLED2 0x160
#define DMEM_ADDR_NOTE_PAN_TEMP 0x200
#define DMEM_ADDR_STEREO_STRONG_TEMP_DRY 0x200
#define DMEM_ADDR_STEREO_STRONG_TEMP_WET 0x340
#define DMEM_ADDR_COMPRESSED_ADPCM_DATA 0x3f0
#define DMEM_ADDR_LEFT_CH 0x4c0
#define DMEM_ADDR_RIGHT_CH 0x600
#define DMEM_ADDR_WET_LEFT_CH 0x740
#define DMEM_ADDR_WET_RIGHT_CH 0x880

#define aSetLoadBufferPair(pkt, c, off)                                                                \
    aSetBuffer(pkt, 0, c + DMEM_ADDR_WET_LEFT_CH, 0, DEFAULT_LEN_1CH - c);                             \
    aLoadBuffer(pkt, VIRTUAL_TO_PHYSICAL2(gSynthesisReverb.ringBuffer.left + (off)));                  \
    aSetBuffer(pkt, 0, c + DMEM_ADDR_WET_RIGHT_CH, 0, DEFAULT_LEN_1CH - c);                            \
    aLoadBuffer(pkt, VIRTUAL_TO_PHYSICAL2(gSynthesisReverb.ringBuffer.right + (off)))

#define aSetSaveBufferPair(pkt, c, d, off)                                                             \
    aSetBuffer(pkt, 0, 0, c + DMEM_ADDR_WET_LEFT_CH, d);                                               \
    aSaveBuffer(pkt, VIRTUAL_TO_PHYSICAL2(gSynthesisReverb.ringBuffer.left + (off)));                  \
    aSetBuffer(pkt, 0, 0, c + DMEM_ADDR_WET_RIGHT_CH, d);                                              \
    aSaveBuffer(pkt, VIRTUAL_TO_PHYSICAL2(gSynthesisReverb.ringBuffer.right + (off)));
#undef ALIGN
#define ALIGN(val, amnt) (((val) + (1 << amnt) - 1) & ~((1 << amnt) - 1))




//CACHEBLOCK SEPERATION: ABOVE: update game sound. BELOW: synthesis_execute

//
//u64 *load_wave_samples(u64 *cmd, struct Note *note, s32 nSamplesToLoad) {
//    s32 a3;
//    s32 i;
//    aSetBuffer(cmd++, /*flags*/ 0, /*dmemin*/ DMEM_ADDR_UNCOMPRESSED_NOTE, /*dmemout*/ 0,
//               /*count*/ sizeof(note->synthesisBuffers->samples));
//    aLoadBuffer(cmd++, VIRTUAL_TO_PHYSICAL2(note->synthesisBuffers->samples));
//    note->samplePosInt &= (note->sampleCount - 1);
//    a3 = 64 - note->samplePosInt;
//    if (a3 < nSamplesToLoad) {
//        for (i = 0; i <= (nSamplesToLoad - a3 + 63) / 64 - 1; i++) {
//            aDMEMMove(cmd++, /*dmemin*/ DMEM_ADDR_UNCOMPRESSED_NOTE,
//                      /*dmemout*/ DMEM_ADDR_UNCOMPRESSED_NOTE
//                          + (1 + i) * sizeof(note->synthesisBuffers->samples),
//                      /*count*/ sizeof(note->synthesisBuffers->samples));
//        }
//    }
//    return cmd;
//}



void fadeInSeqPlayer(s32 player, FadeT fadeInTime, u8 targetVolume) {
    struct SequencePlayer *seqPlayer = &gSequencePlayers[player];

    if (seqPlayer->state == SEQUENCE_PLAYER_STATE_FADE_OUT) {
        return;
    }

    seqPlayer->fadeTimer = fadeInTime;
    if (fadeInTime == 0) {
        seqPlayer->fadeVolume = (FLOAT_CAST(targetVolume) / EU_FLOAT(127.0));
        return;
    }
    seqPlayer->fadeVelocity =
        (((f32) (FLOAT_CAST(targetVolume) / EU_FLOAT(127.0)) - seqPlayer->fadeVolume)
         / (f32) fadeInTime);
    seqPlayer->state = SEQUENCE_PLAYER_STATE_FADE_IN;
}
void fadeInSeqPlayerAlt(s32 player, FadeT fadeTimer) {
    struct SequencePlayer *seqPlayer = &gSequencePlayers[player];

    if (seqPlayer->state == SEQUENCE_PLAYER_STATE_FADE_OUT) {
        return;
    }

    seqPlayer->fadeTimer = fadeTimer;
    if (fadeTimer == 0) {
        seqPlayer->fadeVolume = seqPlayer->volume;
        return;
    }
    seqPlayer->fadeVelocity = (seqPlayer->volume - seqPlayer->fadeVolume) / fadeTimer;
    seqPlayer->state = SEQUENCE_PLAYER_STATE_2;
}
s32 controlSeq0Volume(s32 fadeTimer) {
    s32 vol = 0xff;
    s32 temp;

    if (sPlayer0CurSeqId == SEQUENCE_NONE || sPlayer0CurSeqId == SEQ_EVENT_CUTSCENE_CREDITS) {
        return 0xff;
    }

    if (gSequencePlayers[SEQ_PLAYER_LEVEL].volume == 0.0f && fadeTimer) {
        gSequencePlayers[SEQ_PLAYER_LEVEL].volume = gSequencePlayers[SEQ_PLAYER_LEVEL].fadeVolume;
    }

    if (sPlayer0CurSeqIdTargetVolume != 0) {
        vol = (sPlayer0CurSeqIdTargetVolume & 0x7f);
    }

    if (gMaxBGMVol != 0) {
        temp = (gMaxBGMVol & 0x7f);
        if (vol > temp) {
            vol = temp;
        }
    }

    /*if (D_80332110 != 0 && vol > 20) {
        vol = 20;
    }*/

    if (gSequencePlayers[SEQ_PLAYER_LEVEL].enabled) {
        if (vol != 0xff) {
            fadeInSeqPlayer(SEQ_PLAYER_LEVEL, fadeTimer, vol);
        } else {
            gSequencePlayers[SEQ_PLAYER_LEVEL].volume =
                gSequencePlayers[SEQ_PLAYER_LEVEL].volumeDefault / (127.0f);
            fadeInSeqPlayerAlt(SEQ_PLAYER_LEVEL, fadeTimer);
        }
    }
    return vol;
}

//. sound request code start
void update_background_music_after_sound(register s32 bankIndex,
                                    s32 item) { // makes BGM quieter or louder depending on context?
    if (gSoundBanks[bankIndex][item].soundBits & SOUND_FLAG_LOWER_BGM) {
        controlSeq0Volume(50);
    }
}
ALWAYS_INLINE static void process_sound_request(u32 bits, f32 *pos, u16 banksDisnabled) {
    s32 bankIndex;
    s32 index;
    s32 counter = 0;

    bankIndex = bits >> SOUNDARGS_SHIFT_BANK;
    if (banksDisnabled & (1 << bankIndex)) {
        return;
    }

    index = gSoundBanks[bankIndex][0].next;
    while (index != 0xff && index != 0) {
        if (gSoundBanks[bankIndex][index].pos == pos) {
            if ((gSoundBanks[bankIndex][index].soundBits & SOUNDARGS_MASK_PRIORITY)
                <= (bits & SOUNDARGS_MASK_PRIORITY)) {
                if ((gSoundBanks[bankIndex][index].soundBits & SOUND_DISCRETE) != 0
                    || (bits & SOUNDARGS_MASK_SOUNDID)
                           != (gSoundBanks[bankIndex][index].soundBits & SOUNDARGS_MASK_SOUNDID)) {
                    update_background_music_after_sound(bankIndex, index);
                    gSoundBanks[bankIndex][index].soundBits = bits;
                    gSoundBanks[bankIndex][index].soundStatus = bits & SOUNDARGS_MASK_STATUS;
                }
                gSoundBanks[bankIndex][index].freshness = 10;
            }
            index = 0;
        } else {
            index = gSoundBanks[bankIndex][index].next;
        }
        counter = 1;
    }

    if (!counter) {
        soundBankPitch[bankIndex] = 32;
    }

    if (gSoundBanks[bankIndex][sSoundBankFreeListFront[bankIndex]].next != 0xff && index != 0) {
        index = sSoundBankFreeListFront[bankIndex];
        gSoundBanks[bankIndex][index].pos = pos;
        gSoundBanks[bankIndex][index].distance =
            sqrtf(pos[0] * pos[0] + pos[1] * pos[1] + pos[2] * pos[2]);
        gSoundBanks[bankIndex][index].soundBits = bits;
        gSoundBanks[bankIndex][index].soundStatus = bits & SOUNDARGS_MASK_STATUS;
        gSoundBanks[bankIndex][index].freshness = 10;
        gSoundBanks[bankIndex][index].prev = sSoundBankUsedListBack[bankIndex];
        gSoundBanks[bankIndex][sSoundBankUsedListBack[bankIndex]].next = index;
        sSoundBankUsedListBack[bankIndex] = index;
        sSoundBankFreeListFront[bankIndex] = gSoundBanks[bankIndex][index].next;
        gSoundBanks[bankIndex][sSoundBankFreeListFront[bankIndex]].prev = 0xff;
        gSoundBanks[bankIndex][index].next = 0xff;
    }
}
void delete_sound_from_bank(s32 bankIndex, s32 item) {
    if (sSoundBankUsedListBack[bankIndex] == item) {
        sSoundBankUsedListBack[bankIndex] = gSoundBanks[bankIndex][item].prev;
    } else {
        gSoundBanks[bankIndex][gSoundBanks[bankIndex][item].next].prev =
            gSoundBanks[bankIndex][item].prev;
    }

    gSoundBanks[bankIndex][gSoundBanks[bankIndex][item].prev].next = gSoundBanks[bankIndex][item].next;
    gSoundBanks[bankIndex][item].next = sSoundBankFreeListFront[bankIndex];
    gSoundBanks[bankIndex][item].prev = 255;
    gSoundBanks[bankIndex][sSoundBankFreeListFront[bankIndex]].prev = item;
    sSoundBankFreeListFront[bankIndex] = item;
}
ALWAYS_INLINE static void select_current_sounds(s32 bankIndex) {
    u32 val2;
    s32 soundToOverwrite;
    register s32 soundIndex;
    u32 curPriority = 0x10000000;
    u32 liveSoundIndex = 0xff;
    s32 requestedPriority;

    soundIndex = gSoundBanks[bankIndex][0].next;
    while (soundIndex != 0xff) {
        soundToOverwrite = soundIndex;

        if ((gSoundBanks[bankIndex][soundIndex].soundBits & (SOUND_DISCRETE | SOUNDARGS_MASK_STATUS))
            == (SOUND_DISCRETE | SOUND_STATUS_STARTING)) {
            if (gSoundBanks[bankIndex][soundIndex].freshness-- == 0) {
                gSoundBanks[bankIndex][soundIndex].soundBits = NO_SOUND;
            }
        } else if ((gSoundBanks[bankIndex][soundIndex].soundBits & SOUND_DISCRETE) == 0) {
            if (gSoundBanks[bankIndex][soundIndex].freshness-- == 8) {
                update_background_music_after_sound(bankIndex, soundIndex);
                gSoundBanks[bankIndex][soundIndex].soundBits = NO_SOUND;
            }
        }

        if (gSoundBanks[bankIndex][soundIndex].soundBits == NO_SOUND
            && gSoundBanks[bankIndex][soundIndex].soundStatus == SOUND_STATUS_STARTING) {
            soundToOverwrite = gSoundBanks[bankIndex][soundIndex].prev;
            gSoundBanks[bankIndex][soundIndex].soundStatus = SOUND_STATUS_STOPPED;
            delete_sound_from_bank(bankIndex, soundIndex);
        }

        if (gSoundBanks[bankIndex][soundIndex].soundStatus != SOUND_STATUS_STOPPED
            && soundIndex == soundToOverwrite) {
            /* gSoundBanks[bankIndex][soundIndex].distance = sqrtf(
                 (*gSoundBanks[bankIndex][soundIndex].pos[0] *
               *gSoundBanks[bankIndex][soundIndex].pos[0])
                 + (gSoundBanks[bankIndex][soundIndex].pos[1] *
               gSoundBanks[bankIndex][soundIndex].pos[1])
                 + (gSoundBanks[bankIndex][soundIndex].pos[2] *
               gSoundBanks[bankIndex][soundIndex].pos[2]));*/

            requestedPriority = (gSoundBanks[bankIndex][soundIndex].soundBits & SOUNDARGS_MASK_PRIORITY)
                                >> SOUNDARGS_SHIFT_PRIORITY;
            if (gSoundBanks[bankIndex][soundIndex].soundBits & SOUND_NO_PRIORITY_LOSS) {
                gSoundBanks[bankIndex][soundIndex].priority = 0x4c * (0xff - requestedPriority);
            } /*else if (gSoundBanks[bankIndex][soundIndex].pos[2] > 0.0f) {
                gSoundBanks[bankIndex][soundIndex].priority =
                    (u32) gSoundBanks[bankIndex][soundIndex].distance
                    + (u32) (gSoundBanks[bankIndex][soundIndex].pos[2] / US_FLOAT(6.0))
                    + 0x4c * (0xff - requestedPriority);
            } else {
                gSoundBanks[bankIndex][soundIndex].priority =
                    (u32) gSoundBanks[bankIndex][soundIndex].distance
                    + 0x4c * (0xff - requestedPriority);
            }*/
            else {
                gSoundBanks[bankIndex][soundIndex].priority =
                    (u32) gSoundBanks[bankIndex][soundIndex].distance
                    + (u32) (gSoundBanks[bankIndex][soundIndex].pos[2] / US_FLOAT(6.0))
                    + 0x4c * (0xff - requestedPriority);
            }

            if (curPriority >= gSoundBanks[bankIndex][soundIndex].priority) {
                curPriority = gSoundBanks[bankIndex][soundIndex].priority;
                liveSoundIndex = soundIndex;
            }
        }
        soundIndex = gSoundBanks[bankIndex][soundToOverwrite].next;
    }

    for (soundIndex = 0; soundIndex < MAX_SOUNDS_PER_BANK; soundIndex++) {
        if (liveSoundIndex != 0xff && sCurrentSound[bankIndex][0] == liveSoundIndex) {
            liveSoundIndex = 0xff;
            soundIndex = 0xfe;
        }
    }

    if (soundIndex
        != 0xff) { // it has been determined that a new sound is being played, update accordingly
        if (sCurrentSound[bankIndex][0] != 0xff) {
            if (gSoundBanks[bankIndex][sCurrentSound[bankIndex][0]].soundBits == NO_SOUND) {
                if (gSoundBanks[bankIndex][sCurrentSound[bankIndex][0]].soundStatus
                    == SOUND_STATUS_PLAYING) {
                    gSoundBanks[bankIndex][sCurrentSound[bankIndex][0]].soundStatus =
                        SOUND_STATUS_STOPPED;
                    delete_sound_from_bank(bankIndex, sCurrentSound[bankIndex][0]);
                }
            }
            val2 = gSoundBanks[bankIndex][sCurrentSound[bankIndex][0]].soundBits
                   & (SOUND_DISCRETE | SOUNDARGS_MASK_STATUS);
            if (val2 >= (SOUND_DISCRETE | SOUND_STATUS_PLAYING)
                && gSoundBanks[bankIndex][sCurrentSound[bankIndex][0]].soundStatus
                       != SOUND_STATUS_STOPPED) {
                update_background_music_after_sound(bankIndex, sCurrentSound[bankIndex][0]);
                gSoundBanks[bankIndex][sCurrentSound[bankIndex][0]].soundBits = NO_SOUND;
                gSoundBanks[bankIndex][sCurrentSound[bankIndex][0]].soundStatus = SOUND_STATUS_STOPPED;
                delete_sound_from_bank(bankIndex, sCurrentSound[bankIndex][0]);
            } else {
                if (val2 == SOUND_STATUS_PLAYING
                    && gSoundBanks[bankIndex][sCurrentSound[bankIndex][0]].soundStatus
                           != SOUND_STATUS_STOPPED) {
                    gSoundBanks[bankIndex][sCurrentSound[bankIndex][0]].soundStatus =
                        SOUND_STATUS_STARTING;
                }
            }
        }
        sCurrentSound[bankIndex][0] = 0xff;
    }

    if (liveSoundIndex != 0xff) {
        if (sCurrentSound[bankIndex][0] == 0xff) {
            sCurrentSound[bankIndex][0] = liveSoundIndex;
            gSoundBanks[bankIndex][liveSoundIndex].soundBits =
                (gSoundBanks[bankIndex][liveSoundIndex].soundBits & ~SOUNDARGS_MASK_STATUS) + 1;
            liveSoundIndex = 0xff;
        }
    }
}


void stopSound(s32 bankIndex, s32 index) {
    update_background_music_after_sound(bankIndex, index);
    delete_sound_from_bank(bankIndex, index);
}

/**
 * Given an x and z coordinates, return the pan. This is a value between 0 and
 * 1 that represents the audio direction.
 *
 * Pan:
 * 0.0 - fully left
 * 0.5 - center pan
 * 1.0 - fully right
 */
CONST f32 get_sound_pan(f32 x, f32 z) {
    f32 absX;
    f32 absZ;

    // There are 4 panning equations (12-hr clock used for angles)
    // 1. (0,0) fully-centered pan
    // 2. far right pan: between 1:30 and 4:30
    // 3. far left pan: between 7:30 and 10:30
    // 4. center pan: between 4:30 and 7:30 or between 10:30 and 1:30
    if (x == US_FLOAT(0.0) && z == US_FLOAT(0.0)) {
        // x and y being 0 results in a center pan
        return .5f;
    } else {
        absX = absf(x);
        /*  if (absX > AUDIO_MAX_DISTANCE) {
              absX = AUDIO_MAX_DISTANCE;
          }*/
        absZ = absf(z);
        /*if (absZ > AUDIO_MAX_DISTANCE) {
            absZ = AUDIO_MAX_DISTANCE;
        }*/
        if (x >= US_FLOAT(0.0) && absX >= absZ) {
            // far right pan
            return US_FLOAT(1.0)
                   - (US_FLOAT(44000.0) - absX) / (US_FLOAT(3.0) * (US_FLOAT(44000.0) - absZ));
        } else if (x < 0 && absX > absZ) {
            // far left pan
            return (US_FLOAT(44000.0) - absX) / (US_FLOAT(3.0) * (US_FLOAT(44000.0) - absZ));
        } else {
            // center pan
            return 0.5f + x / (US_FLOAT(6.0) * absZ);
        }
    }
}

#define VOLUMELOSS .85f
// todo: starchime control volume here
ALIGNED32 const f32 varyVolume[16] = {
    0.f,          1.f / 192.f,  2.f / 192.f,  3.f / 192.f,  4.f / 192.f,  5.f / 192.f,
    6.f / 192.f,  7.f / 192.f,  8.f / 192.f,  9.f / 192.f,  10.f / 192.f, 11.f / 192.f,
    12.f / 192.f, 13.f / 192.f, 14.f / 192.f, 15.f / 192.f,
};
PURE f32 get_sound_dynamics(s32 bankIndex, s32 item) {
    f32 intensity;
    f32 f0 = bankIndex < 3 ? 10000.f : 6667.f;
    if (!(gSoundBanks[bankIndex][item].soundBits & SOUND_NO_VOLUME_LOSS)) {
        if (gSoundBanks[bankIndex][item].distance > AUDIO_MAX_DISTANCE) {
            intensity = 0;
        } else if (f0 < gSoundBanks[bankIndex][item].distance) {
            intensity = ((AUDIO_MAX_DISTANCE - gSoundBanks[bankIndex][item].distance)
                         / (AUDIO_MAX_DISTANCE - f0))
                        * (1.0f - VOLUMELOSS);
        } else {
            intensity = 1.0f - gSoundBanks[bankIndex][item].distance / f0 * VOLUMELOSS;
        }

        if (gSoundBanks[bankIndex][item].soundBits & SOUND_VARY) {
            if (intensity >= 0.08f) {
                intensity -= varyVolume[gAudioRandom & 0xf];
            }
        }
    } else {
        intensity = (1.0f);
    }
    if ((gSoundBanks[bankIndex][item].soundBits >> 16) == (SOUND_AIR_PEACH_TWINKLE >> 16)) {
        return intensity * intensity * intensity * intensity * (2.f);
        //   intensity = (coss(intensity*0x8000) + 1.f)*.5f;
    }
    return VOLUMELOSS * intensity * intensity + 1.0f - VOLUMELOSS;
}

f32 adjustFreq = 0.f;
PURE f32 get_sound_freq_scale(s32 bankIndex, s32 item) {
    f32 f2;

    if (!(gSoundBanks[bankIndex][item].soundBits & SOUND_NO_FREQUENCY_LOSS)) {
        f2 = gSoundBanks[bankIndex][item].distance / AUDIO_MAX_DISTANCE;
        if (gSoundBanks[bankIndex][item].soundBits & SOUND_VARY) {
            f2 += (f32) (gAudioRandom & 0xff) / US_FLOAT(64.0);
        }
        if (gSoundBanks[bankIndex][item].soundBits & SOUND_VARIABLE_PITCH) {
            f2 += adjustFreq;
        }
    } else {
        return US_FLOAT(1.0);
    }
    return f2 / US_FLOAT(15.0) + US_FLOAT(1.0);
}
#define VAL 40.0f
ALWAYS_INLINE PURE s32 get_sound_reverb(s32 bankIndex, s32 item, s32 channelIndex) {
    s32 reverb;
    s32 add;

    if (gSoundBanks[bankIndex][item].soundBits & SOUND_NO_ECHO) {
        add = 0;
    } else {
        add = gAreaEchoLevel[(gCurrLevelNum > LEVEL_MAX ? LEVEL_MAX : gCurrLevelNum)]
                            [(gCurrAreaIndex - 1) & 7];
    }
    reverb =
        ((u8) gSequencePlayers[SEQ_PLAYER_SFX].channels[channelIndex]->soundScriptIO[5] + add
         + (US_FLOAT(1.0) - gSequencePlayers[SEQ_PLAYER_SFX].channels[channelIndex]->volume) * VAL);

    /*if (reverb > 0x7f) {
        reverb = 0x7f;
    }*/
    return reverb;
}

#undef VAL

ALWAYS_INLINE void getSFXSoundDynamics(s32 bankIndex, s32 channelIndex, s32 index) {
    f32 ret;
    switch (bankIndex) {
        case 1:
            if (!(gSoundBanks[bankIndex][index].soundBits & SOUND_NO_FREQUENCY_LOSS)) {
                if (soundBankPitch[bankIndex] > 8) {
                    ret = get_sound_dynamics(bankIndex, index);
                    gSequencePlayers[SEQ_PLAYER_SFX].channels[channelIndex]->volume = ret;
                } else {
                    ret = get_sound_dynamics(bankIndex, index);
                    gSequencePlayers[SEQ_PLAYER_SFX].channels[channelIndex]->volume =
                        (soundBankPitch[bankIndex] + 8.0f) / 16 * ret;
                }
                gSequencePlayers[SEQ_PLAYER_SFX].channels[channelIndex]->pan = get_sound_pan(
                    gSoundBanks[bankIndex][index].pos[0], gSoundBanks[bankIndex][index].pos[2]);

                /*  if ((gSoundBanks[bankIndex][index].soundBits & SOUNDARGS_MASK_SOUNDID)
                      == (SOUND_MOVING_FLYING & SOUNDARGS_MASK_SOUNDID)) {
                      ret = get_sound_freq_scale(bankIndex, index);
                      gSequencePlayers[SEQ_PLAYER_SFX].channels[channelIndex]->freqScale =
                          ((f32) soundBankPitch[bankIndex] / US_FLOAT(80.0)) + ret;
                  } else {*/
                ret = get_sound_freq_scale(bankIndex, index);
                gSequencePlayers[SEQ_PLAYER_SFX].channels[channelIndex]->freqScale =
                    ((f32) soundBankPitch[bankIndex] / US_FLOAT(400.0)) + ret;
                // }
                gSequencePlayers[SEQ_PLAYER_SFX].channels[channelIndex]->reverb =
                    get_sound_reverb(bankIndex, index, channelIndex);

                break;
            }
        // fallthrough
        case 7:
            *(u32 *) &gSequencePlayers[SEQ_PLAYER_SFX].channels[channelIndex]->volume = 0x3F800000;
            *(u32 *) &gSequencePlayers[SEQ_PLAYER_SFX].channels[channelIndex]->pan = 0x3F000000;
            *(u32 *) &gSequencePlayers[SEQ_PLAYER_SFX].channels[channelIndex]->freqScale = 0x3F800000;
            break;
        default:
            gSequencePlayers[SEQ_PLAYER_SFX].channels[channelIndex]->volume =
                get_sound_dynamics(bankIndex, index);
            gSequencePlayers[SEQ_PLAYER_SFX].channels[channelIndex]->pan = get_sound_pan(
                gSoundBanks[bankIndex][index].pos[0], gSoundBanks[bankIndex][index].pos[2]);
            gSequencePlayers[SEQ_PLAYER_SFX].channels[channelIndex]->freqScale =
                get_sound_freq_scale(bankIndex, index);
            gSequencePlayers[SEQ_PLAYER_SFX].channels[channelIndex]->reverb =
                get_sound_reverb(bankIndex, index, channelIndex);
            break;
            /*case 3:
            case 4:
            case 5:
            case 6:
            case 8:
            case 9:*/
    }
}



ALWAYS_INLINE void fadeInMusicChannels(s32 player) {
    s32 i;

    for (i = 0; i < CHANNELS_MAX; i++) {
        if (gSequencePlayers[player].channels[i] != 0
            && SeqChannelVelocityFades[player][i].remDuration != 0) {
            SeqChannelVelocityFades[player][i].current += SeqChannelVelocityFades[player][i].velocity;
            gSequencePlayers[player].channels[i]->volumeScale =
                SeqChannelVelocityFades[player][i].current;
            SeqChannelVelocityFades[player][i].remDuration--;
            if (SeqChannelVelocityFades[player][i].remDuration == 0) {
                gSequencePlayers[player].channels[i]->volumeScale =
                    SeqChannelVelocityFades[player][i].target / (127.0f);
            }
        }
    }
}
ALWAYS_INLINE void enableSecondMusicMaybe(void) {
    if (gSequencePlayers[SEQ_PLAYER_ENV].enabled || gMaxBGMVol == 0) {
        return;
    }

    gMaxBGMVol = 0;
    controlSeq0Volume(50);

    if (sPlayer0CurSeqIdTargetVolume != 0
        && (sPlayer1CurSeqId == SEQ_EVENT_MERRY_GO_ROUND
            || sPlayer1CurSeqId == SEQ_EVENT_PIRANHA_PLANT)) {
        play_sequence(SEQ_PLAYER_ENV, sPlayer1CurSeqId, 1);
        if (sPlayer1CurSeqIdTargetVolume != 0xff) {
            fadeInSeqPlayer(SEQ_PLAYER_ENV, 1, sPlayer1CurSeqIdTargetVolume);
        }
    }
}
ALWAYS_INLINE static void process_level_music_dynamics(void) {
    /* s32 conditionBits;      // s0
     u32 tempBits;           // v1
     s32 musicDynIndex;      // sp57 87
     s32 condIndex;          // a0, v1
     s32 i;                  // s1
     s32 j;                  // v0
     s32 conditionValues[8]; // sp44 68
     s32 conditionTypes[8];  // sp3C 60
     s32 dur1;               // sp3A 58
     s32 dur2;               // sp38 56
     s32 bit;                // a1 (in first loop), s0, v1*/
    // room for 16 bits without affecting stack
    s32 i;
    for (i =0; i < 2; i+=2){
    fadeInMusicChannels(i);
    }
    /*fadeInMusicChannels(0);
    fadeInMusicChannels(2);*/
    enableSecondMusicMaybe();
    return; // this feature is unused in RTYI therefore not supported

    /*
        if (sMusicDynamicDelay) {
            sMusicDynamicDelay--;
        } else {
            sBackgroundMusiorDynamics = sPlayer0CurSeqId;
        }

        if (sBackgroundMusiorDynamics != sLevelDynamics[gCurrLevelNum][0]) {
            return;
        }

        // conditionBits uses a3 instead of s0
        // s16 cast is unnecessary, u16 cast fixes regalloc in the switch
        conditionBits = (sLevelDynamics[gCurrLevelNum][1]) & 0xff00;
        musicDynIndex = (sLevelDynamics[gCurrLevelNum][1]) & 0xff;
        i = 2;
        while (conditionBits & 0xff00) {
            j = 0;
            condIndex = 0;
            bit = 0x8000;
            while (j < 8) {
                if (conditionBits & bit) {
                    conditionValues[condIndex] = sLevelDynamics[gCurrLevelNum][i++];
                    conditionTypes[condIndex] = j;
                    condIndex++;
                }

                j++;
                bit = bit >> 1;
            }

            // condIndex uses a0 (the same register as 'bit') instead of v1
            for (j = 0; j < condIndex; j++) {
                switch (conditionTypes[j]) {
                    case MARIO_X_GE: {
                        if (((s16) gMarioStates[0].pos[0]) < conditionValues[j])
                            j = condIndex + 1;
                        break;
                    }
                    case MARIO_Y_GE: {
                        if (((s16) gMarioStates[0].pos[1]) < conditionValues[j])
                            j = condIndex + 1;
                        break;
                    }
                    case MARIO_Z_GE: {
                        if (((s16) gMarioStates[0].pos[2]) < conditionValues[j])
                            j = condIndex + 1;
                        break;
                    }
                    case MARIO_X_LT: {
                        if (((s16) gMarioStates[0].pos[0]) >= conditionValues[j])
                            j = condIndex + 1;
                        break;
                    }
                    case MARIO_Y_LT: {
                        if (((s16) gMarioStates[0].pos[1]) >= conditionValues[j])
                            j = condIndex + 1;
                        break;
                    }
                    case MARIO_Z_LT: {
                        if (((s16) gMarioStates[0].pos[2]) >= conditionValues[j])
                            j = condIndex + 1;
                        break;
                    }
                    case MARIO_IS_IN_AREA: {
                        if (gCurrAreaIndex != conditionValues[j])
                            j = condIndex + 1;
                        break;
                    }
                    case MARIO_IS_IN_ROOM: {
                        j = condIndex + 1;
                        break;
                    }
                }
            }

            if (j == condIndex) {
                // The area matches. Break out of the loop.
                tempBits = 0;
            } else {
                // s16 cast is unnecessary, u16 cast fixes regalloc
                // While conditionBits didn't need a cast above, the opposite is the case here
                tempBits = (sLevelDynamics[gCurrLevelNum][i]) & 0xff00;
                musicDynIndex = (sLevelDynamics[gCurrLevelNum][i]) & 0xff;
                i++;
            }

            conditionBits = tempBits;
        }

        if (sCurrentMusicDynamic != musicDynIndex) {
            // bit keeps using a0, should use v1
            bit = 1;
            if (sCurrentMusicDynamic == 0xff) {
                dur1 = 1;
                dur2 = 1;
            } else {
                dur1 = sMusicDynamics[musicDynIndex].dur1;
                dur2 = sMusicDynamics[musicDynIndex].dur2;
            }

            for (i = 0; i < CHANNELS_MAX; i++) {
                if (sMusicDynamics[musicDynIndex].bits1 & bit) {
                    // The instructions setting a0 and a1 are swapped, but get fixed pretty easily by a
                    // branch or anything that changes regalloc
                    fade_channel_volume_scale(SEQ_PLAYER_LEVEL, i,
       sMusicDynamics[musicDynIndex].volScale1, dur1);
                }
                if (sMusicDynamics[musicDynIndex].bits2 & bit) {
                    fade_channel_volume_scale(SEQ_PLAYER_LEVEL, i,
       sMusicDynamics[musicDynIndex].volScale2, dur2);
                }
                bit <<= 1;
            }

            sCurrentMusicDynamic = musicDynIndex;
        }*/
}




void update_game_sound(void) {
    s32 soundStatus;
    s32 soundId;
    s32 bankIndex;
    register s32 index;
    struct Sound *sound;
    const u16 banksDisnabled = gSoundBanksDisabled;
    while (sSoundRequestID != sProcessedSoundID) {
        sound = &sSoundRequests[sProcessedSoundID];
        process_sound_request(sound->soundBits, sound->position, banksDisnabled);
        sProcessedSoundID++;
    }
    process_level_music_dynamics(); // mangled this one
    if (gSequencePlayers[SEQ_PLAYER_SFX].channels[0] == 0) {
        return;
    }

    for (bankIndex = 0; bankIndex < SOUND_BANK_COUNT; bankIndex++) {
        select_current_sounds(
            bankIndex); // now assumes 1 sound per channel, which was already the assumption
        // for (j = 0; j < MAX_CHANNELS_PER_SOUND; j++) {
        index = sCurrentSound[bankIndex][0];
        if (index < 0xff && gSoundBanks[bankIndex][index].soundStatus != SOUND_STATUS_STOPPED) {
            soundStatus = gSoundBanks[bankIndex][index].soundBits & SOUNDARGS_MASK_STATUS;
            soundId = (gSoundBanks[bankIndex][index].soundBits >> SOUNDARGS_SHIFT_SOUNDID);
            gSoundBanks[bankIndex][index].soundStatus = soundStatus;
            if (soundStatus == SOUND_STATUS_STARTING) {
                if (gSoundBanks[bankIndex][index].soundBits & SOUND_FLAG_LOWER_BGM) {
                    // D_80332110 |= 1 << bankIndex;
                    controlSeq0Volume(50);
                }

                gSoundBanks[bankIndex][index].soundBits++;
                gSoundBanks[bankIndex][index].soundStatus = SOUND_STATUS_PLAYING;
                gSequencePlayers[SEQ_PLAYER_SFX].channels[bankIndex]->soundScriptIO[4] = soundId;
                gSequencePlayers[SEQ_PLAYER_SFX].channels[bankIndex]->soundScriptIO[0] = 1;
                goto computeSounds;
            } else if (gSequencePlayers[SEQ_PLAYER_SFX].channels[bankIndex]->layers[0] == NULL) {
                goto stopSoundAndBank;
            } else if (soundStatus == SOUND_STATUS_STOPPED
                       && gSequencePlayers[SEQ_PLAYER_SFX].channels[bankIndex]->layers[0]->finished
                              == FALSE) {
                stopSound(bankIndex, index);
                gSequencePlayers[SEQ_PLAYER_SFX].channels[bankIndex]->soundScriptIO[0] = 0;
            } else if (gSequencePlayers[SEQ_PLAYER_SFX].channels[bankIndex]->layers[0]->enabled
                       == FALSE) {
            stopSoundAndBank:
                stopSound(bankIndex, index);
                gSoundBanks[bankIndex][index].soundStatus = SOUND_STATUS_STOPPED;
            } else {
            computeSounds:
                getSFXSoundDynamics(bankIndex, bankIndex, index);
            }
        }
        //   }

        // sUsedChannelsForSoundBank[i] = sMaxChannelsForSoundBank[i] = 1, so this doesn't do anything
        // channelIndex += MAX_SOUNDS_PER_BANK - MAX_SOUNDS_PER_BANK;
    }
}


















//CACHEBLOCK SEPERATION: ABOVE: update game sound. BELOW: synthesis_execute





ALWAYS_INLINE void audio_dma_partial_copy_asyncSYNTHESIS(uintptr_t *devAddr, u8 **vAddr, ssize_t *remaining,
                                  OSMesgQueue *queue, OSIoMesg *mesg) {

    ssize_t transfer = (*remaining < 0x1000 ? *remaining : 0x1000);
    *remaining -= transfer;
    osInvalDCache(*vAddr, transfer);
    osPiStartDma(mesg, OS_MESG_PRI_NORMAL, OS_READ, *devAddr, *vAddr, transfer, queue);
    *devAddr += transfer;
    *vAddr += transfer;
}



ALWAYS_INLINE void init_note_listSYNTHESIS(struct AudioListItem *list) {
    list->prev = list;
    list->next = list;
    list->u.count = 0;
}



ALWAYS_INLINE void note_enable(struct Note *note) {
    note->enabled = TRUE;
    note->needsInit = TRUE;
    note->restart = FALSE;
    note->finished = FALSE;
  /*  note->stereoStrongRight = FALSE;
    note->stereoStrongLeft = FALSE;*/
  /*  note->headsetPanLeft = 0;
    note->headsetPanRight = 0;
    note->prevHeadsetPanRight = 0;
    note->prevHeadsetPanLeft = 0;*/
}
ALWAYS_INLINE void note_init_volume(struct Note *note) {
    note->targetVolLeft = 0;
    note->targetVolRight = 0;
    note->reverb = 0;
    note->reverbVol = 0;
    note->curVolLeft = 1;
    note->curVolRight = 1;
    note->frequency = 0.0f;
}
ALWAYS_INLINE void adsr_init(struct AdsrState *adsr, struct AdsrEnvelope *envelope, UNUSED s16 *volOut) {
    adsr->action = 0;
    adsr->state = ADSR_STATE_DISABLED;
    adsr->initial = 0;
    adsr->delay = 0;
    adsr->velocity = 0;
    adsr->envelope = envelope;
    adsr->volOut = volOut;
}
ALWAYS_INLINE void note_init(struct Note *note) {
    if (note->parentLayer->adsr.releaseRate == 0) {
        adsr_init(&note->adsr, note->parentLayer->seqChannel->adsr.envelope, &note->adsrVolScale);
    } else {
        adsr_init(&note->adsr, note->parentLayer->adsr.envelope, &note->adsrVolScale);
    }
    note->adsr.state = ADSR_STATE_INITIAL;
    note_init_volume(note);
    note_enable(note);
}
void note_vibrato_init(struct Note *note) {
    struct VibratoState *vib;

    note->vibratoFreqScale = 1.0f;
    note->portamentoFreqScale = 1.0f;

    vib = &note->vibratoState;

    if (note->parentLayer->seqChannel->vibratoExtentStart == 0
        && note->parentLayer->seqChannel->vibratoExtentTarget == 0
        && note->parentLayer->portamento.mode == 0) {
        vib->active = FALSE;
        return;
    }

    vib->active = TRUE;
    vib->time = 0;

   // vib->curve = (s8 *) gVibratoCurve;
    vib->seqChannel = note->parentLayer->seqChannel;

    if ((vib->extentChangeTimer = vib->seqChannel->vibratoExtentChangeDelay) == 0) {
        vib->extent = vib->seqChannel->vibratoExtentTarget;
    } else {
        vib->extent = vib->seqChannel->vibratoExtentStart;
    }

    if ((vib->rateChangeTimer = vib->seqChannel->vibratoRateChangeDelay) == 0) {
        vib->rate = vib->seqChannel->vibratoRateTarget;
    } else {
        vib->rate = vib->seqChannel->vibratoRateStart;
    }
    vib->delay = vib->seqChannel->vibratoDelay;

    note->portamento = note->parentLayer->portamento;
}
ALWAYS_INLINE void note_set_frequency(struct Note *note, f32 frequency) {
    note->frequency = frequency;
}
ALWAYS_INLINE s8 get_vibrato_pitch_change(struct VibratoState *vib) {
    s32 index;
    vib->time += vib->rate;

    index = (vib->time >> 10) & 0x3F;

    switch (index & 0x30) {
        case 0x10:
            index = 31 - index;

        case 0x00:
            return gVibratoCurve[index];

        case 0x20:
            index -= 0x20;
            break;

        case 0x30:
            index = 63 - index;
            break;
    }

    return -gVibratoCurve[index];
}
ALWAYS_INLINE f32 get_vibrato_freq_scale(struct VibratoState *vib) {
    s32 pitchChange;
    f32 extent;
    f32 result;

    if (vib->delay != 0) {
        vib->delay--;
        return 1;
    }

    if (vib->extentChangeTimer) {
        if (vib->extentChangeTimer == 1) {
            vib->extent = (s32) vib->seqChannel->vibratoExtentTarget;
        } else {
            vib->extent += ((s32) vib->seqChannel->vibratoExtentTarget - vib->extent)
                           / (s32) vib->extentChangeTimer;
        }

        vib->extentChangeTimer--;
    } else if (vib->seqChannel->vibratoExtentTarget != (s32) vib->extent) {
        if ((vib->extentChangeTimer = vib->seqChannel->vibratoExtentChangeDelay) == 0) {
            vib->extent = (s32) vib->seqChannel->vibratoExtentTarget;
        }
    }

    if (vib->rateChangeTimer) {
        if (vib->rateChangeTimer == 1) {
            vib->rate = (s32) vib->seqChannel->vibratoRateTarget;
        } else {
            vib->rate +=
                ((s32) vib->seqChannel->vibratoRateTarget - vib->rate) / (s32) vib->rateChangeTimer;
        }

        vib->rateChangeTimer--;
    } else if (vib->seqChannel->vibratoRateTarget != (s32) vib->rate) {
        if ((vib->rateChangeTimer = vib->seqChannel->vibratoRateChangeDelay) == 0) {
            vib->rate = (s32) vib->seqChannel->vibratoRateTarget;
        }
    }

    if (vib->extent == 0) {
        return 1.0f;
    }

    pitchChange = get_vibrato_pitch_change(vib);
    extent = (f32) vib->extent / US_FLOAT(4096.0);

    result = US_FLOAT(1.0) + extent * (gPitchBendFrequencyScale[pitchChange + 127] - US_FLOAT(1.0));
    return result;
}
ALWAYS_INLINE f32 get_portamento_freq_scale(struct Portamento *p) {
    u32 v0;
    f32 result;
    if (p->mode == 0) {
        return 1.0f;
    }

    p->cur += p->speed;
    v0 = (u32) p->cur;

    if (v0 >= 127) {
        v0 = 127;
    }

    result = US_FLOAT(1.0) + p->extent * (gPitchBendFrequencyScale[v0 + 127] - US_FLOAT(1.0));
    return result;
}
ALWAYS_INLINE void note_vibrato_update(struct Note *note) {
    if (note->vibratoState.active) {
        note->portamentoFreqScale = get_portamento_freq_scale(&note->portamento);
        if (note->parentLayer != NO_LAYER) {
            note->vibratoFreqScale = get_vibrato_freq_scale(&note->vibratoState);
        }
    }
}
ALWAYS_INLINE s32 adsr_update(struct AdsrState *adsr) {
    u8 action = adsr->action;
    switch (adsr->state) {
        case ADSR_STATE_DISABLED:
            return 0;

        case ADSR_STATE_INITIAL: {
            adsr->current = adsr->initial;
            adsr->target = adsr->initial;
            if (action & ADSR_ACTION_HANG) {
                adsr->state = ADSR_STATE_HANG;
                break;
            }
            // fallthrough
        }
        case ADSR_STATE_START_LOOP:
            adsr->envIndex = 0;
            adsr->currentHiRes = adsr->current << 0x10;
            adsr->state = ADSR_STATE_LOOP;
            // fallthrough

        case ADSR_STATE_LOOP:
            adsr->delay = BSWAP16(adsr->envelope[adsr->envIndex].delay);
            switch (adsr->delay) {
                case ADSR_DISABLE:
                    adsr->state = ADSR_STATE_DISABLED;
                    break;
                case ADSR_HANG:
                    adsr->state = ADSR_STATE_HANG;
                    break;
                case ADSR_GOTO:
                    adsr->envIndex = BSWAP16(adsr->envelope[adsr->envIndex].arg);
                    break;
                case ADSR_RESTART:
                    adsr->state = ADSR_STATE_INITIAL;
                    break;

                default:
                    adsr->target = BSWAP16(adsr->envelope[adsr->envIndex].arg);
                    adsr->velocity = ((adsr->target - adsr->current) << 0x10) / adsr->delay;
                    adsr->state = ADSR_STATE_FADE;
                    adsr->envIndex++;
                    break;
            }
            if (adsr->state != ADSR_STATE_FADE) {
                break;
            }
            // fallthrough

        case ADSR_STATE_FADE:
            adsr->currentHiRes += adsr->velocity;
            adsr->current = adsr->currentHiRes >> 0x10;
            if (--adsr->delay <= 0) {
                adsr->state = ADSR_STATE_LOOP;
            }
            // fallthrough

        case ADSR_STATE_HANG:
            break;

        case ADSR_STATE_DECAY:
        case ADSR_STATE_RELEASE: {
            adsr->current -= adsr->fadeOutVel;
            if (adsr->sustain != 0 && adsr->state == ADSR_STATE_DECAY) {
                if (adsr->current < adsr->sustain) {
                    adsr->current = adsr->sustain;
                    adsr->delay = adsr->sustain / 16;
                    adsr->state = ADSR_STATE_SUSTAIN;
                }
                break;
            }
            if (adsr->current < 100) {
                adsr->current = 0;
                adsr->state = ADSR_STATE_DISABLED;
            }
            break;
        }

        case ADSR_STATE_SUSTAIN:
            adsr->delay -= 1;
            if (adsr->delay == 0) {
                adsr->state = ADSR_STATE_RELEASE;
            }
            break;
    }

    if ((action & ADSR_ACTION_DECAY)) {
        adsr->state = ADSR_STATE_DECAY;
        adsr->action = action & ~ADSR_ACTION_DECAY;
    }

    if ((action & ADSR_ACTION_RELEASE)) {
        adsr->state = ADSR_STATE_RELEASE;
        adsr->action = action & ~(ADSR_ACTION_RELEASE | ADSR_ACTION_DECAY);
    }

    *adsr->volOut = adsr->current;
    return 0;
}
s32 note_init_for_layer(struct Note *note, struct SequenceChannelLayer *seqLayer) {
    note->prevParentLayer = NO_LAYER;
    note->parentLayer = seqLayer;
    note->priority = seqLayer->seqChannel->notePriority;
    if (IS_BANK_LOAD_COMPLETE(seqLayer->seqChannel->bankId) == FALSE) {
        return TRUE;
    }

    note->bankId = seqLayer->seqChannel->bankId;
    // note->stereoHeadsetEffects = seqLayer->seqChannel->stereoHeadsetEffects;
    note->sound = seqLayer->sound;
    seqLayer->status = SOUND_LOAD_STATUS_DISCARDABLE; // "loaded"
    seqLayer->note = note;
    /* if (note->sound == NULL) {
         build_synthetic_wave(note, seqLayer);
     }*/
    note_init(note);
    return FALSE;
}
void note_disable(struct Note *note) {
    if (note->needsInit) {
        note->needsInit = FALSE;
    } else {
        //note_set_vel_pan_reverb(note, 0, .5f, 0);
        note->targetVolLeft = 0;
        note->targetVolRight = 0;
        note->reverb = 0;
        note->reverbVol = 0;
        note->envMixerNeedsInit = TRUE;
    }
    note->priority = NOTE_PRIORITY_DISABLED;
    note->enabled = FALSE;
    note->finished = FALSE;
    note->parentLayer = NO_LAYER;
    note->prevParentLayer = NO_LAYER;
}
ALWAYS_INLINE void note_set_vel_pan_reverb(struct Note *note, f32 velocity, f32 pan, s32 reverb) {
    s32 panIndex;
    f32 volLeft;
    f32 volRight;
    panIndex = (s32) (pan * 127.5f) & 127;
    /*if (note->stereoHeadsetEffects) {
        volLeft = gStereoPanVolume[panIndex];
        volRight = gStereoPanVolume[127 - panIndex];*/

    /* u8 strongLeft;
     u8 strongRight;
     strongLeft = FALSE;
     strongRight = FALSE;
     note->headsetPanLeft = 0;
     note->headsetPanRight = 0;*/
    /* if (panIndex < 0x20) {
         strongLeft = TRUE;
     } else if (panIndex > 0x60) {
         strongRight = TRUE;
     }
     note->stereoStrongRight = strongRight;
     note->stereoStrongLeft = strongLeft;
 }  else {*/
    volLeft = gDefaultPanVolume[panIndex];
    volRight = gDefaultPanVolume[127 - panIndex];
    //}

    if (velocity < 0) {
        velocity = 0;
    }
    note->targetVolLeft = (s32) (velocity * volLeft) & ~0x80FF;
    note->targetVolRight = (s32) (velocity * volRight) & ~0x80FF;
    if (note->targetVolLeft == 0) {
        note->targetVolLeft++;
    }
    if (note->targetVolRight == 0) {
        note->targetVolRight++;
    }
    if (note->reverb != reverb) {
        note->reverb = reverb;
        note->reverbVol = reverb << 8;
        note->envMixerNeedsInit = TRUE;
        return;
    }

    if (note->needsInit) {
        note->envMixerNeedsInit = TRUE;
    } else {
        note->envMixerNeedsInit = FALSE;
    }
}
void audio_list_remove_and_push_back(struct AudioListItem *list, struct AudioListItem *item) {
    if (item->prev != NULL) {
        item->prev->next = item->next;
        item->next->prev = item->prev;
        list->prev->next = item;
        item->prev = list->prev;
        item->next = list;
        list->prev = item;
        list->u.count++;
        item->pool = list->pool;
    }
}


void audio_list_remove(struct AudioListItem *item) {
    // remove 'item' from the list it's in, if any
    if (item->prev != NULL) {
        item->prev->next = item->next;
        item->next->prev = item->prev;
        item->prev = NULL;
    }
}
void audio_list_push_front(struct AudioListItem *list, struct AudioListItem *item) {
    // add 'item' to the front of the list given by 'list', if it's not in any list
    if (item->prev == NULL) {
        item->prev = list;
        item->next = list->next;
        list->next->prev = item;
        list->next = item;
        list->u.count++;
        item->pool = list->pool;
    }
}
ALWAYS_INLINE void process_notes(void) {
    f32 scale;
    f32 frequency;
    u8 reverb;
    f32 velocity;
    f32 pan;
    f32 cap;
    struct Note *note;
    struct NoteAttributes *attributes;
    s32 i;
    for (i = 0; i < gMaxSimultaneousNotes; i++) {
        note = &gNotes[i];
        if (note->priority != NOTE_PRIORITY_DISABLED) {
            if (note->priority == NOTE_PRIORITY_STOPPING || note->finished) {
                if (note->adsrVolScale == 0 || note->finished) {
                    if (note->wantedParentLayer != NO_LAYER) {
                        note_disable(note);
                        if (note->wantedParentLayer->seqChannel != NULL) {
                            if (note_init_for_layer(note, note->wantedParentLayer)) {
                                note_disable(note);
                                audio_list_remove(&note->listItem);
                                audio_list_push_front(&note->listItem, &gNoteFreeLists.poolDisabled);
                            } else {
                                note_vibrato_init(note);

                                audio_list_remove_and_push_back(&note->listItem.pool->poolActive,
                                                                &note->listItem);
                                note->wantedParentLayer = NO_LAYER;
                            }
                        } else {
                            note_disable(note);
                            audio_list_remove_and_push_back(&note->listItem.pool->poolDisabled,
                                                            &note->listItem);
                            note->wantedParentLayer = NO_LAYER;
                            continue;
                        }
                    } else {
                        note_disable(note);
                        audio_list_remove_and_push_back(&note->listItem.pool->poolDisabled,
                                                        &note->listItem);
                        continue;
                    }
                }
            } else {
                if (note->adsr.state == ADSR_STATE_DISABLED) {
                    note_disable(note);
                    audio_list_remove_and_push_back(&note->listItem.pool->poolDisabled,
                                                    &note->listItem);
                    continue;
                }
            }

            adsr_update(&note->adsr);
            note_vibrato_update(note);
            attributes = &note->attributes;
            if (note->priority == NOTE_PRIORITY_STOPPING) {
                frequency = attributes->freqScale;
                velocity = attributes->velocity;
                pan = attributes->pan;
                reverb = attributes->reverb;
            } else {
                frequency = note->parentLayer->noteFreqScale;
                velocity = note->parentLayer->noteVelocity;
                pan = note->parentLayer->notePan;
                reverb = note->parentLayer->seqChannel->reverb;
            }

            scale = note->adsrVolScale;
            frequency *= note->vibratoFreqScale * note->portamentoFreqScale;
            cap = 3.99992f;
            if (gAiFrequency != 32006) {
                frequency *= US_FLOAT(32000.0) / (f32) gAiFrequency;
            }
            frequency = (frequency < cap ? frequency : cap);
            scale *= 4.3498e-5f; // ~1 / 23000
            velocity = velocity * scale * scale;
            note_set_frequency(note, frequency);
            note_set_vel_pan_reverb(note, velocity, pan, reverb);
            continue;
        }
    }
}
/*
void build_synthetic_wave(struct Note *note, struct SequenceChannelLayer *seqLayer) {
    s32 i;
    s32 j;
    s32 pos;
    s32 stepSize;
    s32 offset;
    u8 lim;
    u8 origSampleCount = note->sampleCount;

    if (seqLayer->freqScale < US_FLOAT(1.0)) {
        note->sampleCount = 64;
        seqLayer->freqScale *= US_FLOAT(1.0465);
        stepSize = 1;
    } else if (seqLayer->freqScale < US_FLOAT(2.0)) {
        note->sampleCount = 32;
        seqLayer->freqScale *= US_FLOAT(0.52325);
        stepSize = 2;
    } else if (seqLayer->freqScale < US_FLOAT(4.0)) {
        note->sampleCount = 16;
        seqLayer->freqScale *= US_FLOAT(0.26263);
        stepSize = 4;
    } else {
        note->sampleCount = 8;
        seqLayer->freqScale *= US_FLOAT(0.13081);
        stepSize = 8;
    }

    if (note->sampleCount == origSampleCount && seqLayer->seqChannel->instOrWave == note->instOrWave) {
        return;
    }

    // Load wave sample
    note->instOrWave = (u8) seqLayer->seqChannel->instOrWave;
    for (i = -1, pos = 0; pos < 0x40; pos += stepSize) {
        i++;
        note->synthesisBuffers->samples[i] = gWaveSamples[seqLayer->seqChannel->instOrWave - 0x80][pos];
    }

    // Repeat sample
    for (offset = note->sampleCount; offset < 0x40; offset += note->sampleCount) {
        lim = note->sampleCount;
        if (offset < 0 || offset > 0) {
            for (j = 0; j < lim; j++) {
                note->synthesisBuffers->samples[offset + j] = note->synthesisBuffers->samples[j];
            }
        } else {
            for (j = 0; j < lim; j++) {
                note->synthesisBuffers->samples[offset + j] = note->synthesisBuffers->samples[j];
            }
        }
    }

    osWritebackDCache(note->synthesisBuffers->samples, sizeof(note->synthesisBuffers->samples));
}*/
/*
ALWAYS_INLINE void init_synthetic_wave(struct Note *note, struct SequenceChannelLayer *seqLayer) {
    s32 sampleCount = note->sampleCount;
    build_synthetic_wave(note, seqLayer);
    if (sampleCount != 0) {
        note->samplePosInt *= note->sampleCount / sampleCount;
    } else {
        note->samplePosInt = 0;
    }
}*/



ALWAYS_INLINE void note_release_and_take_ownership(struct Note *note,
                                                   struct SequenceChannelLayer *seqLayer) {
    note->wantedParentLayer = seqLayer;
    note->priority = NOTE_PRIORITY_STOPPING;
    note->adsr.fadeOutVel = 0x8000 / gAudioUpdatesPerFrame;
    note->adsr.action |= ADSR_ACTION_RELEASE;
}
void *audio_list_pop_back(struct AudioListItem *list) {
    struct AudioListItem *item = list->prev;
    if (item == list) {
        return NULL;
    }
    item->prev->next = list;
    list->prev = item->prev;
    item->prev = NULL;
    list->u.count--;
    return item->u.value;
}
ALWAYS_INLINE struct Note *pop_node_with_value_less_equal(struct AudioListItem *list, s32 limit) {
    struct AudioListItem *cur = list->next;
    struct AudioListItem *best;

    if (cur == list) {
        return NULL;
    }

    for (best = cur; cur != list; cur = cur->next) {
        if (((struct Note *) best->u.value)->priority >= ((struct Note *) cur->u.value)->priority) {
            best = cur;
        }
    }

    if (limit < ((struct Note *) best->u.value)->priority) {
        return NULL;
    }

    audio_list_remove(best);
    return best->u.value;
}
struct Note *alloc_note_from_disabled(struct NotePool *pool, struct SequenceChannelLayer *seqLayer) {
    struct Note *note = audio_list_pop_back(&pool->poolDisabled);
    if (note != NULL) {
        if (note_init_for_layer(note, seqLayer)) {
            audio_list_push_front(&gNoteFreeLists.poolDisabled, &note->listItem);
            return NULL;
        }
        audio_list_push_front(&pool->poolActive, &note->listItem);
    }
    return note;
}
void audio_list_push_back(struct AudioListItem *list, struct AudioListItem *item) {
    if (item->prev == NULL) {
        list->prev->next = item;
        item->prev = list->prev;
        item->next = list;
        list->prev = item;
        list->u.count++;
        item->pool = list->pool;
    }
}
struct Note *alloc_note_from_decaying(struct NotePool *pool, struct SequenceChannelLayer *seqLayer) {
    struct Note *note = audio_list_pop_back(&pool->poolDecaying);
    if (note != NULL) {
        note_release_and_take_ownership(note, seqLayer);
        audio_list_push_back(&pool->poolReleasing, &note->listItem);
    }
    return note;
}
void seq_channel_layer_decay_release_internal(struct SequenceChannelLayer *seqLayer, const s32 target) {
    struct Note *note;
    struct NoteAttributes *attributes;

    if (seqLayer == NO_LAYER || seqLayer->note == NULL) {
        return;
    }

    note = seqLayer->note;
    attributes = &note->attributes;

    if (seqLayer->seqChannel != NULL && seqLayer->seqChannel->noteAllocPolicy == 0) {
        seqLayer->note = NULL;
    }

    if (note->wantedParentLayer == seqLayer) {
        note->wantedParentLayer = NO_LAYER;
    }

    if (note->parentLayer != seqLayer) {
        return;
    }

    seqLayer->status = SOUND_LOAD_STATUS_NOT_LOADED;
    if (note->adsr.state != ADSR_STATE_DECAY) {
        attributes->freqScale = seqLayer->noteFreqScale;
        attributes->velocity = seqLayer->noteVelocity;
        attributes->pan = seqLayer->notePan;
        if (seqLayer->seqChannel != NULL) {
            attributes->reverb = seqLayer->seqChannel->reverb;
        }
        note->priority = NOTE_PRIORITY_STOPPING;
        note->prevParentLayer = note->parentLayer;
        note->parentLayer = NO_LAYER;
        if (target == ADSR_STATE_RELEASE) {
            note->adsr.fadeOutVel = 0x8000 / gAudioUpdatesPerFrame;
            note->adsr.action |= ADSR_ACTION_RELEASE;
        } else {
            note->adsr.action |= ADSR_ACTION_DECAY;
            if (seqLayer->adsr.releaseRate == 0) {
                note->adsr.fadeOutVel = seqLayer->seqChannel->adsr.releaseRate * 24;
            } else {
                note->adsr.fadeOutVel = seqLayer->adsr.releaseRate * 24;
            }
            note->adsr.sustain = (note->adsr.current * seqLayer->seqChannel->adsr.sustain) / 0x10000;
        }
    }

    if (target == ADSR_STATE_DECAY) {
        audio_list_remove(&note->listItem);
        audio_list_push_front(&note->listItem.pool->poolDecaying, &note->listItem);
        // audio_list_remove_and_push_back(&note->listItem.pool->poolDecaying, &note->listItem);
    }
}

struct Note *alloc_note_from_active(struct NotePool *pool, struct SequenceChannelLayer *seqLayer) {
    struct Note *note =
        pop_node_with_value_less_equal(&pool->poolActive, seqLayer->seqChannel->notePriority);
    if (note != NULL) {
        seq_channel_layer_decay_release_internal(note->parentLayer, ADSR_STATE_RELEASE);
        note->wantedParentLayer = seqLayer;
        audio_list_push_back(&pool->poolReleasing, &note->listItem);
    }
    return note;
}

ALWAYS_INLINE struct Note *alloc_note(struct SequenceChannelLayer *seqLayer) {
    struct Note *ret;
    u32 policy = seqLayer->seqChannel->noteAllocPolicy;

    if (policy & NOTE_ALLOC_LAYER) {
        ret = seqLayer->note;
        if (ret != NULL && ret->prevParentLayer == seqLayer) {
            note_release_and_take_ownership(ret, seqLayer);
            audio_list_remove_and_push_back(&gNoteFreeLists.poolReleasing, &ret->listItem);
            return ret;
        }
    }

    if (policy & NOTE_ALLOC_CHANNEL) {
        if (!(ret = alloc_note_from_disabled(&seqLayer->seqChannel->notePool, seqLayer))
            && !(ret = alloc_note_from_decaying(&seqLayer->seqChannel->notePool, seqLayer))
            && !(ret = alloc_note_from_active(&seqLayer->seqChannel->notePool, seqLayer))) {
            seqLayer->status = SOUND_LOAD_STATUS_NOT_LOADED;
            return NULL;
        }
        return ret;
    }

    //    if (policy & NOTE_ALLOC_SEQ) {
    //        if (!(ret = alloc_note_from_disabled(&seqLayer->seqChannel->notePool, seqLayer))
    //           /* && !(ret = alloc_note_from_disabled(&seqLayer->seqChannel->seqPlayer->notePool,
    //           seqLayer))*/
    //            && !(ret = alloc_note_from_decaying(&seqLayer->seqChannel->notePool, seqLayer))
    //          /*  && !(ret = alloc_note_from_decaying(&seqLayer->seqChannel->seqPlayer->notePool,
    //          seqLayer))*/
    //            && !(ret = alloc_note_from_active(&seqLayer->seqChannel->notePool, seqLayer))
    //           /* && !(ret = alloc_note_from_active(&seqLayer->seqChannel->seqPlayer->notePool,
    //           seqLayer))*/) {
    //            seqLayer->status = SOUND_LOAD_STATUS_NOT_LOADED;
    //            return NULL;
    //        }
    //        return ret;
    //    }

    if (policy & NOTE_ALLOC_GLOBAL_FREELIST) {
        if (!(ret = alloc_note_from_disabled(&gNoteFreeLists, seqLayer))
            && !(ret = alloc_note_from_decaying(&gNoteFreeLists, seqLayer))
            && !(ret = alloc_note_from_active(&gNoteFreeLists, seqLayer))) {
            seqLayer->status = SOUND_LOAD_STATUS_NOT_LOADED;
            return NULL;
        }
        return ret;
    }

    if (!(ret = alloc_note_from_disabled(&seqLayer->seqChannel->notePool, seqLayer))
        /*  && !(ret = alloc_note_from_disabled(&seqLayer->seqChannel->seqPlayer->notePool, seqLayer))*/
        && !(ret = alloc_note_from_disabled(&gNoteFreeLists, seqLayer))
        && !(ret = alloc_note_from_decaying(&seqLayer->seqChannel->notePool, seqLayer))
        /*&& !(ret = alloc_note_from_decaying(&seqLayer->seqChannel->seqPlayer->notePool, seqLayer))*/
        && !(ret = alloc_note_from_decaying(&gNoteFreeLists, seqLayer))
        && !(ret = alloc_note_from_active(&seqLayer->seqChannel->notePool, seqLayer))
        /* && !(ret = alloc_note_from_active(&seqLayer->seqChannel->seqPlayer->notePool, seqLayer))*/
        && !(ret = alloc_note_from_active(&gNoteFreeLists, seqLayer))) {
        seqLayer->status = SOUND_LOAD_STATUS_NOT_LOADED;
        return NULL;
    }
    return ret;
}
ALWAYS_INLINE u8 m64_read_u8(struct M64ScriptState *state) {
    u8 *midiArg = state->pc++;
    return *midiArg;
}

s16 m64_read_s16(struct M64ScriptState *state) {
    s32 ret = *(state->pc++) << 8;
    return *(state->pc++) | ret;
}

u16 m64_read_compressed_u16(struct M64ScriptState *state) {
    s32 ret = *(state->pc++);
    if (ret & 0x80) {
        ret = (ret << 8) & 0x7f00;
        ret = *(state->pc++) | ret;
    }
    return ret;
}
s32 get_instrument(struct SequenceChannel *seqChannel, s32 instId, struct Instrument **instOut,
                   struct AdsrSettings *adsr) {
    struct Instrument *inst;
    if (instId >= gCtlEntries[seqChannel->bankId].numInstruments) {
        instId = gCtlEntries[seqChannel->bankId].numInstruments;
        if (instId == 0) {
            return 0;
        }
        instId--;
    }
    inst = gCtlEntries[seqChannel->bankId].instruments[instId];
    while (inst == NULL) {
        instId--;
        inst = gCtlEntries[seqChannel->bankId].instruments[instId];
    }
    if (((uintptr_t) gBankLoadedPool.persistent.pool.start <= (uintptr_t) inst
         && (uintptr_t) inst <= (uintptr_t) (gBankLoadedPool.persistent.pool.start
                                             + gBankLoadedPool.persistent.pool.size))
        || ((uintptr_t) gBankLoadedPool.temporary.pool.start <= (uintptr_t) inst
            && (uintptr_t) inst <= (uintptr_t) (gBankLoadedPool.temporary.pool.start
                                                + gBankLoadedPool.temporary.pool.size))) {
        adsr->envelope = inst->envelope;
        adsr->releaseRate = inst->releaseRate;
        *instOut = inst;
        instId++;
        return instId;
    }
    *instOut = NULL;
    return 0;
}
void seq_channel_layer_disable(struct SequenceChannelLayer *layer) {
    if (layer != NULL) {
        seq_channel_layer_decay_release_internal(layer, ADSR_STATE_DECAY);
        layer->enabled = FALSE;
        layer->finished = TRUE;
    }
}
void seq_channel_layer_process_script(
    struct SequenceChannelLayer *layer) { // normal layer script processing youd expect
    struct SequencePlayer *seqPlayer;     // sp5C, t4
    struct SequenceChannel *seqChannel;   // sp58, t5
    struct M64ScriptState *state;         // v0
    struct AudioBankSound *sound;         // v0
    struct Instrument *instrument;        // v1
    struct Drum *drum;
    u8 sameSound;  // sp3F
    u8 cmd;        // a0 sp3E, EU s2
    u16 sp3A;      // t2, a0, a1
    s32 vel;       // sp30, t3
    f32 freqScale; // sp28, f0
    f32 sp24;
    s32 usedSemitone; // a1
    f32 temp_f12;
    f32 temp_f2;
    s32 temp_a0_5;
    u8 portamentoTargetNote; // t7
    u8 instId;               // v0
    u8 cmdSemitone;          // v1
    f32 tuning;              // f0

    //! Copt: manually __attribute__((always_inline))  inline these functions in the scope of this
    //! routine

    sameSound = TRUE;
    if (layer->enabled == FALSE) {
        return;
    }

    if (layer->delay > 1) {
        layer->delay--;
        if (!layer->stopSomething && layer->delay <= layer->duration) {
            seq_channel_layer_decay_release_internal(layer, ADSR_STATE_DECAY);
            layer->stopSomething = TRUE;
        }
        return;
    }

    if (!layer->continuousNotes) {
        seq_channel_layer_decay_release_internal(layer, ADSR_STATE_DECAY);
    }

    if (PORTAMENTO_MODE(layer->portamento) == PORTAMENTO_MODE_1
        || PORTAMENTO_MODE(layer->portamento) == PORTAMENTO_MODE_2) {
        layer->portamento.mode = 0;
    }

    seqChannel = (*(layer)).seqChannel;
    seqPlayer = (*(seqChannel)).seqPlayer;
    for (;;) {
        state = &layer->scriptState;
        // M64_READ_U8(state, cmd);
        //  manually inlined because we need _Kqi6 :(
        M64_READ_U8(state, cmd);
        if (cmd <= 0xc0) {
            break;
        }

        switch (cmd) {
            case 0xff: // layer_end; function return or end of script
                if (state->depth == 0) {
                    // N.B. this function call is *not* inlined even though it's
                    // within the same file, unlike in the rest of this function.
                    seq_channel_layer_disable(layer);
                    return;
                }
                state->depth--, state->pc = state->stack[state->depth];
                break;

            case 0xfc: // layer_call
                M64_READ_S16(state, sp3A);
                state->depth++, state->stack[state->depth - 1] = state->pc;
                state->pc = seqPlayer->seqData + sp3A;
                break;

            case 0xf8: // layer_loop; loop start, N iterations (or 256 if N = 0)
                M64_READ_U8(state, state->remLoopIters[state->depth])
                state->depth++, state->stack[state->depth - 1] = state->pc;
                break;

            case 0xf7: // layer_loopend
                if (--state->remLoopIters[state->depth - 1] != 0) {
                    state->pc = state->stack[state->depth - 1];
                } else {
                    state->depth--;
                }
                break;

            case 0xfb: // layer_jump
                M64_READ_S16(state, sp3A);
                state->pc = seqPlayer->seqData + sp3A;
                break;

            case 0xc1: // layer_setshortnotevelocity
            case 0xca: // layer_setpan
                temp_a0_5 = *(state->pc++);
                if (cmd == 0xc1) {
                    layer->velocitySquare = (f32) (temp_a0_5 * temp_a0_5);
                } else {
                    layer->pan = (f32) temp_a0_5 / US_FLOAT(128.0);
                }
                break;

            case 0xc2: // layer_transpose; set transposition in semitones
                layer->transposition = *(state->pc++);
                break;
            case 0xc9: // layer_setshortnoteduration
                layer->noteDuration = *(state->pc++);
                break;

            case 0xc4: // layer_somethingon
            case 0xc5: // layer_somethingoff
                layer->continuousNotes = (cmd == 0xc4) ? TRUE : FALSE;
                seq_channel_layer_decay_release_internal(layer, ADSR_STATE_DECAY);
                break;

            case 0xc3: // layer_setshortnotedefaultplaypercentage
                M64_READ_COMPRESSED_U16(state, sp3A)
                layer->shortNoteDefaultPlayPercentage = sp3A;
                break;

            case 0xc6: // layer_setinstr
                M64_READ_U8(state, instId);
                if (instId < 127) {
                    get_instrument(seqChannel, instId, &(*layer).instrument, &(*layer).adsr);
                }
                break;
            case 0xc7: // layer_portamento
                M64_READ_U8(state, (*layer).portamento.mode);
                M64_READ_U8(state, PORTAMENTO_TGT_NOTE);
                PORTAMENTO_TGT_NOTE = PORTAMENTO_TGT_NOTE + (*layer).transposition
                                      + (*seqPlayer).transposition + (*seqChannel).transposition;
                if (PORTAMENTO_TGT_NOTE >= 0x80) {
                    PORTAMENTO_TGT_NOTE = 0;
                }
                layer->portamentoTargetNote = PORTAMENTO_TGT_NOTE;

                // If special, the next param is u8 instead of var
                if (PORTAMENTO_IS_SPECIAL((*(layer)).portamento)) {
                    layer->portamentoTime = *((state)->pc++);
                    break;
                }

                M64_READ_COMPRESSED_U16(state, sp3A)
                layer->portamentoTime = sp3A;
                break;

            case 0xc8: // layer_disableportamento
                layer->portamento.mode = 0;
                break;
            default:
                switch (cmd & 0xf0) {
                    case 0xd0: // layer_setshortnotevelocityfromtable
                        sp3A = seqPlayer->shortNoteVelocityTable[cmd & 0xf];
                        (*(layer)).velocitySquare = (f32) (sp3A * sp3A);
                        break;
                    case 0xe0: // layer_setshortnotedurationfromtable
                        (*(layer)).noteDuration = seqPlayer->shortNoteDurationTable[cmd & 0xf];
                        break;
                }
        }
    }
    if (cmd == 0xc0) // layer_delay
    {
        M64_READ_COMPRESSED_U16(state, layer->delay);
        layer->stopSomething = TRUE;
    } else {
        layer->stopSomething = FALSE;
        if (seqChannel->largeNotes) {
            switch (cmd & 0xc0) {
                case 0x00: // layer_note0 (play percentage, velocity, duration)
                    M64_READ_COMPRESSED_U16(state, sp3A)
                    vel = *((*state).pc++);
                    layer->noteDuration = *(state->pc++);
                    layer->playPercentage = sp3A;
                    break;

                case 0x40: // layer_note1 (play percentage, velocity)
                    M64_READ_COMPRESSED_U16(state, sp3A)
                    vel = *(state->pc++);
                    layer->noteDuration = 0;
                    layer->playPercentage = sp3A;
                    break;

                case 0x80: // layer_note2 (velocity, duration; uses last play percentage)
                    sp3A = layer->playPercentage;
                    vel = *(state->pc++);
                    layer->noteDuration = *(state->pc++);
                    break;
            }
            cmdSemitone = (cmd & 0x3F);
            layer->velocitySquare = vel * vel;
        } else {
            switch (cmd & 0xc0) {
                case 0x00: // play note, type 0 (play percentage)
                    M64_READ_COMPRESSED_U16(state, sp3A)
                    layer->playPercentage = sp3A;
                    break;

                case 0x40: // play note, type 1 (uses default play percentage)
                    sp3A = layer->shortNoteDefaultPlayPercentage;
                    break;

                case 0x80: // play note, type 2 (uses last play percentage)
                    sp3A = layer->playPercentage;
                    break;
            }
            cmdSemitone = cmd & 0x3F;
        }

        layer->delay = sp3A;
        layer->duration = layer->noteDuration * sp3A / 256;
        if ((seqPlayer->muted && (seqChannel->muteBehavior & MUTE_BEHAVIOR_STOP_NOTES) != 0)
            || !seqChannel->hasInstrument) {
            layer->stopSomething = TRUE;
        } else {
            if (seqChannel->instOrWave == 0) { // drum
                // DRUM_INDEX = cmdSemitone;
                DRUM_INDEX += (*seqChannel).transposition + (*layer).transposition;
                if (DRUM_INDEX >= gCtlEntries[seqChannel->bankId].numDrums) {
                    DRUM_INDEX = gCtlEntries[seqChannel->bankId].numDrums;
                    /* if (DRUM_INDEX == 0) {// WORRY: Does this kill the drum?
                         // this goto looks a bit like a function return...
                         layer->stopSomething = TRUE;
                         goto skip;
                     }*/

                    DRUM_INDEX--;
                }

                drum = gCtlEntries[seqChannel->bankId].drums[DRUM_INDEX];
                /* if (drum == NULL) {
                     layer->stopSomething = TRUE;
                 } else {*/
                layer->adsr.envelope = drum->envelope;
                layer->adsr.releaseRate = drum->releaseRate;
                layer->pan = FLOAT_CAST(drum->pan) / US_FLOAT(128.0);
                layer->sound = &drum->sound;
                layer->freqScale = layer->sound->tuning;
                //}

                // skip:;
            } else { // instrument
                cmdSemitone += (*(seqPlayer)).transposition + (*(seqChannel)).transposition
                               + (*(layer)).transposition;
                if (cmdSemitone >= 0x80) {
                    layer->stopSomething = TRUE;
                } else {
                    instrument = layer->instrument;
                    if (instrument == NULL) {
                        instrument = seqChannel->instrument;
                    }
                    if (layer->portamento.mode != 0) {
                        if (instrument != NULL) {
                            usedSemitone = (layer->portamentoTargetNote < cmdSemitone)
                                               ? cmdSemitone
                                               : layer->portamentoTargetNote;
                            sound = (u8) usedSemitone < instrument->normalRangeLo
                                        ? &instrument->lowNotesSound
                                    : (u8) usedSemitone <= instrument->normalRangeHi
                                        ? &instrument->normalNotesSound
                                        : &instrument->highNotesSound;

                            sameSound = (sound == (*layer).sound);
                            layer->sound = sound;
                            tuning = (*sound).tuning;
                        } else {
                            layer->sound = NULL;
                            tuning = 1.0f;
                        }

                        temp_f2 = gNoteFrequencies[SEMITONE] * tuning;
                        temp_f12 = gNoteFrequencies[layer->portamentoTargetNote] * tuning;

                        if (PORTAMENTO_MODE(layer->portamento) & 1) {
                            /*   case PORTAMENTO_MODE_1:
                               case PORTAMENTO_MODE_3:
                               case PORTAMENTO_MODE_5:*/
                            sp24 = temp_f2;
                            freqScale = temp_f12;

                        } else {
                            /* case PORTAMENTO_MODE_2:
                             case PORTAMENTO_MODE_4:*/
                            freqScale = temp_f2;
                            sp24 = temp_f12;
                        }
                        layer->portamento.extent = sp24 / freqScale - US_FLOAT(1.0);
                        if (PORTAMENTO_IS_SPECIAL((*(layer)).portamento)) {
                            layer->portamento.speed =
                                US_FLOAT(32512.0) * FLOAT_CAST((*(seqPlayer)).tempo)
                                / ((f32) (*(layer)).delay * (f32) gTempoInternalToExternal
                                   * FLOAT_CAST((*(layer)).portamentoTime));
                        } else {
                            layer->portamento.speed =
                                US_FLOAT(127.0) / FLOAT_CAST((*(layer)).portamentoTime);
                        }
                        layer->portamento.cur = 0.0f;
                        layer->freqScale = freqScale;
                        if (PORTAMENTO_MODE((*(layer)).portamento) == PORTAMENTO_MODE_5) {
                            layer->portamentoTargetNote = SEMITONE;
                        }
                    } else /* if (instrument != NULL)*/ { // portamento mode off
                        sound = SEMITONE < instrument->normalRangeLo    ? &instrument->lowNotesSound
                                : SEMITONE <= instrument->normalRangeHi ? &instrument->normalNotesSound
                                                                        : &instrument->highNotesSound;

                        sameSound = (sound == (*(layer)).sound);
                        layer->sound = sound;
                        layer->freqScale = gNoteFrequencies[SEMITONE] * (*(sound)).tuning;
                    } /* else {
                         layer->sound = NULL;
                         layer->freqScale = gNoteFrequencies[SEMITONE];
                     }*/
                }
            }
        }
    }

    if (layer->stopSomething) {
        if (layer->note != NULL || layer->continuousNotes) {
            seq_channel_layer_decay_release_internal(layer, ADSR_STATE_DECAY);
        }
        return;
    }

    cmd = FALSE;
    if (!layer->continuousNotes) {
        cmd = TRUE;
    } else if (layer->note == NULL || layer->status == SOUND_LOAD_STATUS_NOT_LOADED) {
        cmd = TRUE;
    } else if (sameSound == FALSE) {
        seq_channel_layer_decay_release_internal(layer, ADSR_STATE_DECAY);
        cmd = TRUE;
    } /*else if (layer->sound == NULL) {
        init_synthetic_wave(layer->note, layer);
    }*/

    if (cmd != FALSE) {
        (*(layer)).note = alloc_note(layer);
    }

    if (layer->note != NULL && layer->note->parentLayer == layer) {
        note_vibrato_init(layer->note);
    }
}
void note_pool_clear(struct NotePool *pool) {
    s32 i;
    struct AudioListItem *source;
    struct AudioListItem *cur;
    struct AudioListItem *dest;
    s32 j; // unused in EU

    for (i = 0; i < 4; i++) {
        source = &pool->poolEntry[i];
        dest = &gNoteFreeLists.poolEntry[i];
        j = 0;
        do {
            cur = source->next;
            if (cur == source) {
                break;
            }
            audio_list_remove_and_push_back(dest, cur);
            j++;
        } while (j <= gMaxSimultaneousNotes);
    }
}
ALWAYS_INLINE void note_pool_fill(struct NotePool *pool, s32 count) {
    s32 i;
    s32 j;
    struct Note *note;
    struct AudioListItem *source;
    struct AudioListItem *dest;

    note_pool_clear(pool);

    for (i = 0, j = 0; j < count; i++) {
        if (i == 4) {
            return;
        }

        switch (i) {
            case 0:
                source = &gNoteFreeLists.poolDisabled;
                dest = &pool->poolDisabled;
                break;

            case 1:
                source = &gNoteFreeLists.poolDecaying;
                dest = &pool->poolDecaying;
                break;

            case 2:
                source = &gNoteFreeLists.poolReleasing;
                dest = &pool->poolReleasing;
                break;

            case 3:
                source = &gNoteFreeLists.poolActive;
                dest = &pool->poolActive;
                break;
        }

        while (j < count) {
            note = audio_list_pop_back(source);
            if (note == NULL) {
                break;
            }
            audio_list_push_back(dest, &note->listItem);
            j++;
        }
    }
}

void seq_channel_layer_free(struct SequenceChannel *seqChannel, s32 layerIndex) {
    struct SequenceChannelLayer *layer = seqChannel->layers[layerIndex];
    if (layer != NULL) {
        struct AudioListItem *item = &layer->listItem;
        if (item->prev == NULL) {
            gLayerFreeList.prev->next = item;
            item->prev = gLayerFreeList.prev;
            item->next = &gLayerFreeList;
            gLayerFreeList.prev = item;
            gLayerFreeList.u.count++;
            item->pool = gLayerFreeList.pool;
        }
        seq_channel_layer_disable(layer);
        seqChannel->layers[layerIndex] = NULL;
    }
}
void sequence_channel_disable(struct SequenceChannel *seqChannel) {
    s32 i;
    for (i = 0; i < LAYERS_MAX; i++) {
        seq_channel_layer_free(seqChannel, i);
    }

    note_pool_clear(&seqChannel->notePool);
    seqChannel->enabled = FALSE;
    seqChannel->finished = TRUE;
}

ALWAYS_INLINE void reclaim_notes(void) {
    struct Note *note;
    s32 i;
    s32 cond;

    for (i = 0; i < gMaxSimultaneousNotes; i++) {
        note = &gNotes[i];
        if (note->parentLayer != NO_LAYER) {
            cond = FALSE;
            if (!note->parentLayer->enabled && note->priority >= NOTE_PRIORITY_MIN) {
                cond = TRUE;
            } else if (note->parentLayer->seqChannel == NULL) {
                audio_list_push_back(&gLayerFreeList, &note->parentLayer->listItem);
                seq_channel_layer_disable(note->parentLayer);
                note->priority = NOTE_PRIORITY_STOPPING;
            } else if (note->parentLayer->seqChannel->seqPlayer == NULL) {
                sequence_channel_disable(note->parentLayer->seqChannel);
                note->priority = NOTE_PRIORITY_STOPPING;
            } else if (note->parentLayer->seqChannel->seqPlayer->muted) {
                if (note->parentLayer->seqChannel->muteBehavior
                    & (MUTE_BEHAVIOR_STOP_SCRIPT | MUTE_BEHAVIOR_STOP_NOTES)) {
                    cond = TRUE;
                }
            } else {
                cond = FALSE;
            }
            if (cond) {
                seq_channel_layer_decay_release_internal(note->parentLayer, ADSR_STATE_RELEASE);
                audio_list_remove(&note->listItem);
                audio_list_push_front(&note->listItem.pool->poolDisabled, &note->listItem);
                note->priority = NOTE_PRIORITY_STOPPING;
            }
        }
    }
}


CONST s32 get_volume_ramping(s32 sourceVol, s32 targetVol, s32 arg2) {
    // This roughly computes 2^16 * (targetVol / sourceVol) ^ (8 / arg2),
    // but with discretizations of targetVol, sourceVol and arg2.
    f32 ret;
    switch (arg2) {
        default:
            ret = gVolRampingLhs136[targetVol >> 8] * gVolRampingRhs136[sourceVol >> 8];
            break;
        case 128:
            ret = gVolRampingLhs128[targetVol >> 8] * gVolRampingRhs128[sourceVol >> 8];
            break;
        case 136:
            ret = gVolRampingLhs136[targetVol >> 8] * gVolRampingRhs136[sourceVol >> 8];
            break;
        case 144:
            ret = gVolRampingLhs144[targetVol >> 8] * gVolRampingRhs144[sourceVol >> 8];
            break;
    }
    return ret;
}
//u64 *load_wave_samples(u64 *cmd, struct Note *note, s32 nSamplesToLoad);
ALWAYS_INLINE u64 *final_resample(u64 *cmd, struct Note *note, s32 count, s32 pitch, s32 dmemIn, u32 flags) {
    aSetBuffer(cmd++, /*flags*/ 0, dmemIn, /*dmemout*/ 0, count);
    aResample(cmd++, flags, pitch, VIRTUAL_TO_PHYSICAL2(note->synthesisBuffers->finalResampleState));
    return cmd;
}


s32 seq_channel_set_layer(struct SequenceChannel *seqChannel, s32 layerIndex) {
    struct SequenceChannelLayer *layer;

    if (seqChannel->layers[layerIndex] == NULL) {
        layer = audio_list_pop_back(&gLayerFreeList);
        seqChannel->layers[layerIndex] = layer;
        if (layer == NULL) {
            seqChannel->layers[layerIndex] = NULL;
            return -1;
        }
    } else {
        seq_channel_layer_decay_release_internal(seqChannel->layers[layerIndex], ADSR_STATE_DECAY);
    }

    layer = seqChannel->layers[layerIndex];
    layer->seqChannel = seqChannel;
    layer->adsr = seqChannel->adsr;
    layer->adsr.releaseRate = 0;
    layer->enabled = TRUE;
    layer->stopSomething = FALSE;
    layer->continuousNotes = FALSE;
    layer->finished = FALSE;
    layer->portamento.mode = 0;
    layer->scriptState.depth = 0;
    layer->status = SOUND_LOAD_STATUS_NOT_LOADED;
    layer->noteDuration = 0x80;
    layer->transposition = 0;
    layer->delay = 0;
    layer->duration = 0;
    layer->note = NULL;
    layer->instrument = NULL;
    layer->velocitySquare = 0.0f;
    layer->pan = 0.5f;
    return 0;
}

/**
 * Add an item to the end of a list, if it's not already in any list.
 */

/**
 * Remove the last item from a list, and return it (or NULL if empty).
 */

ALWAYS_INLINE void set_instrument(struct SequenceChannel *seqChannel, u8 instId) {
  /*  if (instId >= 0x80) {
        seqChannel->instOrWave = instId;
        seqChannel->instrument = NULL;
    } else */if (instId == 0x7f) {
        seqChannel->instOrWave = 0;
        seqChannel->instrument = (struct Instrument *) 1;
    } else {
        seqChannel->instOrWave =
            get_instrument(seqChannel, instId, &seqChannel->instrument, &seqChannel->adsr);
        if (seqChannel->instOrWave == 0) {
            seqChannel->hasInstrument = FALSE;
            return;
        }
    }
    seqChannel->hasInstrument = TRUE;
}

ALWAYS_INLINE void sequence_channel_set_volume(struct SequenceChannel *seqChannel, u8 volume) {
    seqChannel->volume = FLOAT_CAST(volume) / US_FLOAT(127.0);
}

ALWAYS_INLINE void *get_bank_or_seqSYNTHESIS(struct SoundMultiPool *arg0, s32 arg1, s32 id) {
    u32 i;
    struct TemporaryPool *temporary = &arg0->temporary;

    if (arg1 == 0) {
        // Try not to overwrite sound that we have just accessed, by setting nextSide appropriately.
        if (temporary->entries[0].id == id) {
            temporary->nextSide = 1;
            return temporary->entries[0].ptr;
        } else if (temporary->entries[1].id == id) {
            temporary->nextSide = 0;
            return temporary->entries[1].ptr;
        }
        return NULL;
    } else {
        struct PersistentPool *persistent = &arg0->persistent;
        for (i = 0; i < persistent->numEntries; i++) {
            if (id == persistent->entries[i].id) {
                return persistent->entries[i].ptr;
            }
        }

        if (arg1 == 2) {
            // Prevent tail call optimization by using a temporary.
            // (Did they compile with -Wo,-notail?)
            return get_bank_or_seqSYNTHESIS(arg0, 0, id);
        }
        return NULL;
    }
}
// rodata: 0xf3e30
void sequence_channel_enable(struct SequencePlayer *seqPlayer, u8 channelIndex, void *arg2) {
    struct SequenceChannel *seqChannel = seqPlayer->channels[channelIndex];
    s32 i;
    if (IS_SEQUENCE_CHANNEL_VALID(seqChannel) != FALSE) {
        seqChannel->enabled = TRUE;
        seqChannel->finished = FALSE;
        seqChannel->scriptState.depth = 0;
        seqChannel->scriptState.pc = arg2;
        seqChannel->delay = 0;
        for (i = 0; i < LAYERS_MAX; i++) {
            if (seqChannel->layers[i] != NULL) {
                seq_channel_layer_free(seqChannel, i);
            }
        }
    }
}
ALWAYS_INLINE void
sequence_channel_process_script(struct SequenceChannel *seqChannel) { // boring channel processing
    struct M64ScriptState *state;
    struct SequencePlayer *seqPlayer;
    u16 sp5A;
    u8 cmd;    // v1, s1
    u8 loBits; // t0, a0
    s32 offset;
    s8 value; // sp53, 4b
    u8 temp;
    s8 tempSigned;

    if (!seqChannel->enabled) {
        return;
    }
    if (seqChannel->stopScript) {
        for (loBits = 0; loBits < LAYERS_MAX; loBits++) {
            if (seqChannel->layers[loBits] != NULL) {
                seq_channel_layer_process_script(seqChannel->layers[loBits]);
            }
        }
        return;
    }

    seqPlayer = seqChannel->seqPlayer;
    if (seqPlayer->muted && (seqChannel->muteBehavior & MUTE_BEHAVIOR_STOP_SCRIPT) != 0) {
        return;
    }

    if (seqChannel->delay != 0) {
        seqChannel->delay--;
    }

    state = &seqChannel->scriptState;
    if (seqChannel->delay == 0) {
        for (;;) {
            cmd = m64_read_u8(state);
            if (cmd > 0xc0) {
                switch (cmd) {
                    case 0xff: // chan_end
                        sp5A = state->depth;
                        if (sp5A == 0) {
                            sequence_channel_disable(seqChannel);
                            goto processLayers;
                        }
                        state->depth--, state->pc = state->stack[state->depth];
                        break;
                    case 0xfe:
                        goto processLayers;
                        break;
                    case 0xfd:
                        seqChannel->delay = m64_read_compressed_u16(state);
                        goto processLayers;
                        break;
                    case 0xf3:
                        seqChannel->stopScript = TRUE;
                        goto processLayers;
                        break;
                    case 0xfc: // chan_call
                        sp5A = m64_read_s16(state);
                        state->depth++, state->stack[state->depth - 1] = state->pc;
                        state->pc = seqPlayer->seqData + sp5A;
                        break;
                    case 0xf8: // chan_loop; loop start, N iterations (or 256 if N = 0)
                        state->remLoopIters[state->depth] = m64_read_u8(state);
                        state->depth++, state->stack[state->depth - 1] = state->pc;
                        break;
                    case 0xf7: // chan_loopend
                        state->remLoopIters[state->depth - 1]--;
                        if (state->remLoopIters[state->depth - 1] != 0) {
                            state->pc = state->stack[state->depth - 1];
                        } else {
                            state->depth--;
                        }
                        break;
                    case 0xf6: // chan_break; break loop, if combined with jump
                        state->depth--;
                        break;
                    case 0xfb: // chan_jump
                    case 0xfa: // chan_beqz
                    case 0xf9: // chan_bltz
                    case 0xf5: // chan_bgez
                        sp5A = m64_read_s16(state);
                        if (cmd == 0xfa && value != 0)
                            break;
                        if (cmd == 0xf9 && value >= 0)
                            break;
                        if (cmd == 0xf5 && value < 0)
                            break;
                        state->pc = seqPlayer->seqData + sp5A;
                        break;
                    case 0xf2: // chan_reservenotes
                               // seqChannel->notePool should live in a saved register
                        note_pool_clear(&seqChannel->notePool);
                        temp = m64_read_u8(state);
                        note_pool_fill(&seqChannel->notePool, temp);
                        break;
                    case 0xf1: // chan_unreservenotes
                        note_pool_clear(&seqChannel->notePool);
                        break;
                    case 0xc2: // chan_setdyntable
                        sp5A = m64_read_s16(state);
                        seqChannel->dynTable = (void *) (seqPlayer->seqData + sp5A);
                        break;
                    case 0xc5: // chan_dynsetdyntable
                        if (value != -1) {
                            sp5A = (*seqChannel->dynTable)[value][1]
                                   + ((*seqChannel->dynTable)[value][0] << 8);
                            seqChannel->dynTable = (void *) (seqPlayer->seqData + sp5A);
                        }
                        break;
                    case 0xc1: // chan_setinstr ("set program"?)
                        set_instrument(seqChannel, m64_read_u8(state));
                        break;
                    case 0xc3: // chan_largenotesoff
                        seqChannel->largeNotes = FALSE;
                        break;

                    case 0xc4: // chan_largenoteson
                        seqChannel->largeNotes = TRUE;
                        break;

                    case 0xdf: // chan_setvol
                        sequence_channel_set_volume(seqChannel, m64_read_u8(state));
                        break;

                    case 0xe0: // chan_setvolscale
                        seqChannel->volumeScale = FLOAT_CAST(m64_read_u8(state)) / US_FLOAT(128.0);
                        break;

                    case 0xde: // chan_freqscale; pitch bend using raw frequency multiplier N/2^15 (N is
                               // u16)
                        sp5A = m64_read_s16(state);
                        seqChannel->freqScale = FLOAT_CAST(sp5A) / US_FLOAT(32768.0);
                        break;

                    case 0xd3: // chan_pitchbend; pitch bend by <= 1 octave in either direction
                               // (-127..127)
                        // (m64_read_u8(state) is really s8 here)
                        temp = m64_read_u8(state) + 127;
                        seqChannel->freqScale = gPitchBendFrequencyScale[temp];
                        break;

                    case 0xdd: // chan_setpan
                        seqChannel->pan = FLOAT_CAST(m64_read_u8(state)) / US_FLOAT(128.0);
                        break;

                    case 0xdc: // chan_setpanmix; set proportion of pan to come from channel (0..128)
                        seqChannel->panChannelWeight = FLOAT_CAST(m64_read_u8(state)) / US_FLOAT(128.0);
                        break;

                    case 0xdb: // chan_transpose; set transposition in semitones
                        tempSigned = *state->pc;
                        state->pc++;
                        seqChannel->transposition = tempSigned;
                        break;

                    case 0xda: // chan_setenvelope
                        sp5A = m64_read_s16(state);
                        seqChannel->adsr.envelope = (struct AdsrEnvelope *) (seqPlayer->seqData + sp5A);
                        break;

                    case 0xd9: // chan_setdecayrelease
                        seqChannel->adsr.releaseRate = m64_read_u8(state);
                        break;

                    case 0xd8: // chan_setvibratoextent
                        seqChannel->vibratoExtentTarget = m64_read_u8(state) * 8;
                        seqChannel->vibratoExtentStart = 0;
                        seqChannel->vibratoExtentChangeDelay = 0;
                        break;

                    case 0xd7: // chan_setvibratorate
                        seqChannel->vibratoRateStart = seqChannel->vibratoRateTarget =
                            m64_read_u8(state) * 32;
                        seqChannel->vibratoRateChangeDelay = 0;
                        break;

                    case 0xe2: // chan_setvibratoextentlinear
                        seqChannel->vibratoExtentStart = m64_read_u8(state) * 8;
                        seqChannel->vibratoExtentTarget = m64_read_u8(state) * 8;
                        seqChannel->vibratoExtentChangeDelay = m64_read_u8(state) * 16;
                        break;

                    case 0xe1: // chan_setvibratoratelinear
                        seqChannel->vibratoRateStart = m64_read_u8(state) * 32;
                        seqChannel->vibratoRateTarget = m64_read_u8(state) * 32;
                        seqChannel->vibratoRateChangeDelay = m64_read_u8(state) * 16;
                        break;

                    case 0xe3: // chan_setvibratodelay
                        seqChannel->vibratoDelay = m64_read_u8(state) * 16;
                        break;

                    case 0xd6: // chan_setupdatesperframe_unimplemented
                        temp = m64_read_u8(state);
                        if (temp == 0) {
                            temp = gAudioUpdatesPerFrame;
                        }
                        break;
                    case 0xd4: // chan_setreverb
                        seqChannel->reverb = m64_read_u8(state);
                        break;

                    case 0xc6: // chan_setbank; switch bank within set
                    {
                        u8 temp = m64_read_u8(state);
                        // Switch to the temp's (0-indexed) bank in this sequence's
                        // bank set. Note that in the binary format (not in the JSON!)
                        // the banks are listed backwards, so we counts from the back.
                        // (gAlBankSets[offset] is number of banks)
                        offset = ((u16 *) gAlBankSets)[seqPlayer->seqId];
                        temp = gAlBankSets[offset + gAlBankSets[offset] - temp];
                        // temp should be in a saved register across this call
                        if (get_bank_or_seqSYNTHESIS(&gBankLoadedPool, 2, temp) != NULL) {
                            seqChannel->bankId = temp;
                        }
                    } break;

                    case 0xc7: // chan_writeseq; write to sequence data (!)
                        // sp38 doesn't go on the stack
                        {
                            u8 sp38;
                            u8 temp;
                            sp38 = value;
                            temp = m64_read_u8(state);
                            seqPlayer->seqData[(u16) m64_read_s16(state)] = sp38 + temp;
                        }
                        break;

                    case 0xc8: // chan_subtract
                    case 0xc9: // chan_bitand
                    case 0xcc: // chan_setval
                        temp = m64_read_u8(state);
                        if (cmd == 0xc8) {
                            value -= temp;
                        } else if (cmd == 0xcc) {
                            value = temp;
                        } else {
                            value &= temp;
                        }
                        break;

                    case 0xca: // chan_setmutebhv
                        seqChannel->muteBehavior = m64_read_u8(state);
                        break;

                    case 0xcb: // chan_readseq
                        sp5A = m64_read_s16(state);
                        value = seqPlayer->seqData[sp5A + value];
                        break;

                    case 0xd0: // chan_stereoheadseteffects
                        // seqChannel->stereoHeadsetEffects = m64_read_u8(state);
                        break;

                    case 0xd1: // chan_setnoteallocationpolicy
                        seqChannel->noteAllocPolicy = m64_read_u8(state);
                        break;

                    case 0xd2: // chan_setsustain
                        seqChannel->adsr.sustain = m64_read_u8(state) << 8;
                        break;
                    case 0xe4: // chan_dyncall
                        if (value != -1) {
                            u8(*thingy)[2] = *seqChannel->dynTable;
                            state->depth++, state->stack[state->depth - 1] = state->pc;
                            sp5A = thingy[value][1] + (thingy[value][0] << 8);
                            state->pc = seqPlayer->seqData + sp5A;
                        }
                        break;
                }
            } else {
                loBits = cmd & 0xf;
                switch (cmd & 0xf0) {
                    case 0x00: // chan_testlayerfinished
                        if (seqChannel->layers[loBits] != NULL) {
                            value = seqChannel->layers[loBits]->finished;
                        }
                        break;

                    case 0x70: // chan_iowriteval; write data back to audio lib
                        seqChannel->soundScriptIO[loBits] = value;
                        break;

                    case 0x80: // chan_ioreadval; read data from audio lib
                        value = seqChannel->soundScriptIO[loBits];
                        if (loBits < 4) {
                            seqChannel->soundScriptIO[loBits] = -1;
                        }
                        break;

                    case 0x50: // chan_ioreadvalsub; subtract with read data from audio lib
                        value -= seqChannel->soundScriptIO[loBits];
                        break;
                    case 0x90: // chan_setlayer
                        sp5A = m64_read_s16(state);
                        if (seq_channel_set_layer(seqChannel, loBits) == 0) {
                            seqChannel->layers[loBits]->scriptState.pc = seqPlayer->seqData + sp5A;
                        }
                        break;

                    case 0xa0: // chan_freelayer
                        seq_channel_layer_free(seqChannel, loBits);
                        break;

                    case 0xb0: // chan_dynsetlayer
                        if (value != -1 && seq_channel_set_layer(seqChannel, loBits) != -1) {
                            sp5A = ((*seqChannel->dynTable)[value][0] << 8)
                                   + (*seqChannel->dynTable)[value][1];
                            seqChannel->layers[loBits]->scriptState.pc = seqPlayer->seqData + sp5A;
                        }
                        break;

                    case 0x60: // chan_setnotepriority (arg must be >= 2)
                        seqChannel->notePriority = loBits;
                        break;
                    case 0x10: // chan_startchannel
                        sp5A = m64_read_s16(state);
                        sequence_channel_enable(seqPlayer, loBits, seqPlayer->seqData + sp5A);
                        break;

                    case 0x20: // chan_disablechannel
                        sequence_channel_disable(seqPlayer->channels[loBits]);
                        break;

                    case 0x30: // chan_iowriteval2; write data back to audio lib for another channel
                        seqPlayer->channels[loBits]->soundScriptIO[m64_read_u8(state)] = value;
                        break;

                    case 0x40: // chan_ioreadval2; read data from audio lib from another channel
                        value = seqPlayer->channels[loBits]->soundScriptIO[m64_read_u8(state)];
                        break;
                }
            }
        }
    }
processLayers:
    for (loBits = 0; loBits < LAYERS_MAX; loBits++) {
        if (seqChannel->layers[loBits] != 0) {
            seq_channel_layer_process_script(seqChannel->layers[loBits]);
        }
    }
#undef loBits
}

ALWAYS_INLINE inline struct SequenceChannel *allocate_sequence_channel(void) {
    s32 i;
    for (i = 0; i < ARRAY_COUNT(gSequenceChannels); i++) {
        if (gSequenceChannels[i].seqPlayer == NULL) {
            return gSequenceChannels + i;
        }
    }
    return 0;
}

ALWAYS_INLINE void init_note_listsSYNTEHSIS(struct NotePool *pool) {
    s32 i;
    for (i=0; i < 4; i++){
    init_note_listSYNTHESIS(&pool->poolEntry[i]);
    pool->poolEntry[i].pool = pool;
    }
 /*   init_note_listSYNTHESIS(&pool->poolDisabled);
    init_note_listSYNTHESIS(&pool->poolDecaying);
    init_note_listSYNTHESIS(&pool->poolReleasing);
    init_note_listSYNTHESIS(&pool->poolActive);
    pool->poolDisabled.pool = pool;
    pool->poolDecaying.pool = pool;
    pool->poolReleasing.pool = pool;
    pool->poolActive.pool = pool;*/
}
ALWAYS_INLINE void sequence_channel_init(struct SequenceChannel *seqChannel) {
    s32 i;

    seqChannel->enabled = FALSE;
    seqChannel->finished = FALSE;
    seqChannel->stopScript = FALSE;
    seqChannel->hasInstrument = FALSE;
    // seqChannel->stereoHeadsetEffects = FALSE;
    seqChannel->transposition = 0;
    seqChannel->largeNotes = FALSE;
    seqChannel->scriptState.depth = 0;
    seqChannel->volume = 1.0f;
    seqChannel->volumeScale = 1.0f;
    seqChannel->freqScale = 1.0f;
    seqChannel->pan = 0.5f;
    seqChannel->panChannelWeight = 1.0f;
    seqChannel->reverb = 0;
    seqChannel->notePriority = NOTE_PRIORITY_DEFAULT;
    seqChannel->delay = 0;
    seqChannel->adsr.envelope = (struct AdsrEnvelope *) gDefaultEnvelope;
    seqChannel->adsr.releaseRate = 0x20;
    seqChannel->adsr.sustain = 0;
    seqChannel->vibratoRateTarget = 0x800;
    seqChannel->vibratoRateStart = 0x800;
    seqChannel->vibratoExtentTarget = 0;
    seqChannel->vibratoExtentStart = 0;
    seqChannel->vibratoRateChangeDelay = 0;
    seqChannel->vibratoExtentChangeDelay = 0;
    seqChannel->vibratoDelay = 0;

    for (i = 0; i < 8; i++) {
        seqChannel->soundScriptIO[i] = -1;
    }

    init_note_listsSYNTEHSIS(&seqChannel->notePool);
}
ALWAYS_INLINE void sequence_player_init_channels(struct SequencePlayer *seqPlayer, u16 channelBits) {
    struct SequenceChannel *seqChannel;
    s32 i;

    for (i = 0; i < CHANNELS_MAX; i++) {
        if (channelBits & 1) {
            seqChannel = seqPlayer->channels[i];
            if (IS_SEQUENCE_CHANNEL_VALID(seqChannel) && seqChannel->seqPlayer == seqPlayer) {
                sequence_channel_disable(seqChannel);
                seqChannel->seqPlayer = NULL;
            }
            seqChannel = allocate_sequence_channel();
            if (IS_SEQUENCE_CHANNEL_VALID(seqChannel) == FALSE) {
                seqPlayer->channels[i] = seqChannel;
            } else {
                sequence_channel_init(seqChannel);
                seqPlayer->channels[i] = seqChannel;
                seqChannel->seqPlayer = seqPlayer;
                seqChannel->bankId = seqPlayer->defaultBank[0];
                seqChannel->muteBehavior = seqPlayer->muteBehavior;
                seqChannel->noteAllocPolicy = seqPlayer->noteAllocPolicy;
            }
        }
        channelBits >>= 1;
    }
}
// on US/JP this inlines patch_sound, using some -sopt compiler flag
void sequence_player_disable_channels(struct SequencePlayer *seqPlayer, u16 channelBits) {
    struct SequenceChannel *seqChannel;
    s32 i = 0;

    while (channelBits) {
        if (channelBits & 1) {
            seqChannel = seqPlayer->channels[i];
            if (IS_SEQUENCE_CHANNEL_VALID(seqChannel)) {
                if (seqChannel->seqPlayer == seqPlayer) {
                    sequence_channel_disable(seqChannel);
                    seqChannel->seqPlayer = NULL;
                }
                seqPlayer->channels[i] = 0;
            }
        }
        channelBits >>= 1;
        i++;
    }
}
void sequence_player_disable(struct SequencePlayer *seqPlayer) {
    sequence_player_disable_channels(seqPlayer, 0xffff);
    // note_pool_clear(&seqPlayer->notePool);//question: why both seqplayer and seqchannel notepools?
    seqPlayer->enabled = FALSE;

    if (IS_SEQ_LOAD_COMPLETE(seqPlayer->seqId)) {
        gSeqLoadStatus[seqPlayer->seqId] = SOUND_LOAD_STATUS_DISCARDABLE;
    }

    if (IS_BANK_LOAD_COMPLETE(seqPlayer->defaultBank[0])) {
        gBankLoadStatus[seqPlayer->defaultBank[0]] = SOUND_LOAD_STATUS_DISCARDABLE;
    }

    // (Note that if this is called from alloc_bank_or_seq, the side will get swapped
    // later in that function. Thus, we signal that we want to load into the slot
    // of the bank that we no longer need.)
    if (gBankLoadedPool.temporary.entries[0].id == seqPlayer->defaultBank[0]) {
        gBankLoadedPool.temporary.nextSide = 1;
    } else if (gBankLoadedPool.temporary.entries[1].id == seqPlayer->defaultBank[0]) {
        gBankLoadedPool.temporary.nextSide = 0;
    }
}
extern void patch_audio_bank(struct AudioBank *mem, u8 *offset, u32 numInstruments, u32 numDrums);
ALWAYS_INLINE void sequence_player_process_sequence(struct SequencePlayer *seqPlayer) {
    u8 cmd;
    u8 loBits;
    u8 temp;
    s32 value;
    s32 i;
    u16 u16v;
    u8 *tempPtr;
    struct M64ScriptState *state;

    if (seqPlayer->enabled == FALSE) {
        return;
    }

    if (seqPlayer->bankDmaInProgress) {
        if (seqPlayer->bankDmaMesg == NULL) {
            return;
        }
        if (seqPlayer->bankDmaRemaining == 0) {
            seqPlayer->bankDmaInProgress = FALSE;
            patch_audio_bank(seqPlayer->loadingBank, gAlTbl->seqArray[seqPlayer->loadingBankId].offset,
                             seqPlayer->loadingBankNumInstruments, seqPlayer->loadingBankNumDrums);
            gCtlEntries[seqPlayer->loadingBankId].numInstruments = seqPlayer->loadingBankNumInstruments;
            gCtlEntries[seqPlayer->loadingBankId].numDrums = seqPlayer->loadingBankNumDrums;
            gCtlEntries[seqPlayer->loadingBankId].instruments = seqPlayer->loadingBank->instruments;
            gCtlEntries[seqPlayer->loadingBankId].drums = seqPlayer->loadingBank->drums;
            gBankLoadStatus[seqPlayer->loadingBankId] = SOUND_LOAD_STATUS_COMPLETE;
        } else {
            osCreateMesgQueue(&seqPlayer->bankDmaMesgQueue, &seqPlayer->bankDmaMesg, 1);
            seqPlayer->bankDmaMesg = NULL;
            audio_dma_partial_copy_asyncSYNTHESIS(&seqPlayer->bankDmaCurrDevAddr, &seqPlayer->bankDmaCurrMemAddr,
                                         &seqPlayer->bankDmaRemaining, &seqPlayer->bankDmaMesgQueue,
                                         &seqPlayer->bankDmaIoMesg);
        }
        return;
    }

    if (seqPlayer->seqDmaInProgress) {
        if (seqPlayer->seqDmaMesg == NULL) {
            return;
        }
        seqPlayer->seqDmaInProgress = FALSE;
        gSeqLoadStatus[seqPlayer->seqId] = SOUND_LOAD_STATUS_COMPLETE;
    }

    // If discarded, bail out.
    if (IS_SEQ_LOAD_COMPLETE(seqPlayer->seqId) == FALSE
        || IS_BANK_LOAD_COMPLETE(seqPlayer->defaultBank[0]) == FALSE) {
        sequence_player_disable(seqPlayer);
        return;
    }

    // Remove possible SOUND_LOAD_STATUS_DISCARDABLE marks.
    gSeqLoadStatus[seqPlayer->seqId] = SOUND_LOAD_STATUS_COMPLETE;
    gBankLoadStatus[seqPlayer->defaultBank[0]] = SOUND_LOAD_STATUS_COMPLETE;

    if (seqPlayer->muted && (seqPlayer->muteBehavior & MUTE_BEHAVIOR_STOP_SCRIPT) != 0) {
        return;
    }

    // Check if we surpass the number of ticks needed for a tatum, else stop.
    seqPlayer->tempoAcc += seqPlayer->tempo;
    if (seqPlayer->tempoAcc < gTempoInternalToExternal) {
        return;
    }
    seqPlayer->tempoAcc -= (u16) gTempoInternalToExternal;

    state = &seqPlayer->scriptState;
    if (seqPlayer->delay > 1) {
        seqPlayer->delay--;
    } else {
        for (;;) {
            cmd = m64_read_u8(state);

            if (cmd >= 0xc0) {
                switch (cmd) {
                    case 0xff: // seq_end
                        if (state->depth == 0) {
                            sequence_player_disable(seqPlayer);
                            goto endtheread;
                        }
                        state->depth--, state->pc = state->stack[state->depth];
                    case 0xfd: // seq_delay
                        seqPlayer->delay = m64_read_compressed_u16(state);
                        goto endtheread;
                    case 0xfe: // seq_delay1
                        seqPlayer->delay = 1;
                        goto endtheread;

                    case 0xfc: // seq_call
                        u16v = m64_read_s16(state);
                        state->depth++, state->stack[state->depth - 1] = state->pc;
                        state->pc = seqPlayer->seqData + u16v;
                        break;

                    case 0xf8: // seq_loop; loop start, N iterations (or 256 if N = 0)
                        state->remLoopIters[state->depth] = m64_read_u8(state);
                        state->depth++, state->stack[state->depth - 1] = state->pc;
                        break;

                    case 0xf7: // seq_loopend
                        state->remLoopIters[state->depth - 1]--;
                        if (state->remLoopIters[state->depth - 1] != 0) {
                            state->pc = state->stack[state->depth - 1];
                        } else {
                            state->depth--;
                        }
                        break;

                    case 0xfb: // seq_jump
                    case 0xfa: // seq_beqz; jump if == 0
                    case 0xf9: // seq_bltz; jump if < 0
                    case 0xf5: // seq_bgez; jump if >= 0
                        u16v = m64_read_s16(state);
                        if (cmd == 0xfa && value != 0) {
                            break;
                        }
                        if (cmd == 0xf9 && value >= 0) {
                            break;
                        }
                        if (cmd == 0xf5 && value < 0) {
                            break;
                        }
                        state->pc = seqPlayer->seqData + u16v;
                        break;
                        /* case 0xf2: // seq_reservenotes
                             note_pool_clear(&seqPlayer->notePool);
                             note_pool_fill(&seqPlayer->notePool, m64_read_u8(state));
                             break;

                         case 0xf1: // seq_unreservenotes
                             note_pool_clear(&seqPlayer->notePool);
                             break;
     */
                    case 0xdf: // seq_transpose; set transposition in semitones
                        seqPlayer->transposition = 0;
                    case 0xde: // seq_transposerel; add transposition
                        seqPlayer->transposition += (s8) m64_read_u8(state);
                        break;

                    case 0xdd: // seq_settempo (bpm)
                    case 0xdc: // seq_addtempo (bpm)
                        temp = m64_read_u8(state);
                        if (cmd == 0xdd) {
                            seqPlayer->tempo = temp * TEMPO_SCALE;
                        } else {
                            seqPlayer->tempo += (s8) temp * TEMPO_SCALE;
                        }

                        if (seqPlayer->tempo > gTempoInternalToExternal) {
                            seqPlayer->tempo = gTempoInternalToExternal;
                        }

                        if ((s16) seqPlayer->tempo <= 0) {
                            seqPlayer->tempo = 1;
                        }
                        break;

                    case 0xdb: // seq_setvol
                        temp = m64_read_u8(state);
                        seqPlayer->volumeDefault = FLOAT_CAST(temp) / 127.0f;
                        if (seqPlayer->volumeDefault >= 1.0f) {
                            seqPlayer->volumeDefault = 1.0f;
                        }
                        switch (seqPlayer->state) {
                            case SEQUENCE_PLAYER_STATE_2:
                                if (seqPlayer->fadeTimer != 0) {
                                    f32 targetVolume = FLOAT_CAST(temp) / US_FLOAT(127.0);
                                    seqPlayer->fadeVelocity = (targetVolume - seqPlayer->fadeVolume)
                                                              / FLOAT_CAST(seqPlayer->fadeTimer);
                                    break;
                                }
                                // fallthrough
                            case SEQUENCE_PLAYER_STATE_0:
                                seqPlayer->fadeVolume = FLOAT_CAST(temp) / US_FLOAT(127.0);
                                break;
                            case SEQUENCE_PLAYER_STATE_FADE_OUT:
                            case SEQUENCE_PLAYER_STATE_FADE_IN:
                                seqPlayer->volume = FLOAT_CAST(temp) / US_FLOAT(127.0);
                                break;
                        }
                        break;

                    case 0xda: // seq_changevol
                        temp = m64_read_u8(state);
                        seqPlayer->fadeVolume =
                            seqPlayer->fadeVolume + (f32) (s8) temp / US_FLOAT(127.0);
                        break;

                    case 0xd7: // seq_initchannels
                        u16v = m64_read_s16(state);
                        sequence_player_init_channels(seqPlayer, u16v);
                        break;

                    case 0xd6: // seq_disablechannels
                        u16v = m64_read_s16(state);
                        sequence_player_disable_channels(seqPlayer, u16v);
                        break;

                    case 0xd5: // seq_setmutescale
                        temp = m64_read_u8(state);
                        seqPlayer->muteVolumeScale = (f32) (s8) temp / US_FLOAT(127.0);
                        break;

                    case 0xd4: // seq_mute
                        seqPlayer->muted = TRUE;
                        break;

                    case 0xd3: // seq_setmutebhv
                        seqPlayer->muteBehavior = m64_read_u8(state);
                        break;

                    case 0xd2: // seq_setshortnotevelocitytable
                    case 0xd1: // seq_setshortnotedurationtable
                        u16v = m64_read_s16(state);
                        tempPtr = seqPlayer->seqData + u16v;
                        if (cmd == 0xd2) {
                            seqPlayer->shortNoteVelocityTable = tempPtr;
                        } else {
                            seqPlayer->shortNoteDurationTable = tempPtr;
                        }
                        break;

                    case 0xd0: // seq_setnoteallocationpolicy
                               // seqPlayer->noteAllocPolicy = m64_read_u8(state);
                        break;

                    case 0xcc: // seq_setval
                        value = m64_read_u8(state);
                        break;

                    case 0xc9: // seq_bitand
                        value = m64_read_u8(state) & value;
                        break;

                    case 0xc8: // seq_subtract
                        value = value - m64_read_u8(state);
                        break;
                }
            } else {
                loBits = cmd & 0xf;
                switch (cmd & 0xf0) {
                    case 0x00: // seq_testchdisabled
                        if (IS_SEQUENCE_CHANNEL_VALID(seqPlayer->channels[loBits])) {
                            value = seqPlayer->channels[loBits]->finished;
                        }
                        break;
                    case 0x50: // seq_subvariation
                        value -= seqPlayer->seqVariation;
                        break;
                    case 0x70: // seq_setvariation
                        seqPlayer->seqVariation = value;
                        break;
                    case 0x80: // seq_getvariation
                        value = seqPlayer->seqVariation;
                        break;
                    case 0x90: // seq_startchannel
                        u16v = m64_read_s16(state);
                        sequence_channel_enable(seqPlayer, loBits, seqPlayer->seqData + u16v);
                        break;
                }
            }
        }
    endtheread:
    }
    for (i = 0; i < CHANNELS_MAX; i++) {
        if (seqPlayer->channels[i] != 0) {
            sequence_channel_process_script(seqPlayer->channels[i]);
        }
    }
}
ALWAYS_INLINE void sequence_channel_process_sound(struct SequenceChannel *seqChannel) {
    f32 channelVolume;
    f32 panLayerWeight;
    f32 panFromChannel;
    s32 i;

    channelVolume = seqChannel->volume * seqChannel->volumeScale * seqChannel->seqPlayer->fadeVolume;
    if (seqChannel->seqPlayer->muted && (seqChannel->muteBehavior & MUTE_BEHAVIOR_SOFTEN)) {
        channelVolume *= seqChannel->seqPlayer->muteVolumeScale;
    }

    panFromChannel = seqChannel->pan * seqChannel->panChannelWeight;
    panLayerWeight = US_FLOAT(1.0) - seqChannel->panChannelWeight;

    for (i = 0; i < 4; i++) {
        struct SequenceChannelLayer *layer = seqChannel->layers[i];
        if (layer != NULL && layer->enabled && layer->note != NULL) {
            layer->noteFreqScale = layer->freqScale * seqChannel->freqScale;
            layer->noteVelocity = layer->velocitySquare * channelVolume;
            layer->notePan = (layer->pan * panLayerWeight) + panFromChannel;
        }
    }
}
ALWAYS_INLINE void sequence_player_process_sound(struct SequencePlayer *seqPlayer) {
    s32 i;

    if (seqPlayer->fadeTimer != 0) {
        seqPlayer->fadeVolume += seqPlayer->fadeVelocity;
        if (seqPlayer->fadeVolume > 1.f) {
            seqPlayer->fadeVolume = 1.f;
        }
        if (seqPlayer->fadeVolume < 0) {
            seqPlayer->fadeVolume = 0;
        }

        if (--seqPlayer->fadeTimer == 0) {
            switch (seqPlayer->state) {
                case SEQUENCE_PLAYER_STATE_FADE_OUT:
                    sequence_player_disable(seqPlayer);
                    return;
                case SEQUENCE_PLAYER_STATE_2:
                case SEQUENCE_PLAYER_STATE_3:
                    seqPlayer->state = SEQUENCE_PLAYER_STATE_0;
                    break;
            }
        }
    }

    // Process channels
    for (i = 0; i < CHANNELS_MAX; i++) {
        if (seqPlayer->channels[i] && seqPlayer->channels[i]->enabled) {
            sequence_channel_process_sound(seqPlayer->channels[i]);
        }
    }
}


ALWAYS_INLINE void prepare_reverb_ring_buffer(s32 chunkLen, u32 updateIndex) {//computer positions and lengths in the ringbuffer to be written to
    struct ReverbRingBufferItem *item;
    s32 srcPos;
    s32 dstPos;
    s32 nSamples;
    s32 numSamplesAfterDownsampling;
    s32 excessiveSamples;
    if (gReverbDownsampleRate != 1) {
        if (gSynthesisReverb.framesLeftToIgnore == 0) {
            // Now that the RSP has finished, downsample the samples produced two frames ago by skipping
            // samples.
            item = &gSynthesisReverb.items[gSynthesisReverb.curFrame][updateIndex];

            // Touches both left and right since they are adjacent in memory
            osInvalDCache(item->toDownsampleLeft, DEFAULT_LEN_2CH);

            for (srcPos = 0, dstPos = 0; dstPos < item->lengths[0] / 2;
                 srcPos += gReverbDownsampleRate, dstPos++) {
                gSynthesisReverb.ringBuffer.left[dstPos + item->startPos] =
                    item->toDownsampleLeft[srcPos];
                gSynthesisReverb.ringBuffer.right[dstPos + item->startPos] =
                    item->toDownsampleRight[srcPos];
            }
            for (dstPos = 0; dstPos < item->lengths[1] / 2; srcPos += gReverbDownsampleRate, dstPos++) {
                gSynthesisReverb.ringBuffer.left[dstPos] = item->toDownsampleLeft[srcPos];
                gSynthesisReverb.ringBuffer.right[dstPos] = item->toDownsampleRight[srcPos];
            }
        }
    }
    item = &gSynthesisReverb.items[gSynthesisReverb.curFrame][updateIndex];

    numSamplesAfterDownsampling = chunkLen / gReverbDownsampleRate;
    if (((numSamplesAfterDownsampling + gSynthesisReverb.nextRingBufferPos)
         - gSynthesisReverb.bufSizePerChannel)
        < 0) {
        // There is space in the ring buffer before it wraps around
        item->lengths[0] = numSamplesAfterDownsampling * 2;
        item->lengths[1] = 0;
        item->startPos = gSynthesisReverb.nextRingBufferPos;
        gSynthesisReverb.nextRingBufferPos += numSamplesAfterDownsampling;
    } else {
        // Ring buffer wrapped around
        excessiveSamples = (numSamplesAfterDownsampling + gSynthesisReverb.nextRingBufferPos)
                           - gSynthesisReverb.bufSizePerChannel;
        nSamples = numSamplesAfterDownsampling - excessiveSamples;
        item->lengths[0] = nSamples * 2;
        item->lengths[1] = excessiveSamples * 2;
        item->startPos = gSynthesisReverb.nextRingBufferPos;
        gSynthesisReverb.nextRingBufferPos = excessiveSamples;
    }
    // These fields are never read later
    /*item->numSamplesAfterDownsampling = numSamplesAfterDownsampling;
    item->chunkLen = chunkLen;*/
}



ALWAYS_INLINE void *dma_sample_data(uintptr_t devAddr, u32 size, s32 arg2, u8 *arg3) {
    s32 hasDma = FALSE;
    struct SharedDma *dma;
    uintptr_t dmaDevAddr;
    u32 transfer;
    u32 i;
    u32 dmaIndex;
    ssize_t bufferPos;

    if (arg2 != 0 || *arg3 >= sSampleDmaListSize1) {
        for (i = sSampleDmaListSize1; i < gSampleDmaNumListItems; i++) {

            dma = sSampleDmas + i;
            bufferPos = devAddr - dma->source;
            if (0 <= bufferPos && (size_t) bufferPos <= dma->bufSize - size) {
                // We already have a DMA request for this memory range.
                if (dma->ttl == 0 && sSampleDmaReuseQueueTail2 != sSampleDmaReuseQueueHead2) {
                    // Move the DMA out of the reuse queue, by swapping it with the
                    // tail, and then incrementing the tail.
                    if (dma->reuseIndex != sSampleDmaReuseQueueTail2) {
                        sSampleDmaReuseQueue2[dma->reuseIndex] =
                            sSampleDmaReuseQueue2[sSampleDmaReuseQueueTail2];
                        sSampleDmas[sSampleDmaReuseQueue2[sSampleDmaReuseQueueTail2]].reuseIndex =
                            dma->reuseIndex;
                    }
                    sSampleDmaReuseQueueTail2++;
                }
                dma->ttl = 60;
                *arg3 = (u8) i;

                return (devAddr - dma->source) + dma->buffer;
            }
        }

        if (sSampleDmaReuseQueueTail2 != sSampleDmaReuseQueueHead2 && arg2 != 0) {
            // Allocate a DMA from reuse queue 2. This queue can be empty, since
            // TTL 60 is pretty large.
            dmaIndex = sSampleDmaReuseQueue2[sSampleDmaReuseQueueTail2];
            sSampleDmaReuseQueueTail2++;
            dma = sSampleDmas + dmaIndex;
            hasDma = TRUE;
        }
    } else {

        dma = sSampleDmas + *arg3;
        bufferPos = devAddr - dma->source;
        if (0 <= bufferPos && (size_t) bufferPos <= dma->bufSize - size) {
            // We already have DMA for this memory range.
            if (dma->ttl == 0) {
                // Move the DMA out of the reuse queue, by swapping it with the
                // tail, and then incrementing the tail.
                if (dma->reuseIndex != sSampleDmaReuseQueueTail1) {

                    sSampleDmaReuseQueue1[dma->reuseIndex] =
                        sSampleDmaReuseQueue1[sSampleDmaReuseQueueTail1];
                    sSampleDmas[sSampleDmaReuseQueue1[sSampleDmaReuseQueueTail1]].reuseIndex =
                        dma->reuseIndex;
                }
                sSampleDmaReuseQueueTail1++;
            }
            dma->ttl = 2;

            return (devAddr - dma->source) + dma->buffer;
        }
    }

    if (!hasDma) {
        // Allocate a DMA from reuse queue 1. This queue will hopefully never
        // be empty, since TTL 2 is so small.
        dmaIndex = sSampleDmaReuseQueue1[sSampleDmaReuseQueueTail1++];
        dma = sSampleDmas + dmaIndex;
        hasDma = TRUE;
    }

    transfer = dma->bufSize;
    dmaDevAddr = devAddr & ~0xF;
    dma->ttl = 2;
    dma->source = dmaDevAddr;
    dma->sizeUnused = transfer;
    osInvalDCache(dma->buffer, transfer);
    gCurrAudioFrameDmaCount++;
    osPiStartDma(&gCurrAudioFrameDmaIoMesgBufs[gCurrAudioFrameDmaCount - 1], OS_MESG_PRI_NORMAL,
                 OS_READ, dmaDevAddr, dma->buffer, transfer, &gCurrAudioFrameDmaQueue);
    *arg3 = dmaIndex;
    return dma->buffer + (devAddr - dmaDevAddr);
}
struct VolumeChange {
    u16 sourceLeft;
    u16 sourceRight;
    u16 targetLeft;
    u16 targetRight;
};
ALWAYS_INLINE u64 *process_envelope_inner(u64 *cmd, struct Note *note, s32 nSamples, u32 inBuf,
                                                 struct VolumeChange *vol) {
    s32 mixerFlags;
    s32 rampLeft, rampRight;

    // For aEnvMixer, five buffers and count are set using aSetBuffer.
    // in, dry left, count without A_AUX flag.
    // dry right, wet left, wet right with A_AUX flag.

    // It's a bit unclear what the "stereo strong" concept does.
    // Instead of mixing the opposite channel to the normal buffers, the sound is first
    // mixed into a temporary buffer and then subtracted from the normal buffer.
 /*   if (note->stereoStrongRight) {
        aClearBuffer(cmd++, DMEM_ADDR_STEREO_STRONG_TEMP_DRY, DEFAULT_LEN_2CH);
        aSetBuffer(cmd++, 0, inBuf, DMEM_ADDR_STEREO_STRONG_TEMP_DRY, nSamples * 2);
        aSetBuffer(cmd++, A_AUX, DMEM_ADDR_RIGHT_CH, DMEM_ADDR_STEREO_STRONG_TEMP_WET,
                   DMEM_ADDR_WET_RIGHT_CH);
    } else if (note->stereoStrongLeft) {
        aClearBuffer(cmd++, DMEM_ADDR_STEREO_STRONG_TEMP_DRY, DEFAULT_LEN_2CH);
        aSetBuffer(cmd++, 0, inBuf, DMEM_ADDR_LEFT_CH, nSamples * 2);
        aSetBuffer(cmd++, A_AUX, DMEM_ADDR_STEREO_STRONG_TEMP_DRY, DMEM_ADDR_WET_LEFT_CH,
                   DMEM_ADDR_STEREO_STRONG_TEMP_WET);
    } else */{
        aSetBuffer(cmd++, 0, inBuf, DMEM_ADDR_LEFT_CH, nSamples * 2);
        aSetBuffer(cmd++, A_AUX, DMEM_ADDR_RIGHT_CH, DMEM_ADDR_WET_LEFT_CH, DMEM_ADDR_WET_RIGHT_CH);
    }

    if (vol->targetLeft == vol->sourceLeft && vol->targetRight == vol->sourceRight
        && !note->envMixerNeedsInit) {
        mixerFlags = A_CONTINUE;
    } else {
        mixerFlags = A_INIT;

        rampLeft = get_volume_ramping(vol->sourceLeft, vol->targetLeft, nSamples);
        rampRight = get_volume_ramping(vol->sourceRight, vol->targetRight, nSamples);

        // The operation's parameters change meanings depending on flags

        aSetVolume(cmd++, A_VOL | A_LEFT, vol->sourceLeft, 0, 0);
        aSetVolume(cmd++, A_VOL | A_RIGHT, vol->sourceRight, 0, 0);
        aSetVolume32(cmd++, A_RATE | A_LEFT, vol->targetLeft, rampLeft);
        aSetVolume32(cmd++, A_RATE | A_RIGHT, vol->targetRight, rampRight);
        aSetVolume(cmd++, A_AUX, gVolume, 0, note->reverbVol);
    }

    if (/*gSynthesisReverb.useReverb && */ note->reverb) {
        aEnvMixer(cmd++, mixerFlags | A_AUX,
                  VIRTUAL_TO_PHYSICAL2(note->synthesisBuffers->mixEnvelopeState));
       /* if (note->stereoStrongRight) {
            aSetBuffer(cmd++, 0, 0, 0, nSamples * 2);
            // 0x8000 is -100%, so subtract sound instead of adding...
            aMix(cmd++, 0,  0x8000,  DMEM_ADDR_STEREO_STRONG_TEMP_DRY,
                  DMEM_ADDR_LEFT_CH);
            aMix(cmd++, 0,  0x8000,  DMEM_ADDR_STEREO_STRONG_TEMP_WET,
                  DMEM_ADDR_WET_LEFT_CH);
        } else if (note->stereoStrongLeft) {
            aSetBuffer(cmd++, 0, 0, 0, nSamples * 2);
            aMix(cmd++, 0,  0x8000,  DMEM_ADDR_STEREO_STRONG_TEMP_DRY,
                  DMEM_ADDR_RIGHT_CH);
            aMix(cmd++, 0,  0x8000,  DMEM_ADDR_STEREO_STRONG_TEMP_WET,
                  DMEM_ADDR_WET_RIGHT_CH);
        }*/
    } else {

        aEnvMixer(cmd++, mixerFlags, VIRTUAL_TO_PHYSICAL2(note->synthesisBuffers->mixEnvelopeState));
      /*  if (note->stereoStrongRight) {
            aSetBuffer(cmd++, 0, 0, 0, nSamples * 2);
            aMix(cmd++, 0,  0x8000,  DMEM_ADDR_STEREO_STRONG_TEMP_DRY,
                  DMEM_ADDR_LEFT_CH);
        } else if (note->stereoStrongLeft) {
            aSetBuffer(cmd++, 0, 0, 0, nSamples * 2);
            aMix(cmd++, 0,  0x8000,  DMEM_ADDR_STEREO_STRONG_TEMP_DRY,
                  DMEM_ADDR_RIGHT_CH);
        }*/
    }
    return cmd;
}
ALWAYS_INLINE u64 *process_envelope(u64 *cmd, struct Note *note, s32 nSamples, s32 inBuf) {
    struct VolumeChange vol;
    vol.sourceLeft = note->curVolLeft;
    vol.sourceRight = note->curVolRight;
    vol.targetLeft = note->targetVolLeft;
    vol.targetRight = note->targetVolRight;
    note->curVolLeft = vol.targetLeft;
    note->curVolRight = vol.targetRight;
    return process_envelope_inner(cmd, note, nSamples, inBuf, &vol);
}
ALWAYS_INLINE u64 *synthesis_process_notes(s16 *aiBuf, s32 bufLen, u64 *cmd) {
    s32 noteIndex;                           // sp174
    struct Note *note;                       // s7
    struct AudioBankSample *audioBookSample; // sp164, sp138
    struct AdpcmLoop *loopInfo;              // sp160, sp134
    s16 *curLoadedBook = NULL;               // sp154, sp130
    s32 resamplingRateFixedPoint;            // sp5c, sp11A
    s32 noteFinished;                        // 150 t2, sp124
    s32 restart;                             // 14c t3, sp120
    s32 flags;                               // sp148, sp11C

    s32 sp130;                  // sp128, sp104
    s32 nAdpcmSamplesProcessed; // signed required for US
    s32 t0;

    s32 s6;
    u8 *sampleAddr; // sp120, spF4

    // sp6c is a temporary!

    // Might have been used to store (samplesLenFixedPoint >> 0x10), but doing so causes strange
    // behavior with the break near the end of the loop, causing US and JP to need a goto instead
    s32 samplesLenAdjusted; // 108
    s32 s2;
    s32 endPos;            // sp110,    spE4
    s32 nSamplesToProcess; // sp10c/a0, spE0

    s32 s3;
    s32 s5; // s4

    u32 samplesLenFixedPoint;    // v1_1
    s32 nSamplesInThisIteration; // v1_2
    u32 a3;
    u8 *v0_2;
    s32 nParts;  // spE8, spBC
    s32 curPart; // spE4, spB8

    s32 temp;
    s32 resampledTempLen;                    // spD8, spAC
    s32 noteSamplesDmemAddrBeforeResampling; // spD6, spAA



    
    f32 multiplicator;

    for (noteIndex = 0; noteIndex < gMaxSimultaneousNotes; noteIndex++) {
        note = &gNotes[noteIndex];
        //! This function requires note->enabled to be volatile, but it breaks other functions like
        //! note_enable. Casting to a struct with just the volatile bitfield works, but there may be a
        //! better way to match.
        if (note->enabled && IS_BANK_LOAD_COMPLETE(note->bankId) != FALSE) {
            flags = 0;

            if (note->needsInit) {
                flags = A_INIT;
                note->samplePosInt = 0;
                note->samplePosFrac = 0;
            }

            if (note->frequency <= 1.99996f) {
                nParts = 1;
                //WORRY: maybe this is necessary
                /*if (note->frequency > 1.99996f) {
                    note->frequency = 1.99996f;
                }*/
                //resamplingRate = note->frequency;
                multiplicator = 32768.f;
            } else {
                // If frequency is > 2.0, the processing must be split into two parts
                nParts = 2;
                if (note->frequency >= US_FLOAT(3.99993)) {
                    note->frequency = US_FLOAT(3.99993);
                }
               // resamplingRate = note->frequency * .5f;
                multiplicator = 32768.f*0.5f;
            }

            resamplingRateFixedPoint = note->frequency * multiplicator;
            samplesLenFixedPoint = note->samplePosFrac + (resamplingRateFixedPoint * bufLen) * 2;
            note->samplePosFrac = samplesLenFixedPoint & 0xFFFF; // 16-bit store, can't reuse

           /* if (note->sound == NULL) {//worry might break something. fuck wave synthesis notes.
                // A wave synthesis note (not ADPCM)

                cmd = load_wave_samples(cmd, note, samplesLenFixedPoint >> 0x10);
                noteSamplesDmemAddrBeforeResampling =
                    DMEM_ADDR_UNCOMPRESSED_NOTE + note->samplePosInt * 2;
                note->samplePosInt += (samplesLenFixedPoint >> 0x10);
                flags = 0;
            } else {*/
                // ADPCM note

                audioBookSample = note->sound->sample;

                loopInfo = audioBookSample->loop;
                endPos = loopInfo->end;
                sampleAddr = audioBookSample->sampleAddr;
                resampledTempLen = 0;
                for (curPart = 0; curPart < nParts; curPart++) {
                    nAdpcmSamplesProcessed = 0; // s8
                    s5 = 0;                     // s4

                    if ((nParts != 1) && (samplesLenFixedPoint >> 0x10) & 1) {
                        samplesLenAdjusted = ((samplesLenFixedPoint >> 0x10) & ~1) + (curPart * 2);
                    } else {
                        samplesLenAdjusted = (samplesLenFixedPoint >> 0x10);
                    }

                    if (curLoadedBook != audioBookSample->book->book) {
                        curLoadedBook = audioBookSample->book->book;
                        aLoadADPCM(cmd++, audioBookSample->book->order * audioBookSample->book->npredictors * 16, VIRTUAL_TO_PHYSICAL2(curLoadedBook));
                    }

                    while (nAdpcmSamplesProcessed != samplesLenAdjusted) {
                        s32 samplesRemaining; // v1
                        s32 s0;

                        noteFinished = FALSE;
                        restart = FALSE;
                        nSamplesToProcess = samplesLenAdjusted - nAdpcmSamplesProcessed;

                        s2 = note->samplePosInt & 0xf;
                        samplesRemaining = endPos - note->samplePosInt;

                        if (s2 == 0 && note->restart == FALSE) {
                            s2 = 16;
                        }
                        s6 = 16 - s2; // a1

                        if (nSamplesToProcess < samplesRemaining) {
                            t0 = (nSamplesToProcess - s6 + 0xf) / 16;
                            s0 = t0 * 16;
                            s3 = s6 + s0 - nSamplesToProcess;
                        } else {
                            s0 = samplesRemaining + s2 - 0x10;

                            s3 = 0;
                            if (s0 <= 0) {
                                s0 = 0;
                                s6 = samplesRemaining;
                            }
                            t0 = (s0 + 0xf) / 16;
                            if (loopInfo->count != 0) {
                                // Loop around and restart
                                restart = 1;
                            } else {
                                noteFinished = 1;
                            }
                        }

                        if (t0 != 0) {

                            temp = (note->samplePosInt - s2 + 0x10) / 16;
                            v0_2 = dma_sample_data((uintptr_t) (sampleAddr + temp * 9), t0 * 9, flags,
                                                   &note->sampleDmaIndex);
                            a3 = (u32) ((uintptr_t) v0_2 & 0xf);
                            aSetBuffer(cmd++, 0, DMEM_ADDR_COMPRESSED_ADPCM_DATA, 0, t0 * 9 + a3);
                            aLoadBuffer(cmd++, VIRTUAL_TO_PHYSICAL2(v0_2 - a3));
                        } else {
                            s0 = 0;
                            a3 = 0;
                        }

                        if (note->restart != FALSE) {
                            aSetLoop(cmd++, VIRTUAL_TO_PHYSICAL2(audioBookSample->loop->state));
                            flags = A_LOOP; // = 2
                            note->restart = FALSE;
                        }

                        nSamplesInThisIteration = s0 + s6 - s3;

                        if (nAdpcmSamplesProcessed == 0) {
                            aSetBuffer(cmd++, 0, DMEM_ADDR_COMPRESSED_ADPCM_DATA + a3,
                                       DMEM_ADDR_UNCOMPRESSED_NOTE, s0 * 2);
                            aADPCMdec(cmd++, flags,
                                      VIRTUAL_TO_PHYSICAL2(note->synthesisBuffers->adpcmdecState));
                            sp130 = s2 * 2;
                        } else {
                            aSetBuffer(cmd++, 0, DMEM_ADDR_COMPRESSED_ADPCM_DATA + a3,
                                       DMEM_ADDR_UNCOMPRESSED_NOTE + ALIGN(s5, 5), s0 * 2);
                            aADPCMdec(cmd++, flags,
                                      VIRTUAL_TO_PHYSICAL2(note->synthesisBuffers->adpcmdecState));
                            aDMEMMove(cmd++, DMEM_ADDR_UNCOMPRESSED_NOTE + ALIGN(s5, 5) + (s2 * 2),
                                      DMEM_ADDR_UNCOMPRESSED_NOTE + s5, (nSamplesInThisIteration) *2);
                        }

                        nAdpcmSamplesProcessed += nSamplesInThisIteration;

                        switch (flags) {
                            case A_INIT: // = 1
                                sp130 = 0;
                                s5 = s0 * 2 + s5;
                                break;

                            case A_LOOP: // = 2
                                s5 = nSamplesInThisIteration * 2 + s5;
                                break;

                            default:
                                if (s5 != 0) {
                                    s5 = nSamplesInThisIteration * 2 + s5;
                                } else {
                                    s5 = (s2 + nSamplesInThisIteration) * 2;
                                }
                                break;
                        }
                        flags = 0;

                        if (noteFinished) {
                            aClearBuffer(cmd++, DMEM_ADDR_UNCOMPRESSED_NOTE + s5,
                                         (samplesLenAdjusted - nAdpcmSamplesProcessed) * 2);

                            note->samplePosInt = 0;
                            note->finished = 1;
                            note->enabled = 0;
                            break;
                        }

                        if (restart) {
                            note->restart = TRUE;
                            note->samplePosInt = loopInfo->start;
                        } else {
                            note->samplePosInt += nSamplesToProcess;
                        }
                    }

                    switch (nParts) {
                        case 1:
                            noteSamplesDmemAddrBeforeResampling = DMEM_ADDR_UNCOMPRESSED_NOTE + sp130;
                            break;

                        case 2:
                            switch (curPart) {
                                case 0:
                                    aSetBuffer(cmd++, 0, DMEM_ADDR_UNCOMPRESSED_NOTE + sp130,
                                               DMEM_ADDR_ADPCM_RESAMPLED, samplesLenAdjusted + 4);

                                    aResample(cmd++, A_INIT, 0xff60,
                                              VIRTUAL_TO_PHYSICAL2(
                                                  note->synthesisBuffers->dummyResampleState));
                                    resampledTempLen = samplesLenAdjusted + 4;
                                    noteSamplesDmemAddrBeforeResampling = DMEM_ADDR_ADPCM_RESAMPLED + 4;

                                    if (note->finished != FALSE) {
                                        aClearBuffer(cmd++,
                                                     DMEM_ADDR_ADPCM_RESAMPLED + resampledTempLen,
                                                     samplesLenAdjusted + 0x10);
                                    }
                                    break;

                                case 1:
                                    aSetBuffer(cmd++, 0, DMEM_ADDR_UNCOMPRESSED_NOTE + sp130,
                                               DMEM_ADDR_ADPCM_RESAMPLED2, samplesLenAdjusted + 8);

                                    aResample(cmd++, A_INIT, 0xff60,
                                              VIRTUAL_TO_PHYSICAL2(
                                                  note->synthesisBuffers->dummyResampleState));
                                    aDMEMMove(cmd++, DMEM_ADDR_ADPCM_RESAMPLED2 + 4,
                                              DMEM_ADDR_ADPCM_RESAMPLED + resampledTempLen,
                                              samplesLenAdjusted + 4);
                                    break;
                            }
                    }

                    if (note->finished) {
                        break;
                    }
                }
         //   }

            flags = 0;

            if (note->needsInit) {
                flags = A_INIT;
            }
            note->needsInit = FALSE;

            cmd = final_resample(cmd, note, bufLen * 2, resamplingRateFixedPoint,
                                 noteSamplesDmemAddrBeforeResampling, flags);

            cmd = process_envelope(cmd, note, bufLen, 0);
        }
    }

    aSetBuffer(cmd++, 0, 0, DMEM_ADDR_TEMP, bufLen * 2);
    aInterleave(cmd++, DMEM_ADDR_LEFT_CH, DMEM_ADDR_RIGHT_CH);
    aSetBuffer(cmd++, 0, 0, DMEM_ADDR_TEMP, bufLen * 4);
    aSaveBuffer(cmd++, VIRTUAL_TO_PHYSICAL2(aiBuf));

    return cmd;
}


ALWAYS_INLINE void decrease_sample_dma_ttls() {
    u32 i;

    for (i = 0; i < sSampleDmaListSize1; i++) {
        struct SharedDma *temp = &sSampleDmas[i];
        if (temp->ttl != 0) {
            temp->ttl--;
            if (temp->ttl == 0) {
                temp->reuseIndex = sSampleDmaReuseQueueHead1;
                sSampleDmaReuseQueue1[sSampleDmaReuseQueueHead1++] = (u8) i;
            }
        }
    }

    for (i = sSampleDmaListSize1; i < gSampleDmaNumListItems; i++) {
        struct SharedDma *temp = &sSampleDmas[i];
        if (temp->ttl != 0) {
            temp->ttl--;
            if (temp->ttl == 0) {
                temp->reuseIndex = sSampleDmaReuseQueueHead2;
                sSampleDmaReuseQueue2[sSampleDmaReuseQueueHead2++] = (u8) i;
            }
        }
    }
}

ALWAYS_INLINE u64 *synthesis_do_one_audio_update(s16 *aiBuf, s32 bufLen, u64 *cmd, u32 updateIndex) {
    s32 ra;
    s32 t4;
    struct ReverbRingBufferItem *v1;
    s32 temp;

    v1 = &gSynthesisReverb.items[gSynthesisReverb.curFrame][updateIndex];

    /* if (gSynthesisReverb.useReverb == 0) {
         aClearBuffer(cmd++, DMEM_ADDR_LEFT_CH, DEFAULT_LEN_2CH);
         cmd = synthesis_process_notes(aiBuf, bufLen, cmd);
     } else {*/
    if (gReverbDownsampleRate == 1) {
        // Put the oldest samples in the ring buffer into the wet channels
        aSetLoadBufferPair(cmd++, 0, v1->startPos);
        if (v1->lengths[1] != 0) {
            // Ring buffer wrapped
            aSetLoadBufferPair(cmd++, v1->lengths[0], 0);
            temp = 0;
        }

        // Use the reverb sound as initial sound for this audio update
        aDMEMMove(cmd++, DMEM_ADDR_WET_LEFT_CH, DMEM_ADDR_LEFT_CH, DEFAULT_LEN_2CH);

        // (Hopefully) lower the volume of the wet channels. New reverb will later be mixed into
        // these channels.
        aSetBuffer(cmd++, 0, 0, 0, DEFAULT_LEN_2CH);
        // 0x8000 here is -100%
        aMix(cmd++, 0, /*gain*/ 0x8000 + gSynthesisReverb.reverbGain, /*in*/ DMEM_ADDR_WET_LEFT_CH,
             /*out*/ DMEM_ADDR_WET_LEFT_CH);
    } else {
        // Same as above but upsample the previously downsampled samples used for reverb first
        temp = 0; //! jesus christ
        t4 = (v1->startPos & 7) * 2;
        ra = ALIGN(v1->lengths[0] + t4, 4);
        aSetLoadBufferPair(cmd++, 0, v1->startPos - t4 / 2);
        if (v1->lengths[1] != 0) {
            // Ring buffer wrapped
            aSetLoadBufferPair(cmd++, ra, 0);
            //! We need an empty statement (even an empty ';') here to make the function match
            //! (because IDO). However, copt removes extraneous statements and dead code. So we need
            //! to trick copt into thinking 'temp' could be undefined, and luckily the compiler
            //! optimizes out the useless assignment.
            ra = ra + temp;
        }
        aSetBuffer(cmd++, 0, t4 + DMEM_ADDR_WET_LEFT_CH, DMEM_ADDR_LEFT_CH, bufLen << 1);
        aResample(cmd++, gSynthesisReverb.resampleFlags, (u16) gSynthesisReverb.resampleRate,
                  VIRTUAL_TO_PHYSICAL2(gSynthesisReverb.resampleStateLeft));
        aSetBuffer(cmd++, 0, t4 + DMEM_ADDR_WET_RIGHT_CH, DMEM_ADDR_RIGHT_CH, bufLen << 1);
        aResample(cmd++, gSynthesisReverb.resampleFlags, (u16) gSynthesisReverb.resampleRate,
                  VIRTUAL_TO_PHYSICAL2(gSynthesisReverb.resampleStateRight));
        aSetBuffer(cmd++, 0, 0, 0, DEFAULT_LEN_2CH);
        aMix(cmd++, 0, /*gain*/ 0x8000 + gSynthesisReverb.reverbGain, /*in*/ DMEM_ADDR_LEFT_CH,
             /*out*/ DMEM_ADDR_LEFT_CH);
        aDMEMMove(cmd++, DMEM_ADDR_LEFT_CH, DMEM_ADDR_WET_LEFT_CH, DEFAULT_LEN_2CH);
    }
    cmd = synthesis_process_notes(aiBuf, bufLen, cmd);
    if (gReverbDownsampleRate == 1) {
        aSetSaveBufferPair(cmd++, 0, v1->lengths[0], v1->startPos);
        if (v1->lengths[1] != 0) {
            // Ring buffer wrapped
            aSetSaveBufferPair(cmd++, v1->lengths[0], v1->lengths[1], 0);
        }
    } else {
        // Downsampling is done later by CPU when RSP is done, therefore we need to have double
        // buffering. Left and right buffers are adjacent in memory.
        aSetBuffer(cmd++, 0, 0, DMEM_ADDR_WET_LEFT_CH, DEFAULT_LEN_2CH);
        aSaveBuffer(
            cmd++,
            VIRTUAL_TO_PHYSICAL2(
                gSynthesisReverb.items[gSynthesisReverb.curFrame][updateIndex].toDownsampleLeft));
        gSynthesisReverb.resampleFlags = 0;
    }
    // }
    return cmd;
}
// This runs 240 times per second.
ALWAYS_INLINE void process_sequences() {
    s32 i;
    for (i = 0; i < SEQUENCE_PLAYERS; i++) {
        if (gSequencePlayers[i].enabled) {
            sequence_player_process_sequence(gSequencePlayers + i);
            sequence_player_process_sound(gSequencePlayers + i);
        }
    }
    reclaim_notes();
    process_notes();
}
ALWAYS_INLINE u64 *synthesis_execute(u64 *cmdBuf, s32 *writtenCmds, s16 *aiBuf, s32 bufLen) { // left off here
    s32 chunkLen;
    register s32 i;
    u32 *aiBufPtr = (u32 *) aiBuf;
    u64 *cmd = cmdBuf + 1;
    s32 v0;

    aSegment(cmdBuf, 0, 0);

    for (i = gAudioUpdatesPerFrame; i > 0; i--) {
        if (i == 1) {
            // 'bufLen' will automatically be divisible by 8, no need to round
            chunkLen = bufLen;
        } else {
            v0 = bufLen / i;
            // chunkLen = v0 rounded to nearest multiple of 8
            chunkLen = v0 - (v0 & 7);

            if ((v0 & 7) >= 4) {
                chunkLen += 8;
            }
        }
        process_sequences();
        //  if (gSynthesisReverb.useReverb != 0) {
        prepare_reverb_ring_buffer(chunkLen, gAudioUpdatesPerFrame - i); 
        // }
        cmd = synthesis_do_one_audio_update((s16 *) aiBufPtr, chunkLen, cmd, gAudioUpdatesPerFrame - i);
        bufLen -= chunkLen;
        aiBufPtr += chunkLen;
    }
    if (gSynthesisReverb.framesLeftToIgnore != 0) {
        gSynthesisReverb.framesLeftToIgnore--;
    }
    gSynthesisReverb.curFrame ^= 1;
    *writtenCmds = cmd - cmdBuf;
    return cmd;
}
extern const u16 randomValues[0x10000];
struct SPTask *create_next_audio_frame_task(void) {
    u32 samplesRemainingInAI;
    s32 writtenCmds;
    s32 index;
    OSTask_t *task;
#define PREVAIINDEX ((gCurrAiBufferIndex - 2 + NUMAIBUFFERS) % NUMAIBUFFERS)
    gAudioFrameCount++;
    if (gAudioLoadLock != AUDIO_LOCK_NOT_LOADING) {
        return NULL;
    }

    gAudioTaskIndex ^= 1;
    gCurrAiBufferIndex++;
    gCurrAiBufferIndex %= NUMAIBUFFERS;
    samplesRemainingInAI = osAiGetLength() / 4;

    // Audio is triple buffered; the audio interface reads from two buffers
    // while the third is being written by the RSP. More precisely, the
    // lifecycle is:
    // - this function computes an audio command list
    // - wait for vblank
    // - the command list is sent to the RSP (we could have sent it to the
    //   RSP before the vblank, but that gives the RSP less time to finish)
    // - wait for vblank
    // - the RSP is now expected to be finished, and we can send its output
    //   on to the AI
    // Here we thus send to the AI the sound that was generated two frames ago.
    if (gAiBufferLengths[PREVAIINDEX] != 0) {
        osAiSetNextBuffer(gAiBuffers[PREVAIINDEX], gAiBufferLengths[PREVAIINDEX] * 4);
    }

    gCurrAudioFrameDmaCount = 0;

    gAudioTask = &gAudioTasks[gAudioTaskIndex];
  //  gAudioCmd = ;

    index = gCurrAiBufferIndex;
    gCurrAiBuffer = gAiBuffers[index];
    gAiBufferLengths[index] =
        ((gSamplesPerFrameTarget - samplesRemainingInAI + EXTRA_BUFFERED_AI_SAMPLES_TARGET) & ~0xf)
        + SAMPLES_TO_OVERPRODUCE;
    if (gAiBufferLengths[index] < gMinAiBufferLength) {
        gAiBufferLengths[index] = gMinAiBufferLength;
    }
    if (gAiBufferLengths[index] > gSamplesPerFrameTarget + SAMPLES_TO_OVERPRODUCE) {
        gAiBufferLengths[index] = gSamplesPerFrameTarget + SAMPLES_TO_OVERPRODUCE;
    }

    if (sGameLoopTicked != 0) {
        update_game_sound();
        sGameLoopTicked = 0;
    }

    /*gAudioCmd = */synthesis_execute(gAudioCmdBuffers[gAudioTaskIndex], &writtenCmds, gCurrAiBuffer, gAiBufferLengths[index]);
    gAudioRandom = ((gAudioRandom + gAudioFrameCount) * gAudioFrameCount);
  //  gAudioRandom = randomValues[(u16)gAudioFrameCount];
    index = gAudioTaskIndex;
    gAudioTask->msgqueue = NULL;
    gAudioTask->msg = NULL;

    task = &gAudioTask->task.t;
    task->type = M_AUDTASK;
    task->flags = 0;
    task->ucode_boot = rspbootTextStart;
    task->ucode_boot_size = (u8 *) rspbootTextEnd - (u8 *) rspbootTextStart;
    task->ucode = aspMainTextStart;
    task->ucode_size = 0x800; // (this size is ignored)
    task->ucode_data = aspMainDataStart;
    task->ucode_data_size = (aspMainDataEnd - aspMainDataStart) * sizeof(u64);
    task->dram_stack = NULL;
    task->dram_stack_size = 0;
    task->output_buff = NULL;
    task->output_buff_size = NULL;
    task->data_ptr = gAudioCmdBuffers[index];
    task->data_size = writtenCmds * sizeof(u64);

    // The audio task never yields, so having a yield buffer is pointless.
    // This wastefulness was fixed in US.
    task->yield_data_ptr = NULL;
    task->yield_data_size = 0;

    decrease_sample_dma_ttls();
    return gAudioTask;
}





// size = 0x10

// EU only

/**
 * Performs an immediate DMA copy
 */

/**
 * Performs an asynchronus (normal priority) DMA copy
 */

// (void) must be omitted from parameters


