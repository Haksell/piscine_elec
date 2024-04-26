#include "main.h"

#define AHT20_ADDRESS 0x38
#define I2C_WRITE 0
#define I2C_READ 1

static void print_status() {
    uart_printstr("Status: ");
    switch (TW_STATUS) {
        case TW_START: uart_printstrln("TW_START"); break;
        case TW_REP_START: uart_printstrln("TW_REP_START"); break;
        case TW_MT_SLA_ACK: uart_printstrln("TW_MT_SLA_ACK"); break;
        case TW_MT_SLA_NACK: uart_printstrln("TW_MT_SLA_NACK"); break;
        case TW_MT_DATA_ACK: uart_printstrln("TW_MT_DATA_ACK"); break;
        case TW_MT_DATA_NACK: uart_printstrln("TW_MT_DATA_NACK"); break;
        case TW_MR_SLA_ACK: uart_printstrln("TW_MR_SLA_ACK"); break;
        case TW_MR_SLA_NACK: uart_printstrln("TW_MR_SLA_NACK"); break;
        case TW_MR_DATA_ACK: uart_printstrln("TW_MR_DATA_ACK"); break;
        case TW_MR_DATA_NACK: uart_printstrln("TW_MR_DATA_NACK"); break;
        case TW_ST_SLA_ACK: uart_printstrln("TW_ST_SLA_ACK"); break;
        case TW_ST_ARB_LOST_SLA_ACK: uart_printstrln("TW_ST_ARB_LOST_SLA_ACK"); break;
        case TW_ST_DATA_ACK: uart_printstrln("TW_ST_DATA_ACK"); break;
        case TW_ST_DATA_NACK: uart_printstrln("TW_ST_DATA_NACK"); break;
        case TW_ST_LAST_DATA: uart_printstrln("TW_ST_LAST_DATA"); break;
        case TW_SR_SLA_ACK: uart_printstrln("TW_SR_SLA_ACK"); break;
        case TW_SR_ARB_LOST_SLA_ACK: uart_printstrln("TW_SR_ARB_LOST_SLA_ACK"); break;
        case TW_SR_GCALL_ACK: uart_printstrln("TW_SR_GCALL_ACK"); break;
        case TW_SR_ARB_LOST_GCALL_ACK: uart_printstrln("TW_SR_ARB_LOST_GCALL_ACK"); break;
        case TW_SR_DATA_ACK: uart_printstrln("TW_SR_DATA_ACK"); break;
        case TW_SR_DATA_NACK: uart_printstrln("TW_SR_DATA_NACK"); break;
        case TW_SR_GCALL_DATA_ACK: uart_printstrln("TW_SR_GCALL_DATA_ACK"); break;
        case TW_SR_GCALL_DATA_NACK: uart_printstrln("TW_SR_GCALL_DATA_NACK"); break;
        case TW_SR_STOP: uart_printstrln("TW_SR_STOP"); break;
        case TW_NO_INFO: uart_printstrln("TW_NO_INFO"); break;
        case TW_BUS_ERROR: uart_printstrln("TW_BUS_ERROR"); break;
        case 0x38: uart_printstrln("Arbitration Lost"); break;
        default:
            uart_printstr("Unknown status: ");
            uart_putnbr(TW_STATUS);
            uart_printstrln("");
    }
}

static void i2c_init() { TWBR = (F_CPU / F_I2C - 16) / 2; }

static void i2c_start() {
    TWCR = 1 << TWINT | 1 << TWEN | 1 << TWSTA;
    while (!(TWCR & 1 << TWINT)) {}
    print_status();
    TWDR = AHT20_ADDRESS << 1 | I2C_WRITE;
    TWCR = 1 << TWINT | 1 << TWEN;
    while (!(TWCR & 1 << TWINT)) {}
}

static void i2c_stop() {
    TWCR = 1 << TWINT | 1 << TWEN | 1 << TWSTO;
    while (!(TWCR & 1 << TWSTO)) {}
}

int main() {
    uart_init();
    i2c_init();
    print_status();
    for (uint8_t i = 0; i < 3; ++i) {
        uart_printstrln("");
        i2c_start();
        print_status();
        i2c_stop();
        print_status();
    }
}
