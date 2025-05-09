#include <timer.h>

#include PLATFORM_AVR

#ifdef PLATFORM_AVR

#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint32_t timer_millis = 0;

ISR(TIMER0_OVF_vect) {
    static uint16_t overflow_count = 0;
    overflow_count++;
    
    if (overflow_count >= 244) {
        overflow_count = 0;
        timer_millis++;
    }
}

void timer0_init() {
    TCCR0A = 0;
    TCCR0B = (1 << CS01) | (1 << CS00);
    TIMSK0 = (1 << TOIE0);
    sei();
}

uint32_t millis() {
    uint32_t m;
    
    cli();
    m = timer_millis;
    sei();

    return m;
}

#endif