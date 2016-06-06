#ifndef CONTROL_IO_H
#define CONTROL_IO_H
/*! @ingroup control_salidas
 * @brief Control de los pines de salida en asm, para mayor velocidad de respuesta
 *  @{
 *  */

// Macros para los pines de salida individual
#define setear_RESD() GPIOD->BSRRL = GPIO_Pin_5 //!< Setea RESD: PA4
#define setear_EFD() GPIOA->BSRRL = GPIO_Pin_5 //!< Setea EFD: PA5
#define setear_EN() GPIOC->BSRRL = GPIO_Pin_9 //!< Setea EN: PC9
#define setear_STOP() GPIOC->BSRRL = GPIO_Pin_8 //!< Setea STOP: PC8
#define setear_ACK() GPIOA->BSRRL = GPIO_Pin_15 //!< Setea ACK: PA15

#define resetear_RESD() GPIOD->BSRRH = GPIO_Pin_5 //!< Resetea RESD: PA4
#define resetear_EFD() GPIOA->BSRRH = GPIO_Pin_5 //!< Resetea EFD: PA5
#define resetear_EN() GPIOC->BSRRH = GPIO_Pin_9 //!< Resetea EN: PC9
#define resetear_STOP() GPIOC->BSRRH = GPIO_Pin_8 //!< Resetea STOP: PC8
#define resetear_ACK() GPIOA->BSRRH = GPIO_Pin_15 //!< Resetea ACK: PA15
/*! @} */

#include "main.h"

/*! @ingroup control
 * @{
 */
FlagStatus EIP; //!< Estado de EIP. Se actualiza por interrupcion en capa de enlace.
FlagStatus RQ; //!< Estado de RQ. Se actualiza por interrupcion en capa de enlace.
FlagStatus IDRP; //!< Estado de IDRP. Se actualiza por interrupcion en capa de enlace.
FlagStatus ODRP;//!< Estado de ODRP. Se actualiza por interrupcion en capa de enlace.
//!@}

extern void habilitar_reloj();
extern void deshabilitar_reloj();

extern FlagStatus leer_RQ();
extern FlagStatus leer_EIP();
extern FlagStatus leer_IDRP();
extern FlagStatus leer_ODRP();

// PUERTO DE ENTRADA
extern uint32_t leer_puerto_I();
extern uint8_t leer_puertoDA(); // Funciona OK

// PUERTO DE SALIDA
extern void escribir_puerto_U(uint32_t valor);


// SALIDAS INDIVIDUALES

void poner_salidas_alto();

//FUNCION AUXILIAR
uint32_t negar_24bits(uint32_t palabra);





#endif

