#ifndef _PHY
#define _PHY

#include "main.h"

#define	BUFFER_OUT 100
#define	BUFFER_IN 1000

char mensaje_TX[BUFFER_OUT];

void configurar_UART_TX_pin();
void configurar_UART_TX();
void configurar_UART_TX_DMA();

void configurar_UART_RX_pin();
void configurar_UART_RX();
void configurar_UART_RX_DMA();

void UART_PHY_init();

void enviarDataOutput(char*, int longitud);
int leerDataInput(char*, int longitud);
FlagStatus mensajeRXListo;

#endif
