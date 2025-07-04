#include "libultra_internal.h"
#include "macros.h"
#include "PR/os.h"

void __osDevMgrMain(void *args) {
    OSIoMesg *sp34;
    OSMesg sp30;
    OSMesg sp2c;
    s32 sp28;
    OSMgrArgs *sp24;
    sp34 = NULL;
    sp28 = 0;
    sp24 = (OSMgrArgs *) args;
    while (TRUE) {
        osRecvMesg(sp24->cmdQueue, (OSMesg) &sp34, OS_MESG_BLOCK);
        switch (sp34->hdr.type) {
            case 11:
                osRecvMesg(sp24->accessQueue, &sp2c, OS_MESG_BLOCK);
                sp28 = sp24->dma_func(OS_READ, sp34->devAddr, sp34->dramAddr, sp34->size);
                break;
            case 12:
                osRecvMesg(sp24->accessQueue, &sp2c, OS_MESG_BLOCK);
                sp28 = sp24->dma_func(OS_WRITE, sp34->devAddr, sp34->dramAddr, sp34->size);
                break;
            case 10:
                osSendMesg(sp34->hdr.retQueue, sp34, OS_MESG_NOBLOCK);
                sp28 = -1;
                break;
            default:
                sp28 = -1;
                break;
        }
        if (sp28 == 0) {
            osRecvMesg(sp24->eventQueue, &sp30, OS_MESG_BLOCK);
            osSendMesg(sp34->hdr.retQueue, sp34, OS_MESG_NOBLOCK);
            osSendMesg(sp24->accessQueue, NULL, OS_MESG_NOBLOCK);
        }
    }
}
