#include <drv/display.h>
#include <utils.h>

int main() {
    init_display();
    set_display_pixel(2, 2, *(color_t*)0x000001);

    return 0;
}