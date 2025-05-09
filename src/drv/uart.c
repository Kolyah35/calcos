#include <uart.h>
#include <stdio.h>
#include <stdarg.h>

void uart_puts(const char* str) {
    while(*str) uart_write(*str++);
}

void uart_print(const char* fmt, ...) {
    char buf[MAX_UART_BUF];
    va_list argptr;

    va_start(argptr, fmt);
    vsnprintf(buf, MAX_UART_BUF, fmt, argptr);
    va_end(argptr);
    
    uart_puts(buf);
}