#ifndef ADC_H
#define ADC_H

// Canales ADC
#define ADC_CHANNEL    0   // PA0 (Canal 0)
#define ADC2_CHANNEL    1   // PA1 (Canal 1)

// Direcciones de los registros de datos
#define ADC1_DR_ADDRESS    ((uint32_t)0x4001244C)
#define ADC2_DR_ADDRESS    ((uint32_t)0x4001284C)

// Prototipos de funciones
void ADC1_Init(void);
void ADC2_Init(void);
uint16_t ADC1_Read(void);
uint16_t ADC2_Read(void);

#endif
