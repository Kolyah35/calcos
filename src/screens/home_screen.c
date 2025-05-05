#include <home_screen.h>
#include <gfx.h>
#include <stdlib.h>
#include <platform.h>
#include <flasher_screen.h>
#include <uart.h>
#include <imgui.h>

PGM_P home_elements[] = {"Flasher"};

void draw_home_screen(home_screen_t* home_screen) {
    imgui_begin("Test App");
    imgui_text("Hello Dear ImGui!");
    imgui_separator_text("General");

    static int clicked = 0;
    if (imgui_button("Button")) {
        clicked++;
    }

    if (clicked & 1) {
        imgui_same_line();
        imgui_text("Thanks for clicking me!");
    }

    static bool v = true;
    imgui_checkbox("Checkbox", &v);

    static int e = 0;
    imgui_radio_button("radio a", &e, 0); imgui_same_line();
    imgui_radio_button("radio b", &e, 1); imgui_same_line();
    imgui_radio_button("radio c", &e, 2);

    for (int i = 0; i < 7; i++) {
        if (i > 0) imgui_same_line();
        imgui_button("Click");
    }

    imgui_text("Hold to repeat:");
    imgui_same_line();

    static int counter = 0;
    if (imgui_arrow_button(IMGUI_DIR_LEFT)) { counter--; } 
    imgui_same_line();
    if(imgui_arrow_button(IMGUI_DIR_RIGHT)) { counter++; }
    imgui_same_line();
    imgui_text("%i", counter);

    imgui_button("Tooltip");  
    imgui_tooltip("I am a tooltip");

    imgui_separator_text("Inputs");
    imgui_end();
}

home_screen_t* load_home_screen() {
    home_screen_t* home_screen = (home_screen_t*)malloc(sizeof(home_screen_t));

    init_screen((screen_t*)home_screen, SCREEN_HOME, NULL, NULL, "Menu");

    return home_screen;
}

void update_home_screen(home_screen_t* home_screen) {
    // if(get_pressed_key() == BUTTON_PLUS) {
    //     ui_menu_t* menu = load_ui_menu("Menu", home_elements, 1, home_screen_on_option);
    //     add_node_to_screen((screen_t*)home_screen, (ui_node_t*)menu);
    // }
}

void home_screen_on_option(void* sender, int index) {
    // flasher_screen_t* flasher_screen = load_flasher_screen();
    // push_screen((screen_t*)flasher_screen);
}