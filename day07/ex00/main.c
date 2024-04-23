#include "uart.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>

#define CTC_MODE (1 << WGM12)
#define TIMER_PRESCALER_256 (1 << CS12)
#define PRINT_INTERVAL 0.02

ISR(ADC_vect) { ADCSRA |= 1 << ADSC; }

static void adc_init() {
    ADMUX = 1 << REFS0; // AVCC reference
    ADMUX |= 1 << ADLAR; // 8-bit resolution
    ADMUX |= PC0; // Select potentiometer pin
    // Set pre-scaler to 128 because 50KHz <= 16MHz / 128 = 125KHz <= 200KHz
    ADCSRA = 1 << ADPS2 | 1 << ADPS1 | 1 << ADPS0;
    ADCSRA |= 1 << ADIE; // Enable interrupt
    ADCSRA |= 1 << ADEN | 1 << ADIE; // Enable ADC
    ADCSRA |= 1 << ADSC; // Start first conversion
}

ISR(TIMER1_COMPA_vect) {
    uint8_t potentiometer = ADCH;
    uart_putstr("0x");
    uart_putbyte(potentiometer);
    uart_putstr(" (");
    uart_putfloat(potentiometer * 100.0 / 255.0, 1);
    uart_putstrln("%)");
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
