#pragma once
#include <keyboard.h>
#include <types.h>
#include <icons.h>

typedef enum screen_type_t {
    SCREEN_HOME = 0,
    SCREEN_FLASHER
} screen_type_t;

typedef struct screen_t {
    screen_type_t type;
    bool should_redraw;
} screen_t;

extern screen_t* screen;

void push_screen(screen_t* screen);
screen_t* get_current_screen();

void init_screen(screen_t* screen, uint8_t type, const char* option_top, const char* option_center, const char* option_bottom);
void update_screen(screen_t* screen);
void draw_screen(screen_t* screen);
void unload_screen(screen_t* screen);