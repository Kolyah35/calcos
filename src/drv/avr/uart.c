#include <platform.h>

#include <uart.h>
#include <utils.h>
#include <stdio.h>
#include <debug.h>

#ifdef PLATFORM_AVR
#include <avr/io.h>

void uart_begin(uint32_t baudrate) {
    uint16_t speed = (F_CPU / (8L * baudrate)) - 1;
    UBRR0H = HIGH_BYTE(speed);
    UBRR0L = LOW_BYTE(speed);
    UCSR0A = (1 << U2X0);
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_write(uint8_t data) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}

uint8_t uart_available() {
    return (UCSR0A & (1 << RXC0));
}

uint8_t uart_read() {
    uint8_t data = 0;
    data = UDR0;

    return data;
}

void uart_end() {
    UCSR0B = 0;
}

#endif // PLATFORM_AVR