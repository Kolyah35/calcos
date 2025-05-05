#include <config.h>

#ifdef PLATFORM_SIM

#include <raylib.h>
#include <raymath.h>
#include <display.h>

uint16_t display_width = DISPLAY_WIDTH;
uint16_t display_height = DISPLAY_HEIGHT;

void display_init() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(DISPLAY_WIDTH * 2, DISPLAY_HEIGHT * 2, "CalcOS Simulator");
    SetWindowMinSize(DISPLAY_WIDTH, DISPLAY_HEIGHT);
    SetTargetFPS(10);

    framebuffer = LoadRenderTexture(DISPLAY_WIDTH, DISPLAY_HEIGHT);
}

void display_set_contrast(uint8_t val) {
    return; // not supported yet
}

void display_clear() {
    for(int i = 0; i < DISPLAY_WIDTH * DISPLAY_HEIGHT; i++) {
        display_set_pixel(i / DISPLAY_WIDTH, i % DISPLAY_WIDTH, 0xFFFFFFFF);
    }
}

void display_set_pixel(uint16_t x, uint16_t y, uint32_t color) {
    DrawPixel(x, y, (Color){color >> 24, color >> 16, color >> 8, 0xFF});
}

void display_update(void) {
    float scale = MIN((float)GetScreenWidth() / DISPLAY_WIDTH, (float)GetScreenHeight() / DISPLAY_HEIGHT);

    BeginDrawing();
        ClearBackground(WHITE);

        DrawTexturePro(
            framebuffer.texture, 
            (Rectangle){0.0f, 0.0f, (float)framebuffer.texture.width, (float)-framebuffer.texture.height},
            (Rectangle){(GetScreenWidth() - ((float)DISPLAY_WIDTH * scale)) * 0.5f, (GetScreenHeight() - ((float)DISPLAY_HEIGHT * scale)) * 0.5f,
                        (float)DISPLAY_WIDTH * scale, (float)DISPLAY_HEIGHT * scale},
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