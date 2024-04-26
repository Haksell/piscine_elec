#pragma once

#include <avr/io.h>

#define I2C_READ 1
#define I2C_WRITE 0

void i2c_init();
void i2c_start();
void i2c_stop();
void i2c_write(uint8_t byte);
uint8_t i2c_read();
uint8_t i2c_read_ack();
void i2c_send_cmd1(uint8_t address, uint8_t cmd1);
void i2c_send_cmd2(uint8_t address, uint8_t cmd1, uint8_t cmd2);
void i2c_send_cmd3(uint8_t address, uint8_t cmd1, uint8_t cmd2, uint8_t cmd3);
