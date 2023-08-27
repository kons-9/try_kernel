#ifndef APIDEF_H
#define APIDEF_H

#include "typedef.h"

#define TMO_POL (0)    // time out poll
#define TMO_FEVR (-1)  // time out forever

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


typedef struct t_cflg {
    ATR flgatr;
    UINT iflgptn;
} T_CFLG;

// input (pointer to information of create task)
ID tk_cre_tsk(T_CTSK *pk_ctsk);
// input (task id, status code)
ER tk_sta_tsk(ID tskid, INT stacd);
void tk_ext_tsk(void);
// input (delay time)
ER tk_dly_tsk(RELTIM dlytim);

ER tk_slp_tsk(TMO tmout);
ER tk_wup_tsk(ID tskid);

#define TA_TFIFO 0x00000000
#define TA_TPRI 0x00000001

#define TA_FIRST 0x00000000
#define TA_CNT 0x00000002

#define TA_WSGL 0x00000000
#define TA_WMUL 0x00000008
// event flag
ID tk_cre_flg(const T_CFLG *pk_cflg);
ER tk_set_flg(ID flgid, UINT setptn);
ER tk_clr_flg(ID flgid, UINT clrptn);
ER tk_wai_flg(ID flgid, UINT waiptn, UINT wfmode, UINT *p_flgptn, TMO tmout);

#define TWF_ANDW 0x00000000
#define TWF_ORW 0x00000001
#define TWF_CLR 0x00000010
#define TWF_BITCLR 0x00000020

#endif
