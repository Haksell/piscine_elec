// TODO: print bytes in UART?
// The program must return status values to your computer after each data transmission.

#include "main.h"

#define AHT20 0x38

// TODO: uart.c

void uart_print_nibble(uint8_t byte) { uart_tx(byte < 10 ? byte + '0' : byte + 55); }

void print_hex_value(uint8_t byte) {
    uart_print_nibble(byte >> 4);
    uart_print_nibble(byte & 15);
    uart_tx(' ');
}

// TODO: i2c.c

void i2c_init() {
    TWSR = 0x00; // set prescaler to 1
    TWBR = (uint8_t)((F_CPU / F_SCL - 16) / 2);
}

void i2c_write(uint8_t byte) {
    TWDR = byte;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & 1 << TWINT)) {}
}

void i2c_start(uint8_t address) {
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTA);
    while (!(TWCR & 1 << TWINT)) {}
    i2c_write(address << 1); // TODO: "| 0" or "| 1" depending on read or write
}

uint8_t i2c_read(uint8_t ack) {
    TWCR = (1 << TWINT) | (1 << TWEN) | (!!ack << TWEA);
    while (!(TWCR & 1 << TWINT)) {}
    return TWDR;
}

void i2c_stop() {
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
    while (!(TWCR & 1 << TWSTO)) {}
}

int main(void) {
    uart_init();
    i2c_init();

    while (true) {
        // Trigger measurement command sequence
        i2c_start(AHT20); // Shift address and indicate write operation
        i2c_write(0xAC); // Trigger measurement command
        i2c_write(0x33); // First parameter byte
        i2c_write(0x00); // Second parameter byte
        i2c_stop(); // End of writing sequence

        _delay_ms(80); // Wait for the measurement to be completed

        // Check sensor status until it's not busy
        unsigned char status;
        do {
            i2c_start((AHT20 << 1) | 0x01); // Shift address and indicate read operation
            status = i2c_read(true);
            i2c_stop(); // Stop to re-start the process if necessary
            if (status & 0x80) { // If sensor is busy
                _delay_ms(10); // Wait a bit before trying again
            }
            uart_tx('x');
        } while (status & 0x80);
        uart_tx('\n');

        // Restart and read 7 bytes of data
        i2c_start((AHT20 << 1) | 0x01); // Shift address and indicate read operation
        for (int i = 0; i < 7; i++) {
            bool ack = i < 6; // ACK for all but last byte
            unsigned char data = i2c_read(ack);
            print_hex_value(data);
        }
        i2c_stop(); // End of reading sequence

        uart_printstrln(""); // Print a newline

        _delay_ms(2000); // Delay before the next measurement cycle
    }
}
