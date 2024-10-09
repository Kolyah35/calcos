#include <platform.h>
#include <display.h>
#include <gfx.h>
#include <stdlib.h>
#include <font.h>

#ifdef CALCOS_SIM
    #include <raylib.h>
#endif

rect_t draw_area;

void set_draw_area(int x, int y, int width, int height) {
    draw_area.x = x;
    draw_area.y = y;
    draw_area.w = width;
    draw_area.h = height;
}

vec2_t get_draw_area_size() {
    return (vec2_t){draw_area.w, draw_area.h};
}

void draw_pixel(uint16_t x, uint16_t y, color_t color) {
    x += draw_area.x;
    y += draw_area.y;
    
    if(x <= draw_area.w && y <= draw_area.h) {
#ifdef SCREEN_MONOCHROME
        set_display_pixel(x, y, 765 == (color.r + color.g + color.b));
#else
        set_display_pixel(x, y, color);
#endif
    }
}

void draw_line(int x1, int y1, int x2, int y2, color_t color) {
    int dx =  abs(x2 - x1);
    int sx = x1 < x2 ? 1 : -1;
    int dy = -abs(y2 - y1); 
    int sy = y1 < y2 ? 1 : -1; 
    int err = dx + dy;
    int err2;

    while(1) {
        draw_pixel(x1, y1, color);
        if(x1 == x2 && y1 == y2) {
            break;
        }

        err2 = 2 * err;
        if(err2 >= dy) { 
            err += dy; 
            x1 += sx; 
        }
        
        if(err2 <= dx) { 
            err += dx; 
            y1 += sy; 
        }
    }
}

void draw_rectangle(int x, int y, int width, int height, color_t color) {
    draw_line(x, y, x + width, y, color);
    draw_line(x + width - 1, y + 1, x + width - 1, y + height - 1, color);
    draw_line(x, y + height - 1, x + width - 1, y + height - 1, color);
    draw_line(x, y + 1, x, y + height - 1, color);
}

void draw_rectangle_filled(int x, int y, int width, int height, color_t color) {
    for(int i = 0; i < width * height; i++) {
        draw_pixel(x + i % width, y + i / width, color);
    }
}

void draw_text(const char* str, int x, int y, color_t color) {
    int xoffset = x;
    int yoffset = y;

    while(*str) {
        if(*str == '\n') {
            xoffset = x;
            yoffset += GLYPH_HEIGHT + 1;
            str++;
            continue;
        }

        if(*str == '\t') {
            xoffset += GLYPH_WIDTH * 4;
            str++;
            continue;
        }

        if(*str == ' ') {
            xoffset += GLYPH_WIDTH;
            str++;
            continue;
        }

        uint16_t codepoint = 0x00;

        if ((*str & 0x80) == 0) {
            codepoint = *str++;
        }

        if ((*str & 0xE0) == 0xC0) {
            uint8_t first = *str++;
            uint8_t second = *str++;
            codepoint = ((first << 8) | second);
        }

        glyph_t glyph = get_glyph(codepoint);

        for(int i = 0; i < GLYPH_WIDTH * GLYPH_HEIGHT; i++) {
            uint8_t pixel = BIT_READ(glyph.data, i);

            if(pixel) {
                draw_pixel(xoffset + (i % GLYPH_WIDTH) + glyph.xoff, yoffset + (i / GLYPH_WIDTH) + glyph.yoff, color);
            }
        }

        xoffset += GLYPH_WIDTH + 1 + glyph.xoff;
    }
}

void draw_icon(icon_t icon, int x, int y, color_t color) {
    for(int i = 0; i < icon.width * icon.height; i++) {
        bool pixel = BIT_READ(icon.data, i);

        if(pixel) {
            draw_pixel(x + (i % icon.width), y + (i / icon.width), color);
        }
    }
}