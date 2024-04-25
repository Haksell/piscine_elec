#include "uart.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <math.h>
#include <stdbool.h>

#define CTC_MODE (1 << WGM12)
#define TIMER_PRESCALER_256 (1 << CS12)
#define PRINT_INTERVAL 0.5

ISR(ADC_vect) { ADCSRA |= 1 << ADSC; }

static void adc_init() {
    ADMUX = 1 << REFS0 | 1 << REFS1 | 8;
    ADCSRA = 1 << ADPS2 | 1 << ADPS1 | 1 << ADPS0 | 1 << ADIE | 1 << ADEN | 1 << ADSC;
}

static float convert_adc_to_celsius(uint16_t adc) {
    int16_t diff = adc - 314;
    return 25.0 + (diff > 0 ? diff / 66.0 * 60.0 : diff / 72 * 70);
}

ISR(TIMER1_COMPA_vect) {
    uart_putfloat(convert_adc_to_celsius(ADC), 1);
    uart_putstrln("°C");
}

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
