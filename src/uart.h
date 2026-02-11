#ifndef UART_H
#define UART_H
#include <stdint.h>

int uart_init(uint32_t baudrate);

void uart_putc(char data);
void uart_puts(const char* str);

int uart_getc(void);

void uart_write(const void* data, long len);

long uart_read(void* buffer, long max_len);

int uart_available(void);

void uart_flush(void);

void uart_puts_p(const char* str);

#endif // UART_H