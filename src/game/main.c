#include <ultra64.h>
#include <stdio.h>

#include "sm64.h"
#include "audio/external.h"
#include "game_init.h"
#include "memory.h"
#include "sound_init.h"
#include "profiler.h"
#include "buffers/buffers.h"
#include "segments.h"
#include "segment_symbols.h"
#include "main.h"

// Message IDs
#define MESG_SP_COMPLETE 100
#define MESG_DP_COMPLETE 101
#define MESG_VI_VBLANK 102
#define MESG_START_GFX_SPTASK 103

OSThread gRmonThread; // unused?
OSThread gIdleThread;
OSThread gMainThread;
OSThread gGameLoopThread;
OSThread gSoundThread;

OSIoMesg gDmaIoMesg;
OSMesg gMainReceivedMesg;

OSMesgQueue gDmaMesgQueue;
OSMesgQueue gSIEventMesgQueue;
OSMesgQueue gPIMesgQueue;
OSMesgQueue gIntrMesgQueue;
OSMesgQueue gSPTaskMesgQueue;

OSMesg gDmaMesgBuf[1];
OSMesg gPIMesgBuf[32];
OSMesg gSIEventMesgBuf[1];
OSMesg gIntrMesgBuf[16];
OSMesg gUnknownMesgBuf[16];

struct VblankHandler *gVblankHandler1 = NULL;
struct VblankHandler *gVblankHandler2 = NULL;
struct SPTask *gActiveSPTask = NULL;
struct SPTask *sCurrentAudioSPTask = NULL;
struct SPTask *sCurrentDisplaySPTask = NULL;
struct SPTask *sNextAudioSPTask = NULL;
struct SPTask *sNextDisplaySPTask = NULL;
u32 gNumVblanks = 0;
s16 audCheck = 0;
s8 gDebugLevelSelect = 0;
s8 sysGvdActive = 0;

s8 gShowProfiler = FALSE;
s8 gShowDebugText = 0;

// unused
void handle_debug_key_sequences(void) {
    static u16 sProfilerKeySequence[] = {
        U_JPAD, U_JPAD, D_JPAD, D_JPAD, L_JPAD, R_JPAD, L_JPAD, R_JPAD
    };
    static u16 sDebugTextKeySequence[] = { D_JPAD, D_JPAD, U_JPAD, U_JPAD,
                                           L_JPAD, R_JPAD, L_JPAD, R_JPAD };
    static s16 sProfilerKey = 0;
    static s16 sDebugTextKey = 0;
    if (gPlayer3Controller->buttonPressed != 0) {
        if (sProfilerKeySequence[sProfilerKey++] == gPlayer3Controller->buttonPressed) {
            if (sProfilerKey == ARRAY_COUNT(sProfilerKeySequence)) {
                sProfilerKey = 0, gShowProfiler ^= 1;
            }
        } else {
            sProfilerKey = 0;
        }

        if (sDebugTextKeySequence[sDebugTextKey++] == gPlayer3Controller->buttonPressed) {
            if (sDebugTextKey == ARRAY_COUNT(sDebugTextKeySequence)) {
                sDebugTextKey = 0, gShowDebugText ^= 1;
            }
        } else {
            sDebugTextKey = 0;
        }
    }
}

void unknown_main_func(void) {
    // uninitialized
    OSTime time;
    u32 b;
#ifdef AVOID_UB
    time = 0;
    b = 0;
#endif

    osSetTime(time);
    osMapTLB(0, b, NULL, 0, 0, 0);
    osUnmapTLBAll();

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnonnull"
    sprintf(NULL, NULL);
#pragma GCC diagnostic pop
}

#define STACK_CHECK_CODE 0x8877665544332211LL
#define GET_AS_U64(v) ((u64*)&v)

void InitStackMemory(void) {
#ifdef DEVELOP
    GET_AS_U64(gIdleThreadStack)[256] = STACK_CHECK_CODE;
    GET_AS_U64(gThread3Stack)[256] = STACK_CHECK_CODE;
    GET_AS_U64(gThread4Stack)[256] = STACK_CHECK_CODE;
    GET_AS_U64(gThread5Stack)[256] = STACK_CHECK_CODE;
#endif
}

void CheckStackMemory(void) {
#ifdef DEVELOP
    if (GET_AS_U64(gIdleThreadStack)[256] != STACK_CHECK_CODE)
        rmonpf(("idle thread stack over\n"));
    if (GET_AS_U64(gThread3Stack)[256] != STACK_CHECK_CODE)
        rmonpf(("main thread stack over\n"));
    if (GET_AS_U64(gThread4Stack)[256] != STACK_CHECK_CODE)
        rmonpf(("audio thread stack over\n"));
    if (GET_AS_U64(gThread5Stack)[256] != STACK_CHECK_CODE)
        rmonpf(("graph thread stack over\n"));
#endif
}

void setup_mesg_queues(void) {
    osCreateMesgQueue(&gDmaMesgQueue, gDmaMesgBuf, ARRAY_COUNT(gDmaMesgBuf));
    osCreateMesgQueue(&gSIEventMesgQueue, gSIEventMesgBuf, ARRAY_COUNT(gSIEventMesgBuf));
    osSetEventMesg(OS_EVENT_SI, &gSIEventMesgQueue, NULL);

    osCreateMesgQueue(&gSPTaskMesgQueue, gUnknownMesgBuf, ARRAY_COUNT(gUnknownMesgBuf));
    osCreateMesgQueue(&gIntrMesgQueue, gIntrMesgBuf, ARRAY_COUNT(gIntrMesgBuf));
    osViSetEvent(&gIntrMesgQueue, (OSMesg) MESG_VI_VBLANK, 1);

    osSetEventMesg(OS_EVENT_SP, &gIntrMesgQueue, (OSMesg) MESG_SP_COMPLETE);
    osSetEventMesg(OS_EVENT_DP, &gIntrMesgQueue, (OSMesg) MESG_DP_COMPLETE);
}

void alloc_pool(void) {
    void *start = (void *) SEG_POOL_START;
    void *end = (void *) SEG_POOL_END;

    main_pool_init(start, end);
    gEffectsMemoryPool = mem_pool_init(0x4000, MEMORY_POOL_LEFT);
}

void create_thread(OSThread *thread, OSId id, void (*entry)(void *), void *arg, void *sp, OSPri pri) {
    thread->next = NULL;
    thread->queue = NULL;
    osCreateThread(thread, id, entry, arg, sp, pri);
}

void receive_new_tasks(void) {
    struct SPTask *spTask;

    while (osRecvMesg(&gSPTaskMesgQueue, (OSMesg *) &spTask, OS_MESG_NOBLOCK) != -1) {
        spTask->state = SPTASK_STATE_NOT_STARTED;
        switch (spTask->task.t.type) {
            case 2:
                sNextAudioSPTask = spTask;
                break;
            case 1:
                sNextDisplaySPTask = spTask;
                break;
        }
    }

    if (sCurrentAudioSPTask == NULL && sNextAudioSPTask != NULL) {
        sCurrentAudioSPTask = sNextAudioSPTask;
        sNextAudioSPTask = NULL;
    }

    if (sCurrentDisplaySPTask == NULL && sNextDisplaySPTask != NULL) {
        sCurrentDisplaySPTask = sNextDisplaySPTask;
        sNextDisplaySPTask = NULL;
    }
}

void start_sptask(s32 taskType) {
    UNUSED u8 filler[4];

    if (taskType == M_AUDTASK) {
        rmonpf(("(AUD) start\n"));
        audCheck = 1;
        gActiveSPTask = sCurrentAudioSPTask;
    } else {
        rmonpf(("(GFX) strat\n"));
        gActiveSPTask = sCurrentDisplaySPTask;
        // osDpSetStatus(DPC_CLR_CLOCK_CTR | DPC_CLR_CMD_CTR | DPC_CLR_PIPE_CTR | DPC_CLR_TMEM_CTR);
    }

    osSpTaskLoad(&gActiveSPTask->task);
    osSpTaskStartGo(&gActiveSPTask->task);
    gActiveSPTask->state = SPTASK_STATE_RUNNING;
}

void interrupt_gfx_sptask(void) {
    if (gActiveSPTask->task.t.type == M_GFXTASK) {
        rmonpf(("(GFX) yield\n"));
        gActiveSPTask->state = SPTASK_STATE_INTERRUPTED;
        osSpTaskYield();
    }
}

void start_gfx_sptask(void) {
    if (gActiveSPTask == NULL && sCurrentDisplaySPTask != NULL
        && sCurrentDisplaySPTask->state == SPTASK_STATE_NOT_STARTED) {
        profiler_log_gfx_time(TASKS_QUEUED);
        start_sptask(M_GFXTASK);
    }
}

void handle_vblank(void) {
    UNUSED u8 filler[4];

    gNumVblanks++;

    receive_new_tasks();

    if (audCheck == 1)
        rmonpf(("audio task broken\n")); /* DEBUG */

    // First try to kick off an audio task. If the gfx task is currently
    // running, we need to asynchronously interrupt it -- handle_sp_complete
    // will pick up on what we're doing and start the audio task for us.
    // If there is already an audio task running, there is nothing to do.
    // If there is no audio task available, try a gfx task instead.
    if (sCurrentAudioSPTask != NULL) {
        if (gActiveSPTask != NULL) {
            interrupt_gfx_sptask();
        } else {
            profiler_log_vblank_time();
            start_sptask(M_AUDTASK);
        }
    } else {
        if (gActiveSPTask == NULL && sCurrentDisplaySPTask != NULL
            && sCurrentDisplaySPTask->state != SPTASK_STATE_FINISHED) {
            profiler_log_gfx_time(TASKS_QUEUED);
            start_sptask(M_GFXTASK);
        }
    }

    // Notify the game loop about the vblank.
    if (gVblankHandler1 != NULL) {
        osSendMesg(gVblankHandler1->queue, gVblankHandler1->msg, OS_MESG_NOBLOCK);
    }
    if (gVblankHandler2 != NULL) {
        osSendMesg(gVblankHandler2->queue, gVblankHandler2->msg, OS_MESG_NOBLOCK);
    }
}

void handle_sp_complete(void) {
    struct SPTask *curSPTask = gActiveSPTask;

    gActiveSPTask = NULL;

    if (curSPTask->state == SPTASK_STATE_INTERRUPTED) {
        // handle_vblank tried to start an audio task while there was already a
        // gfx task running, so it had to interrupt the gfx task. That interruption
        // just finished.
        if (osSpTaskYielded(&curSPTask->task) == 0) {
            // The gfx task completed before we had time to interrupt it.
            // Mark it finished, just like below.
            curSPTask->state = SPTASK_STATE_FINISHED;
            rmonpf(("(GFX) abort\n"));
            profiler_log_gfx_time(RSP_COMPLETE);
        }

        // Start the audio task, as expected by handle_vblank.
        profiler_log_vblank_time();
        start_sptask(M_AUDTASK);
    } else {
        curSPTask->state = SPTASK_STATE_FINISHED;
        if (curSPTask->task.t.type == M_AUDTASK) {
            rmonpf(("(AUD) end\n"));
            audCheck = 0;
            // After audio tasks come gfx tasks.
            profiler_log_vblank_time();
            if (sCurrentDisplaySPTask != NULL
                && sCurrentDisplaySPTask->state != SPTASK_STATE_FINISHED) {
                if (sCurrentDisplaySPTask->state != SPTASK_STATE_INTERRUPTED) {
                    profiler_log_gfx_time(TASKS_QUEUED);
                }
                start_sptask(M_GFXTASK);
            }
            sCurrentAudioSPTask = NULL;
            if (curSPTask->msgqueue != NULL) {
                osSendMesg(curSPTask->msgqueue, curSPTask->msg, OS_MESG_NOBLOCK);
            }
        } else {
            // The SP process is done, but there is still a Display Processor notification
            // that needs to arrive before we can consider the task completely finished and
            // null out sCurrentDisplaySPTask. That happens in handle_dp_complete.
            profiler_log_gfx_time(RSP_COMPLETE);
        }
    }
}

void handle_dp_complete(void) {
    // Gfx SP task is completely done.
    if (sCurrentDisplaySPTask->msgqueue != NULL) {
        osSendMesg(sCurrentDisplaySPTask->msgqueue, sCurrentDisplaySPTask->msg, OS_MESG_NOBLOCK);
    }
    rmonpf(("(GFX) end\n"));
    profiler_log_gfx_time(RDP_COMPLETE);
    //  sysTmrst = TRUE;
    //  ResetTime();
    sCurrentDisplaySPTask->state = SPTASK_STATE_FINISHED_DP;
    sCurrentDisplaySPTask = NULL;
}

void thread3_main(UNUSED void *arg) {
    create_thread(&gSoundThread, 4, thread4_sound, NULL, gThread4Stack + 0x2000, 20);
    osStartThread(&gSoundThread);

    create_thread(&gGameLoopThread, 5, thread5_game_loop, NULL, gThread5Stack + 0x2000, 10);
    osStartThread(&gGameLoopThread);

    setup_mesg_queues();
    alloc_pool();
    load_engine_code_segment(); // this line is not in the Feburary 1996 backup, but WHERE ELSE WOULD
                                // THE DATA BE LOADED THEN?

    while (TRUE) {
        OSMesg msg;

        osRecvMesg(&gIntrMesgQueue, &msg, OS_MESG_BLOCK);
        switch ((uintptr_t) msg) {
            case MESG_VI_VBLANK:
                handle_vblank();
                break;
            case MESG_SP_COMPLETE:
                handle_sp_complete();
                break;
            case MESG_DP_COMPLETE:
                handle_dp_complete();
                break;
            case MESG_START_GFX_SPTASK:
                start_gfx_sptask();
                break;
        }
        CheckStackMemory();
    }
}

void set_vblank_handler(s32 index, struct VblankHandler *handler, OSMesgQueue *queue, OSMesg *msg) {
    handler->queue = queue;
    handler->msg = msg;

    switch (index) {
        case 1:
            gVblankHandler1 = handler;
            break;
        case 2:
            gVblankHandler2 = handler;
            break;
    }
}

void dispatch_audio_sptask(struct SPTask *spTask) {
    osWritebackDCacheAll();
    osSendMesg(&gSPTaskMesgQueue, spTask, OS_MESG_NOBLOCK);
}

void exec_display_list(struct SPTask *spTask) {
    osWritebackDCacheAll();
    spTask->state = SPTASK_STATE_NOT_STARTED;
    if (sCurrentDisplaySPTask == NULL) {
        sCurrentDisplaySPTask = spTask;
        sNextDisplaySPTask = NULL;
        osSendMesg(&gIntrMesgQueue, (OSMesg) MESG_START_GFX_SPTASK, OS_MESG_NOBLOCK);
    } else {
        sNextDisplaySPTask = spTask;
    }
}

/**
 * Initialize hardware, start main thread, then idle.
 */
void thread1_idle(UNUSED void *arg) {
    osCreateViManager(OS_PRIORITY_VIMGR);
    osViSetMode(&osViModeTable[OS_VI_NTSC_LAN1]);
    osViBlack(TRUE);
    osViSetSpecialFeatures(OS_VI_DITHER_FILTER_ON);
    osViSetSpecialFeatures(OS_VI_GAMMA_OFF);
    osCreatePiManager(OS_PRIORITY_PIMGR, &gPIMesgQueue, gPIMesgBuf, ARRAY_COUNT(gPIMesgBuf));
#ifdef DEVELOP
    /* Start RMON  */ // rmonthreadstack was removed off final I believe
    // create_thread(&gRmonThread, 0, rmonMain, NULL, rmonThreadStack+RMON_STACKSIZE64,
    // OS_PRIORITY_RMON); osStartThread(&gRmonThread);
#endif
    create_thread(&gMainThread, 3, thread3_main, NULL, gThread3Stack + 0x2000, 100);
    if (sysGvdActive == 0) {
        osStartThread(&gMainThread);
    }
    osSetThreadPri(NULL, 0);

    // halt
    while (TRUE) {
        ;
    }
}

#ifdef RAMROM_ARGUMENTS
/********************************************************************************/
/*	Read argument.
 */
/********************************************************************************/
static void ReadArgument(ArgRecord *argrec) {
    u32 i = 0;
    u64 *argpt = (ulong *) RAMROM_APP_WRITE_ADDR;

    for (i = 0; i < SC_ARGSIZE; i++, argpt++) {
        osPiRawReadIo((ulong) argpt, &argrec->word[index]);
    }
}

/********************************************************************************/
/*	Check debug options.
 */
/********************************************************************************/
static void CheckDebugOption(ArgRecord *argrec) {
    u8 *chrpt = argrec->byte;

    for (chrpt = argrec->byte; *chrpt != '\0'; chrpt++) {
        if (*chrpt == '-') {
            switch (*++chrpt) {
                case 'd':
                    gDebugLevelSelect = 1;
                    break;
                case 'g':
                    sysGvdActive = 1;
                    break;
            }
        }
    }
}
#endif

void main_func(void) {
#ifdef RAMROM_ARGUMENTS
    ArgRecord argrec;
#else
    UNUSED u8 filler[64];
#endif

    osInitialize();
    InitStackMemory();
#ifdef RAMROM_ARGUMENTS
    ReadArgument(&argrec);
    CheckDebugOption(&argrec);
#endif
    create_thread(&gIdleThread, 1, thread1_idle, NULL, gIdleThreadStack + 0x800, 100);
    osStartThread(&gIdleThread);
}
