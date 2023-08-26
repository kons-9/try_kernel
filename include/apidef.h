#ifndef APIDEF_H
#define APIDEF_H

#include "typedef.h"

#define TA_HLNG 0x0000001
#define TA_USERBUF 0x0000020
#define TA_RNG0 0x0000000
#define TA_RNG1 0x0000100
#define TA_RNG2 0x0000200
#define TA_RNG3 0x0000300

typedef struct {
    ATR tskatr;
    FP task;
    PRI itskpri;
    SZ stksz;
    void *bufptr;
} T_CTSK;

ID tk_cre_tsk(T_CTSK *pk_ctsk);
ER tk_sta_tsk(ID tskid, INT stacd);
void tk_ext_tsk(void);

#endif
