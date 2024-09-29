#include <digital.h>
#include <utils.h>
#include <avr/io.h>
// https://alexgyver.ru/arduino-algorithms/

void pin_mode(uint8_t pin, pin_mode_e mode) {
	switch (mode) {
        case INPUT:
            if (pin < 8) {
                BIT_CLEAR(DDRD, pin);    
                BIT_CLEAR(PORTD, pin);
            } else if (pin < 14) {
                BIT_CLEAR(DDRB, (pin - 8));
                BIT_CLEAR(PORTB, (pin - 8));
            } else if (pin < 20) {
                BIT_CLEAR(DDRC, (pin - 14));   // Mode: INPUT
                BIT_CLEAR(PORTC, (pin - 14));  // State: LOW
            }
            return;
        case OUTPUT:
            if (pin < 8) {
                BIT_SET(DDRD, pin);
                BIT_CLEAR(PORTD, pin);
            } else if (pin < 14) {
                BIT_SET(DDRB, (pin - 8));
                BIT_CLEAR(PORTB, (pin - 8));
            } else if (pin < 20) {
                BIT_SET(DDRC, (pin - 14));  // Mode: OUTPUT
                BIT_CLEAR(PORTC, (pin - 14));  // State: LOW
            }
            return;
        case INPUT_PULLUP:
            if (pin < 8) {
                BIT_CLEAR(DDRD, pin);
                BIT_SET(PORTD, pin);
            } else if (pin < 14) {
                BIT_CLEAR(DDRB, (pin - 8));
                BIT_SET(PORTB, (pin - 8));
            } else if (pin < 20) {
                BIT_CLEAR(DDRC, (pin - 14));  // Mode: OUTPUT
                BIT_SET(PORTC, (pin - 14));  // State: HIGH
            }
            return;
    }
}

void digital_write(uint8_t pin, bool val) {
    if (pin < 8) {
        BIT_WRITE(PORTD, pin, val);
    }
    
    if (pin < 14) {
        BIT_WRITE(PORTB, (pin - 8), val);
    }
    
    if (pin < 20) {
        BIT_WRITE(PORTC, (pin - 14), val);    // Set pin to HIGH / LOW 
    }
}

void digital_toggle(uint8_t pin) {
    if (pin < 8) {
        BIT_SET(PIND, pin);
    } else if (pin < 14) {
        BIT_SET(PINB, (pin - 8));
    } else if (pin < 20) {
        BIT_SET(PINC, (pin - 14));    // Toggle pin state (for 'tone()')
    }
}

bool digital_read(uint8_t pin) {
    if (pin < 8) {
        return BIT_READ(PIND, pin);
    }
    
    if (pin < 14) {
        return BIT_READ(PINB, pin - 8);
    } 
    
    if (pin < 20) {
        return BIT_READ(PINC, pin - 14);    // Return pin state
    }

    return false;
}