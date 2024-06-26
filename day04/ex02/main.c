#include "main.h"

#define ROLLING_AVERAGE 3

static float average(float* arr, size_t size) {
    float res = 0.0;
    for (size_t i = 0; i < size; ++i) res += arr[i];
    return res / size;
}

static void weather_report(t_aht20 aht20) {
    static size_t i = 0;
    static float temps[ROLLING_AVERAGE];
    static float hums[ROLLING_AVERAGE];

    temps[i % ROLLING_AVERAGE] = aht20_get_temperature(aht20);
    hums[i % ROLLING_AVERAGE] = aht20_get_humidity(aht20);
    ++i;
    uart_printstr("Temperature: ");
    uart_printfloat(average(temps, MIN(i, ROLLING_AVERAGE)), 1);
    uart_printstr("°C, Humidity: ");
    uart_printfloat(average(hums, MIN(i, ROLLING_AVERAGE)) * 100, 0);
    uart_printstrln("%");
}

int main() {
    t_aht20 aht20;
    uart_init();
    i2c_init();
    aht20_init();
    while (true) {
        aht20_trigger_measurement();
        _delay_ms(AHT20_MEASURE_DELAY);
        switch (aht20_read_sensor(aht20)) {
            case AHT20_SENSOR_SUCCESS: weather_report(aht20); break;
            case AHT20_SENSOR_BUSY: uart_printstrln("AHT20 busy"); break;
            case AHT20_SENSOR_CRC_FAIL: uart_printstrln("CRC failure"); break;
        }
        _delay_ms(1000);
    }
}
