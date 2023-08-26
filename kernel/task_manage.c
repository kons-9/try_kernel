#include "knldef.h"
#include "config.h"
#include "typedef.h"
#include "apidef.h"
#include "error.h"

TCB tcb_tbl[CNF_MAX_TSKID];

// return: task ID, value less than 0 if error
ID tk_cre_tsk(T_CTSK *pk_ctsk) {
    UINT intsts;
    ID tskid;
    INT empty_idx;

    if ((pk_ctsk->tskatr & ~TA_RNG3) != (TA_HLNG | TA_USERBUF)) {
        return E_RSATR;
    }

    if (pk_ctsk->itskpri <= 0 || pk_ctsk->itskpri > CNF_MAX_TSKPRI) {
        return E_PAR;
    }

    if (pk_ctsk->stksz == 0) {
        return E_PAR;
    }

    DI(intsts);

    for (empty_idx = 0; empty_idx < CNF_MAX_TSKID; empty_idx++) {
        if (tcb_tbl[empty_idx].state == TS_NONEXIST)
            break;
    }

    if (empty_idx < CNF_MAX_TSKID) {
        tcb_tbl[empty_idx].state = TS_DORMANT;
        tcb_tbl[empty_idx].pre = NULL;
        tcb_tbl[empty_idx].next = NULL;

        tcb_tbl[empty_idx].tskadr = pk_ctsk->task;
        tcb_tbl[empty_idx].itskpri = pk_ctsk->itskpri;
        tcb_tbl[empty_idx].stksz = pk_ctsk->stksz;
        tcb_tbl[empty_idx].stkadr = pk_ctsk->bufptr;
        tskid = empty_idx + 1;
    } else {
        tskid = (ID)E_LIMIT;
    }

    EI(intsts);
    return tskid;
}

ER tk_sta_tsk(ID tskid, INT stacd) {
    TCB *tcb;
    UINT intsts;
    ER err = E_OK;
    if (tskid <= 0 || tskid > CNF_MAX_TSKID) {
        return E_ID;
    }

    DI(intsts);
    tcb = &tcb_tbl[tskid - 1];
    if (tcb->state == TS_DORMANT) {
        tcb->state = TS_READY;
        tcb->context = make_context((UW *)tcb->stkadr, tcb->stksz, tcb->tskadr);
        tqueue_add_entry(&ready_queue[tcb->itskpri], tcb);
        scheduler();
    } else {
        err = E_OBJ;
    }

    EI(intsts);
    return err;
}

void tk_ext_tsk(void) {
    UINT intsts;

    DI(intsts);

    cur_task->state = TS_DORMANT;
    tqueue_remove_top(&ready_queue[cur_task->itskpri]);
    scheduler();

    EI(intsts);
}
