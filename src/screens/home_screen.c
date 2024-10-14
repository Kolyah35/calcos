#include <home_screen.h>
#include <gfx.h>
#include <stdlib.h>
#include <stdio.h>
#include <text.h>
#include <menu.h>
#include <platform.h>

home_screen_t* home_screen = NULL;
char str[20];

const ui_menu_element home_elements[] PROGMEM = {
    {"Test", NULL},
    {"Test", NULL},
    {"Test", NULL},
    {"Test", NULL}
};

home_screen_t* load_home_screen() {
    if(!home_screen) {
        home_screen = (home_screen_t*)malloc(sizeof(home_screen_t));
    }

    home_screen->screen.type = SCREEN_HOME;
    home_screen->screen.node_count = 0;
    home_screen->screen.locked = false;
    home_screen->screen.ui_nodes = malloc(sizeof(void*));
    home_screen->screen.options[OPTION_TOP] = NULL;
    home_screen->screen.options[OPTION_CENTER] = NULL;
    home_screen->screen.options[OPTION_BOTTOM] = "Menu";

    return home_screen;
}

void unload_home_screen() {
    free(home_screen);
}

void update_home_screen() {
    if(get_pressed_key() == BUTTON_PLUS) {
        ui_menu_element* elements = malloc(sizeof(home_elements));
        memcpy_P(elements, home_elements, sizeof(home_elements));

        ui_menu_t* menu = load_ui_menu("Menu", elements, sizeof(home_elements) / sizeof(ui_menu_element));
        add_node_to_screen((screen_t*)home_screen, (ui_node_t*)menu);
    }
}