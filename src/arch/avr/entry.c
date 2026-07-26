#ifdef __AVR__

#include <kernel.h>

int main(void) {
    kernel_init();
    kernel_loop();
}

#endif /* __AVR__ */