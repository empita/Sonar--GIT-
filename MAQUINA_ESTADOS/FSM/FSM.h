#ifndef _FSM
#define _FSM

#include "main.h"
#include "temporizado.h"
#include "mensajes.h"
#include "consola.h"
#include "app_sonar.h"
#include "io_lib.h"

// POSIBLES ESTADOS DE LA FSM
typedef enum {	ESTADO_INICIAL,
				VERIFICANDO_SYSON_LOCAL,
				VERIFICANDO_SYSON_DHC,
				VERIFICANDO_ESTADO_INPUT,
				VERIFICANDO_ESTADO_OUTPUT,
				ESCRIBIENDO_ICI,
				LEYENDO_PC,
				LEYENDO_ICI,
				ESCRIBIENDO_PC,
				ESPERANDO_TIMER_500,
				EMERGENCIA_INPUT,
				EMERGENCIA_OUTPUT,
				ERROR_TRANSPORTE_INPUT,
				ERROR_TRANSPORTE_OUTPUT,
				EMERGENCIA_PC_INPUT
				} estado_t;

estado_t estadoFSM;

void FSM_Init();
void scheduler();


#endif
