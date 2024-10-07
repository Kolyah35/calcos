#pragma once
#include "node.h"

struct ui_menu_t;

typedef struct ui_menu_element {
    const char* caption;
    void(*callback)(struct ui_menu_t* sender);
} ui_menu_element;


typedef struct ui_menu_t {
    ui_node_t node;

    const char* title;

    ui_menu_element* elements;
    size_t element_count;
    size_t selected_element;
} ui_menu_t;

ui_menu_t* load_ui_menu(const char* title, ui_menu_element* elements, size_t element_count);
void unload_ui_menu(ui_menu_t* menu);
void draw_ui_menu(ui_menu_t* menu);