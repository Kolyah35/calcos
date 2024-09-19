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
    uint16_t glyph_count;
    glyph_t* glyphs;
} font_t;

font_t* get_default_font();
glyph_t* get_glyph(uint16_t codepoint);