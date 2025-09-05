#include "macros.inc"
.set UCODE_SIZE, 0x800

.section .text

.balign 16
glabel rspF3DBootStart
    .incbin "lib/PR/102695/rspboot.bin"
.balign 16
glabel rspF3DBootEnd

.balign 16
glabel rspF3DStart
    #ifdef FAST3D_20E
        .incbin "lib/PR/2.0E/gspFast3D.text.bin"
    #else
        .incbin "lib/PR/102695/gspFast3D.text.bin"
    #endif
glabel rspF3DEnd

/* Audio Bins */

.balign 16
glabel rspAspMainStart
    #ifdef FAST3D_20E
        .incbin "lib/PR/2.0E/aspMain.text.bin"
    #else
        .incbin "lib/PR/102695/aspMain.text.bin"
    #endif
glabel rspAspMainEnd

/* DATA SECTION START */

.section .rodata

.balign 16
glabel rspF3DDataStart
    #ifdef FAST3D_20E
        .incbin "lib/PR/2.0E/gspFast3D.data.bin"
    #else
        .incbin "lib/PR/102695/gspFast3D.data.bin"
    #endif
glabel rspF3DDataEnd

/* Audio Data */

.balign 16
glabel rspAspMainDataStart
    #ifdef FAST3D_20E
        .incbin "lib/PR/2.0E/aspMain.data.bin"
    #else
        .incbin "lib/PR/102695/aspMain.data.bin"
    #endif
glabel rspAspMainDataEnd
