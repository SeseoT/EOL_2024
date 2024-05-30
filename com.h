/*
 * com.h
 *
 *  Created on: 18 mars 2024
 *      Author: Tanguy Suteau
 */

#ifndef COM_COM_H_
#define COM_COM_H_

#include "stm32f1xx_hal.h"
#include "macro_types.h"
#include "stm32f1_uart.h"

#define TRAME_TAB_DATA_SIZE 6
#define TRAME_TOTAL_SIZE 8

typedef enum
{
	DEFAUTL,
	MODE,//SERVO1
	DATA,//SERVO2
	SERVOX,//ETAT,//SERVO 1 ET SERVO 2 -> SERVOX
	DEFAULT,//PARAM,
	PARAM,//DATA,
	CALIBRAGE,
	RECHERCHE,
}type_com;

typedef struct
{
	type_com type;
	uint8_t data[TRAME_TAB_DATA_SIZE];
}trame_struct;

void initCOMs(uart_id_e uart_id, uint32_t baudrate);
bool_e comIRQ(uart_id_e uart_id);
void  getTrame(trame_struct * trame);
void  putTrame(trame_struct * trame);
void videUart();


#endif /* COM_COM_H_ */
