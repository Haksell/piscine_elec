#pragma once

#include "main.h"

#define AHT20_BYTES 7
#define AHT20_CRC (AHT20_BYTES - 1)
typedef uint8_t t_aht20[AHT20_BYTES];

#define AHT20_ADDRESS 0x38
#define AHT20_BUSY 0x80
#define AHT20_GET_STATUS 0x71

#define AHT20_AFTER_POWER_ON_DELAY 142 // >= 100
#define AHT20_DELAY 30
#define AHT20_MEASURE_DELAY 80

void aht20_init();
uint8_t aht20_read_cmd();
void aht20_trigger_measurement();
bool aht20_read_sensor(t_aht20 aht20);
float aht20_get_temperature(t_aht20 aht20);
float aht20_get_humidity(t_aht20 aht20);
