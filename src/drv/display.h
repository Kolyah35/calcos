#pragma once
#include <platform.h>
#include <types.h>
#include <utils.h>

#ifdef PLATFORM_SIM
    #include <raylib.h>
    #define DISPLAY_COLOR_DEPTH 24
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
    
extern uint16_t display_width;
extern uint16_t display_height;

void display_init(void);
void display_set_contrast(uint8_t);
void display_clear(void);
void display_update(void);
void display_set_rotation(uint8_t r);
#if DISPLAY_COLOR_DEPTH <= 8
void display_set_pixel(uint16_t x, uint16_t y, uint8_t color);
void display_draw_rect_filled(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t color);
void display_draw_line_vertical(int16_t x, int16_t y, int16_t h, uint8_t color);
void display_draw_line_horizontal(int16_t x, int16_t y, int16_t w,  uint8_t color);
#elif DISPLAY_COLOR_DEPTH == 16
void display_set_pixel(uint16_t x, uint16_t y, uint16_t color);
void display_draw_rect_filled(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void display_draw_line_vertical(int16_t x, int16_t y, int16_t h, uint16_t color);
void display_draw_line_horizontal(int16_t x, int16_t y, int16_t w,  uint16_t color);
#elif DISPLAY_COLOR_DEPTH == 24
void display_set_pixel(uint16_t x, uint16_t y, uint32_t color);
void display_draw_rect_filled(int16_t x, int16_t y, int16_t w, int16_t h, uint32_t color);
void display_draw_line_vertical(int16_t x, int16_t y, int16_t h, uint32_t color);
void display_draw_line_horizontal(int16_t x, int16_t y, int16_t w,  uint32_t color);
#endif