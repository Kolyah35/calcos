#pragma once
#include "node.h"

typedef struct ui_menu_element {
    const char* caption;
    void(*callback)(struct ui_menu_t* sender);
} ui_menu_element;


typedef struct ui_menu_t {
    ui_node_t node;

    const char* title;

    ui_menu_element* elements;
    int element_count;
} ui_menu_t;

ui_menu_t* load_ui_menu(const char* title, ui_menu_element* elements, int element_count);
