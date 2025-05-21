#include "stm32f103xb.h"
#include "pwm.h"

void delay_ms1(uint32_t ms) {
    for (uint32_t i = 0; i < ms * 8000; i++) __NOP();
}

void pwm_init(void) {
    // 1. Habilitar reloj para GPIOA y TIM3
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    // 2. Configurar PA6 como salida alternativa push-pull (TIM3_CH1)
    GPIOA->CRL &= ~(GPIO_CRL_MODE6 | GPIO_CRL_CNF6);
    GPIOA->CRL |= (GPIO_CRL_MODE6_1 | GPIO_CRL_MODE6_0); // 50 MHz
    GPIOA->CRL |= GPIO_CRL_CNF6_1; // Función alternativa push-pull

    // 3. Configurar TIM3 (PSC=7, ARR=1999 = 4.5 kHz)
    TIM3->PSC = 7;
    TIM3->ARR = 1999;
    TIM3->CCR1 = 0;  // Iniciar apagado

    // 4. Modo PWM (Channel 1)
    TIM3->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2; // PWM Mode 1
    TIM3->CCER |= TIM_CCER_CC1E;
    TIM3->CR1 |= TIM_CR1_CEN;
}

void giro_Izquierda(void) {
	TIM3->CCR1 = PWM_MAX;
	delay_ms1(PULSE_DURATION1);

	TIM3->CCR1 = 0;
	delay_ms(PAUSE_DURATION);
}

void giro_Derecha(void) {
	TIM3->CCR1 = PWM_MIN;
	delay_ms1(PULSE_DURATION2);

	TIM3->CCR1 = 0;
	delay_ms(PAUSE_DURATION);
}

void freno(void) {
	TIM3->CCR1 = 0;
}
