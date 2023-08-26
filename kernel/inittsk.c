#include "apidef.h"
#include "syslib.h"
#include "typedef.h"
#include "knldef.h"

void inittsk(INT stacd, void *exinf);

UB tskstk_ini[256];
ID tskid_ini;

T_CTSK ctsk_ini = {
    .tskatr = TA_HLNG | TA_RNG0 | TA_USERBUF,
    .task = inittsk,
    .itskpri = 1,
    .stksz = sizeof(tskstk_ini),
    .bufptr = tskstk_ini,
};

void inittsk(INT stacd, void *exinf) {
    tm_com_init();
    tm_putstring("Start Try Kernel\n");

    usermain();
    tk_ext_tsk();
}

int main(void) {
    tskid_ini = tk_cre_tsk(&ctsk_ini);
    tk_sta_tsk(tskid_ini, 0);
    while (1)
        ;
}
