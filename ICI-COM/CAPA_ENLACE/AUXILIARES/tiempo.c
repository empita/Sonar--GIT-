/*!
 * @file tiempo.c
 * @brief Archivo auxiliar de la capa de enlace
 * Posee funciones para cumplir los requerimientos temporales de la capa
 */

/*!
 * @ingroup capa_enlace
 * @defgroup tiempo_enlace Libreria de retardos
 * Este modulo posee funciones con retardos de presicion para cumplir los requerimientos
 * temporales de las diversas funciones de la capa de enlace. Las mismas están escritas en
 * assembler, para lograr retardos de exactitud
 *
 * @{
 */

#include "tiempo.h"

/*!
 * @brief Retardo de 250 ns
 * Corresponde a un ciclo de reloj de la MU original
 *
 * @version 1.0
 * @date 05/2015
 * @author Campos
 */
void retardo_250ns(){
	__asm("nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			);
}

/*!
 * @brief Retardo de 1 us
 *
 * @version 1.0
 * @date 05/2015
 * @author Campos
 */
void retardo_1us(){
	retardo_250ns();
	retardo_250ns();
	retardo_250ns();
	retardo_250ns();
}

/*!
 * @brief Retardo de 50 ns
 * Para minimizar los glitches en las entradas de Handshaking del puerto paralelo
 *
 * @version 1.0
 * @date 06/2015
 * @author Campos
 */
void retardo_50ns(){
	__asm("nop\n"
			"nop\n"
			);
}

/*!
 * @brief Retardo de "x" us
 *
 * @version 1.0
 * @date 07/2015
 * @author Corinaldesi
 */
void retardo_us(uint16_t x){
	uint16_t i;

	for (i=0; i<x; i++)
		retardo_1us();
}

