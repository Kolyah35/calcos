#include <home_screen.h>
#include <gfx.h>

home_screen_t home_screen;

void init_home_screen() {
    home_screen.update_callback = update_home_screen;
    home_screen.draw_callback = draw_home_screen;
}

home_screen_t* get_home_screen() {
    return &home_screen;
}

void update_home_screen() {}

void draw_home_screen() {
    draw_text("АБВ", 1, 1, COLOR_WHITE);    
}