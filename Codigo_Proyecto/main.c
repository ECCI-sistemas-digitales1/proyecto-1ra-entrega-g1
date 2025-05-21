#include "stm32f103xb.h"
#include "i2c.h"
#include "i2c_lcd.h"
#include "ina219.h"
#include "adc.h"
#include "pwm.h"
#include <stdint.h>
#include <stdio.h>

#define BUFFER_SIZE 1
uint16_t buffer_pot[BUFFER_SIZE];
uint16_t buffer_pot1[BUFFER_SIZE];
/*char buffer1[32];
char buffer2[32];*/

// Umbral de tolerancia para considerar igualdad (ajústalo según tu ADC)
#define TOLERANCIA 0.02f

int main(void) {
    uint16_t adc_value1;
    uint16_t adc_value2;
    float voltaje1;
    float voltaje2;
    int32_t voltage_diff;
    int i = 0;

    i2c_init();
	lcd_init();
	ina219_init();

	i2c_scan_devices();

	lcd_command(0x01);
	for (volatile int i = 0; i < 200000; i++);

    pwm_init();

    ADC1_Init();
    ADC2_Init();


    lcd_puts("Iniciando...");
    delay_ms(100);
    lcd_clear();

    while (1) {

	   adc_value1 = ADC1_Read();
	   adc_value2 = ADC2_Read();

	   float voltage = ina219_get_bus_voltage();
	   float current = ina219_get_current();

		// Procesar y mostrar en LCD
	   buffer_pot[0] = adc_value1;
	   voltaje1 = (adc_value1 * 3.3f) / 4095.0f;
	   buffer_pot1[0] = adc_value2;
	   voltaje2 = (adc_value2 * 3.3f) / 4095.0f;

	   // Comparar con tolerancia para evitar falsas igualdades
	   if (voltaje1 < (voltaje2 - TOLERANCIA)) {
		   giro_Izquierda();  // Ej: Mover servo a la izquierda
	   }
	   else if (voltaje1 > (voltaje2 + TOLERANCIA)) {
		   giro_Derecha();  // Ej: Mover servo a la derecha
	   }
	   else {
		   freno(); // Ej: Posición neutra del servo
	   }

	   /*snprintf(buffer1, sizeof(buffer1), "V: %.2f V", voltage);
	   snprintf(buffer2, sizeof(buffer2), "I: %.2f mA", current);*/

	   char adc_string01[6];
	   snprintf(adc_string01, sizeof(adc_string01), "%2f", voltage);
	   char adc_string02[6];
	   snprintf(adc_string02, sizeof(adc_string02), "%4f", current);
	   char tex01 []=" V=";
	   char tex02 []=" mA=";

	   char adc_string1[4];
	   snprintf(adc_string1, sizeof(adc_string1), "%2f", voltaje1);
	   char adc_string2[4];
	   snprintf(adc_string2, sizeof(adc_string2), "%2f", voltaje2);
	   char tex []=" V=";

	   char message01[8];
	   snprintf(message01, sizeof(message01), "%s%s", tex01, adc_string01);
	   char message02[10];
	   snprintf(message02, sizeof(message02), "%s%s", tex02, adc_string02);
	   char messageINA[16];
	   snprintf(messageINA, sizeof(messageINA), "%s%s", message01, message02);
	   char message1[8];
	   snprintf(message1, sizeof(message1), "%s%s", tex, adc_string1);
	   char message2[8];
	   snprintf(message2, sizeof(message2), "%s%s", tex, adc_string2);
	   char messageLDR[16];
	   snprintf(messageLDR, sizeof(messageLDR), "%s%s", message1, message2);

	   lcd_command(0x80);
	   lcd_puts(messageINA);
	   lcd_command(0xC0);
	   lcd_puts(messageLDR);
	   //}
    }
}
