#include "uart.h"
#include <avr/io.h>
#include <util/delay.h>

void spi_init() {
    // Set MOSI and SCK as Output
    DDRB |= (1 << PB3) | (1 << PB5);
    // Enable SPI, Master, set clock rate fck/16
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

void spi_send(uint8_t data) {
    uart_putbyte(data);
    uart_newline();
    SPDR = data;
    while (!(SPSR & (1 << SPIF))) {}
    uart_putstrln("jpp");
}

void send_color(uint8_t red, uint8_t green, uint8_t blue) {
    spi_send(0xFF);
    spi_send(0xFF);
    spi_send(0xFF);
    spi_send(0xFF);

    spi_send(0xE4);
    spi_send(blue);
    spi_send(green);
    spi_send(red);

    spi_send(0xFF);
    spi_send(0xFF);
    spi_send(0xFF);
    spi_send(0xFF);
}

int main() {
    uart_init();
    spi_init();

    while (1) {
        send_color(0xFF, 0x00, 0x00);
        _delay_ms(1000);
    }
}
