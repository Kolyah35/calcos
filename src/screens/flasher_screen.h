#pragma once
#include <screen.h>
#include <types.h>

typedef struct flasher_screen_t {
    screen_t screen;

    char* sel_str;
} flasher_screen_t;

flasher_screen_t* load_flasher_screen();
void unload_flasher_screen(flasher_screen_t* flasher_screen);
void update_flasher_screen(flasher_screen_t* flasher_screen);