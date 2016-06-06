#include "consola_PHY.h"


#define BAUD_RATE 115200
//#define BAUD_RATE 2000000


DMA_InitTypeDef  DMA_UART_TX_InitStructure; // La mantengo como variable global para cargar sus
									// parametros solo una vez, y luego modificar solo la
									// cantidad de bytes a enviar

DMA_InitTypeDef  DMA_UART_RX_InitStructure; // La mantengo como variable global para cargar sus
									// parametros solo una vez, y luego modificar solo la
									// cantidad de bytes a enviar
USART_InitTypeDef USART_InitStructure;


FlagStatus primeraTX;

void UART_PHY_init(){
	uint32_t i;

	for(i=0;i<BUFFER_OUT;i++)
		mensaje_TX[i] = 0;

	USART_ClearFlag(UART4, USART_FLAG_TC);  // Baja la bandera de la UART
	DMA_ClearFlag(DMA1_Stream4,DMA_FLAG_TCIF4); // Baja la bandera del DMA
	USART_ClearFlag(UART4, USART_FLAG_RXNE);  // Baja la bandera de la UART
	DMA_ClearFlag(DMA1_Stream2,DMA_FLAG_TCIF2); // Baja la bandera del DMA

	configurar_UART_TX_pin();
	configurar_UART_TX();
	configurar_UART_TX_DMA();

	configurar_UART_RX_pin();
	configurar_UART_RX();
	configurar_UART_RX_DMA();

	mensajeRXListo = SET;

}

void configurar_UART_TX_pin(){
	GPIO_InitTypeDef GPIO_InitStructure;

	// Habilitación del clock del pin TX
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	// Habilita el pin PC10 como función alternativa para UART TX
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART3);

	// Se configura el pin
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	// TX-Pin
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

}

void configurar_UART_TX(){
	USART_InitTypeDef USART_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	// Configuracion de la UART
	USART_InitStructure.USART_BaudRate = BAUD_RATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);

	// Enciendo la UART
	USART_Cmd(USART3, ENABLE);
}

void configurar_UART_TX_DMA(){

	// Clock enable para el DMA
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

	// Iniciliza canal del DMA para la UART4
	DMA_DeInit(DMA1_Stream3);
	DMA_UART_TX_InitStructure.DMA_BufferSize = BUFFER_OUT;
	DMA_UART_TX_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_UART_TX_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	DMA_UART_TX_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single ;
	DMA_UART_TX_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_UART_TX_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_UART_TX_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_UART_TX_InitStructure.DMA_PeripheralBaseAddr =(uint32_t) (&(USART3->DR));
	DMA_UART_TX_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_UART_TX_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_UART_TX_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_UART_TX_InitStructure.DMA_Priority = DMA_Priority_High;

	DMA_UART_TX_InitStructure.DMA_Channel = DMA_Channel_4;
	DMA_UART_TX_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_UART_TX_InitStructure.DMA_Memory0BaseAddr =(uint32_t)mensaje_TX;
	DMA_Init(DMA1_Stream3,&DMA_UART_TX_InitStructure);

	//DMA_Cmd(DMA1_Stream3,ENABLE);
	USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);
}

// FUNCION QUE ENVIA UN MENSAJE MEDIANTE LA UART Y EL DMA
// CUANDO SE INGRESA, SE VERIFICA EL FLAG "TC" DE LA UART4 PARA VERIFICAR
// QUE LA MISMA HAYA TERMINADO. SI NO TERMNO, DEVUELVE '0'. SI YA TERMINO
// ENTONCES COPIA EL MENSAJE RECIBIDO A LA VARIABLE "mensaje" Y CUENTA LA CANTIDAD
// DE BYTES A ENVIAR.

void enviarDataOutput(char* msj, int longitud){
	uint16_t n;

	while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==0);
	USART_ClearFlag(USART3, USART_FLAG_TC);  // Baja la bandera de la UART
	DMA_ClearFlag(DMA1_Stream3,DMA_FLAG_TCIF3); // Baja la bandera del DMA

	//INICIA LA COPIA DEL MENSAJE PASADO COMO PARÁMETRO
	for(n = 0; n< longitud; n++)
		mensaje_TX[n] = msj[n];

	DMA_UART_TX_InitStructure.DMA_BufferSize = longitud;
	DMA_Init(DMA1_Stream3,&DMA_UART_TX_InitStructure);

	DMA_Cmd(DMA1_Stream3,ENABLE);
	USART_Cmd(USART3, ENABLE);
	USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);

	return;
}

void configurar_UART_RX_pin(){
	GPIO_InitTypeDef GPIO_InitStructure;

	// Habilitación del clock del pin RX
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	// Habilita el pin PC11 como función alternativa para UART RX
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_UART4);

	// Se configura el pin
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	// RX-Pin
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void configurar_UART_RX(){

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

	// Configuracion de la UART
	USART_InitStructure.USART_BaudRate = BAUD_RATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx;
	USART_Init(UART4, &USART_InitStructure);

	// Enciendo la UART
	USART_Cmd(UART4, ENABLE);
}

void configurar_UART_RX_DMA(){
	// Clock enable para el DMA
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
	NVIC_InitTypeDef NVIC_InitStructure;

	// enable TX-Interrupt
	NVIC_InitStructure.NVIC_IRQChannel 						= DMA1_Stream2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// Iniciliza canal del DMA para la UART4 para recibir
	DMA_DeInit(DMA1_Stream2);
	DMA_UART_RX_InitStructure.DMA_BufferSize = 1;
	DMA_UART_RX_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_UART_RX_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	DMA_UART_RX_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single ;
	DMA_UART_RX_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_UART_RX_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_UART_RX_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_UART_RX_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) (&(UART4->DR));
	DMA_UART_RX_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_UART_RX_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_UART_RX_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_UART_RX_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;

	DMA_UART_RX_InitStructure.DMA_Channel = DMA_Channel_4;
	DMA_UART_RX_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_UART_RX_InitStructure.DMA_Memory0BaseAddr =(uint32_t)0;

	DMA_Init(DMA1_Stream2,&DMA_UART_RX_InitStructure);
	DMA_ITConfig(DMA1_Stream2, DMA_IT_TC, ENABLE);
	USART_DMACmd(UART4, USART_DMAReq_Rx, ENABLE);

}

// FUNCION QUE ACTIVA EL DMA DE LA UART Y LO CONFIGURA EN FUNCIÓN DE LA LONGITUD DEL MENSAJE
// ESPERADO. EN LA INTERRUPCIÓN DEL DMA SE COPIA EL MENSAJE AL BUFFER INTERNO

 int leerDataInput(char* msj, int longitud){

	if(longitud == 0) return -1;  // Error

	if(mensajeRXListo == RESET) return 0;  // Todavía hay una transferencia


	mensajeRXListo = RESET;
	DMA_UART_RX_InitStructure.DMA_Memory0BaseAddr = (uint32_t)msj;
	DMA_UART_RX_InitStructure.DMA_BufferSize = longitud;
	DMA_Init(DMA1_Stream2, &DMA_UART_RX_InitStructure);

	USART_DMACmd(UART4, USART_DMAReq_Rx, ENABLE);
	USART_Cmd(UART4, ENABLE);
	DMA_Cmd(DMA1_Stream2,ENABLE);

	return 1;
}

void DMA1_Stream2_IRQHandler(){
	USART_DMACmd(UART4, USART_DMAReq_Rx, DISABLE);
	DMA_ClearFlag(DMA1_Stream2,DMA_FLAG_TCIF2); // Baja la bandera del DMA
	USART_ClearFlag(UART4, USART_FLAG_TC);  // Baja la bandera de la UART
	mensajeRXListo = SET;
}

