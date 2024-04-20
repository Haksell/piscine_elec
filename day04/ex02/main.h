#pragma once

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>
#include <util/twi.h>

#define USERNAME "spectre"
#define PASSWORD "oogachaka"

#define BACKSPACE '\x7f'
#define CRLF "\r\n"

#define ROUND_DIV(dividend, divisor)                                                               \
    ({                                                                                             \
        typeof(divisor) _divisor = (divisor);                                                      \
        ((dividend) + (_divisor >> 1)) / _divisor;                                                 \
    })

// uart.c
void uart_init();
void uart_tx(char c);
char uart_rx();
void uart_printstr(const char* str);
void uart_printstrln(const char* str);
void uart_print_nibble(uint8_t byte);
void print_hex_value(uint8_t byte);
void uart_putnbr(uint32_t n);

// i2c.c
#define I2C_F 100000UL // TODO: in Makefile
#define I2C_READ 1
#define I2C_WRITE 0
#define I2C_ACK 1
#define I2C_NACK 0

void i2c_init();
void i2c_start();
void i2c_stop();
void i2c_write(uint8_t byte);
uint8_t i2c_read(uint8_t ack);

// aht20.c
typedef struct {
    uint8_t state;
    uint8_t data[5];
    uint8_t crc;
} aht20_t;

#define AHT20_ADDRESS 0x38

#define AHT20_AFTER_POWER_ON_DELAY 142 // >= 100
#define AHT20_DELAY 30
#define AHT20_MEASURE_DELAY 80

void aht20_init();
void aht20_trigger_measurement();
void aht20_process_measurement();
