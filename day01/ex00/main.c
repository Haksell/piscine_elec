#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

#define LED (1 << PB1)

int main() {
    DDRB = LED;
    while (true) {
        PORTB ^= LED;
        _delay_ms(500);
    }
}
