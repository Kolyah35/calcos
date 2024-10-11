#include <platform.h>

#include <uart.h>
#include <utils.h>
#ifdef PLATFORM_AVR
    #include <avr/io.h>
#endif

#if _WIN32
// HANDLE hSerial;
#endif

void uart_begin(uint16_t baudrate) {
#ifdef PLATFORM_AVR
    uint16_t speed = (F_CPU / (8L * baudrate)) - 1;
    UBRR0H = HIGH_BYTE(speed);
    UBRR0L = LOW_BYTE(speed);
    UCSR0A = (1 << U2X0);
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
#elif _WIN32
    // hSerial = CreateFile(FLASH_PORT, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    // DCB dcbSerialParams = {0};
    // dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    // dcbSerialParams.BaudRate = baudrate;
    // dcbSerialParams.ByteSize = 8;
    // dcbSerialParams.StopBits = ONESTOPBIT;
    // dcbSerialParams.Parity = NOPARITY;

    // SetCommState(hSerial, &dcbSerialParams);
#endif
}

void uart_write(uint8_t data) {
#ifdef PLATFORM_AVR
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
#elif _WIN32
    // WriteFile(hSerial, data, sizeof(data), NULL, NULL);
#endif
}

void uart_print_str(const char* str) {
    while(*str) uart_write(*str++);
}

bool uart_available() {
#ifdef PLATFORM_AVR
    return (UCSR0A & (1 << RXC0));
#elif _WIN32
    // return QueryDosDevice(FLASH_PORT, NULL, NULL);
    return false;
#endif
}

uint8_t uart_read() {
    uint8_t data = 0;
#ifdef PLATFORM_AVR
    data = UDR0;
#elif _WIN32
    // ReadFile(hSerial, &data, sizeof(data), NULL, NULL);
#endif

    return data;
}

void uart_end() {
#ifdef PLATFORM_AVR
    UCSR0B = 0;
#elif _WIN32
    // CloseHandle(hSerial);
#endif
}