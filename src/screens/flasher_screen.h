#pragma once
#include <screen.h>
#include <types.h>

enum flasher_state {
    FLASHER_START,
    FLASHER_RESET,
    FLASHER_PROGRAMMING_MODE,
    FLASHER_BOARD_ID_VERIFY,
    FLASHER_WRITING_BLOCK,
    FLASHER_FINISH
};

enum flasher_method {
    FLASHER_SERIAL,
    FLASHER_SPI
};

typedef struct flasher_screen_t {
    screen_t screen;

    char* sel_str;
    int selected_firm;
    int8_t state;
    int hex_file_pos;
} flasher_screen_t;

flasher_screen_t* load_flasher_screen(void);
void unload_flasher_screen(flasher_screen_t* flasher_screen);
void update_flasher_screen(flasher_screen_t* flasher_screen);
void update_flasher_state(flasher_screen_t* flasher_screen);
void on_select_flasher_firmware(void* sender, int index);