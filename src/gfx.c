#include <platform.h>
#include <display.h>
#include <gfx.h>
#include <stdlib.h>
#include <font.h>
#include <utils.h>

#ifdef CALCOS_SIM
    #include <raylib.h>
#endif

static inline uint32_t convert_color(color_t color) {
#if DISPLAY_COLOR_DEPTH == 1
        uint8_t grayscale = (uint8_t)(0.299 * color.r + 0.587 * color.g + 0.114 * color.b);
        return grayscale > 128;
#elif DISPLAY_COLOR_DEPTH == 8
        return (uint8_t)(color & 0xFF);
#elif DISPLAY_COLOR_DEPTH == 16
        return ((color.r & 0xF8) << 8) | ((color.g & 0xFC) << 3) | (color.b >> 3);
#elif DISPLAY_COLOR_DEPTH == 24
        return (color.r << 24) | (color.g << 16) | (color.b) << 8 | 0xFF;
#else
        #error Unsupported display color depth!
#endif
}

void set_draw_area(int x, int y, int width, int height) {
    draw_area.x = x;
    draw_area.y = y;
    draw_area.w = width;
    draw_area.h = height;
}

void draw_pixel(uint16_t x, uint16_t y, color_t color) {
    x += draw_area.x;
    y += draw_area.y;
    
    if(x <= draw_area.w && y <= draw_area.h) {
        display_set_pixel(x, y, convert_color(color));
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
    display_draw_line_horizontal(x, y, width - 1, convert_color(color));
    display_draw_line_vertical(x + width - 1, y + 1, height - 1, convert_color(color));
    display_draw_line_horizontal(x, y + height - 1, width - 1, convert_color(color));
    display_draw_line_vertical(x, y + 1, height - 1, convert_color(color));
}

void draw_rectangle_filled(int x, int y, int width, int height, color_t color) {
    display_draw_rect_filled(x, y, width, height, convert_color(color));
}

void draw_text(const char* str, int x, int y, color_t color) {
    int xoffset = x;
    int yoffset = y;
    bool is_pgm = IS_PGM(str);

    while((is_pgm ? pgm_read_byte(str) : *str)){
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

// void draw_image(image_t image, int x, int y) {
//     if(image.format == IMAGE_FORMAT_1BIT) {
//         for(int i = 0; i < image.width * image.height; i++) {
//             bool pixel = BIT_READ(image.data[i / 8], i % 8);
            
//             draw_pixel(x + (i % image.width), y + (i / image.width), (pixel ? COLOR_WHITE : COLOR_BLACK));
//         }
//     }
// }