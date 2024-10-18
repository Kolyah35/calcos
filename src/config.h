#pragma once
// hardware
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#ifdef PLATFROM_AVR
    #define SCREEN_MONOCHROME 1
#endif

#define CS_PIN 10
#define DC_PIN 9
#define RST_PIN 8

#define MAX_KEYS_PRESSED 4

// ui
#define DOCK_WIDTH 43
#define MENU_ELEMENT_HEIGHT 5