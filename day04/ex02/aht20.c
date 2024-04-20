#include "main.h"

aht20_t aht20;
uint8_t triggered = 0;

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
    register uint8_t read = i2c_read(I2C_NACK);
    i2c_stop();

    return read;
}

void aht20_init() {
    _delay_ms(AHT20_AFTER_POWER_ON_DELAY);
    aht20_send_cmd(AHT20_SOFT_RESET_CMD);
    _delay_ms(AHT20_DELAY);

    aht20_send_cmd(AHT20_STATUS_WORD);
    uint8_t status_word = aht20_read_cmd();

    if (!(status_word & AHT20_CAL)) {
        aht20_send_full_command(AHT20_INIT_CMD, AHT20_INIT_PARAM_1, AHT20_INIT_PARAM_2);
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

static bool aht20_read() {
    i2c_start();
    i2c_write(AHT20_ADDRESS << 1 | I2C_READ);
    for (uint8_t i = 0; i < 7; i++) {
        uint8_t byte = i2c_read(i == 6 ? I2C_NACK : I2C_ACK);
        // if (i == 3) {
        //     uart_print_nibble(byte >> 4);
        //     uart_tx(' ');
        //     uart_print_nibble(byte & 15);
        // } else if (i != 0 && i != 6) {
        //     uart_print_nibble(byte >> 4);
        //     uart_print_nibble(byte & 15);
        // } else if (i == 0) {
        //     uart_print_nibble(byte >> 4);
        //     uart_print_nibble(byte & 15);
        //     uart_tx(' ');
        // } else if (i == 6) {
        //     uart_tx(' ');
        //     uart_print_nibble(byte >> 4);
        //     uart_print_nibble(byte & 15);
        // }
        *((uint8_t*)&aht20 + i) = byte;
    }
    i2c_stop();

    return crc((uint8_t*)&aht20, 6) == aht20.crc;
}

static float aht20_get_temperature() {
    uint32_t st = (((int32_t)aht20.data[2] & 0x0f) << 16) | ((int32_t)aht20.data[3] << 8) |
                  (aht20.data[4]);
    return (st / 1048576.0) * 200.0 - 50.0;
}

static float aht20_get_humidity() {
    uint32_t srh = ((int32_t)aht20.data[0] << 12) | ((int32_t)aht20.data[1] << 4) |
                   ((aht20.data[2] & 0xf0) >> 4);
    return srh / 1048576.0;
}

void aht20_trigger_measurement() {
    aht20_send_full_command(AHT20_MEASURE_CMD, AHT20_MEASURE_PARAM_1, AHT20_MEASURE_PARAM_2);
    triggered = 1;
}

void weather_report(bool crc_valid) {
    if (crc_valid) {
        // TODO: avg of last 3
        float temp = aht20_get_temperature();
        float hum = aht20_get_humidity();
        uart_printstr("Temperature: ");
        // dtostrf
        uart_putnbr(temp);
        uart_tx(',');
        uart_putnbr((uint32_t)(temp * 10) % 10);
        uart_printstr("°C, Humidity: ");
        uart_putnbr(hum * 100);
        uart_tx(',');
        uart_putnbr((uint32_t)(hum * 1000) % 10);
        uart_printstrln("%");
    } else uart_printstrln(" crc fail");
}

void aht20_event() {
    if (!triggered) return;

    if (!(aht20_read_cmd() & 0x80)) {
        bool crc_success = aht20_read();
        weather_report(crc_success);
        triggered = 0;
    }
}
