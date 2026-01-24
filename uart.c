#include "hardware/uart.h"
#include "hardware/gpio.h"
#include "uart.h"



void dfplayer_uart_init(void) {
    uart_init(DF_UART, DF_BAUD);

    gpio_set_function(DF_TX, GPIO_FUNC_UART);
    gpio_set_function(DF_RX, GPIO_FUNC_UART);

    uart_set_format(DF_UART, 8, 1, UART_PARITY_NONE);
    uart_set_hw_flow(DF_UART, false, false);
    uart_set_fifo_enabled(DF_UART, false);
}

void dfplayer_send(uint8_t cmd, uint16_t param) {
    uint8_t pkt[10] = {
        0x7E, 0xFF, 0x06,
        cmd,
        0x00,
        param >> 8,
        param & 0xFF,
        0x00, 0x00,
        0xEF
    };

    uint16_t sum = 0;
    for (int i = 1; i < 7; i++)
        sum += pkt[i];

    sum = 0 - sum;
    pkt[7] = sum >> 8;
    pkt[8] = sum & 0xFF;

    uart_write_blocking(DF_UART, pkt, 10);
}
