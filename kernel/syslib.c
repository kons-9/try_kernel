#include "trykernel.h"
#include <knldef.h>
void tm_com_init() {
    out_w(UART0_BASE + UARTx_IBRD, 67); /* ボーレート設定 */
    out_w(UART0_BASE + UARTx_FBRD, 52);
    out_w(UART0_BASE + UARTx_LCR_H, 0x70);                                /* データ形式設定 */
    out_w(UART0_BASE + UARTx_CR, UART_CR_RXE | UART_CR_TXE | UART_CR_EN); /* 通信イネーブル */
}


UINT tm_putstring(char *str) {
    UINT cnt = 0;

    while (*str) {
        while ((in_w(UART0_BASE + UARTx_FR) & UART_FR_TXFF) != 0)
            ;                                 /* 送信FIFOの空き待ち */
        out_w(UART0_BASE + UARTx_DR, *str++); /* データ送信 */
        cnt++;
    }
    return cnt;
}
