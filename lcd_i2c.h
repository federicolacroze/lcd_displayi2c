

#ifndef displayi2c_H_
#define displayi2c_H_

#ifdef __cplusplus
extern "C" {
#endif



extern  uint8_t espera; 


#define I2C_SLAVE_ADDRESS1          0x27
#define MASK_ALTOS                  0xF8
#define MASK_BAJOS         			0xF8
#define CLEAR						0x01
#define RETURN						0x20
/******************************************************************************
* Global functions
******************************************************************************/
void Init_LCD(void);
void EscribirComando_LCD(uint8_t comando );
void EscribirByte_LCD( uint8_t  datochar );
void EnviarAlLcd ( uint8_t DATA );
void EnviarPalabra_LCD (uint8_t *word);
void Cursor(uint8_t FILA , uint8_t COL);
void Esperar(void);


#ifdef __cplusplus
}
#endif
#endif /* CRC_H_ */
