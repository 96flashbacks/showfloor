#include "libultra_internal.h"
#include "osint.h"

#pragma GCC diagnostic ignored "-Wunused-but-set-variable"

u32 osSetTimer(OSTimer *timer, OSTime countdown, OSTime interval, OSMesgQueue *mq, OSMesg msg) {
    OSTime time;

    timer->next = NULL;
    timer->prev = NULL;
    timer->interval = interval;
    timer->remaining = countdown != 0 ? countdown : interval;
    timer->mq = mq;
    timer->msg = msg;
    time = __osInsertTimer(timer);
    if (__osTimerList->next == timer) {
        __osSetTimerIntr(time);
    }

    return 0;
}
