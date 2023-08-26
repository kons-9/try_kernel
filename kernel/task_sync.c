#include "typedef.h"
#include <error.h>
#include <knldef.h>

ER tk_dly_tsk(RELTIM dlytim) {
    UINT intsts;
    ER err = E_OK;

    DI(intsts);

    if (dlytim > 0) {
        tqueue_remove_top(&ready_queue[cur_task->itskpri]);
        cur_task->state = TS_WAIT;
        cur_task->waifct = TWFCT_DLY;
        cur_task->waitim = dlytim + TIMER_PERIOD;
        cur_task->waierr = &err;
        tqueue_add_entry(&wait_queue, cur_task);
        scheduler();
    }

    EI(intsts);
    return err;
}

ER tk_slp_tsk(TMO tmout) {
    UINT intsts;
    ER err = E_OK;

    DI(intsts);

    if (cur_task->wupcnt > 0) {
        // already woken up
        cur_task->wupcnt--;
    } else {
        tqueue_remove_top(&ready_queue[cur_task->itskpri]);
        cur_task->state = TS_WAIT;
        cur_task->waifct = TWFCT_SLP;
        cur_task->waitim = (tmout == TMO_FEVR) ? TMO_FEVR : tmout + TIMER_PERIOD;
        cur_task->waierr = &err;

        tqueue_add_entry(&wait_queue, cur_task);
        scheduler();
    }

    EI(intsts);
    return err;
}

ER tk_wup_tsk(ID tskid) {
    TCB *tcb;
    UINT intsts;
    ER err = E_OK;
    if (tskid <= 0 || tskid > CNF_MAX_TSKID) {
        return E_ID;
    }

    DI(intsts);

    tcb = &tcb_tbl[tskid - 1];
    if ((tcb->state == TS_WAIT) && (tcb->waifct == TWFCT_SLP)) {
        tqueue_remove_entry(&wait_queue, tcb);
        tcb->state = TS_READY;
        tcb->waifct = TWFCT_NON;

        tqueue_add_entry(&ready_queue[tcb->itskpri], tcb);
        scheduler();
    } else if (tcb->state == TS_READY || tcb->state == TS_WAIT) {
        tcb->wupcnt++;
    } else {
        err = E_OBJ;
    }

    EI(intsts);
    return err;
}
