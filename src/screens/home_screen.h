#pragma once
#include <screen.h>
#include <types.h>
#include <ui/menu.h>

typedef struct home_screen_t {
    screen_t screen;
} home_screen_t;

home_screen_t* load_home_screen();
void update_home_screen(home_screen_t* home_screen);
void home_screen_on_option(void* sender, int index);