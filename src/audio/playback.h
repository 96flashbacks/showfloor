#ifndef AUDIO_PLAYBACK_H
#define AUDIO_PLAYBACK_H

#include <PR/ultratypes.h>

#include "internal.h"
struct Sound {
    s32 soundBits;
    f32 *position;
}; // size = 0x8

struct ChannelVolumeScaleFade {
    f32 velocity;
    u8 target;
    f32 current;
    u16 remDuration;
}; // size = 0x10

struct SoundCharacteristics {
    f32 *pos;
    f32 distance;
    u32 priority;
    u32 soundBits; // packed bits, same as first arg to play_sound
    u8 soundStatus;
    u8 freshness; // ttl? sometimes set to 10
    u8 prev;
    u8 next;
}; // size = 0x1C

// Mask bits denoting where to allocate notes from, according to a channel's
// noteAllocPolicy. Despite being checked as bitmask bits, the bits are not
// orthogonal; rather, the smallest bit wins, except for NOTE_ALLOC_LAYER,
// which *is* orthogonal to the other. SEQ implicitly includes CHANNEL.
// If none of the CHANNEL/SEQ/GLOBAL_FREELIST bits are set, all three locations
// are tried.
#define NOTE_ALLOC_LAYER 1
#define NOTE_ALLOC_CHANNEL 2
#define NOTE_ALLOC_SEQ 4
#define NOTE_ALLOC_GLOBAL_FREELIST 8

#define PORTAMENTO_TGT_NOTE portamentoTargetNote
#define DRUM_INDEX cmdSemitone
#define SEMITONE cmdSemitone
#define USED_SEMITONE usedSemitone


#define M64_READ_U8(state, dst) dst = m64_read_u8(state);
#define M64_READ_S16(state, dst) dst = m64_read_s16(state);
#define M64_READ_COMPRESSED_U16(state, dst) dst = m64_read_compressed_u16(state);


#endif // AUDIO_PLAYBACK_H
