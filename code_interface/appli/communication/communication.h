/*
 * communication.h
 *
 *  Created on: 10 mai 2024
 *      Author: axel GAILLARD
 */

#ifndef COMMUNICATION_H_
#define COMMUNICATION_H_


#include "stm32f1xx_hal.h"
#include "stm32f1_uart.h"
#include "interface.h"

typedef struct
{
    uint8_t type;
    uint8_t inclinaison;
    uint8_t DTC;
}trame_t;

typedef enum //type de trame principales
{
	mode = 1,
	values,
	height,
	DTC_type,
	T_calibrage,
	calibrage,
	research,
}trame_type_e;

typedef enum   //type de trame de remonter de default
{
	V_max,
	T_max,
	T_min,
	dtc_none,
}dtc_type_e;

typedef enum
{
	uart,
	rf,
}transmition_mode;

typedef enum   //type de trame de paramétrage des consignes
{
	AUTO,
	MANUAL,
	PROTECTED,
	RESEARCH_EOL,
	mode_none,
}mode_type_e;


typedef enum   //type de trame de paramétrage des consignes
{
	hauteur,
	inclinaison,
	pot_none,
}pot_type_e;

void encode_trame(uint8_t * buffer, uint8_t size,trame_type_e type, dtc_type_e dtc_type, mode_type_e mode_type, pot_type_e pot_type, uint32_t value);

void transmit_trame(transmition_mode trsmt_mode,trame_type_e type, dtc_type_e dtc_type, mode_type_e mode_type, pot_type_e pot_type, uint32_t value);

void decode_temp_voltage(uint8_t *buffer);

void decode_DTC(uint8_t code_dtc);

void decode_trame(uint8_t *buffer);

void decode_height(uint8_t *buffer);

void decode_mode(mode_type_e mode);

void uart_communication(void);

#endif /* COMMUNICATION_H_ */
