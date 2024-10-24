#pragma once
#include <keyboard.h>
#include <types.h>
#include <icons.h>
#include <node.h>

typedef enum screen_type_t {
    SCREEN_HOME = 0,
    SCREEN_FLASHER
} screen_type_t;

typedef struct screen_t {
    screen_type_t type;
    bool should_redraw;
    bool locked;
    const char* options[3];

    ui_node_t** ui_nodes;
    int node_count;
} screen_t;

extern screen_t* screen;

void push_screen(screen_t* screen);
screen_t* get_current_screen();

option_t key_to_option(key_t key);
eicon_t opt_to_icon(option_t opt);

int add_node_to_screen(screen_t* screen, ui_node_t* node);
void delete_node_from_screen_i(int index, bool free_node);
void delete_node_from_screen_ptr(ui_node_t* ptr, bool free_node);

void init_screen(screen_t* screen, uint8_t type, const char* option_top, const char* option_center, const char* option_bottom);
void update_screen(screen_t* screen);
void draw_screen(screen_t* screen);
void unload_screen(screen_t* screen);