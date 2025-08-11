#ifndef AUDIO_EXTERNAL_H
#define AUDIO_EXTERNAL_H

#include <PR/ultratypes.h>

#include "types.h"
#include "data.h"
#include "heap.h"
// Sequence arguments, passed to play_sequence. seqId may be bit-OR'ed with
// SEQ_VARIATION; this will load the same sequence, but set a variation
// bit which may be read by the sequence script.


#define SAMPLES_TO_OVERPRODUCE 0x10
#define EXTRA_BUFFERED_AI_SAMPLES_TARGET 0x40

extern f32 gDefaultSoundArgs[3];
extern u8 sBackgroundMusicQueueSize;
struct SPTask *create_next_audio_frame_task(void);
void play_sound(s32 soundBits, f32 *pos);
void sequence_player_fade_out(s32 player, s32 fadeTimer);
void soften_music(s32 player, s32 fadeTimer, s32 arg2);
void sequence_player_unlower(s32 player, s32 fadeTimer);
void set_sound_disabled(s32 disabled);
void sound_init(void);
void killSound(u32 soundBits, f32 *vec);
void kill_obj_sound(f32 *arg0);
void muteCommonBanks(void);
void sound_banks_disable(s32 bankMask);
void sound_banks_enable(s32 bankMask);
void play_dialog_sound(s32 dialogID);
void play_music(s32 player, s32 seqArgs, s32 fadeTimer);
void stop_background_music(s32 seqId);
void fadeout_background_music(s32 arg0, s32 fadeOut);
void drop_queued_background_music(void);
void play_secondary_music(s32 seqId, s32 bgMusicVolume, s32 volume, s32 fadeTimer);
void fadeOutSecondaryMusic(s32 fadeTimer);
void fadeAllMusicOut(s32 fadeOutTime);
void play_course_clear(void);
void play_peachs_jingle(void);
void play_puzzle_jingle(void);
void play_star_fanfare(void);
void play_power_star_jingle(s32 arg0);
void play_race_fanfare(void);
void play_toads_jingle(void);
void audio_set_sound_mode(s32 arg0);
void adjustBankPitch(s32 bankIndex, s32 arg1);
void audio_init(void); // in load.c
extern f32 adjustFreq;
#ifdef VERSION_EU
struct SPTask *unused_80321460(void);
#endif


// --- Vanilla compatibility additions ---
#include <PR/ultratypes.h>
#include "types.h"
extern Vec3f gGlobalSoundSource;

#ifndef seq_player_lower_volume
#define seq_player_lower_volume sequence_player_lower_volume
#endif
#ifndef seq_player_unlower_volume
#define seq_player_unlower_volume sequence_player_unlower_volume
#endif
#ifndef seq_player_fade_out
#define seq_player_fade_out sequence_player_fade_out
#endif

#endif // end external.h
