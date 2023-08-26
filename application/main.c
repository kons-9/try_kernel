#include <trykernel.h>

ID tskid_1;
ID tskid_2;
UW tskstk_1[1024 / sizeof(UW)];
UW tskstk_2[1024 / sizeof(UW)];
void task_1(void);
void task_2(void);

T_CTSK ctsk_1 = {
    .tskatr = TA_HLNG | TA_RNG3 | TA_USERBUF,
    .task = task_1,
    .itskpri = 10,
    .stksz = 1024,
    .bufptr = tskstk_1,
};

T_CTSK ctsk_2 = {
    .tskatr = TA_HLNG | TA_RNG3 | TA_USERBUF,
    .task = task_2,
    .itskpri = 10,
    .stksz = 1024,
    .bufptr = tskstk_2,
};


static void delay_ms(UINT ms) {
    UINT cnt = ms / TIMER_PERIOD;

    while (cnt) {
        if ((in_w(SYST_CSR) & SYST_CSR_COUNTFLAG) != 0) {
            cnt--;
        }
    }
}

void task_1(void) {
    tm_putstring("Task 1: Hello World!\n");
    tk_ext_tsk();
}

void task_2(void) {
    tm_putstring("Task 2: Hello World!\n");
    tk_ext_tsk();
}

int usermain(void) {
    tm_putstring("Initilizing...\n");

    tskid_1 = tk_cre_tsk(&ctsk_1);
    tk_sta_tsk(tskid_1, 0);

    tskid_2 = tk_cre_tsk(&ctsk_2);
    tk_sta_tsk(tskid_2, 0);

    return 0;
}
