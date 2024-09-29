#include <home_screen.h>
#include <gfx.h>
#include <stdlib.h>

home_screen_t* home_screen = NULL;

home_screen_t* load_home_screen() {
    if(!home_screen) {
        home_screen = (home_screen_t*)malloc(sizeof(home_screen_t));
    }

    home_screen->unload_callback = home_screen_unload;
    home_screen->update_callback = home_screen_update;
    home_screen->draw_callback = home_screen_draw;
    home_screen->key_callback = home_screen_on_key;

    home_screen->options[OPTION_TOP] = NULL;
    home_screen->options[OPTION_CENTER] = NULL;
    home_screen->options[OPTION_BOTTOM] = "Menu";

    return home_screen;
}

void home_screen_unload() {
    free(home_screen);
}

void home_screen_update() {}

void home_screen_draw() {
    draw_text("ПРИВЕТ МИР!!", 1, 1, COLOR_WHITE);    
}

void home_screen_on_key(key_t key) {

}