#include "macros.h"
#include "libultra_internal.h"
#include "PR/rcp.h"
#include "piint.h"
#include "stdlib.h"
#include "stdarg.h"

typedef struct {
    /* 0x00 */ u32 magic; // IS64
    /* 0x04 */ u32 get;
    /* 0x08 */ u8 unk_08[0x14 - 0x08];
    /* 0x14 */ u32 put;
    /* 0x18 */ u8 unk_18[0x20 - 0x18];
    /* 0x20 */ u8 data[0x10000 - 0x20];
} ISVDbg;

#define	IO_WRITE(addr,data)	(*(vu32 *)PHYS_TO_K1(addr)=(u32)(data))

#define IS64_MAGIC  0x49533634  // 'IS64'

typedef char *outfun(char*,const char*,size_t);
extern int _Printf(outfun prout, char *arg, const char *fmt, va_list args);

#ifdef DEVELOP
static ISVDbg* gISVDbgPrnAdrs;

static void* __printfunc = NULL;

// implement functions from libultra that weren't added into the vanilla game (forgot the source, might be depot.tar or libultra_modern)
s32 osPiRawWriteIo(u32 devAddr, u32 data) {
    register u32 stat;
    WAIT_ON_IO_BUSY(stat);
    IO_WRITE(osRomBase | devAddr, data);
    return(0);
}

s32 osPiReadIo(u32 devAddr, u32 *data) {
    register s32 ret;
    __osPiGetAccess();
    ret = osPiRawReadIo(devAddr, data);
    __osPiRelAccess();
    return(ret);
}

s32 osPiWriteIo(u32 devAddr, u32 data) {
    register s32 ret;
    __osPiGetAccess();
    ret = osPiRawWriteIo(devAddr, data);
    __osPiRelAccess();
    return(ret);
}

// isPrintfInit, is_proutSyncPrintf, and osSyncPrintf are from https://github.com/ModernN64SDKArchives/libultra_modern
// the following functions originally used the Expanded Peripheral Interface variants (osEPiReadIo, osEPiWriteIo) but
// those only existed in later OS versions so they have been modified to use the regular Peripheral Interface functions
static void isPrintfInit(void) {
    osPiWriteIo((u32)&gISVDbgPrnAdrs->put, 0);
    osPiWriteIo((u32)&gISVDbgPrnAdrs->get, 0);
    osPiWriteIo((u32)gISVDbgPrnAdrs, IS64_MAGIC);
}

static void* is_proutSyncPrintf(UNUSED void* arg, const u8* str, u32 count) {
    u32 data;
    s32 p;
    s32 start;
    s32 end;

    if (gISVDbgPrnAdrs == NULL) {
        return (void*)0;
    }

    osPiReadIo((u32)&gISVDbgPrnAdrs->magic, &data);
    if (data != IS64_MAGIC) {
        return (void*)1;
    }
    osPiReadIo((u32)&gISVDbgPrnAdrs->get, &data);
    p = data;
    osPiReadIo((u32)&gISVDbgPrnAdrs->put, &data);

    start = data;
    end = start + count;

    if (end >= 0xffe0) {
        end -= 0xffe0;
        if (p < end || start < p) {
            return (void*)1;
        }
    } else {
        if (start < p && p < end) {
            return (void*)1;
        }
    }
    while (count) {
        if (*str != '\0') {
            s32 shift = start & 3;
            u32 addr = (u32)&gISVDbgPrnAdrs->data[start & 0xFFFFFFC];

            shift = (3 - shift) * 8;

            osPiReadIo(addr, &data);
            osPiWriteIo(addr, (data & ~(0xff << shift)) | (*str << shift));

            start++;
            if (start >= 0xffe0) {
                start -= 0xffe0;
            }
        }
        count--;
        str++;
    }
    osPiWriteIo((u32)&gISVDbgPrnAdrs->put, start);

    return (void*)1;
}

void osSyncPrintf(const char* fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    if (__printfunc == NULL) { // osSyncPrintf has been modified here to automatically initialize IS-Viewer64 values
        gISVDbgPrnAdrs = (ISVDbg*)0x13FF0000; // cen64 has this hardcoded according to https://github.com/HackerN64/HackerSM64
        __printfunc = is_proutSyncPrintf;
        isPrintfInit();
    }
    if (__printfunc != NULL) {
        _Printf(__printfunc, NULL, fmt, ap);
    }
    va_end(ap);
}
#endif
