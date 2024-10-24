#pragma once
#include <node.h>
#include <screen.h>


typedef void(*menu_callback_t)(void* sender, int index);

typedef struct {
    ui_node_t node;

    const char* title;

    const char** elements;
    int element_count;
    int selected_element;

    menu_callback_t callback;
} ui_menu_t;

ui_menu_t* load_ui_menu(const char* title, const char** elements, int element_count, menu_callback_t callback);
void unload_ui_menu(ui_menu_t* menu);
void draw_ui_menu(ui_menu_t* menu);
void update_ui_menu(ui_menu_t* menu);