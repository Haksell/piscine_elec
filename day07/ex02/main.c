#include "uart.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>

#define CTC_MODE (1 << WGM12)
#define TIMER_PRESCALER_256 (1 << CS12)
#define PRINT_INTERVAL 0.02

#define ADC_POTENTIOMETER 0
#define ADC_PHOTORESISTOR 1
#define ADC_THERMISTOR 2

uint8_t current_pin_conversion = 0;
uint16_t adc[3] = {0, 0, 0};

static void start_conversion() {
    ADMUX = (ADMUX & 0xF0) | (current_pin_conversion & 0x0F);
    ADCSRA |= 1 << ADSC;
}

ISR(ADC_vect) {
    adc[current_pin_conversion] = ADC;
    current_pin_conversion = (current_pin_conversion + 1) % 3;
    start_conversion();
}

static void adc_init() {
    ADMUX = 1 << REFS0; // AVCC reference
    // Set pre-scaler to 128 because 50KHz <= 16MHz / 128 = 125KHz <= 200KHz
    ADCSRA = 1 << ADPS2 | 1 << ADPS1 | 1 << ADPS0;
    ADCSRA |= 1 << ADIE; // Enable interrupt
    ADCSRA |= 1 << ADEN; // Enable ADC
    start_conversion();
}

ISR(TIMER1_COMPA_vect) {
    uart_putnbr(adc[ADC_POTENTIOMETER]);
    uart_putstr(", ");
    uart_putnbr(adc[ADC_PHOTORESISTOR]);
    uart_putstr(", ");
    uart_putnbrln(adc[ADC_THERMISTOR]);
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
