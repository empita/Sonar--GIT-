#ifndef _TIMER_SONAR
#define _TIMER_SONAR

#include "main.h"

FlagStatus timeout_Sonar;

void configurar_timeOut_sonar (uint32_t periodo);
void TIM6_DAC_IRQHandler(); // Rutina de la base de tiempo 1
void iniciar_timeOut_sonar();
void reiniciar_timeOut_sonar();
void parar_timeOut_sonar();

#endif
