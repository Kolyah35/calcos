#include <utils.h>
#include <avr/io.h>
#include <util/delay.h>

int main() {
    BIT_SET(DDRB, 5);

    while(1) {
        BIT_SET(PORTB, 5);
        _delay_ms(DELAY);
        BIT_CLEAR(PORTB, 5);
        _delay_ms(DELAY);
    }
}