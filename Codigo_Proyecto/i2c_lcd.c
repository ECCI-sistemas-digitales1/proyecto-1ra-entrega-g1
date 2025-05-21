#include "stm32f103xb.h"
#include "i2c_lcd.h"
#include "i2c.h"

#define LCD_ADDR 0x4E

void lcd_send_nibble(unsigned char nibble, unsigned char rs) {
    unsigned char data = (nibble << 4) | (rs ? 0x01 : 0x00);
    data |= 0x08;

    i2c_start();
    i2c_write_addr(LCD_ADDR);
    i2c_write_data(data | 0x04);
    i2c_write_data(data & ~0x04);
    i2c_stop();
}

void lcd_send_byte(unsigned char byte, unsigned char rs) {
    lcd_send_nibble(byte >> 4, rs);
    lcd_send_nibble(byte & 0x0F, rs);
}

void lcd_command(unsigned char cmd) {
    lcd_send_byte(cmd, 0);
}

void lcd_data(unsigned char data) {
    lcd_send_byte(data, 1);
}

void delay_ms(int ms) {
    for (int i = 0; i < ms * 8000; i++) {
        __asm__("nop");
    }
}

void lcd_init(void) {
    delay_ms(50);

    lcd_send_nibble(0x03, 0);
    delay_ms(5);
    lcd_send_nibble(0x03, 0);
    delay_ms(5);
    lcd_send_nibble(0x03, 0);
    delay_ms(1);
    lcd_send_nibble(0x02, 0);

    lcd_command(0x28);
    lcd_command(0x0C);
    lcd_command(0x06);
    lcd_command(0x01);
    delay_ms(2);
}

void lcd_puts(char *str) {
    while (*str) {
        lcd_data((unsigned char)*str++);
    }
}

void lcd_clear(void) {
    lcd_command(0x01);
    delay_ms(2);
}
