#include "apidef.h"
#include <trykernel.h>

ID tskid_1;
ID tskid_2;
UW tskstk_1[1024 / sizeof(UW)];
UW tskstk_2[1024 / sizeof(UW)];
void task_1(INT stacd, void *exinf);
void task_2(INT stacd, void *exinf);

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

ID tskid_led;
ID tskid_btn;
UW tskstk_led[1024 / sizeof(UW)];
UW tskstk_btn[1024 / sizeof(UW)];
void task_led(INT stacd, void *exinf);
void task_btn(INT stacd, void *exinf);
T_CTSK ctsk_led = {
    .tskatr = TA_HLNG | TA_RNG3 | TA_USERBUF,
    .task = task_led,
    .itskpri = 10,
    .stksz = 1024,
    .bufptr = tskstk_led,
};

T_CTSK ctsk_btn = {
    .tskatr = TA_HLNG | TA_RNG3 | TA_USERBUF,
    .task = task_btn,
    .itskpri = 10,
    .stksz = 1024,
    .bufptr = tskstk_btn,
};


void task_led(INT stacd, void *exinf) {
    while (1) {
        tk_slp_tsk(TMO_FEVR);
        out_w(GPIO_OUT_SET, (1 << 25));
        tk_dly_tsk(1000);
        out_w(GPIO_OUT_CLR, (1 << 25));
    }
}

void task_btn(INT stacd, void *exinf) {
    UW btn, btn0;

    out_w(GPIO(14), (in_w(GPIO(14)) | GPIO_PUE) & ~GPIO_PDE);
    out_w(GPIO_OE_CLR, (1 << 14));
    out_w(GPIO_CTRL(14), 5);

    // prev button state
    btn0 = in_w(GPIO_IN) & (1 << 14);

    while (1) {
        btn = in_w(GPIO_IN) & (1 << 14);
        if (btn != btn0) {
            if (btn == 0) {
                tm_putstring("Button pressed!\n");
                tk_wup_tsk(tskid_led);
            }
            btn0 = btn;
        }
        tk_dly_tsk(100);
    }
}

void task_1(INT stacd, void *exinf) {
    tm_putstring("Task 1: Hello World!\n");
    while (1) {
        out_w(GPIO_OUT_XOR, (1 << 25));
        tk_dly_tsk(500);
    }
}

void task_2(INT stacd, void *exinf) {
    tm_putstring("Task 2: Hello World!\n");
    while (1) {
        tm_putstring("Task 2: Waiting for 1 second...\n");
        tk_dly_tsk(1000);
    }
}

int usermain(void) {
    tm_putstring("Initilizing...\n");

    // tskid_1 = tk_cre_tsk(&ctsk_1);
    // tk_sta_tsk(tskid_1, 0);
    //
    // tskid_2 = tk_cre_tsk(&ctsk_2);
    // tk_sta_tsk(tskid_2, 0);

    tskid_led = tk_cre_tsk(&ctsk_led);
    tk_sta_tsk(tskid_led, 0);

    tskid_btn = tk_cre_tsk(&ctsk_btn);
    tk_sta_tsk(tskid_btn, 0);

    return 0;
}
