#include "uart.h"

void uart_puts(const char* str) {
    while (*str) {
        uart_putc(*str++);
    }
}

void uart_write(const void* data, long len) {
    const uint8_t* ptr = (const uint8_t*)data;
    while (len--) {
        uart_putc(*ptr++);
    }
}

long uart_read(void* buffer, long max_len) {
    char* out = (char*)buffer;
    long count = 0;

    while (count < max_len) {
        int c = uart_getc();
        if (c < 0) break;

        out[count++] = (char)c;
    }

    return count;
}

#ifndef ARCH_HAS_PROGMEM
void uart_puts_p(const char* str) {
    uart_puts(str);
}
#endif