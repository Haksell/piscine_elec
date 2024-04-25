#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} t_rgb;

#define NUM_LEDS 3

#define RGB_BLACK ((t_rgb){0, 0, 0})
#define RGB_DODGER_BLUE ((t_rgb){0x1e, 0x90, 0xff})

#define SS (1 << PB2)
#define MOSI (1 << PB3)
#define SCK (1 << PB5)

void spi_init_master() {
    DDRB = SS | MOSI | SCK;
    SPCR = 1 << SPE | 1 << MSTR | 1 << SPR0;
}

void spi_send(uint8_t data) {
    SPDR = data;
    while (!(SPSR & 1 << SPIF)) {}
}

void apa102_start() {
    spi_send(0x00);
    spi_send(0x00);
    spi_send(0x00);
    spi_send(0x00);
}

void apa102_send_color(t_rgb rgb) {
    spi_send(0xe2); // brightness
    spi_send(rgb.blue);
    spi_send(rgb.green);
    spi_send(rgb.red);
}

void apa102_end() {
    spi_send(0xff);
    spi_send(0xff);
    spi_send(0xff);
    spi_send(0xff);
}

ISR(ADC_vect) {
    uint8_t led = ADC * NUM_LEDS / 1024;
    apa102_start();
    for (uint8_t i = 0; i < NUM_LEDS; ++i)
        apa102_send_color(i == led ? RGB_DODGER_BLUE : RGB_BLACK);
    apa102_end();
    ADCSRA |= 1 << ADSC;
}

void adc_init() {
    ADMUX = 1 << REFS0 | PC0;
    ADCSRA = 1 << ADPS2 | 1 << ADPS1 | 1 << ADPS0 | 1 << ADIE | 1 << ADEN | 1 << ADSC;
}

int main() {
    adc_init();
    spi_init_master();
    sei();
    while (true) {}
}
