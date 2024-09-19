#pragma once

typedef struct screen_t {
    void(*update_callback)();
    void(*draw_callback)();
} screen_t;

void push_screen(screen_t* screen);
screen_t* get_current_screen();