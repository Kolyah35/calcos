#pragma once
#include <stdint.h>

typedef struct icon_t {
    uint32_t data;
    uint8_t width;
    uint8_t height;
} icon_t;

typedef enum eicon_t {
    ICON_NONE,
    ICON_BTN_MULTIPLY,
    ICON_BTN_MINUS,
    ICON_BTN_PLUS,
    ICON_BATTERY_0,
    ICON_BATTERY_20,
    ICON_BATTERY_40,
    ICON_BATTERY_60,
    ICON_BATTERY_80,
    ICON_BATTERY_100

} eicon_t;

const icon_t* get_icon(eicon_t icon_type);