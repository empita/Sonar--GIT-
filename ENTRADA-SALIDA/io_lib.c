#include "main.h"
#include "io_lib.h"

void configurar_io(){
	configurar_SysOnSonar();
	configurar_SysOnDaisy();

	configurar_LED_Error_PC();					//usa el Pin PC 10, ese pin es el Tx de la uart
	configurar_LED_Error_ICI_ET();
	configurar_LED_Error_ICI_TO();
	configurar_LED_Error_ICI_Enc_DHC();
	configurar_LED_Error_ICI_Enc_Sonar();
	configurar_LED_OnLine();

	apagar_LED_Error_PC();
	apagar_LED_Error_ICI_ET();
	apagar_LED_Error_ICI_TO();
	//encender_LED_Error_ICI_TO();
	apagar_LED_Error_ICI_Enc_DHC();
	//encender_LED_Error_ICI_Enc_DHC();
	apagar_LED_Error_ICI_Enc_Sonar();
	apagar_LED_OnLine();

}

void configurar_SysOnSonar(){
	GPIO_InitTypeDef GPIO_InitStructure;
	// Habilitación del clock del pin RX
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	// Se configura el pin
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void configurar_SysOnDaisy(){
	GPIO_InitTypeDef GPIO_InitStructure;
	// Habilitación del clock del pin RX
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	// Se configura el pin
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void configurar_LED_Error_PC(){
	GPIO_InitTypeDef GPIO_InitStructure;
	// Habilitación del clock del pin RX
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	// Se configura el pin
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

}

void configurar_LED_Error_ICI_ET(){
	GPIO_InitTypeDef GPIO_InitStructure;
	// Habilitación del clock del pin RX
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	// Se configura el pin
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

}

void configurar_LED_Error_ICI_TO(){
	GPIO_InitTypeDef GPIO_InitStructure;
	// Habilitación del clock del pin RX
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	// Se configura el pin
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void configurar_LED_Error_ICI_Enc_DHC(){
	GPIO_InitTypeDef GPIO_InitStructure;
	// Habilitación del clock del pin RX
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	// Se configura el pin
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void configurar_LED_Error_ICI_Enc_Sonar(){
	GPIO_InitTypeDef GPIO_InitStructure;
	// Habilitación del clock del pin RX
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	// Se configura el pin
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void configurar_LED_OnLine(){
	GPIO_InitTypeDef GPIO_InitStructure;
	// Habilitación del clock del pin RX
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	// Se configura el pin
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void encender_LED_Error_PC(){
	GPIO_SetBits(GPIOC, GPIO_Pin_12);
}

void encender_LED_Error_ICI_ET(){
	GPIO_SetBits(GPIOC, GPIO_Pin_13);
}

void encender_LED_Error_ICI_TO(){
	GPIO_SetBits(GPIOD, GPIO_Pin_0);
}

void encender_LED_Error_ICI_Enc_DHC(){
	GPIO_SetBits(GPIOD, GPIO_Pin_2);
}

void encender_LED_Error_ICI_Enc_Sonar(){
	GPIO_SetBits(GPIOD, GPIO_Pin_4);
}

void encender_LED_OnLine(){
	GPIO_SetBits(GPIOA, GPIO_Pin_10);
}

void apagar_LED_Error_PC(){
	GPIO_ResetBits(GPIOC, GPIO_Pin_12);
}

void apagar_LED_Error_ICI_ET(){
	GPIO_ResetBits(GPIOC, GPIO_Pin_13);
}

void apagar_LED_Error_ICI_TO(){
	GPIO_ResetBits(GPIOD, GPIO_Pin_0);
}

void apagar_LED_Error_ICI_Enc_DHC(){
	GPIO_ResetBits(GPIOD, GPIO_Pin_2);
}

void apagar_LED_Error_ICI_Enc_Sonar(){
	GPIO_ResetBits(GPIOD, GPIO_Pin_4);
}

void apagar_LED_OnLine(){
	GPIO_ResetBits(GPIOA, GPIO_Pin_10);
}

FlagStatus leer_SysOn_Sonar(){
	if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6) == 0)
		return RESET;
	else
		return SET;
}

FlagStatus leer_SysOn_DHC(){
	if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_7) == 0)
		return RESET;
	else
		return SET;
}
