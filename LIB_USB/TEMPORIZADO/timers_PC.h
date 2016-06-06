#ifndef _TIMER_PC
#define _TIMER_PC

#include "main.h"

FlagStatus timeout_PC;

void configurar_timeOut_PC (uint32_t periodo);
void TIM7_IRQHandler(); // Rutina de la base de tiempo 1
void iniciar_timeOut_PC();
void reiniciar_timeOut_PC();
void parar_timeOut_PC();

#endif
