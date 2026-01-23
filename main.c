#include "pico/stdlib.h"
#include "screen.h"

int main() {
    stdio_init_all();
    screen_init();

    while (1) {
        screen_fill(0xF800); // red
        sleep_ms(500);
        screen_fill(0x07E0); // green
        sleep_ms(500);
        screen_fill(0x001F); // blue
        sleep_ms(500);
        screen_fill(0x0000);           // black
        //screen_draw_pixel(10, 10, 0xFFFF); // white pixel
        //screen_draw_pixel(20, 20, 0xF800); // red
        //screen_draw_pixel(30, 30, 0x07E0); // green
        //screen_draw_pixel(40, 40, 0x001F); // blue
    }
}
