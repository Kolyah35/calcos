#include <flasher_screen.h>
#include <ui/text.h>
#include <ui/menu.h>
#include <keyboard.h>
#include <stdlib.h>

flasher_screen_t* flasher_screen = NULL;
char* sel_str;

flasher_screen_t* load_flasher_screen() {
    if(!flasher_screen) {
        flasher_screen = (flasher_screen_t*)malloc(sizeof(flasher_screen_t));
    }

    flasher_screen->screen.type = SCREEN_FLASHER;
    flasher_screen->screen.node_count = 0;
    flasher_screen->screen.locked = false;
    flasher_screen->screen.ui_nodes = malloc(sizeof(void*));
    flasher_screen->screen.options[OPTION_TOP] = NULL;
    flasher_screen->screen.options[OPTION_CENTER] = "Select firmware";
    flasher_screen->screen.options[OPTION_BOTTOM] = "Flash!";

    sel_str = (char*)malloc(64);

    ui_text_min_t* selected_firmware = load_ui_text_min(COLOR_BLACK, NULL);
    selected_firmware->text = sel_str;
    selected_firmware->x = 1;
    selected_firmware->y = 1;

    add_node_to_screen((screen_t*)flasher_screen, (ui_node_t*)selected_firmware);
}

void unload_flasher_screen() {
    free(sel_str);
}

void update_flasher_screen() {
    key_t key = get_pressed_key();

    switch(key_to_option(key)) {
        case OPTION_CENTER:
            ui_menu_t* menu = load_ui_menu("Firmwares", elements, sizeof(home_elements) / sizeof(ui_menu_element));
            add_node_to_screen((screen_t*)flasher_screen, (ui_node_t*)menu);
            break;
    }
}