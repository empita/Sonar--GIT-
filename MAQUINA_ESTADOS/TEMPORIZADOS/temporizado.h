#ifndef _TEMPORIZADO
#define _TEMPORIZADO

#include "main.h"

FlagStatus timeout_FSM_500;

void configurar_timeOut_FSM_500 (uint32_t periodo);
void TIM2_IRQHandler(); // Rutina de la base de tiempo 1
void iniciar_timeOut_FSM_500();
void reiniciar_timeOut_FSM_500();
void parar_timeOut_FSM_500();

#endif
