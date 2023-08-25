#include <trykernel.h>

static void delay_ms(UINT ms) {
    UINT cnt = ms / TIMER_PERIOD;

    while (cnt) {
        if ((in_w(SYST_CSR) & SYST_CSR_COUNTFLAG) != 0) {
            cnt--;
        }
    }
}

int main(void) {
    tm_com_init();
    tm_putstring("Hello World!\n");
    while (1) {
        out_w(GPIO_OUT_XOR, (1 << 25));
        delay_ms(500);
        tm_putstring("Hello World!\n");
    }
    return 0;
}
