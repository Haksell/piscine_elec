#include "main.h"

#define CTC_MODE (1 << WGM12)
#define TIMER_PRESCALER_256 (1 << CS12)
#define PRINT_INTERVAL 0.02

static void adc_init() {
    ADMUX = 1 << REFS0; // AVCC reference
    ADMUX |= 1 << ADLAR; // 8-bit resolution
    // Set pre-scaler to 128 because 50KHz <= 16MHz / 128 = 125KHz <= 200KHz
    ADCSRA = 1 << ADPS2 | 1 << ADPS1 | 1 << ADPS0;
    ADCSRA |= 1 << ADEN; // Enable ADC
}

static uint8_t adc_read(uint8_t pin) {
    ADMUX = (ADMUX & 0xF0) | (pin & 0x0F); // Select ADC pin
    ADCSRA |= 1 << ADSC; // Start conversion
    while (ADCSRA & 1 << ADSC) {} // Wait for conversion to complete
    return ADCH; // Return 8-bit ADC result
}

ISR(TIMER1_COMPA_vect) { uart_putstrln("Hello World!"); }

static void setup_timer1() {
    TCCR1B |= CTC_MODE | TIMER_PRESCALER_256;
    OCR1A = F_CPU * PRINT_INTERVAL / 256 - 1;
    TIMSK1 |= 1 << OCIE1A;
}

int main() {
    uart_init();
    adc_init();
    setup_timer1();
    sei();
    while (true) {
        uint8_t value = adc_read(PC0);
        uart_putstr("0x");
        uart_putbyte(value);
        uart_putstr(" (");
        uart_putfloat(value * 100.0 / 255.0, 1);
        uart_putstrln("%)");
        _delay_ms(20);
    }
}
