// Generated from mainscreen.png
#pragma once

#include <stdint.h>

#define MAINSCREEN_W 240
#define MAINSCREEN_H 320

#define TILE_W      240
#define TILE_H      64
#define BYTES_PER_TILE ((TILE_W * TILE_H) / 2)

#define MAINSCREEN_FRAMES (MAINSCREEN_H / TILE_H)

extern const uint16_t mainscreen_palette[16];
extern const uint8_t  mainscreen_data[];
