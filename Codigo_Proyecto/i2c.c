#include "stm32f103xb.h"
#include <stdio.h>
#include "i2c.h"
#include "i2c_lcd.h"

void i2c_init(void) {
    // Reloj para GPIOB e I2C1
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

    // PB6 -> SCL, PB7 -> SDA
    GPIOB->CRL &= ~((0xF << (6 * 4)) | (0xF << (7 * 4)));
    GPIOB->CRL |= ((0xB << (6 * 4)) | (0xB << (7 * 4)));  // Alternate function open-drain 50 MHz

    // Configura I2C1
    I2C1->CR1 &= ~I2C_CR1_PE;
    I2C1->CR2 = 36;         // APB1 = 36 MHz
    I2C1->CCR = 180;        // 100 kHz
    I2C1->TRISE = 37;       // Máx subida
    I2C1->CR1 |= I2C_CR1_PE;
}

void i2c_start(void) {
    I2C1->CR1 |= I2C_CR1_START;
    while (!(I2C1->SR1 & I2C_SR1_SB));
    (void)I2C1->SR1;  // Limpia SB
}

void i2c_stop(void) {
    I2C1->CR1 |= I2C_CR1_STOP;
}

void i2c_write_addr(uint8_t addr) {
    I2C1->DR = addr;
    while (!(I2C1->SR1 & I2C_SR1_ADDR));
    (void)I2C1->SR1;
    (void)I2C1->SR2;
}

void i2c_write_data(uint8_t data) {
    while (!(I2C1->SR1 & I2C_SR1_TXE));
    I2C1->DR = data;
    while (!(I2C1->SR1 & I2C_SR1_BTF));
}

uint8_t i2c_read_data_ack(void) {
    I2C1->CR1 |= I2C_CR1_ACK;
    while (!(I2C1->SR1 & I2C_SR1_RXNE));
    return I2C1->DR;
}

uint8_t i2c_read_data_nack(void) {
    I2C1->CR1 &= ~I2C_CR1_ACK;
    I2C1->CR1 |= I2C_CR1_STOP;
    while (!(I2C1->SR1 & I2C_SR1_RXNE));
    return I2C1->DR;
}

void i2c_scan_devices() {
    char buffer[32];
    uint8_t found = 0;

    lcd_command(0x01);
    for (volatile int i = 0; i < 200000; i++);
    lcd_command(0x80);
    lcd_puts("Scanning...");

    for (uint8_t addr = 1; addr < 127; addr++) {
        i2c_start();
        I2C1->DR = addr << 1;
        int timeout = 1000;
        while (!(I2C1->SR1 & (I2C_SR1_ADDR | I2C_SR1_AF)) && --timeout);

        if (I2C1->SR1 & I2C_SR1_ADDR) {
            (void)I2C1->SR2;
            i2c_stop();

            sprintf(buffer, "0x%02X ", addr);
            lcd_command(0xC0);
            lcd_puts(buffer);

            found = 1;
            for (volatile int i = 0; i < 200000; i++);
        } else {
            i2c_stop();
        }

        I2C1->SR1 &= ~I2C_SR1_AF;
    }

    if (!found) {
        lcd_command(0xC0);
        lcd_puts("No I2C devices");
    }
}

