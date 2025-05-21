#include "stm32f103xb.h"
#include "ina219.h"
#include <stdio.h>
#include "i2c.h"
#include "i2c_lcd.h"

#define INA219_REG_CALIBRATION 0x05
#define INA219_REG_BUS_VOLTAGE 0x02
#define INA219_REG_CURRENT     0x04
#define INA219_ADDRESS         0x40

void ina219_init(void) {
    lcd_puts("init");
    ina219_write_register(INA219_REG_CALIBRATION, 4096);
}

void ina219_write_register(uint8_t reg, uint16_t value) {
    i2c_start();
    i2c_write_addr(INA219_ADDRESS << 1);
    i2c_write_data(reg);
    i2c_write_data((value >> 8) & 0xFF);
    i2c_write_data(value & 0xFF);
    i2c_stop();
}

uint16_t ina219_read_register(uint8_t reg) {
    uint8_t msb, lsb;

    i2c_start();
    i2c_write_addr(INA219_ADDRESS << 1);
    i2c_write_data(reg);

    i2c_start();
    i2c_write_addr((INA219_ADDRESS << 1) | 1);

    msb = i2c_read_data_ack();
    lsb = i2c_read_data_nack();

    return (msb << 8) | lsb;
}

float ina219_get_bus_voltage(void) {
    uint16_t raw = ina219_read_register(INA219_REG_BUS_VOLTAGE);
    raw >>= 3;
    return raw * 4.0f / 1000.0f;
}

float ina219_get_current(void) {
    int16_t raw = (int16_t)ina219_read_register(INA219_REG_CURRENT);
    return raw * 0.1f;
}
