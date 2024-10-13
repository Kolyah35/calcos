#include <dock.h>
#include <icons.h>
#include <time.h>
#include <config.h>
#include <gfx.h>
#include <font.h>
#include <screen.h>
#include <stdio.h>

char time_str[6];
char date_str[9];
char batt_str[5];

uint8_t batt_prc = 100;
uint8_t batt_icon_id = ICON_BATTERY_100;

void update_dock(void) {
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
}

void draw_dock(screen_t* current_screen) {
    set_draw_area(SCREEN_WIDTH - DOCK_WIDTH, 0, DOCK_WIDTH, SCREEN_HEIGHT);

    draw_rectangle_filled(SCREEN_WIDTH - DOCK_WIDTH + 1, 1, DOCK_WIDTH - 2, SCREEN_HEIGHT - 2, COLOR_WHITE);
    draw_rectangle(SCREEN_WIDTH - DOCK_WIDTH, 0, DOCK_WIDTH, SCREEN_HEIGHT, COLOR_BLACK);

    if(!current_screen) {
        return;
    }

    for(int i = OPTION_CENTER; i < OPTION_NONE; i++) {
        if(current_screen->options[i]) {
            icon_t icon = get_icon(opt_to_icon(i));
            const char* text = current_screen->options[OPTION_BOTTOM];

            int icon_x = SCREEN_WIDTH - icon.width - 2;
            int icon_y = SCREEN_HEIGHT - icon.height * i - 1;

            int text_x = icon_x - measure_str_width(text) - 1;
            int text_y = SCREEN_HEIGHT - GLYPH_HEIGHT * i;

            draw_icon(icon, icon_x, icon_y, COLOR_BLACK);
            draw_text(text, text_x, text_y, COLOR_BLACK);
        }
    }
}