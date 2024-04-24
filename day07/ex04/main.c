#include "uart.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <math.h> // TODO: remove?
#include <stdbool.h>

#define CTC_MODE (1 << WGM12)
#define TIMER_PRESCALER_256 (1 << CS12)
#define PRINT_INTERVAL 0.02

ISR(ADC_vect) { ADCSRA |= 1 << ADSC; }

static void adc_init() {
    ADMUX = 1 << REFS0 | PC0;
    ADCSRA = 1 << ADPS2 | 1 << ADPS1 | 1 << ADPS0 | 1 << ADIE | 1 << ADEN | 1 << ADSC;
}

ISR(TIMER1_COMPA_vect) { uart_putnbrln(ADC); }

static void timer1_init() {
    TCCR1B |= CTC_MODE | TIMER_PRESCALER_256;
    OCR1A = F_CPU * PRINT_INTERVAL / 256 - 1;
    TIMSK1 |= 1 << OCIE1A;
}

int main() {
    uart_init();
    adc_init();
    timer1_init();
    sei();
    while (true) {}
}
