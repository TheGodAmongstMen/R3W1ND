#ifndef UART_H
#define UART_H

#include "hardware/uart.h"

#define DF_UART uart1
#define DF_BAUD 9600

#define DF_TX 4   // Pico GPIO -> DFPlayer RX
#define DF_RX 5   // Pico GPIO <- DFPlayer TX

void dfplayer_uart_init(void);
void dfplayer_send(uint8_t cmd, uint16_t arg);

#endif