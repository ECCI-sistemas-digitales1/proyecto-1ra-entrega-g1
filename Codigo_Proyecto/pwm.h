#ifndef PWM_H
#define PWM_H

#define PWM_MIN      90    // Valor mínimo del duty cycle (1 ms aprox.)
#define PWM_MAX      1000   // Valor máximo del duty cycle (2 ms aprox.)
#define PULSE_DURATION1  1.4// Tiempo de activación del PWM en ms
#define PAUSE_DURATION  0  // Tiempo de pausa (PWM apagado) en ms
#define PULSE_DURATION2  4// Tiempo de activación del PWM en ms

void pwm_init(void);
void giro_Izquierda(void);
void giro_Derecha(void);
void freno(void);

#endif
