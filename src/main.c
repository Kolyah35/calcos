#include <raylib.h>
#include <raymath.h>
#include <config.h>
#include <gfx.h>
#include <screen.h>
#include <font.h>
#include <screens/home_screen.h>

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, "CalcOS Simulator");
    SetWindowMinSize(320, 240);
    SetTargetFPS(60);

    RenderTexture2D target = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);

    init_home_screen();
    push_screen((screen_t*)get_home_screen());

    while (!WindowShouldClose()) {
        float scale = MIN((float)GetScreenWidth() / SCREEN_WIDTH, (float)GetScreenHeight() / SCREEN_HEIGHT);

        screen_t* screen = get_current_screen();

        if(screen->update_callback != NULL) {
            screen->update_callback();
        }

        BeginTextureMode(target);
            ClearBackground(BLACK);

            if(screen->draw_callback != NULL) {
                set_draw_area(0, 0, SCREEN_WIDTH - DOCK_WIDTH, SCREEN_HEIGHT);
                screen->draw_callback();
            }

            // unlock screen area
            set_draw_area(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
            draw_rectangle(SCREEN_WIDTH - DOCK_WIDTH, 0, DOCK_WIDTH, SCREEN_HEIGHT, COLOR_WHITE);
            draw_rectangle_filled(SCREEN_WIDTH - DOCK_WIDTH + 1, 1, DOCK_WIDTH - 2, SCREEN_HEIGHT - 2, COLOR_BLACK);
        EndTextureMode();
        
        BeginDrawing();
            ClearBackground(BLACK);

            DrawTexturePro(
                target.texture, 
                (Rectangle){0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height},
                (Rectangle){(GetScreenWidth() - ((float)SCREEN_WIDTH * scale)) * 0.5f, (GetScreenHeight() - ((float)SCREEN_HEIGHT * scale)) * 0.5f,
                           (float)SCREEN_WIDTH * scale, (float)SCREEN_HEIGHT * scale},
                Vector2Zero(), 0.0f, WHITE
            );
        EndDrawing();
    }

    UnloadRenderTexture(target);
    CloseWindow();       

    return 0;
}