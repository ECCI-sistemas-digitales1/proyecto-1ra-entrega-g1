/*
 * i2c.h
 *
 *  Created on: May 20, 2025
 *      Author: dnmaldonador
 */

#ifndef INC_I2C_H_
#define INC_I2C_H_

void i2c_init(void);
void i2c_start(void);
void i2c_stop(void);
void i2c_write_addr(unsigned char addr);
void i2c_write_data(unsigned char data);
void i2c_scan(void);
uint8_t i2c_read_data_ack(void);
uint8_t i2c_read_data_nack(void);
void i2c_scan_devices(void);


#endif /* INC_I2C_H_ */
