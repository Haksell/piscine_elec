#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

#define BLUE (1 << 3)
#define RED (1 << 5)
#define GREEN (1 << 6)

int main() {
    DDRD = 0b1101000;
    PORTD = 0;
    uint8_t n = 0;
    while (true) {
        switch (n) {
            case 0: PORTD = RED; break;
            case 1: PORTD = GREEN; break;
            case 2: PORTD = BLUE; break;
            case 3: PORTD = RED | GREEN; break;
            case 4: PORTD = GREEN | BLUE; break;
            case 5: PORTD = BLUE | RED; break;
            case 6: PORTD = RED | GREEN | BLUE; break;
        }
        _delay_ms(1000);
        n = (n + 1) % 7;
    }
}
