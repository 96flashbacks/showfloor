#ifndef SEQ_IDS_H
#define SEQ_IDS_H

// Sometimes a sequence id is represented as one of the below ids (the base id),
// optionally OR'd with SEQ_VARIATION.
#define SEQ_BASE_ID 0x7f
#define SEQ_VARIATION 0x80

enum SeqId {
    SEQ_SOUND_PLAYER,                 // 0x00
    SEQ_EVENT_CUTSCENE_COLLECT_STAR,  // 0x01
    SEQ_MENU_TITLE_SCREEN,            // 0x02
    SEQ_LEVEL_GRASS,                  // 0x03
    SEQ_LEVEL_INSIDE_CASTLE,          // 0x04
    SEQ_LEVEL_WATER,                  // 0x05
    SEQ_LEVEL_HOT,                    // 0x06
    SEQ_LEVEL_BOSS_KOOPA,             // 0x07
    SEQ_KOOPA_HALLWAY_SCREAM_LEFT,    // 0x08
    SEQ_KOOPA_HALLWAY_SCREAM_RIGHT,   // 0x09
    SEQ_COUNT
};

#endif // SEQ_IDS_H
