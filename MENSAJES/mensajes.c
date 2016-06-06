#include "main.h"
#include "mensajes.h"

void inicializarMensajes(){
	int i;

	for(i=0;i<64;i++) mensajePCInput[i] = 0;
	for(i=0;i<70;i++) mensajePCOutput[i] = 0;
	mensajePCOutput[0] = 'D';
	mensajePCOutput[1] = 'A';
	mensajePCOutput[2] = 'T';
	mensajePCOutput[3] = 'A';
	mensajePCOutput[69] = 13;

	for(i=0;i<208;i++) mensajeICIInput[i] = 0;
	for(i=0;i<152;i++) mensajeICIOutput[i] = 0;

	mensajeInicialICIOutput();
}

void mensajeInicialICIOutput(){

	/* Como todos los elementos ya están en cero, se cargan solo los que restan
	 * que están detallados en el Excel correspondiente
	 */

	mensajeICIOutput[1] = 0xAA;
	mensajeICIOutput[2] = 0xAA;

	mensajeICIOutput[5] = 0x55;
	mensajeICIOutput[6] = 0x55;

	mensajeICIOutput[9] = 'S';
	mensajeICIOutput[10] = 'O';

	mensajeICIOutput[26] = 0xE0; // Full + Activo + Norte
	mensajeICIOutput[25] = 0x20; // Escala = 0x00100000

	mensajeICIOutput[30] = 0xFF; // Ancho de sector
	mensajeICIOutput[29] = 0x00; // Rumbo de sector

	mensajeICIOutput[46] = 0x80; // Dummy 4 MSByte
}

/* En esta función se pasan los parámetros que vinieron desde la PC (cargados en la variable
 * 'mensajePCInput') al buffer de salida de la ICI ('mensajeICIOutput').
 */
int traduccion_PC_a_ICI(){
	char aux=0;
	int i = 0;

	// INFO GENERAL
	mensajeICIOutput[10] = mensajePCInput[4]; 	// ID
	mensajeICIOutput[14] = mensajePCInput[5];	// VALID
	aux = mensajePCInput[6];					// Full + Act + Norte
	aux = aux & 0xE0;
	mensajeICIOutput[26] = aux;
	aux = mensajePCInput[6];					// Escala
	aux = aux & 0x1F;
	mensajeICIOutput[25] = aux;
	mensajeICIOutput[30] = mensajePCInput[7];	// ANCHO SECT
	mensajeICIOutput[29] = mensajePCInput[8]; 	// RUMBO SECT

	// DUMMYS
	for (i=0;i<6;i++){
		mensajeICIOutput[i*4+33] = mensajePCInput[9+2*i];
		mensajeICIOutput[i*4+34] = mensajePCInput[10+2*i];
	}

	// ATT1
	mensajeICIOutput[58] = mensajePCInput[21];
	for(i=0;i<5;i++){
		mensajeICIOutput[i*4+61] = mensajePCInput[22+2*i];
		mensajeICIOutput[i*4+62] = mensajePCInput[23+2*i];
	}

	// ATT2
	mensajeICIOutput[82] = mensajePCInput[32];
	for(i=0;i<5;i++){
		mensajeICIOutput[i*4+85] = mensajePCInput[33+2*i];
		mensajeICIOutput[i*4+86] = mensajePCInput[34+2*i];
	}

	// ATT3
	mensajeICIOutput[106] = mensajePCInput[43];
	for(i=0;i<5;i++){
		mensajeICIOutput[i*4+109] = mensajePCInput[44+2*i];
		mensajeICIOutput[i*4+110] = mensajePCInput[45+2*i];
	}

	// ICD
	mensajeICIOutput[130] = mensajePCInput[54];
	for(i=0;i<3;i++){
		mensajeICIOutput[i*4+133] = mensajePCInput[55+2*i];
		mensajeICIOutput[i*4+134] = mensajePCInput[56+2*i];
	}
	mensajeICIOutput[146] = mensajePCInput[61];  	// DOPPLER
	mensajeICIOutput[150] = mensajePCInput[62];		// SNR

	return 0;
}

int traduccion_ICI_a_PC(){
	char aux=0;
	int i;

	// INFO GENERAL

	mensajePCOutput[4] = mensajeICIInput[10];	// ID
	mensajePCOutput[5] = mensajeICIInput[14];	// STATUS
	mensajePCOutput[6] = mensajeICIInput[17];	// TIME WORD
	mensajePCOutput[7] = mensajeICIInput[18];
	mensajePCOutput[8] = mensajeICIInput[21];	// TIME WORD
	mensajePCOutput[9] = mensajeICIInput[22];
	mensajePCOutput[10] = mensajeICIInput[25]; 	// DUMMY
	mensajePCOutput[11] = mensajeICIInput[26];

	// TRACKS N°

	for(i=0;i<10;i++){
		aux=0;
		aux = mensajeICIInput[30+16*i];  		// VALID + TIPO
		aux = aux | mensajeICIInput[33+16*i];
		mensajePCOutput[12+i*5] = aux;

		mensajePCOutput[13+i*5] = mensajeICIInput[16*i + 37];
		mensajePCOutput[14+i*5] = mensajeICIInput[16*i + 38];

		mensajePCOutput[15+i*5] = mensajeICIInput[16*i + 41];
		mensajePCOutput[16+i*5] = mensajeICIInput[16*i + 42];
	}

	// ICD
	aux=0;
	aux = mensajeICIInput[190];  		// VALID + OP. POS.
	aux = aux | mensajeICIInput[193];
	mensajePCOutput[62] = aux;

	mensajePCOutput[63] = mensajeICIInput[197];
	mensajePCOutput[64] = mensajeICIInput[198];

	mensajePCOutput[65] = mensajeICIInput[201];
	mensajePCOutput[66] = mensajeICIInput[202];

	// LOCAL TIME
	mensajePCOutput[67] = mensajeICIInput[205];
	mensajePCOutput[68] = mensajeICIInput[206];

	return 0;
}
