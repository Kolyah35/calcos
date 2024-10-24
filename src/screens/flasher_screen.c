#include <flasher_screen.h>
#include <ui/text.h>
#include <ui/menu.h>
#include <keyboard.h>
#include <stdlib.h>

flasher_screen_t* load_flasher_screen() {
    flasher_screen_t* flasher_screen = (flasher_screen_t*)malloc(sizeof(flasher_screen_t));

    init_screen((screen_t*)flasher_screen, SCREEN_FLASHER, NULL, "Select", "Flash!");

    flasher_screen->sel_str = (char*)malloc(64);
    flasher_screen->sel_str = "Selected firm:\nNone";

    ui_text_min_t* selected_firmware = load_ui_text_min(COLOR_BLACK, NULL);
    selected_firmware->text = flasher_screen->sel_str;
    selected_firmware->x = 1;
    selected_firmware->y = 1;

    add_node_to_screen((screen_t*)flasher_screen, (ui_node_t*)selected_firmware);
    return flasher_screen;
}

void unload_flasher_screen(flasher_screen_t* flasher_screen) {
    free(flasher_screen->sel_str);
}

void update_flasher_screen(flasher_screen_t* flasher_screen) {
    key_t key = get_pressed_key();

    switch(key_to_option(key)) {
        case OPTION_CENTER:
            const char** elements = malloc(sizeof(char*) * 2);
            elements[0] = "blink500.hex";
            elements[1] = "blink1000.hex";

            ui_menu_t* menu = load_ui_menu("Firmwares", elements, 2, NULL);
            add_node_to_screen((screen_t*)flasher_screen, (ui_node_t*)menu);
            break;

        default: 
            break;
    }
}