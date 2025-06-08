#include "macros.h"
#include "libultra_internal.h"

extern u32 __osBbEepromSize;

s32 __osEepStatus(OSMesgQueue *, OSContStatus *);
s32 osEepromProbe(UNUSED OSMesgQueue *mq) {
    s32 ret = 0;
    OSContStatus status;

    __osSiGetAccess();
    ret = __osEepStatus(mq, &status);
    ret = (ret == 0 && (status.type & CONT_EEPROM) != 0) ? EEPROM_TYPE_4K : 0;
    __osSiRelAccess();
    return ret;
}
