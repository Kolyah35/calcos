#include <home_screen.h>
#include <gfx.h>
#include <stdlib.h>
#include <stdio.h>
#include <text.h>
#include <menu.h>
#include <platform.h>
#include <wallpaper.h>

home_screen_t* home_screen = NULL;
char str[20];

const ui_menu_element home_elements[] PROGMEM = {
    {"Calculator", NULL},
    {"Flasher", NULL},
    {"Explorer", NULL},
    {"Settings", NULL}
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

    int wallpaper_width = (SCREEN_WIDTH - DOCK_WIDTH) / 8;

    if(wallpaper_width * 8 < (SCREEN_WIDTH - DOCK_WIDTH) && wallpaper_width < WALLPAPER_WIDTH / 8) {
        wallpaper_width++;
    }

    uint8_t* img_bytes = (uint8_t*)malloc(wallpaper_width * SCREEN_HEIGHT);

    
    for(int y = 0; y < 64; y++) {
        for(int x = 0; x < wallpaper_width; x++) {
#ifdef PLATFORM_AVR
            img_bytes[y * wallpaper_width + x] = pgm_read_byte(&wallpaper_bytes[y * 16 + x]);
#else
            img_bytes[y * wallpaper_width + x] = wallpaper_bytes[y * 16 + x];
#endif
        }
    }

    ui_image_t* wallpaper = load_ui_image(WALLPAPER_FORMAT, img_bytes, WALLPAPER_WIDTH, WALLPAPER_HEIGHT);
    wallpaper->node.x = 0;
    wallpaper->node.y = 0;

    add_node_to_screen((screen_t*)home_screen, (ui_node_t*)wallpaper);

    return home_screen;
}

void unload_home_screen() {
    free(home_screen);
    home_screen = NULL;
}

void update_home_screen() {
    if(get_pressed_key() == BUTTON_PLUS) {
        ui_menu_element* elements = malloc(sizeof(home_elements));
        memcpy_P(elements, home_elements, sizeof(home_elements));

        ui_menu_t* menu = load_ui_menu("Menu", elements, sizeof(home_elements) / sizeof(ui_menu_element));
        add_node_to_screen((screen_t*)home_screen, (ui_node_t*)menu);
    }
}