#ifndef CAPAFISICA_H
#define  CAPAFISICA_H

/*!
 *  \mainpage ICI-COM - Capa Fisica
 *
 *  Esta libreria permite configurar y controlar el estado de los pines a bajo nivel.
 *  La comunicacion con la ICI requiere forzar el estado de ciertas salidas, asi como
 *  leer el estado de ciertas entradas. Por otro lado, los dos buses de la ICI son
 *  administrados mediante esta libreria. La lectura de las entradas se realiza mediante
 *  interrupciones, en la capa de enlace del sistema.
 *
 *  El control de las siguientes salidas se realiza en assembler para aumentar la velocidad
 *  de respuesta:
 *  	- ACK
 *  	- EFD
 *  	- EN
 *  	- RESD
 *  	- STOP
 *
 *  Las siguientes entradas se leen por interupciones, en la capa de enlace:
 *  	- EIP
 *  	- RQ
 *  	- ODRP
 *  	- IDRP
 *
 *  El bus de entrada es el bus I y el bus de salida es el bus U.
 *
 *  Esta libreria esta formada por tres modulos:
 *  	- configuracion_IO: configura todos los pines del micro
 *  	- control_IO: controla todos los pines del micro
 *  	- test: set de tests para verificar el funcionamiento del HW
 *
 *  Para utilizar esta libreria en un sistema, realizar un #include "CAPA_FISICA.h" en
 *  el archivo en que se desea implementar la capa fisica.
 */

#include "control_IO.h"
#include "configuracion_IO.h"


#endif
