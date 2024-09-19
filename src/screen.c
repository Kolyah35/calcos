#include <screen.h>
#include <stdlib.h>

screen_t* screen = 0;

void push_screen(screen_t* _screen) {
    free(screen);
    screen = _screen;
}

screen_t* get_current_screen() {
    return screen;
}