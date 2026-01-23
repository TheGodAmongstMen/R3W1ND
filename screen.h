#ifndef SCREEN_H
#define SCREEN_H

#include "hardware/spi.h"

static inline void lcd_cmd(uint8_t cmd);
static inline void lcd_data(const uint8_t *data, size_t len);
void screen_init(void);
void screen_fill(uint16_t color);
static inline void screen_set_window(uint16_t x0, uint16_t y0,
                                     uint16_t x1, uint16_t y1);
void screen_draw_pixel(uint16_t x, uint16_t y, uint16_t color);

#endif