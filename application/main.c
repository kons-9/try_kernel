#include <trykernel.h>

ID flgid;
T_CFLG cflg = {
    .flgatr = TA_TFIFO | TA_WMUL,
    .iflgptn = 0,
};

ID tskid_led1;
ID tskid_led2;
ID tskid_btn;
UW tskstk_led1[1024 / sizeof(UW)];
UW tskstk_led2[1024 / sizeof(UW)];
UW tskstk_btn[1024 / sizeof(UW)];
void task_led1(INT stacd, void *exinf);
void task_led2(INT stacd, void *exinf);
void task_btn(INT stacd, void *exinf);
T_CTSK ctsk_led1 = {
    .tskatr = TA_HLNG | TA_RNG3 | TA_USERBUF,
    .task = task_led1,
    .itskpri = 10,
    .stksz = 1024,
    .bufptr = tskstk_led1,
};
T_CTSK ctsk_led2 = {
    .tskatr = TA_HLNG | TA_RNG3 | TA_USERBUF,
    .task = task_led2,
    .itskpri = 10,
    .stksz = 1024,
    .bufptr = tskstk_led2,
};
T_CTSK ctsk_btn = {
    .tskatr = TA_HLNG | TA_RNG3 | TA_USERBUF,
    .task = task_btn,
    .itskpri = 10,
    .stksz = 1024,
    .bufptr = tskstk_btn,
};

void task_led1(INT stacd, void *exinf) {
    UINT flgptn;
    while (1) {
        tk_wai_flg(flgid, 1 << 0, TWF_ANDW | TWF_BITCLR, &flgptn, TMO_FEVR);
        tm_putstring("led1\n");
        for (INT i = 0; i < 3; i++) {
            out_w(GPIO_OUT_SET, (1 << 24));
            tk_dly_tsk(500);
            out_w(GPIO_OUT_CLR, (1 << 24));
            tk_dly_tsk(500);
        }
    }
}

void task_led2(INT stacd, void *exinf) {
    UINT flgptn;
    while (1) {
        ER err = tk_wai_flg(flgid, 1 << 1, TWF_ANDW | TWF_BITCLR, &flgptn, TMO_FEVR);
        if (err == E_NOEXS) {
            tm_putstring("error: E_NOEXS\n");
            continue;
        }
        tm_putstring("led2\n");
        for (INT i = 0; i < 5; i++) {
            out_w(GPIO_OUT_SET, (1 << 25));
            tk_dly_tsk(100);
            out_w(GPIO_OUT_CLR, (1 << 25));
            tk_dly_tsk(100);
        }
    }
}

void task_btn(INT stacd, void *exinf) {
    UW btn, btn0, diff;

    out_w(GPIO(13), (in_w(GPIO(13)) | GPIO_PUE) & ~GPIO_PDE);  // pullup
    out_w(GPIO_OE_CLR, (1 << 13));                             // disable output
    out_w(GPIO(13), 5);                                        // set as input

    out_w(GPIO(14), (in_w(GPIO(14)) | GPIO_PUE) & ~GPIO_PDE);  // pullup
    out_w(GPIO_OE_CLR, (1 << 14));                             // disable output
    out_w(GPIO(14), 5);                                        // set as input

    // prev state
    btn0 = in_w(GPIO_IN) & ((1 << 13) | (1 << 14));

    while (1) {
        btn = in_w(GPIO_IN) & ((1 << 13) | (1 << 14));
        diff = btn ^ btn0;
        if (diff == 0) {
            tk_dly_tsk(100);
            continue;
        }
        if ((diff & (1 << 13)) && !(btn & (1 << 13))) {
            tm_putstring("Button 1 pressed\n");
            tk_set_flg(flgid, 1 << 1);
        }
        if ((diff & (1 << 14)) && !(btn & (1 << 14))) {
            tm_putstring("Button 2 pressed\n");
            tk_set_flg(flgid, 1 << 0);
        }
        btn0 = btn;

        tk_dly_tsk(100);
    }
}


int usermain(void) {
    tm_putstring("Initilizing...\n");

    flgid = tk_cre_flg(&cflg);

    tskid_btn = tk_cre_tsk(&ctsk_btn);
    tk_sta_tsk(tskid_btn, 0);

    tskid_led1 = tk_cre_tsk(&ctsk_led1);
    tk_sta_tsk(tskid_led1, 0);

    tskid_led2 = tk_cre_tsk(&ctsk_led2);
    tk_sta_tsk(tskid_led2, 0);

    return 0;
}
