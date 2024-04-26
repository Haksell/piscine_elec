#pragma once

#include <avr/io.h>
#include <stdbool.h>

void i2c_init();
void i2c_start();
void i2c_stop();
void i2c_write(uint8_t byte, bool ack);
uint8_t i2c_read(bool ack);
