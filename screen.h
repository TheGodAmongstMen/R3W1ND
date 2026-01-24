#ifndef SCREEN_H
#define SCREEN_H

#include "hardware/spi.h"


// pins n ports
#define SPI_PORT spi0

#define PIN_SCK  18
#define PIN_MOSI 19
#define PIN_CS   17
#define PIN_DC   16
#define PIN_RST  20

#define LCD_WIDTH  240
#define LCD_HEIGHT 285


static inline void lcd_cmd(uint8_t cmd);
static inline void lcd_data(const uint8_t *data, size_t len);
void screen_init(void);
void screen_fill(uint16_t color);
static inline void screen_set_window(uint16_t x0, uint16_t y0,
                                     uint16_t x1, uint16_t y1);
void screen_draw_pixel(uint16_t x, uint16_t y, uint16_t color);
void screen_draw_image(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *img);

#endif