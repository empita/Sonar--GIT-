#ifndef _APP_SONAR
#define _APP_SONAR

#include "main.h"
#include "timers_app_sonar.h"
#include "CAPA_ENLACE.h"

typedef enum {	OUTPUT_OK = 0x000000,
				OUTPUT_NO_ESTADO = 0xFFFFFF,
				OUTPUT_ERROR_TRANSP = 0x400000,
				OUTPUT_HW_NO_LISTO = 0x800000,
				OUTPUT_SW_NO_LISTO = 0x200000,
				OUTPUT_TIME_OUT = -1
			} DCL_output_estado_t;

typedef enum {	INPUT_OK = 0x000000,
				INPUT_NO_ESTADO = 0xFFFFFF,
				INPUT_ERROR_TRANSP = 0x400000,
				INPUT_HW_NO_LISTO = 0x800000,
				INPUT_SW_NO_LISTO = 0x200000,
				INPUT_TIME_OUT = -1
			} DCL_input_estado_t;

// VARIABLES FIJAS QUE INDICAN EL TAMAÑO DEL BUFFER DE ENTRADA Y DE SALIDA
uint8_t CANT_PALABRAS_ENTRADA;
uint8_t CANT_PALABRAS_SALIDA;

FlagStatus error_Transporte;
FlagStatus error_Timeout;
FlagStatus error_Enc_DHC;
FlagStatus error_Enc_Sonar;
FlagStatus error_PC;

uint8_t cant_error_Transporte;
uint8_t cant_error_Timeout;
uint8_t cant_error_Enc_DHC;
uint8_t cant_error_Enc_Sonar;
uint8_t cant_error_PC;

void app_sonar_init();

DCL_input_estado_t leerDatosICI();
DCL_output_estado_t escribirDatosICI();

DCL_input_estado_t verificarEstadoInput();
DCL_output_estado_t verificarEstadoOutput();

void protocoloEmergenciaInput();
void protocoloEmergenciaOutput();

void protocoloErrorTransporteInput();
void protocoloErrorTransporteOutput();

void verificarSysOnLocal();
void verificarSysOnDHC();
#endif
