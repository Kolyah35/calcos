#pragma once
#include <node.h>
#include <popup_menu.h>

typedef struct {
    ui_node_t node;

    const char** elements;
    int element_count;
    int selected_element;
} ui_popup_menu_t;

ui_popup_menu_t* load_ui_popup_menu(const char** elements, int element_count);
void draw_ui_popup_menu(ui_popup_menu_t* popup_menu);
void unload_ui_popup_menu(ui_popup_menu_t* popup_menu);
