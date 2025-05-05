#pragma once

#ifndef PLATFORM_SIM

#include <avr/io.h>
#include <stdint.h>
#include <stddef.h>

#define PIN_SPI_SS    (10)
#define PIN_SPI_MOSI  (11)
#define PIN_SPI_MISO  (12)
#define PIN_SPI_SCK   (13)

// mapping of interrupt numbers to bits within SPI_AVR_EIMSK
#if defined(__AVR_ATmega32U4__)
  #define SPI_INT0_MASK  (1<<INT0)
  #define SPI_INT1_MASK  (1<<INT1)
  #define SPI_INT2_MASK  (1<<INT2)
  #define SPI_INT3_MASK  (1<<INT3)
  #define SPI_INT4_MASK  (1<<INT6)
#elif defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__)
  #define SPI_INT0_MASK  (1<<INT0)
  #define SPI_INT1_MASK  (1<<INT1)
  #define SPI_INT2_MASK  (1<<INT2)
  #define SPI_INT3_MASK  (1<<INT3)
  #define SPI_INT4_MASK  (1<<INT4)
  #define SPI_INT5_MASK  (1<<INT5)
  #define SPI_INT6_MASK  (1<<INT6)
  #define SPI_INT7_MASK  (1<<INT7)
#elif defined(EICRA) && defined(EICRB) && defined(EIMSK)
  #define SPI_INT0_MASK  (1<<INT4)
  #define SPI_INT1_MASK  (1<<INT5)
  #define SPI_INT2_MASK  (1<<INT0)
  #define SPI_INT3_MASK  (1<<INT1)
  #define SPI_INT4_MASK  (1<<INT2)
  #define SPI_INT5_MASK  (1<<INT3)
  #define SPI_INT6_MASK  (1<<INT6)
  #define SPI_INT7_MASK  (1<<INT7)
#else
  #ifdef INT0
  #define SPI_INT0_MASK  (1<<INT0)
  #endif
  #ifdef INT1
  #define SPI_INT1_MASK  (1<<INT1)
  #endif
  #ifdef INT2
  #define SPI_INT2_MASK  (1<<INT2)
  #endif
#endif

#define SPI_HAS_TRANSACTION 1
#define SPI_HAS_NOTUSINGINTERRUPT 1
#define SPI_ATOMIC_VERSION 1

#ifndef LSBFIRST
#define LSBFIRST 0
#endif
#ifndef MSBFIRST
#define MSBFIRST 1
#endif

#define SPI_CLOCK_DIV4 0x00
#define SPI_CLOCK_DIV16 0x01
#define SPI_CLOCK_DIV64 0x02
#define SPI_CLOCK_DIV128 0x03
#define SPI_CLOCK_DIV2 0x04
#define SPI_CLOCK_DIV8 0x05
#define SPI_CLOCK_DIV32 0x06

#define SPI_MODE0 0x00
#define SPI_MODE1 0x04
#define SPI_MODE2 0x08
#define SPI_MODE3 0x0C

#define SPI_MODE_MASK 0x0C  // CPOL = bit 3, CPHA = bit 2 on SPCR
#define SPI_CLOCK_MASK 0x03  // SPR1 = bit 1, SPR0 = bit 0 on SPCR
#define SPI_2XCLOCK_MASK 0x01  // SPI2X = bit 0 on SPSR

#define SPI_SET_DATA_MODE(data_mode) \
    SPCR = (SPCR & ~SPI_MODE_MASK) | data_mode;

#define SPI_SET_CLOCK_DIVIDER(clock_div)                                        \
    SPCR = (SPCR & ~SPI_CLOCK_MASK) | (clock_div & SPI_CLOCK_MASK);              \
    SPSR = (SPSR & ~SPI_2XCLOCK_MASK) | ((clock_div >> 2) & SPI_2XCLOCK_MASK);

#define SPI_ATTACH_INTERRUPT SPCR |= _BV(SPIE);
#define SPI_DETACH_INTERRUPT SPCR &= ~_BV(SPIE);

typedef struct spi_settings_t {
    uint8_t spcr;
    uint8_t spsr;
} spi_settings_t;

void spi_settings_init_default(spi_settings_t* settings);
void spi_settings_init(spi_settings_t* settings, uint32_t clock, uint8_t bit_order, uint8_t data_mode);

void spi_begin();
void spi_use_interrupt(uint8_t interrupt_number);
void spi_use_interrupt_invert(uint8_t interrupt_number);
void spi_begin_transaction(spi_settings_t settings);
uint8_t spi_transfer(uint8_t data);
uint16_t spi_transfer16(uint16_t data);
void spi_transfer_n(void *buf, size_t n);
void spi_end_transaction(void);
void spi_end();

#endif // PLATFORM_SIM