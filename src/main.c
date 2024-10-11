#include <drv/display.h>
#include <utils.h>
#include <gfx.h>
#include <screen.h>
#include <time.h>
#include <font.h>
#include <stdio.h>
#include <screens/home_screen.h>
#include <uart.h>

#ifdef PLATFORM_SIM
    #include <raylib.h>
    #include <raymath.h>

    RenderTexture2D framebuffer;
#endif

int main() {
#ifdef PLATFORM_AVR
    bool should_close = false;

    uart_begin(19200);
#endif
    init_display();

    set_display_contrast(13);
    clear_display();
    
    set_draw_area(0, 0, 128, 64);

#ifdef PLATFORM_SIM
    bool should_close = false;
#endif

    push_screen((screen_t*)load_home_screen());
    char time_str[6];
    char date_str[9];
    char batt_str[5];

    while(!should_close) {
        screen_t* screen = get_current_screen();

        if(screen->update_callback != NULL) {
            screen->update_callback();
        }

        if(screen != NULL) {
            set_draw_area(0, 0, SCREEN_WIDTH - DOCK_WIDTH, SCREEN_HEIGHT);

            if(screen->should_redraw) {
                for(int i = 0; i < screen->node_count; i++) {
                    screen->ui_nodes[i]->draw(screen->ui_nodes[i]);
                }

                screen->should_redraw = false;
            }
        }

        uint8_t batt_prc = 100;
        uint8_t batt_icon_id = ICON_BATTERY_100;
        time_t tm = time(NULL);
        struct tm* date = localtime(&tm);

        sprintf(time_str, "%02u:%02u", date->tm_hour, date->tm_min);
        sprintf(date_str, "%02u.%02u.%02u", date->tm_mday, date->tm_mon + 1, date->tm_year % 100);
        sprintf(batt_str, "%u%%", batt_prc);

        if(batt_prc < 90) batt_icon_id = ICON_BATTERY_80;
        if(batt_prc < 80) batt_icon_id = ICON_BATTERY_60;
        if(batt_prc < 60) batt_icon_id = ICON_BATTERY_40;
        if(batt_prc < 40) batt_icon_id = ICON_BATTERY_20;
        if(batt_prc < 20) batt_icon_id = ICON_BATTERY_0;

        update_keyboard();

#ifdef PLATFORM_SIM
        BeginTextureMode(framebuffer);
        should_close = WindowShouldClose();
#endif

        // unlock screen area
        set_draw_area(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        draw_rectangle(SCREEN_WIDTH - DOCK_WIDTH, 0, DOCK_WIDTH, SCREEN_HEIGHT, DISPLAY_BLACK);
        draw_rectangle_filled(SCREEN_WIDTH - DOCK_WIDTH + 1, 1, DOCK_WIDTH - 2, SCREEN_HEIGHT - 2, DISPLAY_WHITE);

        icon_t battery_icon = get_icon(batt_icon_id);

        draw_text(time_str, SCREEN_WIDTH - (DOCK_WIDTH >> 1) - (measure_str_width(time_str) >> 1), 2, DISPLAY_BLACK);
        draw_text(date_str, SCREEN_WIDTH - (DOCK_WIDTH >> 1) - (measure_str_width(date_str) >> 1), GLYPH_HEIGHT + 3, DISPLAY_BLACK);
        draw_text(batt_str, SCREEN_WIDTH - measure_str_width(batt_str) - 1, GLYPH_HEIGHT * 2 + 4, DISPLAY_BLACK);
        draw_icon(battery_icon, SCREEN_WIDTH - DOCK_WIDTH + 2, GLYPH_HEIGHT * 2 + 5, (batt_icon_id == ICON_BATTERY_0 ? DISPLAY_RED : DISPLAY_BLACK));

        // draw_text(ram_usage, SCREEN_WIDTH - measure_str_width(ram_usage) - 1, GLYPH_HEIGHT * 3 + 5, COLOR_BLACK);

        for(int i = 0; i < OPTION_NONE; i++) {
            if(screen->options[i]) {
                icon_t icon = get_icon(opt_to_icon(i));
                const char* text = screen->options[i];

                int icon_x = SCREEN_WIDTH - icon.width - 2;
                int text_x = icon_x - measure_str_width(text) - 1;
                int text_y = SCREEN_HEIGHT - 2 - GLYPH_HEIGHT - GLYPH_HEIGHT * i - i;
                int icon_y = text_y + 1;

                draw_icon(icon, icon_x, icon_y, DISPLAY_BLACK);
                draw_text(text, text_x, text_y, DISPLAY_BLACK);
            }
        }

#ifdef PLATFORM_SIM
        EndTextureMode();
#endif

        update_display();
    }

    return 0;
}