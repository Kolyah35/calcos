#pragma once
#include <keyboard.h>
#include <types.h>
#include <icons.h>
#include <node.h>

typedef struct screen_t {
    void(*unload_callback)();
    void(*update_callback)();

    bool should_redraw;

    const char* options[3];

    ui_node_t** ui_nodes;
    int node_count;
} screen_t;

void push_screen(screen_t* screen);
screen_t* get_current_screen();

option_t key_to_option(key_t key);
eicon_t opt_to_icon(option_t opt);

int add_node_to_screen(screen_t* screen, ui_node_t* node);
void delete_node_from_screen(int index, bool free_node);

// void draw_screen(screen_t* screen);