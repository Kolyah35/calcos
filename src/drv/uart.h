#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <platform.h>

#define MAX_UART_BUF 64

void uart_begin(uint32_t baudrate);
void uart_write(uint8_t data);
void uart_puts(const char* str);
void uart_print(const char* fmt, ...);
uint8_t uart_available();
uint8_t uart_read();
void uart_end();

#ifndef PLATFORM_AVR
void uart_set_dtr_flag(bool flag);
#endif