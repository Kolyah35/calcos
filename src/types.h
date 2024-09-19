#pragma once
#include <stdint.h>

#define COLOR_WHITE (color_t){255, 255, 255}
#define COLOR_BLACK (color_t){0, 0, 0}

typedef struct color_t {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} color_t;

typedef struct vec2_t {
    uint16_t w;
    uint16_t h;
} vec2_t;

typedef struct rect_t {
    uint16_t x;
    uint16_t y;
    uint16_t w;
    uint16_t h;
} rect_t;