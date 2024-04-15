// https://www.youtube.com/watch?v=YR4I0G_ILk4

#include <avr/io.h>

#define PB0 (1 << 0)

int main() {
    DDRB = PB0; // Configure pin PB0 as output
    PORTB = PB0; // Set the pin PB0 to high
    return 0;
}
