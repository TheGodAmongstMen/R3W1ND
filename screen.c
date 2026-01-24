/******************************************************************************
* Code for LCD for R3W1ND project.
*
* screen_init: initializes and sets up SPI and screen
* screen_fill: fills screen with one color
* screen_draw_pixel: draws pixel of specific color at coordinate
*
******************************************************************************/
#include "screen.h"
#include "hardware/gpio.h"
#include "pico/stdlib.h"

static inline void lcd_cmd(uint8_t cmd) {
    gpio_put(PIN_DC, 0);
    gpio_put(PIN_CS, 0);
    spi_write_blocking(SPI_PORT, &cmd, 1);
    gpio_put(PIN_CS, 1);
}

static inline void lcd_data(const uint8_t *data, size_t len) {
    gpio_put(PIN_DC, 1);
    gpio_put(PIN_CS, 0);
    spi_write_blocking(SPI_PORT, data, len);
    gpio_put(PIN_CS, 1);
}

void screen_init(void){
    spi_init(SPI_PORT, 62 * 1000 * 1000);
    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);

    gpio_init(PIN_CS);
    gpio_init(PIN_DC);
    gpio_init(PIN_RST);

    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_set_dir(PIN_DC, GPIO_OUT);
    gpio_set_dir(PIN_RST, GPIO_OUT);

    gpio_put(PIN_RST, 0);
    sleep_ms(50);
    gpio_put(PIN_RST, 1);
    sleep_ms(50);

    lcd_cmd(0x01); // SWRESET
    sleep_ms(150);

    lcd_cmd(0x11); // SLPOUT
    sleep_ms(120);

    uint8_t color_mode = 0x55; // 16-bit color
    lcd_cmd(0x3A);
    lcd_data(&color_mode, 1);

    lcd_cmd(0x36); // MADCTL
    uint8_t madctl = 0x00;
    lcd_data(&madctl, 1);

    lcd_cmd(0x29); // DISPON
}

void screen_fill(uint16_t color){
    uint8_t hi = color >> 8;
    uint8_t lo = color & 0xFF;

    lcd_cmd(0x2A); // CASET
    uint8_t caset[] = {0x00, 0, 0x00, LCD_WIDTH - 1};
    lcd_data(caset, 4);

    lcd_cmd(0x2B); // RASET
    uint8_t raset[] = {0x00, 0, 0x01, LCD_HEIGHT - 1};
    lcd_data(raset, 4);

    lcd_cmd(0x2C); // RAMWR

    gpio_put(PIN_DC, 1);
    gpio_put(PIN_CS, 0);
    for (int i = 0; i < LCD_WIDTH * LCD_HEIGHT; i++) {
        uint8_t px[2] = {hi, lo};
        spi_write_blocking(SPI_PORT, px, 2);
    }
    gpio_put(PIN_CS, 1);
}

static inline void screen_set_window(uint16_t x0, uint16_t y0,
                                     uint16_t x1, uint16_t y1) {
    uint8_t data[4];

    lcd_cmd(0x2A); // CASET
    data[0] = x0 >> 8;
    data[1] = x0 & 0xFF;
    data[2] = x1 >> 8;
    data[3] = x1 & 0xFF;
    lcd_data(data, 4);

    lcd_cmd(0x2B); // RASET
    data[0] = y0 >> 8;
    data[1] = y0 & 0xFF;
    data[2] = y1 >> 8;
    data[3] = y1 & 0xFF;
    lcd_data(data, 4);

    lcd_cmd(0x2C); // RAMWR
}


void screen_draw_pixel(uint16_t x, uint16_t y, uint16_t color){
    if (x >= LCD_WIDTH || y >= LCD_HEIGHT) return;

    screen_set_window(x, y, x, y);

    uint8_t pixel[2] = {
        color >> 8,
        color & 0xFF
    };

    gpio_put(PIN_DC, 1);
    gpio_put(PIN_CS, 0);
    spi_write_blocking(SPI_PORT, pixel, 2);
    gpio_put(PIN_CS, 1);
}

void screen_draw_image(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *img) {
    if (x + y > LCD_WIDTH || y+ h > LCD_HEIGHT) return;

    screen_set_window(x, y, x + w - 1, y + h - 1);

    gpio_put(PIN_DC, 1);
    gpio_put(PIN_CS, 0);

    for(uint32_t i = 0; i < w*h; i++) {
        uint8_t px[2]= {
            img[i] >> 8,
            img[i] & 0xFF
        };
        spi_write_blocking(SPI_PORT, px, 2);
    }

    gpio_put(PIN_CS, 1);
}