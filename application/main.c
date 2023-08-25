#include <trykernel.h>

UINT cur_task = 0;
UINT next_task = 0;
#define MAX_FNC_ID 2
void *ctx_tbl[MAX_FNC_ID];

#define STACK_SIZE 1024
UW stack_1[STACK_SIZE];
UW stack_2[STACK_SIZE];

static void delay_ms(UINT ms) {
    UINT cnt = ms / TIMER_PERIOD;

    while (cnt) {
        if ((in_w(SYST_CSR) & SYST_CSR_COUNTFLAG) != 0) {
            cnt--;
        }
    }
}

void task_1(void) {
    while (1) {
        tm_putstring("Task 1: Hello World!\n");
        out_w(GPIO_OUT_SET, (1 << 25));
        delay_ms(500);
        next_task = 2;
        dispatch();
    }
}

void task_2(void) {
    while (1) {
        tm_putstring("Task 2: Hello World!\n");
        out_w(GPIO_OUT_CLR, (1 << 25));
        delay_ms(500);
        next_task = 1;
        dispatch();
    }
}

int main(void) {
    tm_com_init();
    tm_putstring("Initilizing...\n");

    ctx_tbl[0] = make_context(&stack_1[STACK_SIZE], STACK_SIZE, task_1);
    ctx_tbl[1] = make_context(&stack_2[STACK_SIZE], STACK_SIZE, task_2);

    next_task = 1;
    tm_putstring("Starting...\n");
    dispatch();

    return 0;
}
