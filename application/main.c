#include "typedef.h"
#include "sysdef.h"
#include "syslib.h"

static void delay_ms(UINT ms) {
    UINT cnt = ms / TIMER_PERIOD;

    while (cnt) {
        if ((in_w(SYST_CSR) & SYST_CSR_COUNTFLAG) != 0) {
            cnt--;
        }
    }
}

int main(void) {
    while (1) {
        out_w(GPIO_OUT_XOR, (1 << 25));
        delay_ms(500);
    }
    return 0;
}
