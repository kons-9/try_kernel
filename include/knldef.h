#ifndef KNLDEF_H
#define KNLDEF_H

#ifndef TRYKERNEL_H
#include "trykernel.h"
#endif

extern void Reset_Handler(void);
extern int main(void);
extern int usermain(void);

#define SCB_ICSR 0xE000ED04
#define ICSR_PENDSVSET (1 << 28)

extern void dispatch_entry(void);
static inline void dispatch(void) {
    out_w(SCB_ICSR, ICSR_PENDSVSET);
}

// extern void scheduler(void);
extern void *make_context(UW *sp, UINT ssize, void (*fp)());

typedef enum {
    TS_NONEXIST = 0,
    TS_READY = 1,
    TS_WAIT = 2,
    TS_DORMANT = 3,
} TSTAT;

typedef struct st_tcb {
    void *context;
    struct st_tcb *pre;
    struct st_tcb *next;

    TSTAT state;   // task state
    FP tskadr;     // task address
    PRI itskpri;   // task priority
    void *stkadr;  // stack address
    SZ stksz;      // stack size
} TCB;

extern TCB tcb_tbl[];
extern TCB *ready_queue[];
extern TCB *cur_task;
extern TCB *sche_task;

extern void tqueue_add_entry(TCB **queue, TCB *tcb);
extern void tqueue_remove_top(TCB **que);
extern void tqueue_remove_entry(TCB **que, TCB *tcb);

extern void scheduler(void);

#endif
