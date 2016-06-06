#include "main.h"
#include "mensajes.h"
#include "consola.h"
#include "app_sonar.h"
#include "io_lib.h"
#include "FSM.h"

void configurar_led_discovery();
void encenderLEDDiscovery();
void apagarLEDDiscovery();

int main(void)
{
	//char input[2];

	SystemInit();
	SystemCoreClockUpdate();

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //// IMPORTANTE DEFINIR LA FORMA DE ANIDAR INTERRUPCIONES!!!!!

	//configurar_led_discovery();
	inicializar_consola();			//cnfigura los puerto de la UART
	inicializarMensajes();			//inicia la comunicacion con el sonar
	app_sonar_init();
	//habilitar_reloj();
	configurar_io();				//configuración de los leds indicadores

	FSM_Init();

	while(1)

		scheduler();

	while(1);

	return 0;
}

void configurar_led_discovery(){
	GPIO_InitTypeDef GPIO_InitStructure;
	// Habilitación del clock del pin RX
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	// Se configura el pin
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void encenderLEDDiscovery(){
	GPIO_SetBits(GPIOD,GPIO_Pin_15);
}

void apagarLEDDiscovery(){
	GPIO_ResetBits(GPIOD,GPIO_Pin_15);
}
