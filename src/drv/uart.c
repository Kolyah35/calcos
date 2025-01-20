#include <platform.h>

#include <uart.h>
#include <utils.h>
#include <stdio.h>
#include <debug.h>

#ifdef PLATFORM_AVR
    #include <avr/io.h>
#endif

#if SIM_UART
    #ifdef _WIN32
        HANDLE serial;
    #endif

    #ifdef __linux__
        #include <fcntl.h>
        #include <unistd.h>
        #include <termios.h>
        #include <sys/ioctl.h>
        #include <sys/select.h>
        
        int serial;
    #endif
#endif

void uart_begin(uint32_t baudrate) {
#ifdef PLATFORM_AVR
    uint16_t speed = (F_CPU / (8L * baudrate)) - 1;
    UBRR0H = HIGH_BYTE(speed);
    UBRR0L = LOW_BYTE(speed);
    UCSR0A = (1 << U2X0);
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
#elif defined(_WIN32) && SIM_UART
    serial = CreateFile(FLASH_PORT, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if(serial == INVALID_HANDLE_VALUE) {
        dbg_err("Failed to open serial port \"%s\"", FLASH_PORT)
        return;
    }

    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    dcbSerialParams.BaudRate = baudrate;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    SetCommState(serial, &dcbSerialParams);
#elif defined(__linux__) && SIM_UART
    serial = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY);
    if (serial == -1) {
        dbg_err("Opening port error!");
        return;
    }

    struct termios options;
    tcgetattr(serial, &options);
    cfsetispeed(&options, baudrate);
    cfsetospeed(&options, baudrate);
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_iflag &= ~(IXON | IXOFF | IXANY);
    options.c_oflag &= ~OPOST;
    options.c_oflag &= ~HUPCL;
    tcsetattr(serial, TCSANOW, &options);
    tcflush(serial, TCIOFLUSH);

    // wtf serial opening so slow. IT SHOULD BE FIXED!!!!
    usleep(2000000);
#endif
}

#ifndef PLATFORM_AVR
void uart_set_dtr_flag(bool flag) {
#ifdef __linux__ 
    int dtr_flag = TIOCM_DTR;
    if(ioctl(serial, (flag ? TIOCMBIC : TIOCMBIS), &dtr_flag) == -1) {
        dbg_err("Error while %s dtr flag!", (flag ? "enabling" : "disabling"));
    }
#endif
}
#endif // PLATFORM_AVR

void uart_write(uint8_t data) {
#ifdef PLATFORM_AVR
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
#elif defined(_WIN32) && SIM_UART
    WriteFile(serial, &data, 1, NULL, NULL);
#elif defined(__linux__) && SIM_UART
    write(serial, &data, 1);
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

uint8_t uart_available() {
#ifdef PLATFORM_AVR
    return (UCSR0A & (1 << RXC0));
#elif defined(_WIN32) && SIM_UART
    DWORD errors;
    COMSTAT status;

    if (!ClearCommError(hSerial, &errors, &status)) {
        dbg_err("ClearCommError error!");
        return 0;
    }
    
    return status.cbInQue;
#elif defined(__linux__) && SIM_UART
    int bytesAvailable;
    if (ioctl(serial, FIONREAD, &bytesAvailable) == -1) {
        dbg_err("ioctl error!");
        return 0;
    }

    return (uint8_t)bytesAvailable;
#else
    return 0;
#endif
}

uint8_t uart_read() {
    uint8_t data = 0;
#ifdef PLATFORM_AVR
    data = UDR0;
#elif _WIN32 && SIM_UART
    unsigned long size = 0;
    ReadFile(serial, &data, 1, &size, NULL);
#elif defined(__linux__) && SIM_UART
    read(serial, &data, 1);
#endif
    return data;
}

void uart_end() {
#ifdef PLATFORM_AVR
    UCSR0B = 0;
#elif defined(_WIN32) && SIM_UART
    CloseHandle(serial);
#elif defined(__linux__) && SIM_UART
    close(serial);
#endif
}