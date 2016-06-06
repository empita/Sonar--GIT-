#include "temporizado.h"

/* BASE DE TIEMPO DESTINADA A CONTROLAR LOS LA CADENCIA DE TRANSFERENCIAS ENTRE LA ICI
 * Y EL MICRO-CONTROLADOR
 * SE PROVEEN FUNCIONES PARA:
 * 		- Configurar la base de tiempo
 * 		- Iniciarla
 * 		- Detenerla
 * 		- Programar la rutina de la base de tiempo
 *
 * 		UTILIZA EL TIMER TIM2
 * 		LA BASE DE TIEMPO CORRE CONTINUAMENTE UNA VEZ QUE SE LA INICIA
 */

// CONFIGURA EL TIM2 PARA QUE GENERE UNA INTERRUPCIÓN CADA DETERMINADO PERIODO EN ms
// NO INICIA LA BASE DE TIEMPO, SOLO LA CONFIGURA
void configurar_timeOut_FSM_500 (uint32_t periodo){
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef   NVIC_InitStruct ;

	//// USO DEL TIM6
	// El clock de APB1 es 80 MHz

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = (2*periodo)-1;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 40000-1; // Clock de 0.5 KHz
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStruct);

	NVIC_InitStruct.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=3;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	 NVIC_Init (&NVIC_InitStruct );

	 TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
	 timeout_FSM_500 = RESET;
}

///  INICIA LA BASE DE TIEMPO DESDE EL PUNTO EN QUE SE DETUVO
void iniciar_timeOut_FSM_500(){
	TIM_Cmd(TIM5,ENABLE);
}

///  RE-INICIA EL CONTADOR Y VUELVE A ACTIVAR LA BASE DE TIEMPO DESDE CERO
void reiniciar_timeOut_FSM_500(){
	parar_timeOut_FSM_500();
	TIM_SetCounter(TIM5,0);
	iniciar_timeOut_FSM_500();
}

/// DETIENE LA BASE DE TIEMPO, MAS NO REINICIA EL CONTADOR
void parar_timeOut_FSM_500(){
	TIM_Cmd(TIM5,DISABLE);
}


void TIM5_IRQHandler(){
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update);
	TIM_ClearFlag(TIM5,TIM_FLAG_Update);

	timeout_FSM_500 = SET;

}
