#include "main.h"

#define FLOAT_SHIFT_20 0.00000095367431640625

static void aht20_send_full_command(uint8_t cmd, uint8_t param1, uint8_t param2) {
    i2c_start();
    i2c_write(AHT20_ADDRESS << 1 | I2C_WRITE);
    i2c_write(cmd);
    i2c_write(param1);
    i2c_write(param2);
    i2c_stop();
}

static void aht20_send_cmd(uint8_t cmd) {
    i2c_start();
    i2c_write(AHT20_ADDRESS << 1 | I2C_WRITE);
    i2c_write(cmd);
    i2c_stop();
}

static uint8_t aht20_read_cmd() {
    i2c_start();
    i2c_write(AHT20_ADDRESS << 1 | I2C_READ);
    uint8_t read = i2c_read();
    i2c_stop();
    return read;
}

void aht20_init() {
    _delay_ms(AHT20_AFTER_POWER_ON_DELAY);
    aht20_send_cmd(0xBA);
    _delay_ms(AHT20_DELAY);
    aht20_send_cmd(AHT20_GET_STATUS);
    if (aht20_read_cmd() & AHT20_CALIBRATED) uart_printstrln("AHT20 already calibrated");
    else {
        aht20_send_full_command(0xBE, 0x08, 0x00);
        _delay_ms(AHT20_AFTER_POWER_ON_DELAY);
        uart_printstrln("AHT20 calibration complete");
    }
}

static bool checksum(t_aht20 aht20) {
    uint8_t c = 0xFF;
    for (uint8_t i = 0; i < AHT20_CRC; ++i) {
        c ^= aht20[i];
        for (uint8_t j = 0; j < 8; ++j) c = c & 0x80 ? c << 1 ^ 0x31 : c << 1;
    }
    return c == aht20[AHT20_CRC];
}

aht20_sensor_result_t aht20_read_sensor(t_aht20 aht20) {
    i2c_start();
    i2c_write(AHT20_ADDRESS << 1 | I2C_READ);
    for (uint8_t i = 0; i < AHT20_CRC; ++i) aht20[i] = i2c_read_ack();
    aht20[AHT20_CRC] = i2c_read();
    i2c_stop();
    if (!checksum(aht20)) return AHT20_SENSOR_CRC_FAIL;
    return aht20[AHT20_STATE] & AHT20_BUSY ? AHT20_SENSOR_BUSY : AHT20_SENSOR_SUCCESS;
}

float aht20_get_temperature(t_aht20 aht20) {
    uint32_t st = ((int32_t)aht20[3] & 0x0F) << 16 | (int32_t)aht20[4] << 8 | aht20[5];
    return st * FLOAT_SHIFT_20 * 200.0 - 50.0;
}

float aht20_get_humidity(t_aht20 aht20) {
    uint32_t srh = (int32_t)aht20[1] << 12 | (int32_t)aht20[2] << 4 | aht20[3] >> 4;
    return srh * FLOAT_SHIFT_20;
}

void aht20_trigger_measurement() { aht20_send_full_command(0xAC, 0x33, 0x00); }
