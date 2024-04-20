#include "main.h"

#define FLOAT_SHIFT_20 0.00000095367431640625

aht20_t aht20;

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
    uint8_t read = i2c_read(I2C_NACK);
    i2c_stop();
    return read;
}

void aht20_init() {
    _delay_ms(AHT20_AFTER_POWER_ON_DELAY);
    aht20_send_cmd(0xBA);
    _delay_ms(AHT20_DELAY);

    aht20_send_cmd(0x71);
    uint8_t status_word = aht20_read_cmd();

    if (!(status_word & 3)) {
        aht20_send_full_command(0xBE, 0x08, 0x00);
        _delay_ms(AHT20_AFTER_POWER_ON_DELAY);
    }
}

static uint8_t crc(uint8_t* data, uint8_t len) {
    uint8_t c = 0xff;
    for (uint8_t i = 0; i < len; i++) {
        c ^= data[i];
        for (uint8_t j = 0; j < 8; j++) {
            if ((c & 0x80) != 0) c = (uint8_t)((c << 1) ^ 0x31);
            else c <<= 1;
        }
    }
    return c;
}

static bool aht20_read_sensor() {
    i2c_start();
    i2c_write(AHT20_ADDRESS << 1 | I2C_READ);
    for (uint8_t i = 0; i < 7; ++i) {
        uint8_t byte = i2c_read(i == 6 ? I2C_NACK : I2C_ACK);
        *((uint8_t*)&aht20 + i) = byte;
    }
    i2c_stop();

    return crc((uint8_t*)&aht20, 6) == aht20.crc;
}

static float aht20_get_temperature() {
    uint32_t st = (((int32_t)aht20.data[2] & 0x0f) << 16) | ((int32_t)aht20.data[3] << 8) |
                  (aht20.data[4]);
    return st * FLOAT_SHIFT_20 * 200.0 - 50.0;
}

static float aht20_get_humidity() {
    uint32_t srh = ((int32_t)aht20.data[0] << 12) | ((int32_t)aht20.data[1] << 4) |
                   ((aht20.data[2] & 0xf0) >> 4);
    return srh * FLOAT_SHIFT_20;
}

void aht20_trigger_measurement() { aht20_send_full_command(0xAC, 0x33, 0x00); }

static void weather_report() {
    // TODO: avg of last 3
    float temp = aht20_get_temperature();
    float hum = aht20_get_humidity();
    uart_printstr("Temperature: ");
    uart_printfloat(temp);
    uart_printstr("°C, Humidity: ");
    uart_printfloat(hum * 100);
    uart_printstrln("%");
}

void aht20_process_measurement() {
    if (aht20_read_cmd() & AHT20_BUSY) uart_printstrln("AHT20 busy");
    else if (aht20_read_sensor()) weather_report();
    else uart_printstrln("CRC failure");
}
