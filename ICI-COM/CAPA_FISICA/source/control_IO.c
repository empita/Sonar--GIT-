
/*!
 * @file control_IO.c
 * @brief Este archivo posee todas las funciones para el control a bajo nivel de los pines I/O.
 *
 * Se disponen de funciones para setear o resetear los pines de salida, para leer las entradas,
 * para escribir el puerto U, para leer los puertos I y DA y para habilitar y deshabilitar el CLK.
 *
 * @date 05/2015
 * @version 1.0
 * @author Campos
 */

/*!
 * @defgroup control Control I/O
 * @brief Este archivo posee todas las funciones para el control a bajo nivel de los pines I/O.
 *
 * Se disponen de funciones para setear o resetear los pines de salida, para leer las entradas,
 * para escribir el puerto U, para leer los puertos I y DA y para habilitar y deshabilitar el CLK.
 *
 * @date 05/2015
 * @version 1.0
 * @author Campos
 */

/*!
 * @ingroup control
 * @defgroup control_puertos Puertos I/O
 * @brief Escritura en puerto U o lectura de los puertos I y DA
 */

/*!
 * @ingroup control
 * @defgroup control_entradas Pines de entrada
 * @brief Lectura de los pines de entrada por pooling.
 *
 * Las variables EIP, RQ, IDRP, ODRP se actualizan en forma automatica
 * mediante interrupcion en la capa de enlace. Las funciones de esta
 * libreria se utilizan como auxiliares.
 */

/*!
 * @ingroup control
 * @defgroup control_salidas Pines de salida
 * @brief Escritura en los pines de salida.
 */

/*!
 * @ingroup control
 * @defgroup control_reloj Clock
 * @brief Habilitacion o deshabilitacion del clock de la ICI
 */

#include "control_IO.h"

/*! @ingroup control_reloj
 * @{
 */

/*!
 * @brief Habilita el reloj de la ICI
 * @version 1.0
 * @date 05/2015
 * @author Campos
 */
void habilitar_reloj(){
	TIM_Cmd(TIM2, ENABLE);
}

/*!
 * @brief Desabilita el reloj de la ICI
 * @version 1.0
 * @date 05/2015
 * @author Campos
 */
void deshabilitar_reloj(){
	TIM_Cmd(TIM2, DISABLE);
}
//! @}




/*! @ingroup control_entradas
 *  @{
 */

/*!
 * @brief Devuelve el estado del pin RQ (PA0)
 * @param Nada
 * @return FlagStatus (SET o RESET) Estado del pin
 *
 * @version 1.0
 * @date 05/2015
 * @author Campos
 */
extern FlagStatus leer_RQ(){
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == 0) return RESET;
	else return SET;
}

/*!
 * @brief Devuelve el estado del pin EIP (PA2)
 * @param Nada
 * @return FlagStatus (SET o RESET) Estado del pin
 *
 * @version 1.0
 * @date 05/2015
 * @author Campos
 */extern FlagStatus leer_EIP(){
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2) == 0) return RESET;
	else return SET;
}

 /*!
  * @brief Devuelve el estado del pin IDRP (PD1)
  * @param Nada
  * @return FlagStatus (SET o RESET) Estado del pin
  *
  * @version 1.0
  * @date 05/2015
  * @author Campos
  */
 extern FlagStatus leer_IDRP(){
	if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_1) == 0) return RESET;
	else return SET;
}


 /*!
  * @brief Devuelve el estado del pin ODRP (PD3)
  * @param Nada
  * @return FlagStatus (SET o RESET) Estado del pin
  *
  * @version 1.0
  * @date 05/2015
  * @author Campos
  */extern FlagStatus leer_ODRP(){
	if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3) == 0) return RESET;
	else return SET;
}
//! @}


/*! @ingroup control_puertos
 * @{
 */

/*!
 * @brief Lectura del puerto I
 *
 * Esta funcion lee el puerto completo de entrada y devuelve en valor como
 * un número de 24 bits alineado a la derecha sobre la variable de 32 bits (uint32_t).
 *
 * Estructura de los puertos:
 *
 *  El puerto 	D8 al D15 corresponde a
 *
 *				I1 al I8
 *
 *
 *  El puerto 	PE0 al PE15 corresponden a
 *
 * 				I9  al I24
 *
 *
 *  En total la estructura queda así:
 *
 *  	 I24 -------- I9 - I8 ----------- I1
 *
 *		 PE15 ------ PE0 - PD15 -------- PD8
 *
 * @param Nada
 * @return uint32_t Valor del puerto I
 * @version 1.0
 * @date 05/2015
 * @author Campos
 */
extern uint32_t leer_puerto_I(){

    uint16_t parteAltaPD;
    uint16_t todoPE;
    uint32_t valorEntrada;
    // El puerto 	D8 al D15 corresponde a
    //				I1 al I8

    // El puerto 	PE0 al PE15 corresponden a
    //				I9  al I24

    // En total quedaría así:
    //   I24 -------- I9 - I8 ----------- I1
    //   PE15 ------ PE0 - PD15 -------- PD8

    // Primero leo los bits más altos del puerto D
    parteAltaPD = GPIO_ReadInputData(GPIOD);
    parteAltaPD = parteAltaPD & 0xFF00; // Me quedo con la parte alta
    parteAltaPD = parteAltaPD >> 8; // Desplazo todo ocho lugares a la derecha

    // Desp leo todo el puerto E
    todoPE = GPIO_ReadInputData(GPIOE);

    // Coloco todoPE dentro de valorEntrada y lo desplazo 8 lugares a la izquierda
    valorEntrada = todoPE;
    valorEntrada = valorEntrada << 8;

    // Por ultimo se hace la OR entre parteAltaPD y valorEntrada
    valorEntrada = valorEntrada | parteAltaPD;

    //valorEntrada = (~valorEntrada) & 0x00FFFF;
    //Niego el valor de entrada.
    return negar_24bits(valorEntrada); //Retorna el valor del puerto I;
}

/*!
 * @brief Lectura del puerto DA
 *
 * Esta funcion lee el puerto DA y devuelve el valor como
 * un número de 6 bits alineado a la derecha sobre la variable de 8 bits (uint8_t).
 *
 * Estructura de los puertos:
 *
 *  El puerto 	DA6 - DA5 - DA4 - DA3 - DA2 - DA1 corresponde a
 *
 *				A8  -  A7 -  A6 - C15 - C14 -  A3
 *
 *
 * @param Nada
 * @return uint8_t Valor del puerto DA
 * @version 1.0
 * @date 05/2015
 * @author Campos
 */
extern uint8_t leer_puertoDA(){

	// La distribución de pines es:
	// DA6  DA5  DA4  DA3  DA2  DA1
	// PA8  PA7  PA6  PC15 PC14 PA3

	uint8_t puertoDA = 0;

	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)) puertoDA = puertoDA | 1;
	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_14)) puertoDA = puertoDA | 2;
	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_15)) puertoDA = puertoDA | 4;
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)) puertoDA = puertoDA | 8;
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)) puertoDA = puertoDA | 16;
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)) puertoDA = puertoDA | 32;

	return puertoDA;
}



/*!
 * @brief Escribe en el puerto U
 *
 * Escribe el valor de 24 bits pasado como parámetro en el puerto U de la ICI
 * Los 24 bits deben estar alineados a la derecha del parametro (uint32_t).
 *
 *
 * Estructura de los puertos:
 *
 *  El puerto 	B0 al B15 corresponde a
 *
 *				U1 al U16
 *
 *
 *  El puerto 	PC0 al PC7 corresponden a
 *
 * 				U17  al U24
 *
 *
 *  En total la estructura queda así:
 *
 *  	 U24 -------- U17 - U16 ----------- U1
 *
 *		 PC7 -------- PC0 - PB15 --------- PB0
 *
 * @param uint32_t Valor del puerto U que se desea escribir
 * @return Nada
 * @version 1.0
 * @date 05/2015
 * @author Campos
 */

extern void escribir_puerto_U(uint32_t valor){

	uint16_t puertoPB=0;
	uint16_t puertoPC=0;
	uint16_t aux=0;

	// La conformación del puerto completo de salida es como sigue:
	// U1	hasta	U16
	// PB0  hasta 	PB15

	// U17	hasta	U24
	// PC0 	hasta 	PC7

	// En definitiva:
	// U24 ------ U17 - U16 ------------ U1
	// PC7 ------ PC0 - PB15 ----------- PB0

	//Niego el valor de salida
	valor = negar_24bits(valor);
	//valor = ~valor;
	//valor = valor & 0x00FFFFFF;

	puertoPB = valor & 0x0000FFFF; // Me quedo con la parte baja de "valor"
	GPIO_Write(GPIOB,puertoPB); // Esto va derecho al puerto B

	// Luego me tengo que quedar con el byte más alto de valor
	valor = valor & 0x00FF0000;
	valor = valor >> 16; // Desp lo desplazo hasta alinearlo a la derecha
	aux = GPIO_ReadInputData(GPIOC); // Leo el puerto C para no sobreescribir otros pines
	puertoPC = aux & 0xFF00; // Borro el primer byte de lo leido
	puertoPC = puertoPC | valor; // Hago OR con el valor desplazado
	GPIO_Write(GPIOC,puertoPC);
	return;
}
//! @}

/*!
 * @ingroup control_salidas
 * @brief Coloca todas las salidas en '1' (estado Idle del bus)
 * @version 1.0
 * @date 05/2015
 * @author Campos
 */
void poner_salidas_alto(){
	setear_ACK();
	setear_STOP();
	setear_RESD();
	setear_EN();
	setear_EFD();
}


/*!
 * @brief Negar a nivel de bits - palabra de 24 bits
 * @param uint32_t Palabra a negar
 * @return uint32_t Palabra negada
 * @version 1.0
 * @date 07/2015
 * @author Corinaldesi
 */

uint32_t negar_24bits(uint32_t palabra){
	return (~palabra) & 0xFFFFFF;
}



