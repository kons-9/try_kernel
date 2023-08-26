#include "config.h"
#include "knldef.h"
#include "typedef.h"

TCB *ready_queue[CNF_MAX_TSKPRI];
TCB *cur_task;
TCB *sche_task;
UW disp_running;

void scheduler(void) {
    INT i;

    for (i = 0; i < CNF_MAX_TSKPRI; i++) {
        if (ready_queue[i] != NULL) {
            break;
        }
    }

    if (i < CNF_MAX_TSKPRI) {
        sche_task = ready_queue[i];
    } else {
        sche_task = NULL;
    }
    if (cur_task != sche_task && !disp_running) {
        dispatch();
    }
}
