#include <home_screen.h>
#include <gfx.h>
#include <stdlib.h>
#include <text.h>
#include <menu.h>
#include <platform.h>
#include <wallpaper.h>
#include <flasher_screen.h>
#include <uart.h>

PGM_P home_elements[] = {"Flasher"};

home_screen_t* load_home_screen() {
    home_screen_t* home_screen = (home_screen_t*)malloc(sizeof(home_screen_t));

    init_screen((screen_t*)home_screen, SCREEN_HOME, NULL, NULL, "Menu");

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
            img_bytes[y * wallpaper_width + x] = wallpaper_bytes[y * (WALLPAPER_WIDTH / 8) + x];
#endif
        }
    }

    ui_image_t* wallpaper = load_ui_image(WALLPAPER_FORMAT, img_bytes, (wallpaper_width * 8), WALLPAPER_HEIGHT);
    wallpaper->node.x = 0;
    wallpaper->node.y = 0;
    wallpaper->node.width = (wallpaper_width * 8);
    wallpaper->node.height = WALLPAPER_HEIGHT;

    add_node_to_screen((screen_t*)home_screen, (ui_node_t*)wallpaper);

    return home_screen;
}

void update_home_screen(home_screen_t* home_screen) {
    if(get_pressed_key() == BUTTON_PLUS) {
        const char** elements = malloc(sizeof(home_elements));
        memcpy_P(elements, home_elements, sizeof(home_elements));

        ui_menu_t* menu = load_ui_menu("Menu", elements, 1, home_screen_on_option);
        add_node_to_screen((screen_t*)home_screen, (ui_node_t*)menu);
    }
}

void home_screen_on_option(void* sender, int index) {
    flasher_screen_t* flasher_screen = load_flasher_screen();
    push_screen((screen_t*)flasher_screen);
}