#include "typedef.h"
#include "knldef.h"

TCB *wait_queue;

void systimer_handler(void) {
    TCB *tcb;
    for (tcb = wait_queue; tcb != NULL; tcb = tcb->next) {
        if (tcb->waitim == TMO_FEVR) {
            // forever
            continue;
        } else if (tcb->waitim > TIMER_PERIOD) {
            tcb->waitim -= TIMER_PERIOD;
        } else {
            // wake up
            tqueue_remove_entry(&wait_queue, tcb);
            *tcb->waierr = E_OK;
            tcb->state = TS_READY;
            tcb->waifct = TWFCT_NON;

            tqueue_add_entry(&ready_queue[tcb->itskpri], tcb);
        }
    }
    scheduler();
}
