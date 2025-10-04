#include <ultra64.h>

#include "actors/common1.h"
#include "area.h"
#include "audio/external.h"
#include "camera.h"
#include "course_table.h"
#include "dialog_ids.h"
#include "engine/math_util.h"
#include "game_init.h"
#include "gfx_dimensions.h"
#include "ingame_menu.h"
#include "level_update.h"
#include "levels/castle_grounds/header.h"
#include "memory.h"
#include "print.h"
#include "save_file.h"
#include "segment2.h"
#include "segment7.h"
#include "seq_ids.h"
#include "sm64.h"
#include "text_strings.h"
#include "types.h"

FORCE_BSS s8 gLastDialogLineNum;
FORCE_BSS s32 gDialogVariable;
FORCE_BSS u16 gMenuTextAlpha;
FORCE_BSS s16 gCutsceneMsgXOffset;
FORCE_BSS s16 gCutsceneMsgYOffset;

enum MenuState {
    MENU_STATE_0,
    MENU_STATE_1,
    MENU_STATE_2,
    MENU_STATE_3,
    MENU_STATE_DEFAULT = MENU_STATE_0,

    // Dialog
    MENU_STATE_DIALOG_OPENING = MENU_STATE_0,
    MENU_STATE_DIALOG_OPEN = MENU_STATE_1,
    MENU_STATE_DIALOG_SCROLLING = MENU_STATE_2,
    MENU_STATE_DIALOG_CLOSING = MENU_STATE_3,

    // Pause Screen
    MENU_STATE_PAUSE_SCREEN_OPENING = MENU_STATE_0,
    MENU_STATE_PAUSE_SCREEN_COURSE = MENU_STATE_1,
    MENU_STATE_PAUSE_SCREEN_CASTLE = MENU_STATE_2,

    // Course Complete Screen
    MENU_STATE_COURSE_COMPLETE_SCREEN_OPENING = MENU_STATE_0,
    MENU_STATE_COURSE_COMPLETE_SCREEN_OPEN = MENU_STATE_1
};

enum DialogBoxPageState { DIALOG_PAGE_STATE_NONE, DIALOG_PAGE_STATE_SCROLL, DIALOG_PAGE_STATE_END };

enum DialogBoxType {
    DIALOG_TYPE_ROTATE, // used in NPCs and level messages
    DIALOG_TYPE_ZOOM    // used in signposts and wall signs and etc
};

#define DIALOG_BOX_ANGLE_DEFAULT 90.0f
#define DIALOG_BOX_SCALE_DEFAULT 19.0f

s8 gMenuState = MENU_STATE_DEFAULT;
f32 gDialogBoxAngle = DIALOG_BOX_ANGLE_DEFAULT;
f32 gDialogBoxScale = DIALOG_BOX_SCALE_DEFAULT;
s16 gDialogScrollOffsetY = 0;
s8 gDialogBoxType = DIALOG_TYPE_ROTATE;
s16 gDialogID = DIALOG_NONE;
s16 gNextDialogPageStartStrIndex = 0;
s16 gDialogPageStartStrIndex = 0;
s8 gMenuLineNum = 1;
s8 gDialogWithChoice = FALSE;
u8 gMenuHoldKeyIndex = 0;
u8 gMenuHoldKeyTimer = 0;
s32 gDialogResponse = DIALOG_RESPONSE_NONE;

void create_dl_identity_matrix(void) {
    Mtx *matrix = (Mtx *) alloc_display_list(sizeof(Mtx));

    if (matrix == NULL) {
        rmonpf(("AllocDynamic error in message!!\n"));
        return;
    }

#ifndef GBI_FLOATS
    matrix->m[0][0] = 0x00010000;
    matrix->m[1][0] = 0x00000000;
    matrix->m[2][0] = 0x00000000;
    matrix->m[3][0] = 0x00000000;
    matrix->m[0][1] = 0x00000000;
    matrix->m[1][1] = 0x00010000;
    matrix->m[2][1] = 0x00000000;
    matrix->m[3][1] = 0x00000000;
    matrix->m[0][2] = 0x00000001;
    matrix->m[1][2] = 0x00000000;
    matrix->m[2][2] = 0x00000000;
    matrix->m[3][2] = 0x00000000;
    matrix->m[0][3] = 0x00000000;
    matrix->m[1][3] = 0x00000001;
    matrix->m[2][3] = 0x00000000;
    matrix->m[3][3] = 0x00000000;
#else
    guMtxIdent(matrix);
#endif

    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(matrix),
              G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(matrix),
              G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
}

void create_dl_translation_matrix(s8 pushOp, f32 x, f32 y, f32 z) {
    Mtx *matrix = (Mtx *) alloc_display_list(sizeof(Mtx));

    if (matrix == NULL) {
        rmonpf(("AllocDynamic error in message!!\n"));
        return;
    }

    guTranslate(matrix, x, y, z);

    if (pushOp == MENU_MTX_PUSH) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(matrix),
                  G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_PUSH);
    }

    if (pushOp == MENU_MTX_NOPUSH) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(matrix),
                  G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);
    }
}

void create_dl_rotation_matrix(s8 pushOp, f32 a, f32 x, f32 y, f32 z) {
    Mtx *matrix = (Mtx *) alloc_display_list(sizeof(Mtx));

    if (matrix == NULL) {
        rmonpf(("AllocDynamic error in message!!\n"));
        return;
    }

    guRotate(matrix, a, x, y, z);

    if (pushOp == MENU_MTX_PUSH) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(matrix),
                  G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_PUSH);
    }

    if (pushOp == MENU_MTX_NOPUSH) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(matrix),
                  G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);
    }
}

void create_dl_scale_matrix(s8 pushOp, f32 x, f32 y, f32 z) {
    Mtx *matrix = (Mtx *) alloc_display_list(sizeof(Mtx));

    if (matrix == NULL) {
        rmonpf(("AllocDynamic error in message!!\n"));
        return;
    }

    guScale(matrix, x, y, z);

    if (pushOp == MENU_MTX_PUSH) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(matrix),
                  G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_PUSH);
    }

    if (pushOp == MENU_MTX_NOPUSH) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(matrix),
                  G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);
    }
}

void create_dl_ortho_matrix(void) {
    Mtx *matrix = (Mtx *) alloc_display_list(sizeof(Mtx));

    if (matrix == NULL) {
        rmonpf(("AllocDynamic error in message!!\n"));
        return;
    }

    create_dl_identity_matrix();

    guOrtho(matrix, 0.0f, SCREEN_WIDTH, 0.0f, SCREEN_HEIGHT, -10.0f, 10.0f, 1.0f);

    // Should produce G_RDPHALF_1 in Fast3D
    gSPPerspNormalize(gDisplayListHead++, 0xFFFF);

    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(matrix),
              G_MTX_PROJECTION | G_MTX_MUL | G_MTX_NOPUSH)
}

static u8 *alloc_ia8_text_from_i1(u16 *in, s16 width, s16 height) {
    s32 inPos;
    u16 bitMask;
    u8 *out;
    s16 outPos = 0;

    out = (u8 *) alloc_display_list((u32) width * (u32) height);

    if (out == NULL) {
        rmonpf(("AllocDynamic error in message!!\n"));
        return NULL;
    }

    for (inPos = 0; inPos < (width * height) / 16; inPos++) {
        bitMask = 0x8000;

        while (bitMask != 0) {
            if (in[inPos] & bitMask) {
                out[outPos] = 0xFF;
            } else {
                out[outPos] = 0x00;
            }

            bitMask /= 2;
            outPos++;
        }
    }

    return out;
}

void render_generic_char(u8 c) {
    void **fontLUT = segmented_to_virtual(main_font_lut);
    void *packedTexture = segmented_to_virtual(fontLUT[c]);
    void *unpackedTexture = alloc_ia8_text_from_i1(packedTexture, 8, 16);

    gDPPipeSync(gDisplayListHead++);
    gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_IA, G_IM_SIZ_8b, 1,
                       VIRTUAL_TO_PHYSICAL(unpackedTexture));
    gSPDisplayList(gDisplayListHead++, dl_ia_text_tex_settings);
}

#define CUR_CHAR str[strPos]
#define MAX_STRING_WIDTH 18
#define CHAR_WIDTH_SPACE 5.0f
#define CHAR_WIDTH_DEFAULT 10.0f

/**
 * Prints a generic white string.
 * In JP/EU a IA1 texture is used but in US a IA4 texture is used.
 */
void print_generic_string(s16 x, s16 y, const u8 *str) {
    UNUSED s8 mark = DIALOG_MARK_NONE; // unused in EU
    s32 strPos = 0;
    u8 lineNum = 1;

    create_dl_translation_matrix(MENU_MTX_PUSH, x, y, 0.0f);

    while (str[strPos] != SPECIAL_CHAR(DIALOG_CHAR_TERMINATOR)) {
        switch (CUR_CHAR) {
            case SPECIAL_CHAR(DIALOG_CHAR_DAKUTEN):
                mark = DIALOG_MARK_DAKUTEN;
                break;
            case SPECIAL_CHAR(DIALOG_CHAR_PERIOD_OR_HANDAKUTEN):
                mark = DIALOG_MARK_HANDAKUTEN;
                break;
            case SPECIAL_CHAR(DIALOG_CHAR_NEWLINE):
                gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
                create_dl_translation_matrix(MENU_MTX_PUSH, x, y - (lineNum * MAX_STRING_WIDTH), 0.0f);
                lineNum++;
                break;
            case SPECIAL_CHAR(DIALOG_CHAR_PERIOD):
                create_dl_translation_matrix(MENU_MTX_PUSH, -2.0f, -5.0f, 0.0f);
                render_generic_char(DIALOG_CHAR_PERIOD_OR_HANDAKUTEN);
                gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
                break;

            case SPECIAL_CHAR(DIALOG_CHAR_SPACE):
                create_dl_translation_matrix(MENU_MTX_NOPUSH, CHAR_WIDTH_SPACE, 0.0f, 0.0f);
                break;
                break; // ? needed to match

            default:
                render_generic_char(CUR_CHAR);
                if (mark != DIALOG_MARK_NONE) {
                    create_dl_translation_matrix(MENU_MTX_PUSH, 5.0f, 5.0f, 0.0f);
                    render_generic_char(DIALOG_CHAR_MARK_START + mark);
                    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
                    mark = DIALOG_MARK_NONE;
                }

                create_dl_translation_matrix(MENU_MTX_NOPUSH, CHAR_WIDTH_DEFAULT, 0.0f, 0.0f);
        }
        strPos++;
    }

    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
}

/**
 * Prints a hud string depending of the hud table list defined.
 */
void print_hud_lut_string(s8 hudLUT, s16 x, s16 y, const u8 *str) {
    s32 strPos = 0;
    void **hudLUT2 = segmented_to_virtual(main_hud_lut); // 0-9 A-Z HUD Color Font
    u32 curX = x;
    u32 curY = y;

    u32 xStride; // X separation

    if (hudLUT == HUD_LUT_JPMENU) {
        xStride = 16;
    } else { // HUD_LUT_GLOBAL
        xStride = 14;
    }

    while (str[strPos] != GLOBAL_CHAR_TERMINATOR) {
        gDPPipeSync(gDisplayListHead++);

        if (hudLUT == HUD_LUT_GLOBAL) {
            gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1,
                               hudLUT2[str[strPos]]);
        }

        gSPDisplayList(gDisplayListHead++, dl_rgba16_load_tex_block);
        gSPTextureRectangle(gDisplayListHead++, curX << 2, curY << 2, (curX + 16) << 2,
                            (curY + 16) << 2, G_TX_RENDERTILE, 0, 0, 1 << 10, 1 << 10);

        curX += xStride;
        strPos++;
    }
}

void handle_menu_scrolling(s8 scrollDirection, s8 *currentIndex, s8 minIndex, s8 maxIndex) {
    u8 index = 0;

    if (scrollDirection == MENU_SCROLL_VERTICAL) {
        if (gPlayer3Controller->rawStickY > 60) {
            index++;
        }

        if (gPlayer3Controller->rawStickY < -60) {
            index += 2;
        }
    } else if (scrollDirection == MENU_SCROLL_HORIZONTAL) {
        if (gPlayer3Controller->rawStickX > 60) {
            index += 2;
        }

        if (gPlayer3Controller->rawStickX < -60) {
            index++;
        }
    }

    if (((index ^ gMenuHoldKeyIndex) & index) == 2) {
        if (*currentIndex == maxIndex) {
            //! Probably originally a >=, but later replaced with an == and an else statement.
            *currentIndex = maxIndex;
        } else {
            play_sound(SOUND_MENU_CHANGE_SELECT, gGlobalSoundSource);
            (*currentIndex)++;
        }
    }

    if (((index ^ gMenuHoldKeyIndex) & index) == 1) {
        if (*currentIndex == minIndex) {
            // Same applies to here as above
        } else {
            play_sound(SOUND_MENU_CHANGE_SELECT, gGlobalSoundSource);
            (*currentIndex)--;
        }
    }

    if (gMenuHoldKeyTimer == 10) {
        gMenuHoldKeyTimer = 8;
        gMenuHoldKeyIndex = 0;
    } else {
        gMenuHoldKeyTimer++;
        gMenuHoldKeyIndex = index;
    }

    if ((index & 3) == 0) {
        gMenuHoldKeyTimer = 0;
    }
}

s16 get_str_x_pos_from_center_scale(s16 centerPos, u8 *str, f32 scale) {
    s16 strPos = 0;
    f32 charsWidth = 0.0f;
    f32 spacesWidth = 0.0f;

    while (str[strPos] != DIALOG_CHAR_TERMINATOR) {
        //! EU checks for dakuten and handakuten despite dialog code unable to handle it
        if (str[strPos] == DIALOG_CHAR_SPACE) {
            spacesWidth += 1.0;
        } else if (str[strPos] != DIALOG_CHAR_DAKUTEN
                   && str[strPos] != DIALOG_CHAR_PERIOD_OR_HANDAKUTEN) {
            charsWidth += 1.0;
        }
        strPos++;
    }
    // return the x position of where the string starts as half the string's
    // length from the position of the provided center.
    return (f32) centerPos - (scale * (charsWidth / 2.0)) - ((scale / 2.0) * (spacesWidth / 2.0));
}

u8 gHudSymCoin[] = { GLYPH_COIN, GLYPH_SPACE };
u8 gHudSymX[] = { GLYPH_MULTIPLY, GLYPH_SPACE };

void print_hud_my_score_coins(s32 useCourseCoinScore, s8 fileIndex, s8 courseIndex, s16 x, s16 y) {
    u8 strNumCoins[4];
    s16 numCoins;

    if (!useCourseCoinScore) {
        numCoins = (u16) (save_file_get_max_coin_score(courseIndex) & 0xFFFF);
    } else {
        numCoins = save_file_get_course_coin_score(fileIndex, courseIndex);
    }

    if (numCoins != 0) {
        print_hud_lut_string(HUD_LUT_GLOBAL, x, y, gHudSymCoin);
        print_hud_lut_string(HUD_LUT_GLOBAL, x + 16, y, gHudSymX);
        int_to_str(numCoins, strNumCoins);
        print_hud_lut_string(HUD_LUT_GLOBAL, x + 32, y, strNumCoins);
    }
}

void int_to_str(s32 num, u8 *dst) {
    s32 digit1;
    s32 digit2;
    s32 digit3;

    s8 pos = 0;

    if (num > 999) {
        dst[0] = 0x00;
        dst[1] = DIALOG_CHAR_TERMINATOR;
        return;
    }

    digit1 = num / 100;
    digit2 = (num - digit1 * 100) / 10;
    digit3 = (num - digit1 * 100) - (digit2 * 10);

    if (digit1 != 0) {
        dst[pos] = digit1;
        pos++;
    }

    if (digit2 != 0 || digit1 != 0) {
        dst[pos] = digit2;
        pos++;
    }

    dst[pos] = digit3;
    pos++;
    dst[pos] = DIALOG_CHAR_TERMINATOR;
}

s16 get_dialog_id(void) {
    return gDialogID;
}

void create_dialog_box(s16 dialog) {
    if (gDialogID == DIALOG_NONE) {
        gDialogID = dialog;
        gDialogBoxType = DIALOG_TYPE_ROTATE;
    }
}

void create_dialog_box_with_var(s16 dialog, s32 dialogVar) {
    if (gDialogID == DIALOG_NONE) {
        gDialogID = dialog;
        gDialogVariable = dialogVar;
        gDialogBoxType = DIALOG_TYPE_ROTATE;
    }
}

void create_dialog_inverted_box(s16 dialog) {
    if (gDialogID == DIALOG_NONE) {
        gDialogID = dialog;
        gDialogBoxType = DIALOG_TYPE_ZOOM;
    }
}

void create_dialog_box_with_response(s16 dialog) {
    if (gDialogID == DIALOG_NONE) {
        gDialogID = dialog;
        gDialogBoxType = DIALOG_TYPE_ROTATE;
        gDialogWithChoice = TRUE;
    }
}

void reset_dialog_render_state(void) {
    level_set_transition(0, NULL);

    if (gDialogBoxType == DIALOG_TYPE_ZOOM) {
        trigger_cutscene_dialog(2);
    }

    gDialogBoxScale = DIALOG_BOX_SCALE_DEFAULT;
    gDialogBoxAngle = DIALOG_BOX_ANGLE_DEFAULT;
    gMenuState = MENU_STATE_DEFAULT;
    gDialogID = DIALOG_NONE;
    gDialogPageStartStrIndex = 0;
    gDialogWithChoice = FALSE;
    gNextDialogPageStartStrIndex = 0;
    gDialogResponse = DIALOG_RESPONSE_NONE;
}

#define X_VAL1 -1.0f
#define Y_VAL1 0.0
#define Y_VAL2 4.0f

void render_dialog_box_type(struct DialogEntry *dialog, s8 linesPerBox) {
    UNUSED u8 filler[4];

    create_dl_translation_matrix(MENU_MTX_NOPUSH, dialog->leftOffset, dialog->width, 0);

    switch (gDialogBoxType) {
        case DIALOG_TYPE_ROTATE: // Renders a dialog black box with zoom and rotation
            if (gMenuState == MENU_STATE_DIALOG_OPENING || gMenuState == MENU_STATE_DIALOG_CLOSING) {
                create_dl_scale_matrix(MENU_MTX_NOPUSH, 1.0 / gDialogBoxScale, 1.0 / gDialogBoxScale,
                                       1.0f);
                // convert the speed into angle
                create_dl_rotation_matrix(MENU_MTX_NOPUSH, gDialogBoxAngle * 4.0f, 0, 0, 1.0f);
            }
            gDPSetEnvColor(gDisplayListHead++, 0, 0, 0, 150);
            break;
        case DIALOG_TYPE_ZOOM: // Renders a dialog white box with zoom
            if (gMenuState == MENU_STATE_DIALOG_OPENING || gMenuState == MENU_STATE_DIALOG_CLOSING) {
                create_dl_translation_matrix(MENU_MTX_NOPUSH, 65.0 - (65.0 / gDialogBoxScale),
                                             (40.0 / gDialogBoxScale) - 40, 0);
                create_dl_scale_matrix(MENU_MTX_NOPUSH, 1.0 / gDialogBoxScale, 1.0 / gDialogBoxScale,
                                       1.0f);
            }
            gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, 150);
            break;
    }

    create_dl_translation_matrix(MENU_MTX_PUSH, X_VAL1, Y_VAL1, 0);
    create_dl_scale_matrix(MENU_MTX_NOPUSH, 1.0f, ((f32) linesPerBox / Y_VAL2) + 0.0, 1.0f);

    gSPDisplayList(gDisplayListHead++, dl_draw_text_bg_box);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
}

#define X_VAL3 5.0f
#define Y_VAL3 20

void handle_dialog_scroll_page_state(s8 lineNum, s8 totalLines, s8 *pageState, s8 *xMatrix,
                                     s16 *linePos) {
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    if (lineNum == totalLines) {
        *pageState = DIALOG_PAGE_STATE_SCROLL;
        return;
    }
    create_dl_translation_matrix(MENU_MTX_PUSH, X_VAL3, 2 - (lineNum * Y_VAL3), 0);

    *linePos = 0;
    *xMatrix = 1;
}

void adjust_pos_and_print_period_char(s8 *xMatrix, s16 *linePos) {
    if (*linePos != 0) {
        create_dl_translation_matrix(MENU_MTX_NOPUSH, 10 * *xMatrix, 0, 0);
    }

    create_dl_translation_matrix(MENU_MTX_PUSH, -2.0f, -5.0f, 0);
    render_generic_char(DIALOG_CHAR_PERIOD_OR_HANDAKUTEN);

    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    (*linePos)++;
    *xMatrix = 1;
}

void render_star_count_dialog_text(s8 *xMatrix, s16 *linePos) {
    s8 tensDigit = gDialogVariable / 10;
    s8 onesDigit = gDialogVariable - (tensDigit * 10); // remainder

    if (tensDigit != 0) {
        create_dl_translation_matrix(MENU_MTX_NOPUSH, 10 * *xMatrix, 0, 0);
        render_generic_char(tensDigit);
    } else {
        (*xMatrix)++;
    }

    create_dl_translation_matrix(MENU_MTX_NOPUSH, 10 * *xMatrix, 0, 0);
    render_generic_char(onesDigit);
    (*linePos)++;
    *xMatrix = 1;
}

u32 ensure_nonnegative(s16 value) {
    if (value < 0) {
        value = 0;
    }

    return value;
}

void handle_dialog_text_and_pages(struct DialogEntry *dialog) {
    UNUSED u64 filler;
    u8 strChar;
    u8 *str = segmented_to_virtual(dialog->str);
    s8 lineNum = 1;
    s8 totalLines;
    s8 pageState = DIALOG_PAGE_STATE_NONE;
    UNUSED s8 mark = DIALOG_MARK_NONE; // only used in JP and SH
    s8 xMatrix = 1;
    s8 linesPerBox = dialog->linesPerBox;
    s16 strIndex;
    s16 linePos = 0;

    if (gMenuState == MENU_STATE_DIALOG_SCROLLING) {
        // If scrolling, consider the number of lines for both
        // the current page and the page being scrolled to.
        totalLines = linesPerBox * 2 + 1;
    } else {
        totalLines = linesPerBox + 1;
    }

    gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);

    strIndex = gDialogPageStartStrIndex;

    if (gMenuState == MENU_STATE_DIALOG_SCROLLING) {
        create_dl_translation_matrix(MENU_MTX_NOPUSH, 0, (f32) gDialogScrollOffsetY, 0);
    }

    create_dl_translation_matrix(MENU_MTX_PUSH, X_VAL3, 2 - lineNum * Y_VAL3, 0);

    while (pageState == DIALOG_PAGE_STATE_NONE) {
        strChar = str[strIndex];

        switch (strChar) {
            case SPECIAL_CHAR(DIALOG_CHAR_TERMINATOR):
                pageState = DIALOG_PAGE_STATE_END;
                gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
                break;
            case SPECIAL_CHAR(DIALOG_CHAR_NEWLINE):
                lineNum++;
                handle_dialog_scroll_page_state(lineNum, totalLines, &pageState, &xMatrix, &linePos);
                break;
            case SPECIAL_CHAR(DIALOG_CHAR_DAKUTEN):
                mark = DIALOG_MARK_DAKUTEN;
                break;
            case SPECIAL_CHAR(DIALOG_CHAR_PERIOD_OR_HANDAKUTEN):
                mark = DIALOG_MARK_HANDAKUTEN;
                break;

            case SPECIAL_CHAR(DIALOG_CHAR_SPACE):
                if (linePos != 0) {
                    xMatrix++;
                }
                linePos++;
                break;

            case SPECIAL_CHAR(DIALOG_CHAR_PERIOD):
                adjust_pos_and_print_period_char(&xMatrix, &linePos);
                break;

            case SPECIAL_CHAR(DIALOG_CHAR_STAR_COUNT):
                render_star_count_dialog_text(&xMatrix, &linePos);
                break;

            default: // any other character
                if (linePos != 0) {
                    create_dl_translation_matrix(MENU_MTX_NOPUSH, 10 * xMatrix, 0, 0);
                }

                render_generic_char(strChar);
                xMatrix = 1;
                linePos++;

                if (mark != DIALOG_MARK_NONE) {
                    create_dl_translation_matrix(MENU_MTX_PUSH, 5.0f, 7.0f, 0);
                    render_generic_char(DIALOG_CHAR_MARK_START + mark);
                    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
                    mark = DIALOG_MARK_NONE;
                }
        }

        if (linePos == 12) {
            if (str[strIndex + 1] == DIALOG_CHAR_PERIOD) {
                adjust_pos_and_print_period_char(&xMatrix, &linePos);
                strIndex++;
            }

            if (str[strIndex + 1] == DIALOG_CHAR_COMMA) {
                create_dl_translation_matrix(MENU_MTX_NOPUSH, 10 * xMatrix, 0, 0);
                render_generic_char(DIALOG_CHAR_COMMA);
                strIndex++;
            }

            if (str[strIndex + 1] == DIALOG_CHAR_NEWLINE) {
                strIndex++;
            }

            if (str[strIndex + 1] == DIALOG_CHAR_TERMINATOR) {
                pageState = DIALOG_PAGE_STATE_END;
                gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
                break; // exit loop
            } else {
                lineNum++;
                handle_dialog_scroll_page_state(lineNum, totalLines, &pageState, &xMatrix, &linePos);
            }
        }

        strIndex++;
    }

    gSPDisplayList(gDisplayListHead++, dl_ia_text_end);

    if (gMenuState == MENU_STATE_DIALOG_OPEN) {
        if (pageState == DIALOG_PAGE_STATE_END) {
            gNextDialogPageStartStrIndex = -1;
        } else {
            gNextDialogPageStartStrIndex = strIndex;
        }
    }

    gLastDialogLineNum = lineNum;
}

#define X_VAL4_1 50
#define X_VAL4_2 25
#define Y_VAL4_1 1
#define Y_VAL4_2 20

void render_dialog_triangle_choice(void) {
    if (gMenuState == MENU_STATE_DIALOG_OPEN) {
        handle_menu_scrolling(MENU_SCROLL_HORIZONTAL, &gMenuLineNum, 1, 2);
    }

    create_dl_translation_matrix(MENU_MTX_NOPUSH, ((gMenuLineNum - 1) * X_VAL4_1) + X_VAL4_2,
                                 Y_VAL4_1 - (gLastDialogLineNum * Y_VAL4_2), 0);

    if (gDialogBoxType == DIALOG_TYPE_ROTATE) { // White Text
        gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, 255);
    } else { // Black Text
        gDPSetEnvColor(gDisplayListHead++, 0, 0, 0, 255);
    }

    gSPDisplayList(gDisplayListHead++, dl_draw_triangle);
}

#define X_VAL5 123.0f
#define Y_VAL5_1 -20
#define Y_VAL5_2 2
#define X_Y_VAL6 0.8f

void handle_special_dialog_text(s16 dialogID) { // dialog ID tables, in order
    // King Bob-omb (Start), Whomp (Start), King Bob-omb (throw him out), Eyerock (Start), Wiggler
    // (Start)
    s16 dialogBossStart[] = { DIALOG_017, DIALOG_114, DIALOG_128, DIALOG_117, DIALOG_150 };
    // Koopa the Quick (BoB), Koopa the Quick (THI), Penguin Race, Fat Penguin Race (120 stars)
    s16 dialogRaceSound[] = { DIALOG_005, DIALOG_009, DIALOG_055, DIALOG_164 };
    // Red Switch, Green Switch, Blue Switch, 100 coins star, Bowser Red Coin Star
    s16 dialogStarSound[] = { DIALOG_010, DIALOG_011, DIALOG_012, DIALOG_013, DIALOG_014 };
    // King Bob-omb (Start), Whomp (Defeated), King Bob-omb (Defeated, missing in JP), Eyerock
    // (Defeated), Wiggler (Defeated)
#if BUGFIX_KING_BOB_OMB_FADE_MUSIC
    s16 dialogBossStop[] = { DIALOG_017, DIALOG_115, DIALOG_116, DIALOG_118, DIALOG_152 };
#else
    //! @bug JP misses King Bob-omb defeated DIALOG_116, meaning that the boss music will still
    //! play after King Bob-omb is defeated until BoB loads it's music after the star cutscene
    s16 dialogBossStop[] = { DIALOG_017, DIALOG_115, DIALOG_118, DIALOG_152 };
#endif
    s16 i;

    for (i = 0; i < (s16) ARRAY_COUNT(dialogBossStart); i++) {
        if (dialogBossStart[i] == dialogID) {
            seq_player_unlower_volume(SEQ_PLAYER_LEVEL, 60);
            play_music(SEQ_PLAYER_LEVEL, SEQUENCE_ARGS(4, SEQ_EVENT_BOSS), 0);
            return;
        }
    }

    for (i = 0; i < (s16) ARRAY_COUNT(dialogRaceSound); i++) {
        if (dialogRaceSound[i] == dialogID && gMenuLineNum == DIALOG_RESPONSE_YES) {
            play_race_fanfare();
            return;
        }
    }

    for (i = 0; i < (s16) ARRAY_COUNT(dialogStarSound); i++) {
        if (dialogStarSound[i] == dialogID && gMenuLineNum == DIALOG_RESPONSE_YES) {
            play_sound(SOUND_MENU_STAR_SOUND, gGlobalSoundSource);
            return;
        }
    }

    for (i = 0; i < (s16) ARRAY_COUNT(dialogBossStop); i++) {
        if (dialogBossStop[i] == dialogID) {
            seq_player_fade_out(SEQ_PLAYER_LEVEL, 1);
            return;
        }
    }
}

s16 gMenuMode = MENU_MODE_NONE;

u8 gEndCutsceneStrEn0[] = { TEXT_FILE_MARIO_EXCLAMATION };
u8 gEndCutsceneStrEn1[] = { TEXT_POWER_STARS_RESTORED };
u8 gEndCutsceneStrEn2[] = { TEXT_THANKS_TO_YOU };
u8 gEndCutsceneStrEn3[] = { TEXT_THANK_YOU_MARIO };
u8 gEndCutsceneStrEn4[] = { TEXT_SOMETHING_SPECIAL };
u8 gEndCutsceneStrEn5[] = { TEXT_LISTEN_EVERYBODY };
u8 gEndCutsceneStrEn6[] = { TEXT_LETS_HAVE_CAKE };
u8 gEndCutsceneStrEn7[] = { TEXT_FOR_MARIO };
u8 gEndCutsceneStrEn8[] = { TEXT_FILE_MARIO_QUESTION };

u8 *gEndCutsceneStringsEn[] = {
    gEndCutsceneStrEn0, gEndCutsceneStrEn1, gEndCutsceneStrEn2, gEndCutsceneStrEn3, gEndCutsceneStrEn4,
    gEndCutsceneStrEn5, gEndCutsceneStrEn6, gEndCutsceneStrEn7,
    // This [8] string is actually unused. In the cutscene handler, the developers do not
    // set the 8th one, but use the first string again at the very end, so Peach ends up
    // saying "Mario!" twice. It is likely that she was originally meant to say "Mario?" at
    // the end but the developers changed their mind, possibly because the line recorded
    // sounded more like an exclamation than a question.
    gEndCutsceneStrEn8, NULL
};

u16 gCutsceneMsgFade = 0;
s16 gCutsceneMsgIndex = -1;
s16 gCutsceneMsgDuration = -1;
s16 gCutsceneMsgTimer = 0;
s8 gDialogCourseActNum = 1;

#define DIAG_VAL1 20
#define DIAG_VAL2 240
#define DIAG_VAL3 130
#define DIAG_VAL4 4

void render_dialog_entries(void) {
    void **dialogTable;
    struct DialogEntry *dialog;

    dialogTable = segmented_to_virtual(seg2_dialog_table);
    dialog = segmented_to_virtual(dialogTable[gDialogID]);

    // if the dialog entry is invalid, set the ID to DIALOG_NONE.
    if (dialog == segmented_to_virtual(NULL)) {
        rmonpf(("Message not defined. (%d)\n", gDialogID));
        gDialogID = DIALOG_NONE;
        return;
    }

    switch (gMenuState) {
        case MENU_STATE_DIALOG_OPENING:
            if (gDialogBoxAngle == DIALOG_BOX_ANGLE_DEFAULT) {
                play_sound(SOUND_MENU_MESSAGE_APPEAR, gGlobalSoundSource);
            }

            if (gDialogBoxType == DIALOG_TYPE_ROTATE) {
                gDialogBoxAngle -= 5.0;
                gDialogBoxScale -= 1.0;
            } else {
                gDialogBoxAngle -= 5.0;
                gDialogBoxScale -= 1.0;
            }

            if (gDialogBoxAngle == 0.0f) {
                gMenuState = MENU_STATE_DIALOG_OPEN;
                gMenuLineNum = 1;
            }
            break;

        case MENU_STATE_DIALOG_OPEN:
            gDialogBoxAngle = 0.0f;

            if ((gPlayer3Controller->buttonPressed & A_BUTTON)
                || (gPlayer3Controller->buttonPressed & B_BUTTON)) {
                if (gNextDialogPageStartStrIndex == -1) {
                    handle_special_dialog_text(gDialogID);
                    gMenuState = MENU_STATE_DIALOG_CLOSING;
                } else {
                    gMenuState = MENU_STATE_DIALOG_SCROLLING;
                    play_sound(SOUND_MENU_MESSAGE_NEXT_PAGE, gGlobalSoundSource);
                }
            }
            break;

        case MENU_STATE_DIALOG_SCROLLING:
            gDialogScrollOffsetY += dialog->linesPerBox * 2;

            if (gDialogScrollOffsetY >= dialog->linesPerBox * DIAG_VAL1) {
                gDialogPageStartStrIndex = gNextDialogPageStartStrIndex;
                gMenuState = MENU_STATE_DIALOG_OPEN;
                gDialogScrollOffsetY = 0;
            }
            break;

        case MENU_STATE_DIALOG_CLOSING:
            if (gDialogBoxAngle == 20.0f) {
                level_set_transition(0, NULL);
                play_sound(SOUND_MENU_MESSAGE_DISAPPEAR, gGlobalSoundSource);

                if (gDialogBoxType == DIALOG_TYPE_ZOOM) {
                    trigger_cutscene_dialog(2);
                }

                gDialogResponse = gMenuLineNum;
            }

            gDialogBoxAngle += 5.0f;
            gDialogBoxScale += 1.0f;

            if (gDialogBoxAngle == DIALOG_BOX_ANGLE_DEFAULT) {
                gMenuState = MENU_STATE_DEFAULT;
                gDialogID = DIALOG_NONE;
                gDialogPageStartStrIndex = 0;
                gDialogWithChoice = FALSE;
                gNextDialogPageStartStrIndex = 0;
                gDialogResponse = DIALOG_RESPONSE_NONE;
            }
            break;
    }

    render_dialog_box_type(dialog, dialog->linesPerBox);

    gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE,
    // Horizontal scissoring isn't really required and can potentially mess up widescreen enhancements.
#ifdef WIDESCREEN
                  0,
#else
                  ensure_nonnegative(dialog->leftOffset),
#endif
                  ensure_nonnegative(DIAG_VAL2 - dialog->width),
#ifdef WIDESCREEN
                  SCREEN_WIDTH,
#else
                  ensure_nonnegative(dialog->leftOffset + DIAG_VAL3),
#endif
                  ensure_nonnegative((240 - dialog->width) + (dialog->linesPerBox * 80 / DIAG_VAL4)));

    handle_dialog_text_and_pages(dialog);

    if (gNextDialogPageStartStrIndex == -1 && gDialogWithChoice == TRUE) {
        render_dialog_triangle_choice();
    }

    gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 2, 2, SCREEN_WIDTH - BORDER_HEIGHT / 2,
                  SCREEN_HEIGHT - BORDER_HEIGHT / 2);
}

// Calls a gMenuMode value defined by render_menus_and_dialogs cases
void set_menu_mode(s16 mode) {
    if (gMenuMode == MENU_MODE_NONE) {
        gMenuMode = mode;
    }
}

/* some necessary macro defintions I'm too lazy to find the decomp equivalents to */
#define SELECT 1
#define NA_SYS_CURSOR 0x7000F881
#define NA_SYS_ENTER 0x70010081
#define NA_SYS_PAUSE_ON 0x7002FF81
#define NA_SYS_PAUSE_OFF 0x7003FF81

void DrawSelectCursor(char line_min, char line_max) {
    unsigned char str;

    str = 0x00;

    if (gPlayer3Controller->rawStickY > 60) {
        str += 0x08;
    }

    if (gPlayer3Controller->rawStickY < -60) {
        str += 0x04;
    }

    if (gPlayer3Controller->rawStickX > 60) {
        str += 0x02;
    }

    if (gPlayer3Controller->rawStickX < -60) {
        str += 0x01;
    }

    if (((str ^ gMenuHoldKeyIndex) & str) == 0x08) {
        gMenuLineNum--;

        if (gMenuLineNum < line_min) {
            gMenuLineNum = line_min;
        } else {
            play_sound(NA_SYS_CURSOR, gGlobalSoundSource); // Sound CURSOL
        }
    }

    if (((str ^ gMenuHoldKeyIndex) & str) == 0x04) {
        gMenuLineNum++;

        if (gMenuLineNum > line_max) {
            gMenuLineNum = line_max;
        } else {
            play_sound(NA_SYS_CURSOR, gGlobalSoundSource); // Sound CURSOL
        }
    }

    if (gMenuHoldKeyTimer == 10) {
        gMenuHoldKeyTimer = 8;
        gMenuHoldKeyIndex = 0;
    } else {
        gMenuHoldKeyTimer++;
        gMenuHoldKeyIndex = str;
    }

    if ((str & 0x0c) == 0) {
        gMenuHoldKeyTimer = 0;
    }

    create_dl_translation_matrix(MENU_MTX_PUSH, 5.0f, 2.0f - (gMenuLineNum * 20), 0.0f);
    gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, 255);
    gSPDisplayList(gDisplayListHead++, dl_draw_triangle);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
}

short SelectMessageEvent(void) {
    short ret_num;

    void **mlist = segmented_to_virtual(seg2_debug_text_table);
    struct DialogEntry *message = segmented_to_virtual(mlist[gMenuMode]);

    if (message == NULL) {
        rmonpf(("Message not defined. (%d)\n", gMenuMode));
        gMenuMode = MENU_MODE_NONE;
        return 0;
    }

    switch (gMenuState) {
        case 0:
            if (gDialogBoxAngle == 70.0f) {
                level_set_transition(-1, NULL);                  // Window Open!
                play_sound(NA_SYS_PAUSE_ON, gGlobalSoundSource); // Sound ON
            }

            gDialogBoxAngle -= 5.0f;
            gDialogBoxScale -= 1.0f;

            if (gDialogBoxAngle == 0.0f) {
                gMenuState = 1;
                gMenuLineNum = 2;
            }

            break;
        case 1:
            gDialogBoxAngle = 0.0f; // Message Draw!

            if (gPlayer3Controller->buttonPressed & A_BUTTON
                || gPlayer3Controller->buttonPressed & START_BUTTON) {
                play_sound(NA_SYS_ENTER, gGlobalSoundSource); // Sound Enter
                gMenuState = 3;
            }

            break;
        case 3:
            if (gDialogBoxAngle == 20.0f) {
                level_set_transition(0, NULL);                    // Window Close!
                play_sound(NA_SYS_PAUSE_OFF, gGlobalSoundSource); // Sound ON
            }

            gDialogBoxAngle += 5.0f;
            gDialogBoxScale += 1.0f;

            if (gDialogBoxAngle == 90.0f) {
                gMenuState = 0;
                gMenuMode = MENU_MODE_NONE;
                gDialogPageStartStrIndex = 0;
                ret_num = gMenuLineNum - 1;
                return ret_num;
            }
            break;
    }

    render_dialog_box_type(message, message->linesPerBox);
    DrawSelectCursor(2, message->linesPerBox);
    handle_dialog_text_and_pages(message);

    return 0;
}

void reset_cutscene_msg_fade(void) {
    gCutsceneMsgFade = 0;
}

void dl_rgba16_begin_cutscene_msg_fade(void) {
    gSPDisplayList(gDisplayListHead++, dl_rgba16_text_begin);
    gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, gCutsceneMsgFade);
}

void dl_rgba16_stop_cutscene_msg_fade(void) {
    gSPDisplayList(gDisplayListHead++, dl_rgba16_text_end);

    if (gCutsceneMsgFade < 250) {
        gCutsceneMsgFade += 25;
    } else {
        gCutsceneMsgFade = 255;
    }
}

void set_cutscene_message(s16 xOffset, s16 yOffset, s16 msgIndex, s16 msgDuration) {
    // is message done printing?
    if (gCutsceneMsgIndex == -1) {
        gCutsceneMsgIndex = msgIndex;
        gCutsceneMsgDuration = msgDuration;
        gCutsceneMsgTimer = 0;
        gCutsceneMsgXOffset = xOffset;
        gCutsceneMsgYOffset = yOffset;
        gCutsceneMsgFade = 0;
    }
}

void do_cutscene_handler(void) {
    s16 x;

    // is a cutscene playing? do not perform this handler's actions if so.
    if (gCutsceneMsgIndex == -1) {
        return;
    }

    create_dl_ortho_matrix();

    gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);
    gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, gCutsceneMsgFade);

    // get the x coordinate of where the cutscene string starts.
    x = get_str_x_pos_from_center(gCutsceneMsgXOffset, gEndCutsceneStringsEn[gCutsceneMsgIndex], 10.0f);
    print_generic_string(x, 240 - gCutsceneMsgYOffset, gEndCutsceneStringsEn[gCutsceneMsgIndex]);

    gSPDisplayList(gDisplayListHead++, dl_ia_text_end);

    // if the timing variable is less than 5, increment
    // the fade until we are at full opacity.
    if (gCutsceneMsgTimer < 5) {
        gCutsceneMsgFade += 50;
    }

    // if the cutscene frame length + the fade-in counter is
    // less than the timer, it means we have exceeded the
    // time that the message is supposed to remain on
    // screen. if (message_duration = 50) and (msg_timer = 55)
    // then after the first 5 frames, the message will remain
    // on screen for another 50 frames until it starts fading.
    if (gCutsceneMsgDuration + 5 < gCutsceneMsgTimer) {
        gCutsceneMsgFade -= 50;
    }

    // like the first check, it takes 5 frames to fade out, so
    // perform a + 10 to account for the earlier check (10-5=5).
    if (gCutsceneMsgDuration + 10 < gCutsceneMsgTimer) {
        gCutsceneMsgIndex = -1;
        gCutsceneMsgFade = 0;
        gCutsceneMsgTimer = 0;
        return;
    }

    gCutsceneMsgTimer++;
}

s16 render_menus_and_dialogs(void) {
    s16 index = MENU_OPT_NONE;

    create_dl_ortho_matrix();

    if (gMenuMode != MENU_MODE_NONE) {
        switch (gMenuMode) {
            case MENU_MODE_UNUSED_0:
                index = SelectMessageEvent();
                break;
            case MENU_MODE_RENDER_PAUSE_SCREEN:
                index = SelectMessageEvent();
                break;
        }

    } else if (gDialogID != DIALOG_NONE) {
        render_dialog_entries();
    }

    return index;
}
