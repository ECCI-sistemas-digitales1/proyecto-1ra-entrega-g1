#include "stm32f103xb.h"
#include "adc.h"

// --------------------------------------
// Configuración de ADC1 (PA0 - Canal 0)
// --------------------------------------
void ADC1_Init(void) {
    // 1. Habilitar reloj para GPIOA y ADC1
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_ADC1EN;

    // 2. Configurar PA0 como entrada analógica (Canal 0)
    GPIOA->CRL &= ~(GPIO_CRL_MODE0 | GPIO_CRL_CNF0);  // PA0 analógico

    // 3. Encender ADC1 y esperar inicialización
    ADC1->CR2 |= ADC_CR2_ADON;
    for (volatile int i = 0; i < 1000; i++);

    // 4. Calibración del ADC1
    ADC1->CR2 |= ADC_CR2_RSTCAL;
    while (ADC1->CR2 & ADC_CR2_RSTCAL);
    ADC1->CR2 |= ADC_CR2_CAL;
    while (ADC1->CR2 & ADC_CR2_CAL);

    // 5. Configurar canal 0 y modo continuo
    ADC1->SQR3 = ADC_CHANNEL;   // Canal 0
    ADC1->CR2 |= ADC_CR2_CONT;
	ADC1->CR2 |= ADC_CR2_ADON;
	ADC1->CR2 |= ADC_CR2_SWSTART;
}

uint16_t ADC1_Read(void) {
    ADC1->CR2 |= ADC_CR2_SWSTART; // Iniciar conversión
    while (!(ADC1->SR & ADC_SR_EOC)); // Esperar fin
    return (uint16_t)(*(volatile uint32_t*) ADC1_DR_ADDRESS);
}

// --------------------------------------
// Configuración de ADC2 (PA1 - Canal 1)
// --------------------------------------
void ADC2_Init(void) {
    // 1. Habilitar reloj para GPIOA y ADC2
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_ADC2EN;  // ¡ADC2EN!

    // 2. Configurar PA1 como entrada analógica (Canal 1)
    GPIOA->CRL &= ~(GPIO_CRL_MODE1 | GPIO_CRL_CNF1);  // PA1 analógico

    // 3. Encender ADC2 y esperar inicialización
    ADC2->CR2 |= ADC_CR2_ADON;
    for (volatile int i = 0; i < 1000; i++);

    // 4. Calibración del ADC2
    ADC2->CR2 |= ADC_CR2_RSTCAL;
    while (ADC2->CR2 & ADC_CR2_RSTCAL);
    ADC2->CR2 |= ADC_CR2_CAL;
    while (ADC2->CR2 & ADC_CR2_CAL);

    // 5. Configurar canal 1 y modo continuo
    ADC2->SQR3 = ADC2_CHANNEL;   // Canal 1
    ADC2->CR2 |= ADC_CR2_CONT;
	ADC2->CR2 |= ADC_CR2_ADON;
	ADC2->CR2 |= ADC_CR2_SWSTART;
}

uint16_t ADC2_Read(void) {
    ADC2->CR2 |= ADC_CR2_SWSTART; // Iniciar conversión
    while (!(ADC2->SR & ADC_SR_EOC)); // Esperar fin
    return (uint16_t)(*(volatile uint32_t*) ADC2_DR_ADDRESS);
}
