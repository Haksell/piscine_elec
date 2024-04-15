#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

#define DEBOUNCE_ITERATIONS 20

int main() {
    DDRB = 1 << PB0; // Configure pin PB0 as output
    int counter = 0;
    while (true) {
        bool is_released = (PIND >> PD2) & 1; // Check state of PD2
        counter = is_released ? 0 : counter > DEBOUNCE_ITERATIONS ? counter : counter + 1;
        if (counter == DEBOUNCE_ITERATIONS) {
            PORTB ^= 1 << PB0; // Switch the state of the LED
        }
        _delay_ms(1);
    }
    return 0;
}
