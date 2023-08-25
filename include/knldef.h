#ifndef KNLDEF_H
#define KNLDEF_H

#include "syslib.h"
extern void Reset_Handler(void);
extern int main(void);

#define SCB_ICSR 0xE000ED04
#define ICSR_PENDSVSET (1 << 28)

static inline void dispatch(void) {
    out_w(SCB_ICSR, ICSR_PENDSVSET);
}

// extern void scheduler(void);
extern void *make_context(UW *sp, UINT ssize, void (*fp)());

#endif
