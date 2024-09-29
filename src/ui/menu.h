#pragma once
#include "node.h"

struct ui_menu_element {
    const char* caption;
    void(*callback)(ui_menu_t* sender);
};


struct ui_menu_t {
    ui_node_t node;

    const char* title;

    ui_menu_element* elements;
    size_t element_count;
};

ui_menu_t* load_ui_menu(const char* title, ui_menu_element* elements, size_t element_count);
