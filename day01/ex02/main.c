// https://lab-allen.fr/atmega328-configuration-du-timer1/
// https://www.arxterra.com/tutorial-on-fast-pulse-width-modulation/

#include <avr/io.h>
#include <stdbool.h>

#define DUTY_CYCLE 0.1

static void set_pwm_mode() {
    TCCR1A |= 1 << WGM11;
    TCCR1B |= 1 << WGM12 | 1 << WGM13;
}

int main() {
    DDRB = (1 << PB1);
    TCCR1A = TCCR1B = 0;
    set_pwm_mode();
    TCCR1A |= 1 << COM1A1; // light up when timer1 < OCR1A
    TCCR1B |= 1 << CS10 | 1 << CS12; // prescaler = 1024
    ICR1 = F_CPU / 1024 - 1; // top value before reset to 0
    OCR1A = ICR1 * DUTY_CYCLE;
    while (true) {}
}
