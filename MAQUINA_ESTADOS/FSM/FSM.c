#include "FSM.h"

void FSM_Init(){

	configurar_timeOut_FSM_500(500);
	estadoFSM = ESTADO_INICIAL;
}

void scheduler(){

	DCL_input_estado_t estadoInput;
	DCL_output_estado_t estadoOutput;
	error_PC_read_t errorPC;

	if(estadoFSM == ESTADO_INICIAL){
		estadoFSM = VERIFICANDO_SYSON_LOCAL;
		return;;
	}

	else if(estadoFSM == VERIFICANDO_SYSON_LOCAL){

		verificarSysOnLocal();				// comprueba si el System on del sonar esta activo
		estadoFSM = VERIFICANDO_SYSON_DHC;
		return;;
	}

	else if(estadoFSM == VERIFICANDO_SYSON_DHC){

		verificarSysOnDHC();				//comprueba si esta activo el System ON de Daisy/Easy
		estadoFSM = VERIFICANDO_ESTADO_INPUT;
		return;;
	}

	else if(estadoFSM == VERIFICANDO_ESTADO_INPUT){

		if(verificarEstadoInput() != INPUT_OK)
			estadoFSM = VERIFICANDO_ESTADO_INPUT;

		else{
			estadoFSM = VERIFICANDO_ESTADO_OUTPUT;
			EF_DISABLE_DCL_INPUT();

		}

		return;;
	}

	else if(estadoFSM == VERIFICANDO_ESTADO_OUTPUT){

		if(verificarEstadoOutput() != OUTPUT_OK)
			estadoFSM = VERIFICANDO_ESTADO_OUTPUT;

		else{
			estadoFSM = ESCRIBIENDO_ICI;
			EF_DISABLE_DCL_OUTPUT();
			iniciar_timeOut_FSM_500();
		}

		return;;
	}

	else if(estadoFSM == ESCRIBIENDO_ICI){

		estadoOutput = escribirDatosICI();

		if(estadoOutput == OUTPUT_HW_NO_LISTO || estadoOutput == OUTPUT_SW_NO_LISTO)
			estadoFSM = EMERGENCIA_OUTPUT;
		else if(estadoOutput == OUTPUT_TIME_OUT || estadoOutput == OUTPUT_NO_ESTADO)
		{
			 resetear_ICI();
			 estadoFSM = VERIFICANDO_SYSON_DHC;
		}
		else if (estadoOutput == OUTPUT_ERROR_TRANSP)
			estadoFSM = ERROR_TRANSPORTE_OUTPUT;
		else if (estadoOutput == OUTPUT_OK)
			{estadoFSM = LEYENDO_ICI;//LEYENDO_PC;
			//GPIO_ToggleBits(GPIOD, GPIO_Pin_0);
			}
		else
			estadoFSM = ESCRIBIENDO_ICI;

		return;
	}

	else if(estadoFSM == LEYENDO_PC){

		/*errorPC = leerDatosPC(mensajePCInput, 64);

		if(errorPC != PC_READ_OK){
			//estadoFSM = EMERGENCIA_PC_INPUT;
			estadoFSM = LEYENDO_PC;
			return;
		}
		else{
			traduccion_PC_a_ICI();
			estadoFSM = LEYENDO_ICI;
			return;
		}*/
		estadoFSM = LEYENDO_ICI;		//para probar la comunicacion sin intervencion de PC. Sacar
		return;
	}

	else if(estadoFSM == LEYENDO_ICI){

		estadoInput = leerDatosICI();

		if(estadoInput == INPUT_HW_NO_LISTO)
			estadoFSM = EMERGENCIA_INPUT;
		else if(estadoInput == INPUT_ERROR_TRANSP)
			estadoFSM = ERROR_TRANSPORTE_INPUT;
		else if(estadoInput == INPUT_NO_ESTADO || estadoInput == INPUT_TIME_OUT)
		{
			resetear_ICI();
			estadoFSM = VERIFICANDO_SYSON_DHC;
		}
		else if(estadoInput == INPUT_OK)
			{estadoFSM = ESPERANDO_TIMER_500;//ESCRIBIENDO_PC;
			//GPIO_ToggleBits(GPIOD, GPIO_Pin_2);
			}
		else
			estadoFSM = LEYENDO_ICI;

		return;
	}

	else if(estadoFSM == ESCRIBIENDO_PC){

		//escribirDatosPC(mensajePCOutput,70);
		estadoFSM = ESPERANDO_TIMER_500;
		return;
	}

	else if(estadoFSM == ESPERANDO_TIMER_500){

		// SI SE VENCIÓ EL TIMER -> ESTAMOS PASADOS ERROR GRAVE
		if(timeout_FSM_500 == SET){
			//indicar que no se cumplio con los requerimientos temporales
			estadoFSM = ESPERANDO_TIMER_500;
		}

		while(timeout_FSM_500 == RESET);
		timeout_FSM_500 = RESET;
		estadoFSM = ESCRIBIENDO_ICI;
		reiniciar_timeOut_FSM_500();
		//GPIO_ToggleBits(GPIOD, GPIO_Pin_15);		//escribe sobre el puerto I8 de la ICI
		return;
	}

	else if(estadoFSM == EMERGENCIA_INPUT){

		protocoloEmergenciaInput();
		estadoFSM = ESCRIBIENDO_ICI;
		return;
	}

	else if(estadoFSM == EMERGENCIA_OUTPUT){

		protocoloEmergenciaOutput();
		estadoFSM = ESCRIBIENDO_ICI;
		return;
	}

	else if(estadoFSM == ERROR_TRANSPORTE_INPUT){

		protocoloErrorTransporteInput();
		estadoFSM = ESCRIBIENDO_PC;
		return;
	}

	else if(estadoFSM == ERROR_TRANSPORTE_OUTPUT){

		protocoloErrorTransporteOutput();
		estadoFSM = LEYENDO_PC;
		return;
	}

	else if(estadoFSM == EMERGENCIA_PC_INPUT){

		protocoloEmergenciaPCInput();
		estadoFSM = ESCRIBIENDO_ICI;
		return;
	}

	return;

}
