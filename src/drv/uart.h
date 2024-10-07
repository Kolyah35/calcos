#pragma once
#include <stdint.h>
#include <stdbool.h>

void uart_begin(uint32_t baudrate);
void uart_write(uint8_t data);
void uart_print_str(const char* str);
bool uart_available();
uint8_t uart_read();
void uart_end();