#include "tests.h"

void leds_discovery(){
	GPIO_InitTypeDef GPIOConf;

		GPIOConf.GPIO_Pin =  GPIO_Pin_12| GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15 ;
		GPIOConf.GPIO_Mode = GPIO_Mode_OUT;
		GPIOConf.GPIO_OType = GPIO_OType_PP;
		GPIOConf.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIOConf.GPIO_Speed = GPIO_Speed_25MHz;
		GPIO_Init(GPIOD, &GPIOConf);

		GPIO_SetBits(GPIOD,GPIO_Pin_12);
		GPIO_SetBits(GPIOD,GPIO_Pin_13);
		GPIO_SetBits(GPIOD,GPIO_Pin_14);
		GPIO_SetBits(GPIOD,GPIO_Pin_15);
}

void probar_puerto_U(){
	uint32_t delay = 100000000;
	uint32_t i=0, dato=0;
	configurar_puerto_U();

	escribir_puerto_U(0x1);
	dato = 0x1;

	for(i=0;i<23;i++){
		while(delay--);
		dato = dato * 2;
		escribir_puerto_U(dato);
		delay=100000000;
	}
}

void probar_salidas(){
	uint32_t delay = 100000000;

	configurarEFD();
	configurarEN();
	configurarSTOP();
	configurarACK();

	poner_salidas_alto();
	while(delay--);
	delay = 100000000;
	resetear_EFD();
	while(delay--);
	delay = 100000000;
	setear_EFD();
	resetear_EN();
	while(delay--);
	delay = 100000000;
	setear_EN();
	resetear_ACK();
	while(delay--);
		delay = 100000000;
	setear_ACK();
	resetear_STOP();
	while(delay--);
		delay = 100000000;
	setear_STOP();
}

void probar_entradas_int(){
	leds_discovery();

	while(1){
		if(EIP == SET){
			GPIO_SetBits(GPIOD,GPIO_Pin_12);
		}
		else{
			GPIO_ResetBits(GPIOD,GPIO_Pin_12);
		}

		if(RQ == SET){
			GPIO_SetBits(GPIOD,GPIO_Pin_13);
		}
		else{
			GPIO_ResetBits(GPIOD,GPIO_Pin_13);
		}

		if(IDRP == SET){
			GPIO_SetBits(GPIOD,GPIO_Pin_14);
		}
		else{
			GPIO_ResetBits(GPIOD,GPIO_Pin_14);
		}

		if(ODRP == SET){
			GPIO_SetBits(GPIOD,GPIO_Pin_15);
		}
		else{
			GPIO_ResetBits(GPIOD,GPIO_Pin_15);
		}
	}

}

