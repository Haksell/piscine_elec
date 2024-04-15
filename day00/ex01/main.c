// https://www.youtube.com/watch?v=YR4I0G_ILk4

#include <avr/io.h>

int main() {
    DDRB = 1 << PB0; // Configure pin PB0 as output
    PORTB = 1 << PB0; // Set the pin PB0 to high
    return 0;
}
