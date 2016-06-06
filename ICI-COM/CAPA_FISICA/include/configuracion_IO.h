#ifndef IOCONF_H
#define IOCONF_H

#include "main.h"


extern void encender_puertos(); // Funciona

// PUERTOS
extern void configurar_puerto_U();
extern void configurar_puerto_I();
extern void configurar_DA();

////////// PINES INDIVIDUALES
// Funcionan todas
// ENTRADAS
extern void configurarRQ();
extern void configurarRQ_INT();  // configura la interrupcion en ese pin

extern void configurarEIP();
extern void configurarEIP_INT();  // configura la interrupcion en ese pin

extern void configurarIDRP();
extern void configurarIDRP_INT();  // configura la interrupcion en ese pin

extern void configurarODRP();
extern void configurarODRP_INT();  // configura la interrupcion en ese pin

// SALIDAS  -- FUNCIONAN OK
extern void configurarRESD();
extern void configurarEFD();
extern void configurarEN();
extern void configurarSTOP();
extern void configurarACK();

// CLOCK DEL SISTEMA. IMPLICA CONFIGURAR UN TIMER PARA GENERARLO
//  Falta comprobar la frecuencia
extern void configurarCLOCK();

extern void CAPA_FISICA_Init();

#endif
