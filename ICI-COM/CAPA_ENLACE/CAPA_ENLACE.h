#ifndef SALIDAS_H
#define SALIDAS_H

#include "main.h"
#include "tiempo.h"
#include <stdio.h>
#include "CAPA_FISICA.h"


/*!
 * @ingroup capa_enlace
 * @defgroup variables_enlace Variables Capa de Enlace
 * @{
 */

FlagStatus fin_bloque_IN; 	//!< Determina si ha terminado una transferencia en el input DCL
uint8_t cont_IN; 			//!< Determina la cantidad de palabras a contar con el pulso IDRP
uint8_t total_IN; 			//!< Determina la cantidad efectiva de palabras que ingresaron
uint32_t buffer_IN[64];		//!< Buffer donde se desea guardar lo que ingreso al DCL INPUT

///////////////////////////////

FlagStatus fin_bloque_OUT;	//!< Determina si ya se ha enviado un bloque de palabras al output DCL
uint8_t cont_OUT;			//!< Determina la cantidad de palabras a contar con el pulso ODRP
uint8_t total_OUT;			//!< Determina la cantidad de palabras efectivas que salieron por el DCL INPUT
uint32_t * buffer_OUT;	 	//!< Apunta al area de memoria que se desea sacar por el DCL OUTPUT

///////////////////////////////

FlagStatus llegoEstadoIN;	//!< Determina si llego una interrupcion con un estado de entrada
FlagStatus llegoEstadoOUT; 	//!< Determina si llego una interrupcion con un estado de salida
uint32_t estadoIN;			//!< Guarda el valor del ultimo estado de entrada que llego
uint32_t estadoOUT;			//!< Guarda el valor del ultimo estado de salida que llego

FlagStatus modo_service;
///////////////////////////////
//!@}


void CAPA_ENLACE_Init();

void ACK();
void ACK_STOP();
void EFD();
void EF_ENABLE_DCL_INPUT();
void EF_ENABLE_DCL_OUTPUT();
void EF_DISABLE_DCL_INPUT();
void EF_DISABLE_DCL_OUTPUT();
void EF_STATREQ_DCL_INPUT();
void EF_STATREQ_DCL_OUTPUT();
void EF_INPUT();
void EF_OUTPUT();

void resetear_ICI();

#endif
