#pragma once

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <stdlib.h>
#include <util/delay.h>
#include <util/twi.h>

#define MIN(A, B) ((A < B) ? (A) : (B))
#define MAX(A, B) ((A > B) ? (A) : (B))

// uart.c
#define CRLF "\r\n"

void uart_init();
void uart_tx(char c);
char uart_rx();
void uart_printstr(const char* str);
void uart_printstrln(const char* str);
void uart_print_nibble(uint8_t byte);
void print_hex_value(uint8_t byte);
void uart_putnbr(uint32_t n);
void uart_printfloat(float x);

// i2c.c
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
} aht20_t; // TODO: just a uint8_t

#define AHT20_ADDRESS 0x38
#define AHT20_BUSY 0x80

#define AHT20_AFTER_POWER_ON_DELAY 142 // >= 100
#define AHT20_DELAY 30
#define AHT20_MEASURE_DELAY 80

void aht20_init();
uint8_t aht20_read_cmd();
bool aht20_read_sensor(aht20_t* aht20);
void aht20_trigger_measurement();
float aht20_get_temperature(aht20_t aht20);
float aht20_get_humidity(aht20_t aht20);
