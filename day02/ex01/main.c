#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

#define PWM_FREQ 1000
#define OOGA 0.000001

volatile float dutyCycle = 0;
bool increasing = true;

ISR(TIMER0_COMPA_vect) {
    if (increasing) {
        dutyCycle += OOGA;
        if (dutyCycle >= 1) {
            dutyCycle = 1;
            increasing = false;
        }
    } else {
        dutyCycle -= OOGA;
        if (dutyCycle <= 0) {
            dutyCycle = 0;
            increasing = true;
        }
    }
    OCR1A = dutyCycle * 0x1fff;
}

void setupTimer0() {
    TCCR0A = 0;
    TCCR0B = (TCCR0B & 0b11111000) | (1 << CS00);
    TIMSK0 |= 1 << OCIE0A;
}

void setupTimer1() {
    DDRB |= 1 << PB1;
    TCCR1A |= 1 << WGM10 | 1 << WGM11 | 1 << COM1A1;
    TCCR1B |= 1 << WGM12 | 1 << CS10;
    OCR1A = 0;
}

int main() {
    setupTimer0();
    setupTimer1();
    sei();
    while (true) _delay_ms(1);
}
