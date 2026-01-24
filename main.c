#include "pico/stdlib.h"
#include "screen.h"
#include "uart.h"

int main() {
    stdio_init_all();
    screen_init();
    dfplayer_uart_init();

    screen_fill(0xF800); 
    sleep_ms(1500);           // DFPlayer needs time
    dfplayer_send(0x06, 25);  // volume
    dfplayer_send(0x0A, 0);   // normal EQ
    dfplayer_send(0x09, 0);   // select TF card
    dfplayer_send(0x03, 1);   // play

}
