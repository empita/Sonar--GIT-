/* Versionado */
#ifndef _CONSOLA
#define _CONSOLA

#include "consola_PHY.h"
#include "timers_PC.h"
#include <stdlib.h>
#include <string.h>

typedef enum { 	PC_READ_OK,
				PC_READ_TIMEOUT,
				PC_READ_ERROR_ENC,
				PC_READ_ERROR_TERM,
				PC_READ_ERROR_LEN
			} error_PC_read_t;

void inicializar_consola();

void bloquear_consola();
void consolaOn();

void vaciar_recepcion();

void VCP_send_str(char* cadena);

error_PC_read_t leerDatosPC(char* buffer, int len);
void escribirDatosPC(char* buffer, int len);

void protocoloEmergenciaPCInput();


#endif
