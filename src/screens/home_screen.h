#pragma once
#include <screen.h>

typedef struct home_screen_t {
    void(*update_callback)();
    void(*draw_callback)();
} home_screen_t;

void init_home_screen();
home_screen_t* get_home_screen();
void update_home_screen();
void draw_home_screen();