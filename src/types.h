#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <config.h>

typedef struct color_t {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} color_t;

#define COLOR_WHITE (color_t){255, 255, 255}
#define COLOR_BLACK (color_t){0, 0, 0}
#define COLOR_RED   (color_t){255, 0, 0}
#define COLOR_GREEN (color_t){0, 255, 0}
#define COLOR_BLUE  (color_t){0, 0, 255}

typedef struct vec2_t {
    union {
        int x;
        int w;
    };

    union {
        int y;
        int h;
    };
} vec2_t;

typedef struct rect_t {
    int x;
    int y;
    int w;
    int h;
} rect_t;

typedef enum option_t {
    OPTION_BOTTOM,
    OPTION_CENTER,
    OPTION_TOP,
    OPTION_NONE
} option_t;