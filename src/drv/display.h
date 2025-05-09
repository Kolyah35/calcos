#pragma once
#include <platform.h>
#include <types.h>
#include <utils.h>

#ifdef PLATFORM_SIM
    #include <raylib.h>
#endif

#ifdef PLATFORM_AVR
    #include <avr/io.h>
    #include <spi.h>

    #define SEND_CMD(cmd)   \
        DISPLAY_CS_ACTIVE;          \
        DISPLAY_DC_COMMAND;         \
        spi_transfer(cmd);

    #define SEND_DATA(data) \
        DISPLAY_CS_ACTIVE;          \
        DISPLAY_DC_DATA;            \
        spi_transfer(data);
#endif

#ifdef PLATFORM_SIM
    extern RenderTexture2D framebuffer;
#endif
    
extern int display_width;
extern int display_height;

void display_init(void);
void display_set_contrast(uint8_t);
void display_clear(void);
void display_update(void);
void display_set_rotation(uint8_t r);

void display_set_pixel(int x, int y, pixel_color_t color);
void display_draw_rect_filled(int x, int y, int w, int h, pixel_color_t color);
void display_draw_line_vertical(int x, int y, int h, pixel_color_t color);
void display_draw_line_horizontal(int x, int y, int w, pixel_color_t color);
