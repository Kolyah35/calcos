#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <config.h>

#ifdef DISPLAY_MONOCHROME
    #define DISPLAY_WHITE (display_color_t)0x00
    #define DISPLAY_BLACK (display_color_t)0x01
    #define DISPLAY_RED   DISPLAY_BLACK
#else
    #define DISPLAY_WHITE (display_color_t){255, 255, 255}
    #define DISPLAY_BLACK (display_color_t){0, 0, 0}
    #define DISPLAY_RED   (display_color_t){255, 0, 0}
#endif

typedef struct color_t {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} color_t;

#ifdef DISPLAY_MONOCHROME
typedef bool display_color_t;
#else
typedef color_t display_color_t;
#endif

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

typedef enum option_t {
    OPTION_BOTTOM,
    OPTION_CENTER,
    OPTION_TOP,
    OPTION_NONE
} option_t;