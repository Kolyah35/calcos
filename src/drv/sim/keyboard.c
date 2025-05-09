#include <keyboard.h>
#include <platform.h>

#ifdef PLATFORM_SIM
#include <raylib.h>
#include <debug.h>

void init_keyboard() {
    dbg_info("Using raylib keyboard");
}

void update_keyboard() {
    // Nothing to do.
}

const int g_binds[] = {
    BUTTON_DOWN, KEY_DOWN,
    BUTTON_LEFT, KEY_LEFT,
    BUTTON_OK, KEY_ENTER,
    BUTTON_RIGHT, KEY_RIGHT,
    BUTTON_UP, KEY_UP
};

bool is_key_pressed(calc_key_t key) {
    for (int i = 0; i < sizeof(g_binds) / sizeof(int) - 1; i += 2) {
        if (key == g_binds[i] && IsKeyPressed(g_binds[i + 1])) {
            return true;
        }
    }

    return IsKeyPressed(key);
}

bool is_key_down(calc_key_t key) {
    for (int i = 0; i < sizeof(g_binds) / sizeof(int) - 1; i += 2) {
        if (key == g_binds[i] && IsKeyDown(g_binds[i + 1])) {
            return true;
        }
    }

    return IsKeyDown(key);
}

#endif