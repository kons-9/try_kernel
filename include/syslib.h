#ifndef SYSLIB_H
#define SYSLIB_H

#include "typedef.h"
//#include "sysdef.h"

static inline UW in_w(UW adr) {
    return *(_UW *)adr;
}

static inline void out_w(UW adr, UW data) {
    *(_UW *)adr = data;
}

// bit operation
#define OP_CLR 0x3000
static inline void clr_w(UW adr, UW data) {
    *(_UW *)(adr + OP_CLR) = data;
}
#define OP_SET 0x2000
static inline void set_w(UW adr, UW data) {
    *(_UW *)(adr + OP_SET) = data;
}
#define OP_XOR 0x2000
static inline void xset_w(UW adr, UW data) {
    *(_UW *)(adr + OP_XOR) = data;
}

static inline void set_primask(INT pm) {
    __asm__ volatile("msr primask, %0" ::"r"(pm));
}

static inline UW get_primask(void) {
    UW pm;
    __asm__ volatile("mrs %0, primask" : "=r"(pm));
    return pm;
}

void tm_com_init(void);

UINT tm_putstring(char *str);

#define DI(intsts) (intsts = get_primask(), set_primask(1))
#define EI(intsts) (set_primask(intsts))

#endif
