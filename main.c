#include "mainscreen.h"
#include "screen.h"
#include "uart.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"

int main(void){
    screen_init();
    int frame = 0;
    int boot = 0;
    
    uint16_t red = 0xF800;
    screen_fill(0xFFFF);
    sleep_ms(500);

    while (boot < 20)
    {
        draw_mainscreen_frame(04, 32, frame);

        frame++;
        if (frame >= MAINSCREEN_FRAMES)
            frame = 0;

        sleep_ms(50);  // 20 FPS 
        boot++;
    }

}
