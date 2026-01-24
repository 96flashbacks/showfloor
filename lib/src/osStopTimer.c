#include "libultra_internal.h"
#include "osint.h"

#pragma GCC diagnostic ignored "-Wunused-but-set-variable"

// FROM https://github.com/ModernN64SDKArchives/libultra_modern
int osStopTimer(OSTimer* t) {
    register u32 savedMask;
    OSTimer* timep;

    if (t->next == NULL) {
        return -1;
    }

    savedMask = __osDisableInt();
    timep = t->next;

    if (timep != __osTimerList) {
        timep->remaining += t->remaining;
    }

    t->prev->next = t->next;
    t->next->prev = t->prev;
    t->next = NULL;
    t->prev = NULL;

    if (__osTimerList->next == __osTimerList) {
        __osSetCompare(0);
    }

    __osRestoreInt(savedMask);
    return 0;
}
