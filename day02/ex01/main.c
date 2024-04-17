#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

const float DUTY_STEP = 256.0 * 4.0 / F_CPU; // Still not sure why *4 and not *2
volatile float dutyCycle = 0;
bool increasing = true;

ISR(TIMER0_OVF_vect) {
    if (increasing) {
        dutyCycle += DUTY_STEP;
        if (dutyCycle >= 1) {
            dutyCycle = 1;
            increasing = false;
        }
    } else {
        dutyCycle -= DUTY_STEP;
        if (dutyCycle <= 0) {
            dutyCycle = 0;
            increasing = true;
        }
    }
    OCR1A = dutyCycle * ICR1;
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
    // Light LED on when 0 <= TCNT1 < OCR1A, off when OCR1A <= TCNT1 < ICR1
    // No prescaler, to have the smoothest possible light
    TCCR1A |= 1 << WGM11 | 1 << COM1A1;
    ICR1 = 0xFFF;
    TCCR1B |= 1 << WGM12 | 1 << WGM13 | 1 << CS10;
}

int main() {
    setupTimer0();
    setupTimer1();
    sei();
    while (true) _delay_ms(1);
}
