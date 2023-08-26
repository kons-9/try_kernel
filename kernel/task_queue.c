#include "config.h"
#include "knldef.h"

// queue: pointer to the head of the queue
// tcb: pointer to the TCB to be added
void tqueue_add_entry(TCB **queue, TCB *tcb) {
    TCB *queue_end;
    if (*queue == NULL) {
        *queue = tcb;
        tcb->pre = tcb;
        tcb->next = NULL;
    } else {
        queue_end = (*queue)->pre;
        queue_end->next = tcb;
        tcb->pre = queue_end;
        (*queue)->pre = tcb;
        tcb->next = NULL;
    }
}

// queue: pointer to the head of the queue
void tqueue_remove_top(TCB **queue) {
    TCB *top;

    if (*queue == NULL)
        return;

    top = *queue;
    TCB *next = top->next;
    if (next == NULL) {
        *queue = NULL;
    } else {
        next->pre = top->pre;
        *queue = next;
    }
}

void tqueue_remove_entry(TCB **queue, TCB *tcb) {
    if (*queue == tcb) {
        tqueue_remove_top(queue);
        return;
    }
    TCB *pre = tcb->pre;
    TCB *next = tcb->next;

    pre->next = next;
    if (next == NULL) {
        (*queue)->pre = pre;
    } else {
        next->pre = pre;
    }
}
