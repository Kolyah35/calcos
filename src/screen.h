#pragma once
#include <types.h>

typedef struct screen_t {
    void(*unload_callback)();
    void(*update_callback)();
    void(*draw_callback)();
    void(*key_callback)(key_t key);

    const char* options[3];
} screen_t;

void push_screen(screen_t* screen);
screen_t* get_current_screen();