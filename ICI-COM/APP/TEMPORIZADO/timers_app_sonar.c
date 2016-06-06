#include "timers_app_sonar.h"

/* BASE DE TIEMPO DESTINADA A CONTROLAR LOS TIMEOUT DE LAS TRANSFERENCIAS
 * ENTRE LA ICI Y EL MICROCONTROLADOR
 * SE PROVEEN FUNCIONES PARA:
 * 		- Configurar la base de tiempo
 * 		- Iniciarla
 * 		- Detenerla
 * 		- Programar la rutina de la base de tiempo
 *
 * 		UTILIZA EL TIMER TIM6
 * 		LA BASE DE TIEMPO ES ONESHOT, ES DECIR, CUANDO SE INICIA, AL LLEGAR AL
 * 		VENCIMIENTO, SE DETIENE
 */

// CONFIGURA EL TIM6 PARA QUE GENERE UNA INTERRUPCIÓN CADA DETERMINADO PERIODO EN ms
// NO INICIA LA BASE DE TIEMPO, SOLO LA CONFIGURA
void configurar_timeOut_sonar (uint32_t periodo){
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef   NVIC_InitStruct ;

	//// USO DEL TIM6
	// El clock de APB1 es 80 MHz

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = (2*periodo)-1;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 40000-1; // Clock de 0.5 KHz
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStruct);

	NVIC_InitStruct.NVIC_IRQChannel = TIM6_DAC_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=3;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	 NVIC_Init (&NVIC_InitStruct );

	 TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
	 timeout_Sonar = RESET;
}

///  INICIA LA BASE DE TIEMPO DESDE EL PUNTO EN QUE SE DETUVO
void iniciar_timeOut_sonar(){
	TIM_Cmd(TIM6,ENABLE);
}

///  RE-INICIA EL CONTADOR Y VUELVE A ACTIVAR LA BASE DE TIEMPO DESDE CERO
void reiniciar_timeOut_sonar(){
	parar_timeOut_sonar();
	TIM_SetCounter(TIM6,0);
	iniciar_timeOut_sonar();
}

/// DETIENE LA BASE DE TIEMPO, MAS NO REINICIA EL CONTADOR
void parar_timeOut_sonar(){
	TIM_Cmd(TIM6,DISABLE);
}


void TIM6_DAC_IRQHandler(){
	TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
	TIM_ClearFlag(TIM6,TIM_FLAG_Update);

	timeout_Sonar = SET;
	parar_timeOut_sonar();

}

