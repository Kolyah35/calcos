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
        CS_ACTIVE;          \
        DC_COMMAND;         \
        spi_transfer(cmd);

    #define SEND_DATA(data) \
        CS_ACTIVE;          \
        DC_DATA;            \
        spi_transfer(data);
#else
    extern RenderTexture2D framebuffer;
#endif

void init_display(void);
void set_display_contrast(uint8_t);
void clear_display(void);
void update_display(void);
#ifdef SCREEN_MONOCHROME
void set_display_pixel(uint16_t x, uint16_t y, uint8_t color);
#else
void set_display_pixel(uint16_t x, uint16_t y, color_t color);
#endif