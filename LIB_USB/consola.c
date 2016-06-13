#include "main.h"
#include "consola.h"

void inicializar_consola(){

	UART_PHY_init();
	timeout_PC = RESET;
}

void bloquear_consola(){

	// Bloquea la UART, la int del timer y el DMA.
	USART_Cmd(UART4, DISABLE);
	DMA_Cmd(DMA1_Stream2, DISABLE);
}

void consolaOn(){

	// Borra los datos contenidos en el comando.
	// mensaje_RX se sobreescribirá cuando la consola vuelva a funcionar.

	// Borra todas las banderas de los dispositivos.
	USART_ClearFlag(UART4, USART_FLAG_TC);
	DMA_ClearFlag(DMA1_Stream2, DMA_FLAG_TCIF2);

	// Habilita nuevamente todos los dispositivos
	USART_Cmd(UART4, ENABLE);
	DMA_Cmd(DMA1_Stream2, ENABLE);

}

void vaciar_recepcion(){
	DMA_Cmd(DMA1_Stream2, DISABLE);
	USART_DMACmd(UART4, USART_DMAReq_Rx, DISABLE);
	USART_Cmd(UART4, DISABLE);
	mensajeRXListo = SET;
	DMA_DeInit(DMA1_Stream2);
	USART_DeInit(UART4);
	configurar_UART_RX();
	configurar_UART_RX_DMA();
}

void VCP_send_str(char* cadena){
	int len = strlen(cadena);
	enviarDataOutput(cadena,len);
}

error_PC_read_t leerDatosPC(char* buffer, int len){

	int i;

	for(i=0;i<len;i++) buffer[i] = 0;

	enviarDataOutput("READY", 5);
	configurar_timeOut_PC(500); // Aca ya hace timeOut_PC = 0
	//iniciar_timeOut_PC();

	// Hay que entrar siempre con SET para leer datos desde la PC
	while(leerDataInput(buffer, len) != 1);

	while(mensajeRXListo == RESET && timeout_PC == RESET); // Espera por uno de los dos

	//parar_timeOut_PC();
	// Cuando sale del while, es necesario ver por cuál de las dos condiciones salió
	if(timeout_PC == SET){
		return PC_READ_TIMEOUT;
	}
	else if(mensajeRXListo == SET){
		// Si llego un mensaje de PC hay que determinar si no posee errores. Por ello,
		// se realizan las siguientes tareas:

		if(buffer[0] != 'D' || buffer[1] != 'A' || buffer[2] != 'T' || buffer[3] != 'A')
			// ERROR EN EL ENCABEZADO DEL MENSAJE
			return PC_READ_ERROR_ENC;

		else if(buffer[63] != 13 && buffer[63] != 10)
			// ERROR EN EL TERMINADOR DEL MENSAJE
			return PC_READ_ERROR_TERM;

		else
			// SINO ESTÁ BIEN
			GPIO_ToggleBits(GPIOD, GPIO_Pin_4);
			return PC_READ_OK;
	}
	else {  /// ERROR GRAVE
		;
	}

	return PC_READ_TIMEOUT; // ACA NO DEBERÍA LLEGAR NUNCA

}

void escribirDatosPC(char* buffer, int len){
	enviarDataOutput(buffer, len);
}

void protocoloEmergenciaPCInput(){
	// A DETERMINAR
	return;
}

