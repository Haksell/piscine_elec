#pragma once

#include "main.h"

#define AHT20_BYTES 7
#define AHT20_CRC (AHT20_BYTES - 1)
#define AHT20_STATE 0

typedef uint8_t t_aht20[AHT20_BYTES];

#define AHT20_CALIBRATED (1 << 3)
#define AHT20_BUSY (1 << 7)

#define AHT20_ADDRESS 0x38
#define AHT20_GET_STATUS 0x71

#define AHT20_AFTER_POWER_ON_DELAY 142 // >= 100
#define AHT20_DELAY 30
#define AHT20_MEASURE_DELAY 80

typedef enum {
    AHT20_SENSOR_SUCCESS,
    AHT20_SENSOR_CRC_FAIL,
    AHT20_SENSOR_BUSY
} aht20_sensor_result_t;

void aht20_init();
void aht20_trigger_measurement();
aht20_sensor_result_t aht20_read_sensor(t_aht20 aht20);
float aht20_get_temperature(t_aht20 aht20);
float aht20_get_humidity(t_aht20 aht20);
