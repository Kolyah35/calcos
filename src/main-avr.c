#include <drv/display.h>
#include <utils.h>
#include <gfx.h>
#include <screen.h>
#include <time.h>
#include <font.h>
#include <stdio.h>
#include <screens/home_screen.h>
#include <uart.h>

int main() {
    uart_begin(19200);
    init_display();
    set_display_contrast(13);
    clear_display();
    set_draw_area(0, 0, 128, 64);

    push_screen((screen_t*)load_home_screen());
    char time_str[6];
    char date_str[9];
    char batt_str[5];

    while(1) {
        screen_t* screen = get_current_screen();

        if(screen->update_callback != NULL) {
            screen->update_callback();
        }

        if(screen->draw_callback != NULL) {
            set_draw_area(0, 0, SCREEN_WIDTH - DOCK_WIDTH, SCREEN_HEIGHT);
            screen->draw_callback();
        }

        // unlock screen area
        set_draw_area(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        draw_rectangle(SCREEN_WIDTH - DOCK_WIDTH, 0, DOCK_WIDTH, SCREEN_HEIGHT, COLOR_BLACK);
        draw_rectangle_filled(SCREEN_WIDTH - DOCK_WIDTH + 1, 1, DOCK_WIDTH - 2, SCREEN_HEIGHT - 2, COLOR_WHITE);

        uint8_t batt_prc = 100;
        uint8_t batt_icon_id = ICON_BATTERY_100;
        time_t tm = time(NULL);
        struct tm* date = localtime(&tm);

        sprintf(time_str, "%02u:%02u", date->tm_hour, date->tm_min);
        sprintf(date_str, "%02u.%02u.%02u", date->tm_mday, date->tm_mon + 1, CLAMP(date->tm_year - 100, 0, 99));
        sprintf(batt_str, "%u%%", batt_prc);

        if(batt_prc < 90) batt_icon_id = ICON_BATTERY_80;
        if(batt_prc < 80) batt_icon_id = ICON_BATTERY_60;
        if(batt_prc < 60) batt_icon_id = ICON_BATTERY_40;
        if(batt_prc < 40) batt_icon_id = ICON_BATTERY_20;
        if(batt_prc < 20) batt_icon_id = ICON_BATTERY_0;

        icon_t battery_icon = get_icon(batt_icon_id);

        draw_text(time_str, SCREEN_WIDTH - DOCK_WIDTH / 2 - measure_str_width(time_str) / 2, 2, COLOR_BLACK);
        draw_text(date_str, SCREEN_WIDTH - DOCK_WIDTH / 2 - measure_str_width(date_str) / 2, 9, COLOR_BLACK);
        draw_text(batt_str, SCREEN_WIDTH - measure_str_width(batt_str) - 1, 16, COLOR_BLACK);
        draw_icon(battery_icon, SCREEN_WIDTH - DOCK_WIDTH + 2, 18, (batt_icon_id == ICON_BATTERY_0 ? COLOR_RED : COLOR_BLACK));

        for(int i = OPTION_CENTER; i < OPTION_NONE; i++) {
            if(screen->options[i]) {
                icon_t icon = get_icon(opt_to_icon(i));
                const char* text = screen->options[OPTION_BOTTOM];

                int icon_x = SCREEN_WIDTH - icon.width - 2;
                int icon_y = SCREEN_HEIGHT - icon.height * i - 1;

                int text_x = icon_x - measure_str_width(text) - 1;
                int text_y = SCREEN_HEIGHT - GLYPH_HEIGHT * i;

                draw_icon(icon, icon_x, icon_y, COLOR_BLACK);
                draw_text(text, text_x, text_y, COLOR_BLACK);
            }
        }

        update_display();
    }

    return 0;
}