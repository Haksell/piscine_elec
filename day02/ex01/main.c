#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

#define OCRA1_STEP 0.000064 // slightly too fast
#define IRC1_RESET 999

volatile float dutyCycle = 0;
bool increasing = true;

ISR(TIMER0_OVF_vect) {
    if (increasing) {
        dutyCycle += OCRA1_STEP;
        if (dutyCycle >= 1) {
            dutyCycle = 1;
            increasing = false;
        }
    } else {
        dutyCycle -= OCRA1_STEP;
        if (dutyCycle <= 0) {
            dutyCycle = 0;
            increasing = true;
        }
    }
    OCR1A = dutyCycle * IRC1_RESET;
}

void setupTimer0() {
    // Normal mode, no prescaling
    TCCR0A = 0;
    TCCR0B = (TCCR0B & 0b11111000) | (1 << CS00);
    TIMSK0 |= 1 << TOIE0;
}

void setupTimer1() {
    DDRB |= 1 << PB1;
    OCR1A = 0;
    // Light OC1A on when 0 <= TCNT1 < OCR1A, off when OCR1A <= TCNT1 < ICR1
    TCCR1A |= 1 << WGM11 | 1 << COM1A1;
    ICR1 = IRC1_RESET;
    TCCR1B |= 1 << WGM12 | 1 << WGM13 | 1 << CS10;
}

int main() {
    setupTimer0();
    setupTimer1();
    sei();
    while (true) _delay_ms(1);
}
