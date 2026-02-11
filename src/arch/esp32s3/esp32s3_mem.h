#ifndef ESP32S3_MEM
#define ESP32S3_MEM
#include <stdint.h>

#define UART0_LOW   0x60000000
#define UART0_HIGH  0x60000FFF

#define SPI1_LOW    0x60002000
#define SPI1_HIGH   0x60002FFF

#define SPI0_LOW    0x60003000
#define SPI0_HIGH   0x60003FFF

#define GPIO_LOW    0x60004000
#define GPIO_HIGH   0x60004FFF

// TODO: eFuse Controller
// TODO: Low-Power Management
// TODO: IO MUX

// TODO: I2S Controller 0
#define UART1_LOW   0x60010000
#define UART1_HIGH  0x60010FFF

#define I2C_LOW     0x60013000
#define I2C_HIGH    0x60013FFF

#define UHCIO_LOW   0x60014000
#define UHCIO_HIGH  0x60014FFF

// TODO: Remote Control Peripheral
// TODO: Pulse Count Controller

// TODO: LED PWM Controller

#define SYS_LOW     0x600C0000
#define SYS_HIGH    0x600C0FFF
#endif