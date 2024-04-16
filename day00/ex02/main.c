#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

int main() {
    DDRB = 1 << PB0; // Configure pin PB0 as output
    while (true) {
        bool is_released = (PIND >> PD2) & 1; // Check state of PD2
        PORTB = is_released ? 0 : 1 << PB0; // Set the pin PB0 to on or off
        _delay_ms(20);
    }
}
