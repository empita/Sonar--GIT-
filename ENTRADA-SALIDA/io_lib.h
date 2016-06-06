#ifndef _IO_LIB
#define _IO_LIB

void configurar_io();
void configurar_SysOnSonar();
void configurar_SysOnDaisy();

void configurar_LED_Error_PC();
void configurar_LED_Error_ICI_ET();
void configurar_LED_Error_ICI_TO();
void configurar_LED_Error_ICI_Enc_DHC();
void configurar_LED_Error_ICI_Enc_Sonar();
void configurar_LED_OnLine();

void encender_LED_Error_PC();
void encender_LED_Error_ICI_ET();
void encender_LED_Error_ICI_TO();
void encender_LED_Error_ICI_Enc_DHC();
void encender_LED_Error_ICI_Enc_Sonar();
void encender_LED_OnLine();

void apagar_LED_Error_PC();
void apagar_LED_Error_ICI_ET();
void apagar_LED_Error_ICI_TO();
void apagar_LED_Error_ICI_Enc_DHC();
void apagar_LED_Error_ICI_Enc_Sonar();
void apagar_LED_OnLine();

FlagStatus leer_SysOn_Sonar();
FlagStatus leer_SysOn_DHC();
#endif
