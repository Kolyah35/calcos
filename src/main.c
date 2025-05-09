#include "mem.h"
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
#include <drv/mem.h>
#include <timer.h>

#ifdef PLATFORM_SIM
    #include <raylib.h>
    #include <raymath.h>

    RenderTexture2D framebuffer;
#endif

screen_t* screen = NULL;

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

    set_clip(0, 0, display_width, display_height);

    push_screen((screen_t*)load_home_screen());
    dbg_info("Home screen loaded.");

    char time_str[6];
    char date_str[9];
    char ram_str[12];

    while(!should_close) {
        update_keyboard();

        if(screen != NULL) {
            update_screen(screen);
        }

#ifdef PLATFORM_SIM
        time_t tm = time(NULL);
#else
        time_t tm = millis() / 1000;
#endif
        struct tm* date = localtime(&tm);

        const char* metrics[] = {"B", "KB", "MB", "GB"};
        float used_mem = (float)get_used_mem();
        int metric_index = 0;

        while (used_mem >= 1024 && metric_index < sizeof(metrics) / sizeof(const char*)) {
            used_mem /= 1024;
            metric_index++;
        }

        sprintf(time_str, "%02u:%02u", date->tm_hour, date->tm_min);
        sprintf(date_str, "%02u.%02u.%02u", date->tm_mday, date->tm_mon + 1, date->tm_year % 100);
        sprintf(ram_str, "%.1f %s", used_mem, metrics[metric_index]);

#ifdef PLATFORM_SIM
        BeginTextureMode(framebuffer);
        should_close = WindowShouldClose();
#endif
 
        if(screen != NULL && (screen->should_redraw || imgui_should_redraw())) {
            draw_screen(screen);
            screen->should_redraw = false;
        }

        int dock_center = display_width - (DOCK_WIDTH >> 1);

        fill_rect(display_width - DOCK_WIDTH, 0, DOCK_WIDTH, display_height, COLOR_BLACK);
        draw_rect(display_width - DOCK_WIDTH, 0, DOCK_WIDTH, display_height, COLOR_WHITE);
        draw_text(time_str, dock_center - (measure_str_width(time_str) >> 1), 2, COLOR_WHITE);
        draw_text(date_str, dock_center - (measure_str_width(date_str) >> 1), GLYPH_HEIGHT + 3, COLOR_WHITE);
        draw_text(ram_str, dock_center - (measure_str_width(ram_str) >> 1), GLYPH_HEIGHT * 3, COLOR_WHITE);

#ifdef PLATFORM_SIM
        EndTextureMode();
#endif

        display_update();
    }

    unload_screen(get_current_screen());
    uart_end();

    return 0;
}