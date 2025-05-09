#include <config.h>

#ifdef PLATFORM_SIM

#include <raylib.h>
#include <raymath.h>
#include <display.h>

#ifndef SIM_DISPLAY_WIDTH
    #define SIM_DISPLAY_WIDTH DISPLAY_WIDTH
#endif

#ifndef SIM_DISPLAY_HEIGHT
    #define SIM_DISPLAY_HEIGHT DISPLAY_HEIGHT
#endif

int display_width = SIM_DISPLAY_WIDTH;
int display_height = SIM_DISPLAY_HEIGHT;

void display_init() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(display_width * 2, display_height * 2, "CalcOS Simulator");
    SetWindowMinSize(display_width, display_height);
    SetTargetFPS(60);

    framebuffer = LoadRenderTexture(display_width, display_height);
}

void display_set_contrast(uint8_t val) {
    return; // not supported yet
}

void display_clear() {
    for(int i = 0; i < display_width * display_height; i++) {
        display_set_pixel(i / display_width, i % display_width, 0xFFFFFFFF);
    }
}

void display_set_pixel(uint16_t x, uint16_t y, uint32_t color) {
    DrawPixel(x, y, (Color){color >> 24, color >> 16, color >> 8, 0xFF});
}

void display_update(void) {
    float scale = MIN((float)GetScreenWidth() / display_width, (float)GetScreenHeight() / display_height);

    BeginDrawing();
        ClearBackground(BLACK);

        DrawTexturePro(
            framebuffer.texture, 
            (Rectangle){0.0f, 0.0f, (float)framebuffer.texture.width, (float)-framebuffer.texture.height},
            (Rectangle){(GetScreenWidth() - ((float)display_width * scale)) * 0.5f, (GetScreenHeight() - ((float)display_height * scale)) * 0.5f,
                        (float)display_width * scale, (float)display_height * scale},
            Vector2Zero(), 0.0f, WHITE
        );
    EndDrawing();
}

void display_draw_rect_filled(int16_t x, int16_t y, int16_t w, int16_t h, uint32_t color) {
    DrawRectangle(x, y, w, h, (Color){color >> 24, color >> 16, color >> 8, 0xFF});
}

void display_draw_line_vertical(int16_t x, int16_t y, int16_t h, uint32_t color) {
    DrawLine(x, y, x, y + h, (Color){color >> 24, color >> 16, color >> 8, 0xFF});
}

void display_draw_line_horizontal(int16_t x, int16_t y, int16_t w, uint32_t color) {
    DrawLine(x, y, x + w, y, (Color){color >> 24, color >> 16, color >> 8, 0xFF});
}

#endif // PLATFORM_SIM