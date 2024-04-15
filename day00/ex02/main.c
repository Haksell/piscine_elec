// https://www.youtube.com/watch?v=YR4I0G_ILk4

#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

int main() {
    DDRB = 1 << PB0; // Configure pin PB0 as output
    while (true) {
        bool is_pressed = ((PIND >> PD2) & 1) == 0; // Check state of PD2 (0 if pressed)
        PORTB = is_pressed ? 1 << PB0 : 0; // Set the pin PB0 to on or off
        _delay_ms(20);
    }
    return 0;
}
