#include <config.h>

#ifdef PLATFORM_SIM

#include <raylib.h>
#include <raymath.h>
#include <display.h>

void init_display() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, "CalcOS Simulator");
    SetWindowMinSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    SetTargetFPS(60);

    framebuffer = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
}

void set_display_contrast(uint8_t val) {
    return; // not supported yet
}

void clear_display() {
    for(int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        set_display_pixel(i / SCREEN_WIDTH, i % SCREEN_WIDTH, DISPLAY_WHITE);
    }
}

void set_display_pixel(uint16_t x, uint16_t y, display_color_t color) {
#ifdef DISPLAY_MONOCHROME
    DrawPixel(x, y, (color ? BLACK : WHITE));
#else
    DrawPixel(x, y, (Color){color.r, color.g, color.b, 255});
#endif
}

void update_display(void) {
    float scale = MIN((float)GetScreenWidth() / SCREEN_WIDTH, (float)GetScreenHeight() / SCREEN_HEIGHT);

    BeginDrawing();
        ClearBackground(WHITE);

        DrawTexturePro(
            framebuffer.texture, 
            (Rectangle){0.0f, 0.0f, (float)framebuffer.texture.width, (float)-framebuffer.texture.height},
            (Rectangle){(GetScreenWidth() - ((float)SCREEN_WIDTH * scale)) * 0.5f, (GetScreenHeight() - ((float)SCREEN_HEIGHT * scale)) * 0.5f,
                        (float)SCREEN_WIDTH * scale, (float)SCREEN_HEIGHT * scale},
            Vector2Zero(), 0.0f, WHITE
        );
    EndDrawing();
}

#endif // PLATFORM_SIM