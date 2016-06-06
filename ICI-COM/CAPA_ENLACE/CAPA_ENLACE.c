/*!
 * @file CAPA_ENLACE.c
 * Este archivo posee todas las funciones necesarias para implementar la capa de enlace
 * para establecer la comunicacion con la ICI.
 *
 * @version 2.0
 * @date 05/2015
 * @author Campos
 */

/*!
 * @mainpage Capa de enlace para la comunicacion con la ICI - V2
 *
 * Esta libreria posee todas las funciones necesarias para generar comunicarse con la
 * ICI a nivel de capa de enlace. Desde esta libreria se pueden realizar las siguientes
 * tareas:
 *
 * - Generar External Functions
 * - Generar Ackowledge y Stop
 * - Responder a los requerimientos (Como un selector de prioridades)
 * - Responder a interrupciones de la ICI (EIP)
 * - Responder a los pedidos para sacar palabras (ODRP)
 * - Responder a los pedidos para entrar palabras (IDRP)
 *
 * 	Adicionalmente, en forma auxiliar, se dispone de una variedad de retardos para generar
 * 	la temporizacion necesaria.
 */


/*!
 * @defgroup capa_enlace Capa de enlace
 * @brief Capa de enlace para la comunicacion con la ICI
 *
 * Los modulos de esta libreria permiten establecer comunicaciones a nivel de capa de enlace
 *
 */

/*!
 * @ingroup capa_enlace
 * @{
 */
#include "CAPA_ENLACE.h"
#include "consola.h"



/*!
 * @brief Inicializa la capa de enlace
 *
 * Esta funcion realiza el siguiente conjunto de acciones:
 * - Inicializa la capa fisica
 * - Resetea todas las variables de la capa de enlace
 *
 * @version 1.0
 * @date 05/2015
 * @author Campos
 */
void CAPA_ENLACE_Init(){

	
	CAPA_FISICA_Init();

	fin_bloque_IN= RESET;
	cont_IN = 0;
	total_IN = 0;

	fin_bloque_OUT = RESET;
	cont_OUT = 0;
	total_OUT = 0;

	llegoEstadoIN = RESET;
	llegoEstadoOUT = RESET;
	estadoIN = 0;
	estadoOUT = 0;
	
	modo_service = RESET;

	habilitar_reloj();
	resetear_ICI();

}
//!@}

/*!
 * @ingroup capa_enlace
 * @defgroup salidas_enlace Respuestas de la capa de enlace
 * Posee las funciones de respuesta de la capa de enlace.
 *
 * @{
 */

/*!
 * @brief Genera un pulso ACK en dicha salida
 *
 * Esta funcion genera un pulso ACK como respuesta a algun pedido de la ICI.
 * Responde a requerimientos temporales estrictos y definidos en los trabajos de
 * desarrollo de la AR-ICI.
 *
 * @version 1.0
 * @date 05/2015
 * @author Campos
 *
 */
void ACK(){
	resetear_ACK();
	retardo_250ns();
	retardo_250ns();
	retardo_250ns();
	retardo_250ns();
	setear_ACK();
}


/*!
 * @brief Genera un pulso ACK+STOP en dicha salida
 *
 * Esta funcion genera un pulso ACK y otro de STOP como respuesta a algun pedido
 * de la ICI, para indicar que esa es la ultima palabra de la trama.
 * Responde a requerimientos temporales estrictos y definidos en los trabajos de
 * desarrollo de la AR-ICI.
 *
 * @version 1.0
 * @date 05/2015
 * @author Campos
 *
 */
void ACK_STOP(){
	resetear_STOP();
	retardo_250ns();
	retardo_250ns();
	resetear_ACK();
	retardo_250ns();
	retardo_250ns();
	retardo_250ns();
	retardo_250ns();
	setear_ACK();
	retardo_250ns();
	setear_STOP();
}
//!@}  Fin del grupo de respuestas_enlace



/*!
 * @ingroup capa_enlace
 * @defgroup external_functions
 * Posee las funciones para generar EF a la ICI.
 *
 * @{
 */

/*!
 * @brief Genera un pulso EFD en dicha salida
 *
 * Esta funcion genera un pulso EFD para generar un pedido a la ICI.
 * Responde a requerimientos temporales estrictos y definidos en los trabajos de
 * desarrollo de la AR-ICI.
 *
 * @version 1.0
 * @date 05/2015
 * @author Campos
 *
 */
void EFD(){
	resetear_EFD();
	retardo_250ns();
	retardo_250ns();
	retardo_250ns();
	retardo_250ns();
	setear_EFD();
}


/*!
 * @brief Genera en pedido de ENABLE al DCL-INPUT
 *
 * Esta funcion genera escribe el puerto de entrada de la ICI y genera el pulso
 * EFD correspondiente, para realizar el pedido a la ICI.
 * Responde a requerimientos temporales estrictos y definidos en los trabajos de
 * desarrollo de la AR-ICI.
 *
 * @version 1.0
 * @date 05/2015
 * @author Campos
 *
 */
void EF_ENABLE_DCL_INPUT(){
	escribir_puerto_U(~0x1AFFFF);
	retardo_250ns();
	EFD();
	retardo_250ns();
	escribir_puerto_U(~0xFFFFFF);
}


/*!
 * @brief Genera en pedido de ENABLE al DCL-OUTPUT
 *
 * Esta funcion genera escribe el puerto de entrada de la ICI y genera el pulso
 * EFD correspondiente, para realizar el pedido a la ICI.
 * Responde a requerimientos temporales estrictos y definidos en los trabajos de
 * desarrollo de la AR-ICI.
 *
 * @version 1.0
 * @date 05/2015
 * @author Campos
 *
 */
void EF_ENABLE_DCL_OUTPUT(){
	escribir_puerto_U(~0x1EFFFF);
	retardo_250ns();
	EFD();
	retardo_250ns();
	escribir_puerto_U(~0xFFFFFF);
}


/*!
 * @brief Genera en pedido de DISABLE al DCL-INPUT
 *
 * Esta funcion genera escribe el puerto de entrada de la ICI y genera el pulso
 * EFD correspondiente, para realizar el pedido a la ICI.
 * Responde a requerimientos temporales estrictos y definidos en los trabajos de
 * desarrollo de la AR-ICI.
 *
 * @version 1.0
 * @date 05/2015
 * @author Campos
 *
 */
void EF_DISABLE_DCL_INPUT(){
	escribir_puerto_U(~0x1BFFFF);
	retardo_250ns();
	EFD();
	retardo_250ns();
	escribir_puerto_U(~0xFFFFFF);
}


/*!
 * @brief Genera en pedido de DISABLE al DCL-OUTPUT
 *
 * Esta funcion genera escribe el puerto de entrada de la ICI y genera el pulso
 * EFD correspondiente, para realizar el pedido a la ICI.
 * Responde a requerimientos temporales estrictos y definidos en los trabajos de
 * desarrollo de la AR-ICI.
 *
 * @version 1.0
 * @date 05/2015
 * @author Campos
 *
 */
void EF_DISABLE_DCL_OUTPUT(){
	escribir_puerto_U(~0x1FFFFF);
	retardo_250ns();
	EFD();
	retardo_250ns();
	escribir_puerto_U(~0xFFFFFF);
}

/*!
 * @brief Genera en pedido de REQUERIMIENTO DE ESTADO al DCL-INPUT
 *
 * Esta funcion genera escribe el puerto de entrada de la ICI y genera el pulso
 * EFD correspondiente, para realizar el pedido a la ICI.
 * Responde a requerimientos temporales estrictos y definidos en los trabajos de
 * desarrollo de la AR-ICI.
 *
 * @version 1.0
 * @date 05/2015
 * @author Campos
 *
 */
void EF_STATREQ_DCL_INPUT(){
	escribir_puerto_U(~0x19FFFF);
	retardo_250ns();
	EFD();
	retardo_250ns();
	escribir_puerto_U(~0xFFFFFF);
}


/*!
 * @brief Genera en pedido de REQUERIMIENTO DE ESTADO al DCL-OUTPUT
 *
 * Esta funcion genera escribe el puerto de entrada de la ICI y genera el pulso
 * EFD correspondiente, para realizar el pedido a la ICI.
 * Responde a requerimientos temporales estrictos y definidos en los trabajos de
 * desarrollo de la AR-ICI.
 *
 * @version 1.0
 * @date 05/2015
 * @author Campos
 *
 */
void EF_STATREQ_DCL_OUTPUT(){
	escribir_puerto_U(~0x1DFFFF);
	retardo_250ns();
	EFD();
	retardo_250ns();
	escribir_puerto_U(~0xFFFFFF);
}


/*!
 * @brief Genera en pedido EF INPUT para ingresar una palabra desde la ICI
 *
 * Esta funcion genera escribe el puerto de entrada de la ICI y genera el pulso
 * EFD correspondiente, para realizar el pedido a la ICI.
 * Responde a requerimientos temporales estrictos y definidos en los trabajos de
 * desarrollo de la AR-ICI.
 *
 * @version 1.0
 * @date 05/2015
 * @author Campos
 *
 */
void EF_INPUT(){
	escribir_puerto_U(~0x18FFFF);
	retardo_250ns();
	EFD();
	retardo_250ns();
	escribir_puerto_U(~0xFFFFFF);
}


/*!
 * @brief Genera en pedido EF OUTPUT para sacar una palabra a traves de la ICI
 *
 * Esta funcion genera escribe el puerto de entrada de la ICI y genera el pulso
 * EFD correspondiente, para realizar el pedido a la ICI.
 * Responde a requerimientos temporales estrictos y definidos en los trabajos de
 * desarrollo de la AR-ICI.
 *
 * @version 1.0
 * @date 05/2015
 * @author Campos
 *
 */
void EF_OUTPUT(){
	escribir_puerto_U(~0x1CFFFF);
	retardo_250ns();
	EFD();
	retardo_250ns();
	escribir_puerto_U(~0xFFFFFF);
}

/*!
 * @brief Resetear la ICI
 * @param Nada
 * @return Nada
 * @version 1.0
 * @date 07/2015
 * @author Campos
 */

void resetear_ICI(){
	resetear_RESD();  // Reseteo la ICI
	retardo_us(10000);
	setear_RESD();
}

//! @}


/*!
 * @ingroup capa_enlace
 * @defgroup interrupciones Interrupciones en pines de entrada
 * Rutinas de interrupcion de los pines RQ, EIP, IDRP y ODRP.
 * Este modulo posee las cuatro rutinas de interrupcion de estos pines.
 *
 * En dichas rutinas se realizan las acciones necesarias para responder a los flancos
 * de bajada de estas señales que genera la ICI
 *
 * @{
 *
 */

/*!
 * @brief Respuesta al pin RQ
 *
 * Cuando se genera un flanco de bajada en el pin RQ, actualiza la variable RQ de
 * la capa fisica y, adicionalmente, se espera 1 us y se
 * baja el pin EN. Esta funcion hace las veces del selector de prioridades de la MU.
 *
 * @version 1.0
 * @date 05/2015
 * @author Campos
 */
void EXTI0_IRQHandler(void){ // Pin RQ

	RQ = leer_RQ();

	if(RQ == RESET){   // Se requiere al PS
		retardo_1us();
		resetear_EN();
	}
	EXTI_ClearITPendingBit(EXTI_Line0);
	EXTI_ClearFlag(EXTI_Line0);
}

/*!
 * @brief Respuesta al pin IDRP
 *
 * Cuando se genera un flanco de bajada en el pin IDRP, actualiza la variable IDRP de
 * la capa fisica y, adicionalmente se llevan a cabo las siguientes acciones:
 *
 * - Leer el puerto de direcciones
 * - Comprobar que la direccion es la correcta y que el DCL es el de entrada
 * - Setear la variable 'llegoPalabra'
 * - Lee el puerto I y lo guarda en 'palabraIN'
 * - Disminuye el contador 'cont_in', que lleva la cuenta de palabras de entrada.
 * - Si el 'cont_in' llego a cero, se genera un ACK+STOP, caso contrario, solo se genera ACK.
 * - Exactamente 250 ns despues del ACK o ACK+STOP, se levanta el pin EN, emulando así el hecho
 * que el ACK afecte al selector de prioridades de la MU.
 *
 *
 * @version 1.0
 * @date 05/2015
 * @author Campos
 */
void EXTI1_IRQHandler(void){  // Pin IDRP
	uint8_t DIR;
	//char cadena[40];

	if(leer_RQ() == SET){   // Si no hubo requerimiento -> Glitch
		EXTI_ClearITPendingBit(EXTI_Line1);
		EXTI_ClearFlag(EXTI_Line1);
		return;
	}

	FlagStatus IDRP_ant;

	IDRP_ant = leer_IDRP();
	retardo_50ns();
	IDRP = leer_IDRP();
	if(IDRP != IDRP_ant) return;

	if(IDRP == RESET){  // Se produjo una interrupcion

		DIR = leer_puertoDA();

		if(DIR == 6){
			retardo_1us();
			buffer_IN[total_IN] = leer_puerto_I();
			retardo_250ns();
			total_IN++;
			cont_IN--;
			if(cont_IN == 0){
				ACK_STOP(); // es la ultima palabra
				fin_bloque_IN = SET;
				total_IN=0; // reseteo el registro para que no me lea cualquier cosa
			}
			else ACK();
		}
		else{
			retardo_1us();
			ACK();
		}

		 retardo_250ns();
		 setear_EN();
	}

	EXTI_ClearITPendingBit(EXTI_Line1);
	EXTI_ClearFlag(EXTI_Line1);
	return;
}

/*!
 * @brief Respuesta al pin EIP
 *
 * Cuando se genera un flanco de bajada en el pin EIP, actualiza la variable EIP de
 * la capa fisica y, adicionalmente se llevan a cabo las siguientes acciones:
 *
 * -- Leer el puerto de direcciones
 *
 * -- Comprobar que la direccion es la correcta. En este punto, se pueden tomar dos caminos:
 *
 * -- DCL es el de entrada:
 *  - Setear la variable 'llegoEstadoIN'
 *  - Lee el puerto I y lo guarda en 'estadoIN'
 *  - Mostrar en consola el estado
 *  - Generar el ACK
 *
 * -- DCL es el de salida:
 *  - Setear la variable 'llegoEstadoOUT'
 *  - Lee el puerto I y lo guarda en 'estadoOUT'
 *  - Mostrar en consola el estado
 *  - Generar el ACK
 *
 *  -- Exactamente 250 ns despues del ACK o ACK+STOP, se levanta el pin EN, emulando así el hecho
 * que el ACK afecte al selector de prioridades de la MU.
 *
 *
 * @version 1.0
 * @date 05/2015
 * @author Campos
 */
void EXTI2_IRQHandler(void){  // Pin EIP
	uint8_t DIR;
	char cadena[40];
	FlagStatus EIP_ant;

	if(leer_RQ() == SET){  // Si no hubo requerimiento -> Glitch
		EXTI_ClearITPendingBit(EXTI_Line2);
		EXTI_ClearFlag(EXTI_Line2);
		return;
	}

	EIP_ant = leer_EIP();
	retardo_250ns();
	EIP = leer_EIP();
	if(EIP != EIP_ant) return;

	if(EIP == RESET){  // Se produjo una interrupcion

		DIR = leer_puertoDA();

		if(DIR == 6){
			retardo_1us();
			llegoEstadoIN = SET;
			estadoIN = leer_puerto_I();
			if(modo_service == SET){
				sprintf(cadena, "\n\nEI INPUT: %X",(int)estadoIN);
				VCP_send_str(cadena);
			}
			ACK();
		}
		else if(DIR == 7){
			retardo_1us();
			llegoEstadoOUT = SET;
			estadoOUT = leer_puerto_I();
			if(modo_service == SET){
				sprintf(cadena, "\n\nEI OUTPUT: %X",(int)estadoOUT);
				VCP_send_str(cadena);
			}
			ACK();
		}
		else{
			llegoEstadoIN = RESET;
			llegoEstadoOUT = RESET;
			retardo_1us();
			ACK();
		}

		EXTI_ClearITPendingBit(EXTI_Line2);
		EXTI_ClearFlag(EXTI_Line2);
		retardo_250ns();
		setear_EN();
	}
	EXTI_ClearITPendingBit(EXTI_Line2);
	EXTI_ClearFlag(EXTI_Line2);
	return;
}


/*!
 * @brief Respuesta al pin ODRP
 *
 * Cuando se genera un flanco de bajada en el pin ODRP, actualiza la variable ODRP de
 * la capa fisica y, adicionalmente se llevan a cabo las siguientes acciones:
 *
 * - Leer el puerto de direcciones
 * - Comprobar que la direccion es la correcta y que el DCL es el de salida
 * - Setear la variable 'salioPalabra'
 * - Escribe el valor de 'palabraOUT' en el puerto U
 * - Disminuye el contador 'cont_out', que lleva la cuenta de palabras de salida.
 * - Si el 'cont_out' llego a cero, se genera un ACK+STOP, caso contrario, solo se genera ACK.
 * - Exactamente 250 ns despues del ACK o ACK+STOP, se levanta el pin EN, emulando así el hecho
 * que el ACK afecte al selector de prioridades de la MU.
 *
 *
 * @version 1.0
 * @date 05/2015
 * @author Campos
 */
void EXTI3_IRQHandler(void){ // Pin ODRP
	uint8_t DIR;
	FlagStatus ODRP_ant;
	//char cadena[40];

	if(leer_RQ() == SET){ // Si no hubo requerimiento -> Glitch
		EXTI_ClearITPendingBit(EXTI_Line3);
		EXTI_ClearFlag(EXTI_Line3);
		return;
	}

	ODRP_ant = leer_ODRP();
	retardo_50ns();
	ODRP = leer_ODRP();
	if(ODRP != ODRP_ant) return;

	if(ODRP == RESET){  // Se produjo una interrupcion

		DIR = leer_puertoDA();

		if(DIR == 7){
			//VCP_send_str("\t DLC OUTPUT");
			retardo_1us();
			escribir_puerto_U(buffer_OUT[total_OUT]);
			retardo_1us();
			total_OUT++;
			cont_OUT--;
			if(cont_OUT == 0){
				ACK_STOP(); // es la ultima palabra
				fin_bloque_OUT = SET;
				total_OUT= 0;//reseteo el registro para que no me escriba cualquier cosa (error que se encontro)
			}
			else
				ACK();
		}
		else{
			retardo_1us();
			ACK();
		}

		retardo_250ns();
		setear_EN();
	}
	EXTI_ClearITPendingBit(EXTI_Line3);
	EXTI_ClearFlag(EXTI_Line3);
}

//!@} Fin del grupo interrupciones


