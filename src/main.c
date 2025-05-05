#include <drv/display.h>
#include <utils.h>
#include <gfx.h>
#include <screen.h>
#include <time.h>
#include <font.h>
#include <stdio.h>
#include <screens/home_screen.h>
#include <uart.h>
#include <platform.h>
#include <stdlib.h>
#include <debug.h>
#include <config.h>
#include <imgui.h>

#ifdef PLATFORM_SIM
    #include <raylib.h>
    #include <raymath.h>

    RenderTexture2D framebuffer;
#endif

screen_t* screen;

int main() {
    bool should_close = false;
    
    uart_begin(19200);
    
    dbg_info("\n\n\nStarting CalcOS...");
    init_keyboard();
    dbg_info("Inited keyboard.");
    display_init();
    dbg_info("Inited display.");

    display_set_contrast(0);
    display_clear();

#ifdef PLATFORM_SIM
    SetExitKey(KEY_F1);
#endif

    set_draw_area(0, 0, display_width - DOCK_WIDTH, display_height);

    push_screen((screen_t*)load_home_screen());
    dbg_info("Home screen loaded.");

#if ENABLE_DOCK
    char time_str[6];
    char date_str[9];
    char batt_str[5];
#endif

    while(!should_close) {
#ifdef PLATFORM_SIM
        should_close = WindowShouldClose();
#endif
        update_keyboard();

        if(screen != NULL) {
            update_screen(screen);
        }
#if ENABLE_DOCK
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

#endif

#ifdef PLATFORM_SIM
        BeginTextureMode(framebuffer);
        should_close = WindowShouldClose();
#endif
        update_screen(screen);

        if(screen != NULL && (screen->should_redraw || imgui_should_redraw())) {
            draw_screen(screen);
            screen->should_redraw = false;
        }

#if ENABLE_DOCK
        // unlock screen area
        set_draw_area(0, 0, display_width, display_height);
        draw_rectangle(display_width - DOCK_WIDTH, 0, DOCK_WIDTH, display_height, COLOR_WHITE);
        icon_t battery_icon = get_icon(batt_icon_id);

        draw_text(time_str, display_width - (DOCK_WIDTH >> 1) - (measure_str_width(time_str) >> 1), 2, COLOR_WHITE);
        draw_text(date_str, display_width - (DOCK_WIDTH >> 1) - (measure_str_width(date_str) >> 1), GLYPH_HEIGHT + 3, COLOR_WHITE);
        draw_text(batt_str, display_width - measure_str_width(batt_str) - 1, GLYPH_HEIGHT * 2 + 4, COLOR_WHITE);
        draw_icon(battery_icon, display_width - DOCK_WIDTH + 2, GLYPH_HEIGHT * 2 + 5, (batt_icon_id == ICON_BATTERY_0 ? COLOR_RED : COLOR_WHITE));
        // draw_text(ram_usage, DISPLAY_WIDTH - measure_str_width(ram_usage) - 1, GLYPH_HEIGHT * 3 + 5, COLOR_BLACK);

        for(int i = 0; i < OPTION_NONE; i++) {
            if(screen->options[i] != NULL) {
                icon_t icon = get_icon(opt_to_icon(i));
                const char* text = screen->options[i];

                int icon_x = DISPLAY_WIDTH - icon.width - 2;
                int text_x = icon_x - measure_str_width(text) - 1;
                int text_y = DISPLAY_HEIGHT - 2 - GLYPH_HEIGHT - GLYPH_HEIGHT * i - i;
                int icon_y = text_y + 1;

                draw_icon(icon, icon_x, icon_y, COLOR_BLACK);
                draw_text(text, text_x, text_y, COLOR_BLACK);
            }
        }

        set_draw_area(0, 0, DISPLAY_WIDTH - DOCK_WIDTH, DISPLAY_HEIGHT);
#endif

#ifdef PLATFORM_SIM
        EndTextureMode();
#endif

        display_update();
    }

    unload_screen(get_current_screen());
    uart_end();

    return 0;
}