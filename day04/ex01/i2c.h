#pragma once

#include "main.h"

#define I2C_READ 1
#define I2C_WRITE 0

void i2c_init();
void i2c_start();
void i2c_stop();
void i2c_write(uint8_t byte);
uint8_t i2c_read();
uint8_t i2c_read_ack();