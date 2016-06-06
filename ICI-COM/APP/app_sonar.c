#include "app_sonar.h"
#include "mensajes.h"
#include "io_lib.h"

void app_sonar_init(){

	CAPA_ENLACE_Init();

	CANT_PALABRAS_ENTRADA = 52;
	CANT_PALABRAS_SALIDA = 38;


	error_Transporte = RESET;
	error_Timeout = RESET;
	error_Enc_DHC = RESET;
	error_Enc_Sonar = RESET;
	error_PC = RESET;

	cant_error_Transporte = 0;
	cant_error_Timeout = 0;
	cant_error_Enc_DHC = 0;
	cant_error_Enc_Sonar = 0;
	cant_error_PC = 0;

	timeout_Sonar = RESET;
}

DCL_input_estado_t leerDatosICI(){

	cont_IN = CANT_PALABRAS_ENTRADA;
	int contador;

	llegoEstadoIN = RESET;
	fin_bloque_IN = RESET;
	timeout_Sonar = RESET;
	configurar_timeOut_sonar(300); // Espero 300 ms. 250 ms es el máx permitido por el sonar
	EF_INPUT();
	iniciar_timeOut_sonar();

	while(timeout_Sonar == RESET && llegoEstadoIN == RESET);

	parar_timeOut_sonar();

	if(timeout_Sonar==SET)
	{
		// NO llegó nada de nada -> salir directamente
		encender_LED_Error_ICI_TO();
		cant_error_Timeout++;
		return OUTPUT_TIME_OUT;
	}
	else if ( llegoEstadoIN == SET)
	{
		if(estadoIN == INPUT_OK){
			// Leo el mensaje que llegó y lo guardo
			for(contador = 0; contador < total_IN; contador++)
				((uint32_t*)mensajeICIInput)[contador] = buffer_IN[contador];
			//traduccion_ICI_a_PC(); 		// Traduccion
			cant_error_Transporte = 0; 	// Para contabilizar los que se dan seguidos
			apagar_LED_Error_ICI_ET();
			apagar_LED_Error_ICI_TO();
			return INPUT_OK;
		}
		else if(estadoIN == INPUT_HW_NO_LISTO){
			return estadoIN;
		}
		else if(estadoIN == INPUT_ERROR_TRANSP){
			cant_error_Transporte++;
			encender_LED_Error_ICI_ET();
			if(cant_error_Transporte > 3) return INPUT_ERROR_TRANSP;
		}
		else
			return INPUT_NO_ESTADO; // No debería llegar nunca acá
	}

	return INPUT_NO_ESTADO; // No debería llegar nunca acá
}

DCL_output_estado_t escribirDatosICI(){

	DCL_output_estado_t state;

	if(llegoEstadoOUT == RESET)
	{
		// Si no llegó ningún estado es porque es la primera transmisión o porque
		// se colgó del otro lado del enlace. En este caso se le pide explícitamente el estado
		// a la ICI para verificarlo.
		state = verificarEstadoOutput();

		if(state == OUTPUT_ERROR_TRANSP){
			cant_error_Transporte++;
			encender_LED_Error_ICI_ET();
			if(cant_error_Transporte > 3) return state; // Si no hubo más de tres seguidos -> sigue
		}
		else if( state == OUTPUT_HW_NO_LISTO || state == OUTPUT_SW_NO_LISTO){
			return state;
		}
	}
	else
	{
		// En cambio si ya había un estado esperando, lo verificamos:
		// Si el estado proviene de la transferencia anterior solo puede ser un error de
		// transporte, ya que cualquier otro lo podría detectar al finalizar esta
		// rutina.
		state = estadoOUT;

		if(state == OUTPUT_ERROR_TRANSP){
			encender_LED_Error_ICI_ET();
			cant_error_Transporte++;
			if(cant_error_Transporte > 3) return state; // Si no hubo más de tres seguidos -> sigue
		}
	}

	// Sea cual sea el camino que se siguió arriba, si llegó la ejecución a este punto es
	// porque no hubo errores en la ICI.

	llegoEstadoOUT = RESET;
	fin_bloque_OUT = RESET;

	cont_OUT = CANT_PALABRAS_SALIDA;
	buffer_OUT = (uint32_t*) mensajeICIOutput;
	EF_OUTPUT();  // Saco el mensaje

	timeout_Sonar = RESET;
	configurar_timeOut_sonar(50); // Si pasan 100 ms sin la respuesta, se considera timeout
	iniciar_timeOut_sonar();

	while( timeout_Sonar == RESET && fin_bloque_OUT == RESET && llegoEstadoOUT == RESET);

	parar_timeOut_sonar();

	if(timeout_Sonar == SET)
	{
		// NO llegó nada de nada -> salir directamente
		encender_LED_Error_ICI_TO();
		cant_error_Timeout++;
		return OUTPUT_TIME_OUT;
	}
	else if(llegoEstadoOUT == SET){
		// Si me devuelve el estado inmediatamente es porque hubo error de HWNR o SWNR}
		cant_error_Transporte = 0; // para contabilizar solo los seguidos
		return estadoOUT;
	}
	else if(fin_bloque_OUT == SET){
		// Si llegó acá es porque efectivamente mandó la trama
		cant_error_Transporte = 0; // para contabilizar solo los seguidos
		apagar_LED_Error_ICI_ET();
		apagar_LED_Error_ICI_TO();
		return OUTPUT_OK;
	}
	else{
		return OUTPUT_NO_ESTADO; // Nunca debería llegar acá
	}


	return OUTPUT_NO_ESTADO; // Nunca devería llegar acá
}


DCL_input_estado_t verificarEstadoInput(){

	llegoEstadoIN = RESET;
	EF_STATREQ_DCL_INPUT();
	while(llegoEstadoIN == RESET);
	return estadoIN;
}

DCL_output_estado_t verificarEstadoOutput(){

	llegoEstadoOUT = RESET;
	EF_STATREQ_DCL_OUTPUT();
	while(llegoEstadoOUT == RESET);
	return estadoOUT;
}


void protocoloEmergenciaInput(){

}

void protocoloEmergenciaOutput(){


}


void protocoloErrorTransporteInput(){

}

void protocoloErrorTransporteOutput(){
	GPIO_ToggleBits(GPIOD, GPIO_Pin_2);
}


void verificarSysOnLocal(){
	// Entrada activas por alto
	while(leer_SysOn_Sonar() == RESET);
	return;
}

void verificarSysOnDHC(){
	// Entrada activas por alto
	while(leer_SysOn_DHC() == RESET);
	return;
}

