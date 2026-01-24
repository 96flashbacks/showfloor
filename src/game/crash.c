#include <ultra64.h>
#include <stdarg.h>
#include <string.h>

#include "sm64.h"

#if defined(TARGET_N64)

char *gCauseDesc[18] = {
    "Interrupt",
    "TLB modification",
    "TLB exception on load",
    "TLB exception on store",
    "Address error on load",
    "Address error on store",
    "Bus error on inst.",
    "Bus error on data",
    "System call exception",
    "Breakpoint exception",
    "Reserved instruction",
    "Coprocessor unusable",
    "Arithmetic overflow",
    "Trap exception",
    "Virtual coherency on inst.",
    "Floating point exception",
    "Watchpoint exception",
    "Virtual coherency on data",
};

char *gFpcsrDesc[6] = {
    "Unimplemented operation", "Invalid operation", "Division by zero", "Overflow", "Underflow",
    "Inexact operation",
};

extern uintptr_t gPhysicalFramebuffers[3];
extern u16 sRenderedFramebuffer;
extern u64 osClockRate;

struct {
    OSThread thread;
    u64 stack[0x800 / sizeof(u64)];
    OSMesgQueue mesgQueue;
    OSMesg mesg;
    char *errorMsg;
} gCrashLogger;

#define crash_logger_print rmonpf

void crash_logger_print_float_reg(s32 regNum, void *addr) {
    u32 bits;
    s32 exponent;

    bits = *(u32 *) addr;
    exponent = ((bits & 0x7f800000U) >> 0x17) - 0x7f;
    if ((exponent >= -0x7e && exponent <= 0x7f) || bits == 0) {
        crash_logger_print("F%02d:%.3e  ", regNum, *(f32 *) addr);
    } else {
        crash_logger_print("F%02d:--------- ", regNum);
    }
}

void crash_logger_print_fpcsr(u32 fpcsr) {
    s32 i;
    u32 bit;

    bit = 1 << 17;
    crash_logger_print("FPCSR:%08XH", fpcsr);
    for (i = 0; i < 6; i++) {
        if (fpcsr & bit) {
            crash_logger_print("  (%s)", gFpcsrDesc[i]);
            return;
        }
        bit >>= 1;
    }
}

void print_crash(OSThread *thread) {
    s16 cause;
    __OSThreadContext *tc = &thread->context;

    cause = (tc->cause >> 2) & 0x1f;
    if (cause == 23) { // EXC_WATCH
        cause = 16;
    }
    if (cause == 31) { // EXC_VCED
        cause = 17;
    }

    crash_logger_print("THREAD:%d  (%s)\n", thread->id, gCauseDesc[cause]);
    crash_logger_print("PC:%08XH   SR:%08XH   VA:%08XH\n\n", tc->pc, tc->sr, tc->badvaddr);
    osWritebackDCacheAll();
    crash_logger_print("AT:%08XH   V0:%08XH   V1:%08XH\n", (u32) tc->at, (u32) tc->v0,
                       (u32) tc->v1);
    crash_logger_print("A0:%08XH   A1:%08XH   A2:%08XH\n", (u32) tc->a0, (u32) tc->a1,
                       (u32) tc->a2);
    crash_logger_print("A3:%08XH   T0:%08XH   T1:%08XH\n", (u32) tc->a3, (u32) tc->t0,
                       (u32) tc->t1);
    crash_logger_print("T2:%08XH   T3:%08XH   T4:%08XH\n", (u32) tc->t2, (u32) tc->t3,
                       (u32) tc->t4);
    crash_logger_print("T5:%08XH   T6:%08XH   T7:%08XH\n", (u32) tc->t5, (u32) tc->t6,
                       (u32) tc->t7);
    crash_logger_print("S0:%08XH   S1:%08XH   S2:%08XH\n", (u32) tc->s0, (u32) tc->s1,
                       (u32) tc->s2);
    crash_logger_print("S3:%08XH   S4:%08XH   S5:%08XH\n", (u32) tc->s3, (u32) tc->s4,
                       (u32) tc->s5);
    crash_logger_print("S6:%08XH   S7:%08XH   T8:%08XH\n", (u32) tc->s6, (u32) tc->s7,
                       (u32) tc->t8);
    crash_logger_print("T9:%08XH   GP:%08XH   SP:%08XH\n", (u32) tc->t9, (u32) tc->gp,
                       (u32) tc->sp);
    crash_logger_print("S8:%08XH   RA:%08XH\n\n", (u32) tc->s8, (u32) tc->ra);
    crash_logger_print_fpcsr(tc->fpcsr);
    osWritebackDCacheAll();
    crash_logger_print("\n\n");
    crash_logger_print_float_reg(0, &tc->fp0.f.f_even);
    crash_logger_print_float_reg(2, &tc->fp2.f.f_even);
    crash_logger_print_float_reg(4, &tc->fp4.f.f_even);
    crash_logger_print("\n");
    crash_logger_print_float_reg(6, &tc->fp6.f.f_even);
    crash_logger_print_float_reg(8, &tc->fp8.f.f_even);
    crash_logger_print_float_reg(10, &tc->fp10.f.f_even);
    crash_logger_print("\n");
    crash_logger_print_float_reg(12, &tc->fp12.f.f_even);
    crash_logger_print_float_reg(14, &tc->fp14.f.f_even);
    crash_logger_print_float_reg(16, &tc->fp16.f.f_even);
    crash_logger_print("\n");
    crash_logger_print_float_reg(18, &tc->fp18.f.f_even);
    crash_logger_print_float_reg(20, &tc->fp20.f.f_even);
    crash_logger_print_float_reg(22, &tc->fp22.f.f_even);
    crash_logger_print("\n");
    crash_logger_print_float_reg(24, &tc->fp24.f.f_even);
    crash_logger_print_float_reg(26, &tc->fp26.f.f_even);
    crash_logger_print_float_reg(28, &tc->fp28.f.f_even);
    crash_logger_print("\n");
    crash_logger_print_float_reg(30, &tc->fp30.f.f_even);
    osWritebackDCacheAll();
}

void freeze_screen() {
    osViBlack(FALSE);
    osViSwapBuffer((u16 *)(gPhysicalFramebuffers[sRenderedFramebuffer] | 0xa0000000));
}

OSThread *get_crashed_thread(void) {
    OSThread *thread;

    thread = __osGetCurrFaultedThread();
    while (thread->priority != -1) {
        if (thread->priority > OS_PRIORITY_IDLE && thread->priority < OS_PRIORITY_APPMAX
            && (thread->flags & 3) != 0) {
            return thread;
        }
        thread = thread->tlnext;
    }
    return NULL;
}

void error(char *msg) {
    gCrashLogger.errorMsg = msg;
    *(vs8*)0 = 0;
}

void thread2_crash_logger(UNUSED void *arg) {
    OSMesg mesg;
    OSThread *thread;

    osSetEventMesg(OS_EVENT_CPU_BREAK, &gCrashLogger.mesgQueue, (OSMesg) 1);
    osSetEventMesg(OS_EVENT_FAULT, &gCrashLogger.mesgQueue, (OSMesg) 2);
    do {
        osRecvMesg(&gCrashLogger.mesgQueue, &mesg, 1);
        thread = get_crashed_thread();
    } while (thread == NULL);
    if (gCrashLogger.errorMsg) {
        crash_logger_print(gCrashLogger.errorMsg);
    } else {
        print_crash(thread);
    }
    freeze_screen();
    for (;;) {
    }
}

void crash_log_init(void) {
    osCreateMesgQueue(&gCrashLogger.mesgQueue, &gCrashLogger.mesg, 1);
    osCreateThread(
        &gCrashLogger.thread, 2, thread2_crash_logger, NULL,
        (u8 *) gCrashLogger.stack + sizeof(gCrashLogger.stack),
        OS_PRIORITY_RMON
    );
    osStartThread(&gCrashLogger.thread);
}

#endif
