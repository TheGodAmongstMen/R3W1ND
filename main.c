#include "pico/stdlib.h"
#include "screen.h"
#include "uart.h"
#include "mainscreen.h"

#define PIN_BACK 10
#define PIN_PAUSE 11
#define PIN_NEXT 12
#define PIN_MODE 13

int main() {
    stdio_init_all();
    screen_init();
    dfplayer_uart_init();
    screen_fill(0x0000); 
    
    int frame = 0;

    while (1) {
        
        draw_mainscreen_frame(2, 30, frame);
        frame = (frame + 1) % MAINSCREEN_FRAMES;
        sleep_ms(50); // 20 FPS
    }
}
