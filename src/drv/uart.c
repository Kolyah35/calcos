#include <platform.h>

#include <uart.h>
#include <utils.h>
#include <stdio.h>
#include <debug.h>
#ifdef PLATFORM_AVR
    #include <avr/io.h>
#endif

#if _WIN32
HANDLE hSerial;
#endif

void uart_begin(uint32_t baudrate) {
#ifdef PLATFORM_AVR
    uint16_t speed = (F_CPU / (8L * baudrate)) - 1;
    UBRR0H = HIGH_BYTE(speed);
    UBRR0L = LOW_BYTE(speed);
    UCSR0A = (1 << U2X0);
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
#elif _WIN32 && SIM_UART
    hSerial = CreateFile(FLASH_PORT, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if(hSerial == INVALID_HANDLE_VALUE) {
        dbg_err("Failed to open serial port \"%s\"", FLASH_PORT)
    } 

    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    dcbSerialParams.BaudRate = baudrate;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    SetCommState(hSerial, &dcbSerialParams);
#endif
}

void uart_write(uint8_t data) {
#ifdef PLATFORM_AVR
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
#elif _WIN32 && SIM_UART
    uint8_t buf[] = { data }; 
    WriteFile(hSerial, (void*)&buf, sizeof(data), NULL, NULL);
#endif
}

void uart_puts(const char* str) {
    while(*str) uart_write(*str++);
}

void uart_print(const char* fmt, ...) {
    char buf[MAX_UART_BUF];
    va_list argptr;

    va_start(argptr, fmt);
    snprintf(buf, MAX_UART_BUF, fmt, argptr);
    va_end(argptr);
    
    uart_puts(buf);
}

bool uart_available() {
#ifdef PLATFORM_AVR
    return (UCSR0A & (1 << RXC0));
#elif _WIN32 && SIM_UART
    LARGE_INTEGER size;
    GetFileSizeEx(hSerial, &size);
    return (bool)(*(uint64_t*)&size);
#else
    return false;
#endif
}

uint8_t uart_read() {
    uint8_t data = 0;
#ifdef PLATFORM_AVR
    data = UDR0;
#elif _WIN32 && SIM_UART
    unsigned long size = 0;
    ReadFile(hSerial, &data, sizeof(data), &size, NULL);
#endif

    return data;
}

void uart_end() {
#ifdef PLATFORM_AVR
    UCSR0B = 0;
#elif _WIN32 && SIM_UART
    CloseHandle(hSerial);
#endif
}