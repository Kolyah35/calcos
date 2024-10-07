#pragma once
#include <stdint.h>

#define GLYPH_WIDTH 5
#define GLYPH_HEIGHT 6

typedef struct glyph_t {
    uint16_t value;
    uint32_t data;
    int8_t xoff;
    int8_t yoff;
} glyph_t;

typedef struct font_t {
    const uint16_t glyph_count;
    const glyph_t* glyphs;
} font_t;

const font_t* get_default_font();
const glyph_t* get_glyph(uint16_t codepoint);
const uint16_t measure_str_width(const char* str);