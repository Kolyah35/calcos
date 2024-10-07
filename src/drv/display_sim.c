#include <display.h>
#include <config.h>

#ifdef PLATFORM_SIM

#include <raylib.h>

void init_display() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, "CalcOS Simulator");
    SetWindowMinSize(320, 240);
    SetTargetFPS(60);
}

void set_display_invert(bool val) {
    // not supported yet
}

void clear_display() {
    for(int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        draw_pixel(i / SCREEN_WIDTH, i % SCREEN_WIDTH, 0x00);
    }
}

void set_display_pixel(uint16_t x, uint16_t y, color_t color) {
    DrawPixel(x, y, (Color){color.r, color.g, color.b, 255});
}

#endif // PLATFORM_SIM