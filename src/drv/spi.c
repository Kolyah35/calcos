#include <spi.h>
#include <digital.h>
#include <stdbool.h>
#include <utils.h>
#include <avr/io.h>

bool spi_initialized = 0;
bool spi_in_transaction_flag = 0;
uint8_t spi_interrupt_mode = 0;  // 0=none, 1=mask, 2=global
uint8_t spi_interrupt_mask = 0;  // which interrupts to mask
uint8_t spi_interrupt_save = 0;  // temp storage, to restore state


void spi_settings_init_default(spi_settings_t* settings) {
    spi_settings_init(settings, 4000000, MSBFIRST, SPI_MODE0);
}

void spi_settings_init(spi_settings_t* settings, uint32_t clock, uint8_t bit_order, uint8_t data_mode) {
    uint8_t clock_div;

    if (__builtin_constant_p(clock)) {
        clock_div = 6;

        if (clock >= F_CPU >> 1) {
            clock_div = 0;
        }
        
        if (clock >= F_CPU >> 2) {
            clock_div = 1;
        }
        
        if (clock >= F_CPU >> 3) {
            clock_div = 2;
        }
        
        if (clock >= F_CPU >> 4) {
            clock_div = 3;
        }
        
        if (clock >= F_CPU >> 5) {
            clock_div = 4;
        }
        
        if (clock >= F_CPU >> 6) {
            clock_div = 5;
        }
    } else {
        uint32_t clockSetting = F_CPU >> 1;

        clock_div = 0;
        
        while (clock_div < 6 && clock < clockSetting) {
            clockSetting /= 2;
            clock_div++;
        }
    }

     // Compensate for the duplicate fosc/64
    if (clock_div == 6) clock_div++;

    // Invert the SPI2X bit
    clock_div ^= 0x1;

    // Pack into the SPISettings class
    settings->spcr = _BV(SPE) | _BV(MSTR) | ((bit_order == LSBFIRST) ? _BV(DORD) : 0) |
    (data_mode & SPI_MODE_MASK) | ((clock_div >> 1) & SPI_CLOCK_MASK);
    settings->spsr = clock_div & SPI_2XCLOCK_MASK;
}

void spi_begin() {
    uint8_t sreg = SREG;

    __asm volatile("cli");

    if(!spi_initialized) {
        digital_write(PIN_SPI_SS, true);
        pin_mode(PIN_SPI_SS, OUTPUT);

        SPCR |= _BV(MSTR);
        SPCR |= _BV(SPE);

        pin_mode(PIN_SPI_SCK, OUTPUT);
        pin_mode(PIN_SPI_MOSI, OUTPUT);

        spi_initialized = true;
    }

    SREG = sreg;
}

void spi_end() {
    uint8_t sreg = SREG;

    __asm volatile("cli");

    if(spi_initialized) {
        SPCR &= ~_BV(SPE);
        spi_interrupt_mode = 0;
    }

    SREG = sreg;
}

void spi_use_interrupt(uint8_t interrupt_number) {
    uint8_t mask = 0;

    __asm volatile("cli");

    switch (interrupt_number) {
#ifdef SPI_INT0_MASK
        case 0: mask = SPI_INT0_MASK; break;
#endif
#ifdef SPI_INT1_MASK
        case 1: mask = SPI_INT1_MASK; break;
#endif
#ifdef SPI_INT2_MASK
        case 2: mask = SPI_INT2_MASK; break;
#endif
#ifdef SPI_INT3_MASK
        case 3: mask = SPI_INT3_MASK; break;
#endif
#ifdef SPI_INT4_MASK
        case 4: mask = SPI_INT4_MASK; break;
#endif
#ifdef SPI_INT5_MASK
        case 5: mask = SPI_INT5_MASK; break;
#endif
#ifdef SPI_INT6_MASK
        case 6: mask = SPI_INT6_MASK; break;
#endif
#ifdef SPI_INT7_MASK
        case 7: mask = SPI_INT7_MASK; break;
#endif
        default:
            spi_interrupt_mode = 2;
            break;
    }

    spi_interrupt_mask |= mask;

    if (!spi_interrupt_mode) {
        spi_interrupt_mode = 1;
    }
}

void spi_use_interrupt_invert(uint8_t interrupt_number) {
    uint8_t mask = 0;

    __asm volatile("cli");

    switch (interrupt_number) {
#ifdef SPI_INT0_MASK
        case 0: mask = SPI_INT0_MASK; break;
#endif
#ifdef SPI_INT1_MASK
        case 1: mask = SPI_INT1_MASK; break;
#endif
#ifdef SPI_INT2_MASK
        case 2: mask = SPI_INT2_MASK; break;
#endif
#ifdef SPI_INT3_MASK
        case 3: mask = SPI_INT3_MASK; break;
#endif
#ifdef SPI_INT4_MASK
        case 4: mask = SPI_INT4_MASK; break;
#endif
#ifdef SPI_INT5_MASK
        case 5: mask = SPI_INT5_MASK; break;
#endif
#ifdef SPI_INT6_MASK
        case 6: mask = SPI_INT6_MASK; break;
#endif
#ifdef SPI_INT7_MASK
        case 7: mask = SPI_INT7_MASK; break;
#endif
        default:
            break;
    }

    spi_interrupt_mask &= ~mask;

    if (!spi_interrupt_mode) {
        spi_interrupt_mode = 0;
    }
}

void spi_begin_transaction(spi_settings_t settings) {
    uint8_t sreg = SREG;
    if (spi_interrupt_mode > 0) {
        __asm volatile("cli");

#ifdef SPI_AVR_EIMSK
        if (spi_interrupt_mode == 1) {
                spi_interrupt_save = SPI_AVR_EIMSK;
                SPI_AVR_EIMSK &= ~spi_interrupt_mask;
                SREG = sreg;
        } else
#endif
        {
            spi_interrupt_save = sreg;
        }
    }

#ifdef SPI_TRANSACTION_MISMATCH_LED
    if (spi_in_transaction_flag) {
        pin_mode(SPI_TRANSACTION_MISMATCH_LED, OUTPUT);
        digital_write(SPI_TRANSACTION_MISMATCH_LED, true);
    }

    spi_in_transaction_flag = 1;
#endif

    SPCR = settings.spcr;
    SPSR = settings.spsr;
}

uint8_t spi_transfer(uint8_t data) {
    SPDR = data;

    /*
     * The following NOP introduces a small delay that can prevent the wait
     * loop form iterating when running at the maximum speed. This gives
     * about 10% more speed, even if it seems counter-intuitive. At lower
     * speeds it is unnoticed.
     */
    __asm volatile("nop");
    while (!(SPSR & _BV(SPIF)));
    return SPDR;
}

uint16_t spi_transfer16(uint16_t data) {
    union { uint16_t val; struct { uint8_t lsb; uint8_t msb; }; } in, out;

    in.val = data;

    if (!(SPCR & _BV(DORD))) {
        SPDR = in.msb;
        __asm volatile("nop"); // See transfer(uint8_t) function
        while (!(SPSR & _BV(SPIF)));
        
        out.msb = SPDR;
        SPDR = in.lsb;
        
        __asm volatile("nop");
        
        while (!(SPSR & _BV(SPIF)));
        out.lsb = SPDR;
    } else {
        SPDR = in.lsb;
        __asm volatile("nop");
        while (!(SPSR & _BV(SPIF)));
        
        out.lsb = SPDR;
        SPDR = in.msb;
        __asm volatile("nop");
        while (!(SPSR & _BV(SPIF)));
        
        out.msb = SPDR;
    }

    return out.val;
}

void spi_transfer_n(void *buf, size_t n) {
    if (n == 0) return;

    uint8_t *p = (uint8_t *)buf;
    SPDR = *p;

    while (--n > 0) {
        uint8_t out = *(p + 1);
        while (!(SPSR & _BV(SPIF)));

        uint8_t in = SPDR;
        SPDR = out;
        *p++ = in;
    }

    while (!(SPSR & _BV(SPIF))) ;
    
    *p = SPDR;
}

void spi_end_transaction(void) {
#ifdef SPI_TRANSACTION_MISMATCH_LED
    if (!spi_in_transaction_flag) {
        pin_mode(SPI_TRANSACTION_MISMATCH_LED, OUTPUT);
        digital_write(SPI_TRANSACTION_MISMATCH_LED, true);
    }

    spi_in_transaction_flag = 0;
#endif

    if (spi_interrupt_mode > 0) {
#ifdef SPI_AVR_EIMSK
        uint8_t sreg = SREG;
#endif
        __asm volatile("cli");
#ifdef SPI_AVR_EIMSK
        if (spi_interrupt_mode == 1) {
            SPI_AVR_EIMSK = spi_interrupt_save;
            SREG = sreg;
        } else
#endif
        {
            SREG = spi_interrupt_save;
        }
    }
}