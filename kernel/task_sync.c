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
