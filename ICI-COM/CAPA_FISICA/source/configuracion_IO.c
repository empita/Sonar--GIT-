/*!
 * @file: configuracion_IO.c
 * @brief: Configura todos los pines de entrada y salida
 */


#include "configuracion_IO.h"
#include "control_IO.h"


/*!
 * @defgroup configuracion Configurarion I/O
 * \brief En este archivo se configuran todos los pines del micro
 *
 *
 * Para depurar con la placa en mano es necesario colocarle PULL-UPs internas a las entradas
 * En caso de utilizar la placa base propuesta no hay que usar las PULL-UPs internas
 *
 * @version 1.0
 * @date 04/2015
 *
 *
 */

/*! \ingroup configuracion
 *  \defgroup puertos Puertos I/O
 *
 *  @brief Configuracion de puertos y buses del sistema
 *
 */

/*!
 * @ingroup configuracion
 * @defgroup entradas Pines de entrada
 *
 * @brief Configuracion de las entradas y sus interrupciones
 *
 */

/*! @ingroup configuracion
 * @defgroup salidas Pines de salida
 *
 * @brief Configuracion de las salidas individuales del sistema
 */

/*!
 * \ingroup configuracion
 * \defgroup reloj Pin de clock
 *
 * @brief Configuracion de un timer y un PWM para generar el reloj de 4 MHz
 */

/*!
 * @ingroup configuracion
 * @{
 */
GPIO_InitTypeDef GPIOConf; //!< Estructura de configuracion de los GPIO
NVIC_InitTypeDef   NVIC_InitStructure; //!< Estructura de configuracion de interrupciones
EXTI_InitTypeDef   EXTI_InitStructure; //!< Estructura de configuracion de EXTI

/*!
 * \brief Inicializa la capa fisica.
 *
 * Esta funcion llama a todas las funciones de configuracion de la libreria.
 *
 * Las variables EIP, IDRP, ODRP y RQ son variables que almacenan el estado actual de dichos
 * pines. Se actualizan automaticamente mediante interrupciones, en la capa de enlace.
 * Sus valores pueden ser SET (pin en '1') o RESET (pin en '0').
 *
 *
 * La funcion NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1) permite definir la forma
 * en que se organizan los niveles de prioridades para las interrupciones. En este caso
 * se asignan dos niveles para la prioridad y 128 niveles para la subprioridad.
 *
 * La prioridad define que interrupcion puede anidarse y en que orden. '0' es la prioridad mas grande.
 * La subprioridad define que rutina se ejecuta cuando dos interrupciones se dan al mismo tiempo.
 *
 * @date 05/2015
 * @version 1.0
 * @author Campos
 */
extern void CAPA_FISICA_Init(){

	EIP = SET;
	IDRP = SET;
	ODRP = SET;
	RQ = SET;


	encender_puertos();

	configurarEFD();
	configurarEN();
	configurarSTOP();
	configurarACK();
	configurarRESD();
	poner_salidas_alto();

	configurarEIP();
	configurarEIP_INT();
	configurarIDRP();
	configurarIDRP_INT();
	configurarODRP();
	configurarODRP_INT();
	configurarRQ();
	configurarRQ_INT();

	configurar_puerto_U();
	configurar_puerto_I();
	configurar_DA();

	escribir_puerto_U(0xFFFFFF);

	configurarCLOCK();
	deshabilitar_reloj();

}
//!@} Fin del grupo configuracion

/*! \ingroup puertos
 * @{
 */

/*!
 * @brief Habilita el clock de cada GPIO y del SYSCFG.
 *
 * Se utiliza GPIOA, B, C, D y E.
 *
 * SYSCFG se utiliza para las interrupciones externas
 *
 * @date 05/2015
 * @version 1.0
 * @author Campos
 */
void encender_puertos(){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	// Inicia el controlador de interrupciones
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
}



/*!
 * @brief Configura todos los pines correspondientes al puerto U (salida).
 *
 * Puerto de salida de la MU. Se configuran los pines como salida sin PULL UP, modo
 * PUSHPULL, velocidad 25 MHz.
 *
 * Asignacion:
 * 	- PB completo
 * 	- Primer byte del puerto C
 *
 * @date 05/2015
 * @version 1.0
 * @author Campos
 */
extern void configurar_puerto_U(){

	GPIOConf.GPIO_Pin = GPIO_Pin_All;
	GPIOConf.GPIO_Mode = GPIO_Mode_OUT;
	GPIOConf.GPIO_OType = GPIO_OType_PP;
	GPIOConf.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIOConf.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_Init(GPIOB, &GPIOConf);

	GPIOConf.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7  ;
	GPIOConf.GPIO_Mode = GPIO_Mode_OUT;
	GPIOConf.GPIO_OType = GPIO_OType_PP;
	GPIOConf.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIOConf.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_Init(GPIOC, &GPIOConf);
}

/*!
 * @brief Configura todos los pines correspondientes al puerto I (entrada).
 *
 * Puerto de entrada de la MU. Se configuran los pines como entrada sin PULL UP, velocidad 25 MHz.
 *
 * Asignacion:
 * 	- Ultimo byte del puerto D
 * 	- Puerto E completo
 *
 * @date 05/2015
 * @version 1.0
 * @author Campos
 */
extern void configurar_puerto_I(){

	GPIOConf.GPIO_Pin = GPIO_Pin_All;
	GPIOConf.GPIO_Mode = GPIO_Mode_IN;
	GPIOConf.GPIO_OType = GPIO_OType_PP;
	GPIOConf.GPIO_PuPd = GPIO_PuPd_NOPULL;
	//GPIOConf.GPIO_PuPd = GPIO_PuPd_UP;
	GPIOConf.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_Init(GPIOE, &GPIOConf);

	GPIOConf.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10| GPIO_Pin_11| GPIO_Pin_12| GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15 ;
	GPIOConf.GPIO_Mode = GPIO_Mode_IN;
	GPIOConf.GPIO_OType = GPIO_OType_PP;
	GPIOConf.GPIO_PuPd = GPIO_PuPd_NOPULL;
	//GPIOConf.GPIO_PuPd = GPIO_PuPd_UP;
	GPIOConf.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_Init(GPIOD, &GPIOConf);
}


/*!
 * @brief Configura todos los pines correspondientes al puerto DA (direcciones - entrada).
 *
 * Puerto de direcciones de la MU. Se configuran los pines como entrada sin PULL UP,
 *  velocidad 25 MHz.
 *
 * Asignacion:
 *		- Pines 3, 6, 7 y 8 del puerto A
 * 		- Pines 14 y 15 del puerto C
 *
 * @date 05/2015
 * @version 1.0
 * @author Campos
 */
extern void configurar_DA(){

	GPIOConf.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
	GPIOConf.GPIO_Mode = GPIO_Mode_IN;
	GPIOConf.GPIO_OType = GPIO_OType_PP;
	GPIOConf.GPIO_PuPd = GPIO_PuPd_NOPULL;
	//GPIOConf.GPIO_PuPd = GPIO_PuPd_UP;
	GPIOConf.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_Init(GPIOA, &GPIOConf);

	GPIOConf.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14;
	GPIOConf.GPIO_Mode = GPIO_Mode_IN;
	GPIOConf.GPIO_OType = GPIO_OType_PP;
	GPIOConf.GPIO_PuPd = GPIO_PuPd_NOPULL;
	//GPIOConf.GPIO_PuPd = GPIO_PuPd_UP;
	GPIOConf.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_Init(GPIOC, &GPIOConf);

}
//!@} Fin del grupo puertos



/*! @ingroup entradas
 * @{
 */


/*!
 * \brief Configura el pin RQ como entrada.
 *
 * RQ es el pin PA0. Es necesario desconectar el pulsador de la Discovery.
 * No se conectan resistencias de PULL UP o PULL DOWN.
 *
 * @date 05/2015
 * @version 1.0
 * @author Campos
 */
extern void configurarRQ(){

	GPIOConf.GPIO_Pin = GPIO_Pin_0;
	GPIOConf.GPIO_Mode = GPIO_Mode_IN;
	GPIOConf.GPIO_OType = GPIO_OType_PP;
	GPIOConf.GPIO_PuPd = GPIO_PuPd_NOPULL;
	//GPIOConf.GPIO_PuPd = GPIO_PuPd_UP;
	GPIOConf.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_Init(GPIOA, &GPIOConf);
}

/*!
 * @brief Configura la interrupcion externa para el pin RQ
 *
 * Esta funcion configura la interrupcion externa para el pin RQ.
 *
 * Prioridad: 0
 *
 * Subprioridad: 0
 *
 * Este pin posee la mas alta prioridad de todas.
 *
 * @date 05/2015
 * @version 1.0
 * @author Campos
 */
extern void configurarRQ_INT(){
	/* Connect EXTI Line0 to PA0 pin */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

	/* Configure EXTI Line0 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set EXTI Line0 Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel =   EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}  // configura la interrupcion en ese pin


/*!
 * \brief Configura el pin EIP como entrada.
 *
 * EIP es el pin PA2.
 * No se conectan resistencias de PULL UP o PULL DOWN.
 *
 * @date 05/2015
 * @version 1.0
 * @author Campos
 */
extern void configurarEIP(){

	GPIOConf.GPIO_Pin = GPIO_Pin_2;
	GPIOConf.GPIO_Mode = GPIO_Mode_IN;
	GPIOConf.GPIO_OType = GPIO_OType_PP;
	GPIOConf.GPIO_PuPd = GPIO_PuPd_NOPULL;
	//GPIOConf.GPIO_PuPd = GPIO_PuPd_UP;
	GPIOConf.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_Init(GPIOA, &GPIOConf);
}

/*!
 * @brief Configura la interrupcion externa para el pin EIP
 *
 * Esta funcion configura la interrupcion externa para el pin EIP.
 *
 * Prioridad: 1
 *
 * Subprioridad: 0
 *
 * Este pin no puede anidar a RQ.
 *
 * @date 05/2015
 * @version 1.0
 * @author Campos
 */
extern void configurarEIP_INT(){

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource2);

	/* Configure EXTI Line2 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set EXTI Line2 Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel =   EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}  // configura la interrupcion en ese pin


/*!
 * \brief Configura el pin IDRP como entrada.
 *
 * IDRP es el pin PD1.
 * No se conectan resistencias de PULL UP o PULL DOWN.
 *
 * @date 05/2015
 * @version 1.0
 * @author Campos
 */
extern void configurarIDRP(){

	GPIOConf.GPIO_Pin = GPIO_Pin_1;
	GPIOConf.GPIO_Mode = GPIO_Mode_IN;
	GPIOConf.GPIO_OType = GPIO_OType_PP;
	GPIOConf.GPIO_PuPd = GPIO_PuPd_NOPULL;
	//GPIOConf.GPIO_PuPd = GPIO_PuPd_UP;
	GPIOConf.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_Init(GPIOD, &GPIOConf);
}

/*!
 * @brief Configura la interrupcion externa para el pin IDRP
 *
 * Esta funcion configura la interrupcion externa para el pin IDRP.
 *
 * Prioridad: 1
 *
 * Subprioridad: 2
 *
 * Este pin no puede anidar a RQ.
 *
 * @date 05/2015
 * @version 1.0
 * @author Campos
 */
extern void configurarIDRP_INT(){
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource1);

	/* Configure EXTI Line1 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set EXTI Line1 Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel =   EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}  // configura la interrupcion en ese pin


/*!
 * \brief Configura el pin ODRP como entrada.
 *
 * ODRP es el pin PD3.
 * No se conectan resistencias de PULL UP o PULL DOWN.
 *
 * @date 05/2015
 * @version 1.0
 * @author Campos
 */
extern void configurarODRP(){

	GPIOConf.GPIO_Pin = GPIO_Pin_3;
	GPIOConf.GPIO_Mode = GPIO_Mode_IN;
	GPIOConf.GPIO_OType = GPIO_OType_PP;
	GPIOConf.GPIO_PuPd = GPIO_PuPd_NOPULL;
	//GPIOConf.GPIO_PuPd = GPIO_PuPd_UP;
	GPIOConf.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_Init(GPIOD, &GPIOConf);
}


/*!
 * @brief Configura la interrupcion externa para el pin ODRP
 *
 * Esta funcion configura la interrupcion externa para el pin ODRP.
 *
 * Prioridad: 1
 *
 * Subprioridad: 1
 *
 * Este pin no puede anidar a RQ.
 *
 * @date 05/2015
 * @version 1.0
 * @author Campos
 */
extern void configurarODRP_INT(){

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource3);

	/* Configure EXTI Line3 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line3;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set EXTI Line3 Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel =   EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}  // configura la interrupcion en ese pin

//!@}  Fin del grupo entradas



/*!
 * @ingroup salidas
 * @{
 */


/*!
 * @brief Configura el pin RESD como salida.
 *
 * Velocidad: 25 MHz
 *
 * Modo: Push-pull
 *
 * Pin: PA4
 */
extern void configurarRESD(){

	GPIOConf.GPIO_Pin = GPIO_Pin_5;
	GPIOConf.GPIO_Mode = GPIO_Mode_OUT;
	GPIOConf.GPIO_OType = GPIO_OType_PP;
	GPIOConf.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIOConf.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_Init(GPIOD, &GPIOConf);

}

/*!
 * @brief Configura el pin EFD como salida.
 *
 * Velocidad: 25 MHz
 *
 * Modo: Push-pull
 *
 * Pin: PA5
 */
extern void configurarEFD(){

	GPIOConf.GPIO_Pin = GPIO_Pin_5;
	GPIOConf.GPIO_Mode = GPIO_Mode_OUT;
	GPIOConf.GPIO_OType = GPIO_OType_PP;
	GPIOConf.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIOConf.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIOConf);
}

/*!
 * @brief Configura el pin EN como salida.
 *
 * Velocidad: 25 MHz
 *
 * Modo: Push-pull
 *
 * Pin: PC9
 */
extern void configurarEN(){
	// Lo pasamos por el tema del debugger
	GPIOConf.GPIO_Pin = GPIO_Pin_9;
	GPIOConf.GPIO_Mode = GPIO_Mode_OUT;
	GPIOConf.GPIO_OType = GPIO_OType_PP;
	GPIOConf.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIOConf.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_Init(GPIOC, &GPIOConf);
}

/*!
 * @brief Configura el pin STOP como salida.
 *
 * Velocidad: 25 MHz
 *
 * Modo: Push-pull
 *
 * Pin: PC8
 */
extern void configurarSTOP(){

	// Lo pasamos por el tema del debugger
	GPIOConf.GPIO_Pin = GPIO_Pin_8;
	GPIOConf.GPIO_Mode = GPIO_Mode_OUT;
	GPIOConf.GPIO_OType = GPIO_OType_PP;
	GPIOConf.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIOConf.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIOConf);
}

/*!
 * @brief Configura el pin ACK como salida.
 *
 * Velocidad: 25 MHz
 *
 * Modo: Push-pull
 *
 * Pin: PA15
 */
extern void configurarACK(){

	GPIOConf.GPIO_Pin = GPIO_Pin_15;
	GPIOConf.GPIO_Mode = GPIO_Mode_OUT;
	GPIOConf.GPIO_OType = GPIO_OType_PP;
	GPIOConf.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIOConf.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIOConf);
}

//!@} Fin del grupo salidas


/*!
 * @ingroup reloj
 * @{
 */

/*! @brief Clock generado para la ICI.
 *
 * Se utiliza el pin PA1 que corresponde al TIM2 canal 2.
 *
 *
 * Para generar un clock hay que configurar dos dispositivos del uC:
 * 		- Un timer
 * 		- Un modulo PWM
 *
 * 	El clock asignado al timer es de 40 MHz. Referirse a la configuracion de clock global para mas informacion.
 *
 *
 * 	TIM_TimeBaseInit() permite configurar el timer. Los parametros mas importantes son:
 * 		- Modo ascendente
 * 		- Prescaler = 1 (no divide el clock global)
 * 		- Periodo = 10. Se debe dividir el clock del timer (40 MHz) por 10. Como cuenta desde 0, el valor
 * 			del parametro 'Period' debe ser 9.
 *
 *
 * 	TIM_OC2Init() configura el canal PWM 2 del timer 2. El parametro mas importante es 'Pulse'.
 * 	Este parametro define el porcentaje del PWM. Si Periodo es 10, Pulse debe ser 5 para que el
 * 	Duty Cicle sea de 50 %.
 *
 *
 * 	TIM_Cmd () habilita o deshabilita el clock segun el parametro que se pase (ENABLE o DISABLE).
 */
extern void configurarCLOCK(){

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // Activa el timer
	// El clock del timer para la conf actual es de 40 MHz

	SYSCFG_CompensationCellCmd (ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;  // Configura el pin PA1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM2);  // Conecta el pin PA1 al timer

	/* Time Base configuration */
	TIM_TimeBaseStructure.TIM_Prescaler = 1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 10-1; // Para obtener los 4 MHz en el pin
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	/* Channel 2 Configuration in PWM mode */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_Pulse = 5; // PWM al 50 %
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);  // Configura OC2 (chanel 2)


	/* TIM1 counter disable
	 *  Cuando se quiera habilitar el clock hay que habilitarlo
	 *  */
	TIM_Cmd(TIM2, DISABLE);

	/* Main Output Enable */
	TIM_CtrlPWMOutputs(TIM2, ENABLE);

}

//!@}



