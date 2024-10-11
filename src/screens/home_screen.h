#pragma once
#include <screen.h>
#include <types.h>

typedef struct home_screen_t {
    screen_t screen;
} home_screen_t;

home_screen_t* load_home_screen();
void unload_home_screen();

void home_screen_update();
void home_screen_draw();