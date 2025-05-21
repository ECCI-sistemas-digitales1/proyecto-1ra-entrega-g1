#ifndef INA219_H
#define INA219_H

#include <stdint.h>

#define INA219_ADDRESS 0x40  // Dirección de 7 bits

void ina219_init(void);
uint16_t ina219_read_register(uint8_t reg);
void ina219_write_register(uint8_t reg, uint16_t value);
float ina219_get_bus_voltage(void);
float ina219_get_current(void);

#endif
