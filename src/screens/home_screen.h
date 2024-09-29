#pragma once
#include <screen.h>
#include <types.h>

typedef struct home_screen_t {
    void(*unload_callback)();
    void(*update_callback)();
    void(*draw_callback)();
    void(*key_callback)(key_t key);
    
    const char* options[3];
} home_screen_t;

home_screen_t* load_home_screen();
void home_screen_unload();

void home_screen_update();
void home_screen_draw();
void home_screen_on_key(key_t key);