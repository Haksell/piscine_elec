#include "uart.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>

static void rgb_set(uint8_t r, uint8_t g, uint8_t b) {
    OCR0B = 255 - r;
    OCR0A = 255 - g;
    OCR2B = 255 - b;
}

static void rgb_wheel(uint8_t pos) {
    pos = 255 - pos;
    if (pos < 85) {
        rgb_set(255 - pos * 3, 0, pos * 3);
    } else if (pos < 170) {
        pos = pos - 85;
        rgb_set(0, pos * 3, 255 - pos * 3);
    } else {
        pos = pos - 170;
        rgb_set(pos * 3, 255 - pos * 3, 0);
    }
}

static void rgb_init() {
    DDRD = 0b1101000;
    PORTD = 0;
    TCCR0A = 1 << WGM00 | 1 << WGM01 | 1 << COM0A0 | 1 << COM0A1 | 1 << COM0B0 | 1 << COM0B1;
    TCCR0B = 1 << CS00;
    TCCR2A = 1 << WGM20 | 1 << WGM21 | 1 << COM2B0 | 1 << COM2B1;
    TCCR2B = 1 << CS20;
    rgb_wheel(ADC >> 2);
}

ISR(ADC_vect) {
    rgb_wheel(ADC >> 2);
    ADCSRA |= 1 << ADSC;
}

static void adc_init() {
    ADMUX = 1 << REFS0 | PC0;
    ADCSRA = 1 << ADPS2 | 1 << ADPS1 | 1 << ADPS0 | 1 << ADIE | 1 << ADEN | 1 << ADSC;
}

int main() {
    uart_init();
    adc_init();
    rgb_init();
    sei();
    while (true) {}
}
