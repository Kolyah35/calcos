#include <raylib.h>
#include <raymath.h>
#include <config.h>
#include <gfx.h>
#include <screen.h>
#include <font.h>
#include <screens/home_screen.h>
#include <time.h>

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, "CalcOS Simulator");
    SetWindowMinSize(320, 240);
    SetTargetFPS(60);

    RenderTexture2D target = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);

    push_screen((screen_t*)load_home_screen());

    while (!WindowShouldClose()) {
        float scale = MIN((float)GetScreenWidth() / SCREEN_WIDTH, (float)GetScreenHeight() / SCREEN_HEIGHT);

        screen_t* screen = get_current_screen();

        key_t pressed_key = 0x00;

        switch(GetKeyPressed()) {
            case KEY_KP_0: pressed_key = BUTTON_ZERO; break;
            case KEY_KP_1: pressed_key = BUTTON_ONE; break;
            case KEY_KP_2: pressed_key = BUTTON_TWO; break;
            case KEY_KP_3: pressed_key = BUTTON_THREE; break;
            case KEY_KP_4: pressed_key = BUTTON_FOUR; break;
            case KEY_KP_5: pressed_key = BUTTON_FIVE; break;
            case KEY_KP_6: pressed_key = BUTTON_SIX; break;
            case KEY_KP_7: pressed_key = BUTTON_SEVEN; break;
            case KEY_KP_8: pressed_key = BUTTON_EIGHT; break;
            case KEY_KP_9: pressed_key = BUTTON_NINE; break;

            case KEY_END: pressed_key = BUTTON_POWER; break;
            case KEY_BACKSPACE: pressed_key = BUTTON_CE; break;
            case KEY_P: pressed_key = BUTTON_PERCENT; break;
            case KEY_S: pressed_key = BUTTON_SQRT; break;
            case KEY_SLASH: pressed_key = BUTTON_DIVIDE; break;
            case KEY_PAGE_UP: pressed_key = BUTTON_M_PLUS; break;
            case KEY_PAGE_DOWN: pressed_key = BUTTON_M_MINUS; break;
            case KEY_HOME: pressed_key = BUTTON_MRC; break;
            case KEY_KP_MULTIPLY: pressed_key = BUTTON_MULTIPLY; break;
            case KEY_KP_SUBTRACT: pressed_key = BUTTON_MINUS; break;
            case KEY_KP_ADD: pressed_key = BUTTON_PLUS; break;
            case KEY_KP_EQUAL: pressed_key = BUTTON_EQUAL; break;
            case KEY_COMMA: pressed_key = BUTTON_DOT; break;
        }

        if(pressed_key) {
            screen->key_callback(pressed_key);
        }

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

            uint8_t batt_prc = 100;
            uint8_t batt_icon_id = ICON_BATTERY_100;
            time_t tm = time(NULL);
            struct tm* date = localtime(&tm);
            const char* time_str = TextFormat("%02i:%02i", date->tm_hour, date->tm_min);
            const char* date_str = TextFormat("%02i.%02i.%02i", date->tm_mday, date->tm_mon + 1, MAX(date->tm_year - 100, 0));
            const char* batt_str = TextFormat("%i%%", batt_prc);

            if(batt_prc < 90) batt_icon_id = ICON_BATTERY_80;
            if(batt_prc < 80) batt_icon_id = ICON_BATTERY_60;
            if(batt_prc < 60) batt_icon_id = ICON_BATTERY_40;
            if(batt_prc < 40) batt_icon_id = ICON_BATTERY_20;
            if(batt_prc < 20) batt_icon_id = ICON_BATTERY_0;
            

            icon_t* battery_icon = get_icon(batt_icon_id);

            draw_text(time_str, SCREEN_WIDTH - DOCK_WIDTH / 2 - measure_str_width(time_str) / 2, 2, COLOR_WHITE);
            draw_text(date_str, SCREEN_WIDTH - DOCK_WIDTH / 2 - measure_str_width(date_str) / 2, 9, COLOR_WHITE);
            draw_text(batt_str, SCREEN_WIDTH - measure_str_width(batt_str) - 1, 16, COLOR_WHITE);
            draw_icon(battery_icon, SCREEN_WIDTH - DOCK_WIDTH + 2, 18, (battery_icon == ICON_BATTERY_0 ? COLOR_RED : COLOR_WHITE));

            for(int i = OPTION_CENTER; i < OPTION_NONE; i++) {
                if(screen->options[i]) {
                    icon_t* icon = get_icon(opt_to_icon(i));
                    const char* text = screen->options[OPTION_BOTTOM];

                    int icon_x = SCREEN_WIDTH - icon->width - 2;
                    int icon_y = SCREEN_HEIGHT - icon->height * i - 1;

                    int text_x = icon_x - measure_str_width(text) - 1;
                    int text_y = SCREEN_HEIGHT - GLYPH_HEIGHT * i;

                    draw_icon(icon, icon_x, icon_y, COLOR_WHITE);
                    draw_text(text, text_x, text_y, COLOR_WHITE);
                }
            }
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