
#include "common.h"
#include "i2c.h"
#include "pit.h"
#include "displayi2c.h"




/******************************************************************************
* Local variables
******************************************************************************/
uint8_t paso1   = 0x38;
uint8_t paso2   = 0x28;

/*************************/
uint8_t paso3   = 0x28;
uint8_t paso4   = 0x0E;
uint8_t paso5   = 0x06;
uint8_t paso6   = 0x01;
uint8_t MASK_RS = 0x08;
/******************************************************************************/
/* Local functions*/
/**************************************************************/



 
void Init_LCD(void)
{    
			/* MANDO 3 VECES 0x30 PARA EMPEZAR LA INICIALIZACION DEL DISPLAY 	*/
	EnviarAlLcd ( paso1 );
	EnviarAlLcd ( paso1 );
	EnviarAlLcd ( paso1 );
	
			/* MANDO 1 VEZ 0x20 PARA CONFIGURAR EL DISPLAY EN MODO 4 BITS 	*/
	
	EnviarAlLcd ( paso2 );
	
			/* INICIA EL DISPLAY EN MODO 4 BITS*/

	EscribirByte_LCD(paso3 );  	// FUNCTION SET   :  4 BITS , 4 LINES , 5X8 DOTS
	EscribirByte_LCD(paso4 );  	// DISPLAY CONTROL:  DISPLAY ON , CURSOR ON, BLINK OFF 	
	EscribirByte_LCD(paso5 );	// ENTRY MODE SET :	 CURSOR SHIFT ON , SHIFT DISPLAY OFF	
	EscribirByte_LCD(paso6 );	// CLEAR DISPLAT  :  DISPLAY CLEAR
	
			/* FIN DE LA INICIALIZACION DEL DISPLAY */
}
/*****************************************************************************//*
*                    FUNCION de ESCRIBIR COMANDOS
*****************************************************************************/
void EscribirComando_LCD(uint8_t comando )

{
	MASK_RS = 0x08;	             	//RS = 0
	EscribirByte_LCD(comando );		//ESCRIBO COMANDO
	MASK_RS = 0x09;					//RS=1;
}
/*****************************************************************************//*
*                    FUNCION de ESCRIBIR UN BYTE EN DOS PARTES
*****************************************************************************/
void EscribirByte_LCD( uint8_t datochar )

{	
	uint8_t BitsAltos=0; 
	uint8_t BitsBajos=0;
	BitsAltos = (datochar & MASK_ALTOS);		
	BitsBajos = ((datochar << 4)& MASK_BAJOS);		
	EnviarAlLcd (BitsAltos);	 				//	ENVIAR LOS 4 BITS SUPERIORES
	EnviarAlLcd (BitsBajos); 					//	ENVIAR LOS 4 BITS INFERIORES
	
}

/*****************************************************************************//*
*                    FUNCION de ENVIAR DATOS AL LCD
*****************************************************************************/
void EnviarAlLcd ( uint8_t DATA)

{
	DATA |= MASK_RS ;
	I2C_MasterSendWait(I2C0 , I2C_SLAVE_ADDRESS1 , &DATA , 1);
	Esperar();
	DATA ^= 0x04;   //XOR AL BIT E->1
	I2C_MasterSendWait(I2C0 , I2C_SLAVE_ADDRESS1 , &DATA , 1);
	Esperar();
	DATA ^= 0x04;   //XOR AL BIT E->0
	I2C_MasterSendWait(I2C0 , I2C_SLAVE_ADDRESS1 , &DATA , 1);
	Esperar();
}
/*****************************************************************************//*
*                    FUNCION de ENVIAR UN STRING
*****************************************************************************/
void EnviarPalabra_LCD(uint8_t *word)
{
	uint8_t i=0;
	MASK_RS = 0x09 ; 
	
	for(i=0 ; word[i] != '\0' ; i++)
		EscribirByte_LCD(word[i]);
}


/*****************************************************************************//*
*                    FUNCION de POSICIONAR EL CURSOR
*****************************************************************************/
void Cursor (uint8_t FILA , uint8_t COL)

{	
	uint8_t POS=0;
	POS = FILA + COL ;
	EscribirComando_LCD (POS);
	
}


void Esperar (void)
{  	
	PIT_ConfigType  sPIT_Config = {0};
		uint8_t cont=0;
		espera = 1;
		//PIT_ChannelClrFlags(0);
		sPIT_Config.bFreeze    		= 1;    /*!< 1: stop in debug mode, 0: run in debug mode */
		sPIT_Config.bModuleDis      = 1;    /*!< 1: PIT module is disable, 0: PIT module is enable */    
		sPIT_Config.bTimerEn        = 1;    /*!< 1: channel timer is enable, 0: channel timer is disable */ 
		sPIT_Config.bInterruptEn    = 1;    /*!< 1: channel timer interrupt is enable, 0: channel timer interrupt is disable */ 
		sPIT_Config.bChainMode      = 0;    /*!< 1: chain mode is enable, 0: chain mode is disable */
		sPIT_Config.bFlag           = 1;    /*!< 1: flag is set,and write 1 to clear flag, 0: no flag is set */
		sPIT_Config.u32LoadValue   = 0x0000300F ;    /*!< 32-bit channel load value */ 
		PIT_Init(PIT_CHANNEL0 , &sPIT_Config);
		PIT_Enable();	    
		 while (espera)
			 cont++;
	    
	    PIT_ChannelClrFlags(0);
	    PIT_DeInit();

}

