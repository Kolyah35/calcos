#include <platform.h>
#include <analog.h>
#include <digital.h>
#include <utils.h>

#ifdef CALCOS_AVR

#include <avr/io.h>

void analog_write(uint8_t pin, uint16_t duty) {
    if (!duty) {
        digital_write(pin, false);
        return;
    }

    switch (pin) {
        case 5:
            BIT_SET(TCCR0A, COM0B1);    // Enable hardware timer output
            OCR0B = duty;               // Load duty to compare register
            return;
        case 6:
            BIT_SET(TCCR0A, COM0A1);
            OCR0A = duty;
            return;
        case 10:
            BIT_SET(TCCR1A, COM1B1);
            OCR1B = duty;
            return;
        case 9:
            BIT_SET(TCCR1A, COM1A1);
            OCR1A = duty;
            return;
        case 3:
            BIT_SET(TCCR2A, COM2B1);
            OCR2B = duty;
            return;
        case 11:
            BIT_SET(TCCR2A, COM2A1);
            OCR2A = duty;
            return;
    }
}

uint16_t analog_read(uint8_t pin) {
    pin = ((pin < 8) ? pin : pin - 14);
    ADMUX = (DEFAULT << 6) | pin;
    BIT_SET(ADCSRA, ADSC);

    while (ADCSRA & (1 << ADSC));
    return ADC;
}

#endif // CALCOS_AVR