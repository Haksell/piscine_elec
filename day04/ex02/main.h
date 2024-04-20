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

// uart
void uart_init();
void uart_tx(char c);
char uart_rx();
void uart_printstr(const char* str);
void uart_printstrln(const char* str);
void uart_print_nibble(uint8_t byte);
void print_hex_value(uint8_t byte);

// i2c
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

//  aht20
#define CRC_SAVE_SPACE 0
#define CRC_SAVE_CPU 1

// ===== Config =====
#define CRC_TYPE CRC_SAVE_SPACE
// ==================

typedef struct {
    uint8_t state;
    uint8_t data[5];
    uint8_t crc;
} aht20_t;

#define AHT20_ADDRESS 0x38
#define AHT20_INIT_CMD 0xBE
#define AHT20_INIT_PARAM_1 0x08
#define AHT20_INIT_PARAM_2 0x00
#define AHT20_MEASURE_CMD 0xAC
#define AHT20_MEASURE_PARAM_1 0x33
#define AHT20_MEASURE_PARAM_2 0x00
#define AHT20_SOFT_RESET_CMD 0xBA
#define AHT20_STATUS_WORD 0x71

#define AHT20_BUSY 7
#define AHT20_CAL 3

#define AHT20_AFTER_POWER_ON_DELAY 100 + 42 // Extra 42ms to be sure :p
#define AHT20_DELAY 30
#define AHT20_MEASURE_DELAY 80

void aht20_init();
void aht20_trigger_measurement();
void aht20_event();

// uart.c
void uart_init();
void uart_tx(char c);
char uart_rx();
void uart_printstr(const char* str);
void uart_printstrln(const char* str);
void uart_putnbr(uint32_t n);