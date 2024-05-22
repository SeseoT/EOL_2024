#include "communication.h"


void decode_trame(uint8_t *buffer)
{
	switch(buffer[0])
			{
				case mode:
					decode_mode(buffer[1]);
					break;
				case values:
					decode_temp_voltage(buffer);
					break;
				case height:
					decode_height(buffer);
					break;
				case DTC_type:
					decode_mode(PROTECTED);
					decode_DTC(buffer[1]);
					break;
				case T_calibrage:
					//coté eolienne
					break;
				case research:
					decode_mode(buffer[1]);
					break;
				default:
					break;
			}
}

void encode_trame(uint8_t * buffer, uint8_t size,trame_type_e type, dtc_type_e dtc_type, mode_type_e mode_type, pot_type_e pot_type, uint32_t value)
{
	for(int i=0; i<size;i++)
	{
		buffer[i]=0;
	}

	buffer[0] = type;

	if(dtc_type != dtc_none && type == DTC_type)
	{
		buffer[1] = dtc_type;
	}
	else if(mode_type != mode_none && type == mode)
	{
		buffer[1] = mode_type;
	}
	else if(pot_type != pot_none && type == height)
	{
		buffer[1] = pot_type;
		buffer[2] = (uint8_t)value;
	}
	else if(type == T_calibrage) //t_calibrage for now
	{
		buffer[2] = value & 0xFF;
		buffer[1] = (uint8_t)(value >> 8);
	}
	else if(type == calibrage) //calibrage for now
	{
		buffer[1] = (uint8_t)value;
	}
	buffer[6] = 13;
	buffer[7] = 10;
}

void transmit_trame(transmition_mode trsmt_mode,trame_type_e type, dtc_type_e dtc_type, mode_type_e mode_type, pot_type_e pot_type, uint32_t value)
{
	uint8_t buffer[8];
	encode_trame(buffer, 8,type,dtc_type, mode_type, pot_type, value);
	if(trsmt_mode == uart)
	{
		UART_puts(UART2_ID, buffer, 8);
	}
	else if(trsmt_mode == rf)
	{

	}
}
void decode_temp_voltage(uint8_t *buffer)
{
	float temp = ((float)((buffer[1]<<8)+(buffer[2])))/10;
	float voltage = ((float)((buffer[3]<<8)+(buffer[4])))/10;
	uint8_t null_data = (uint8_t)(0xFF);
	fill_values_param(temp, voltage, null_data,null_data,null_data,null_data);
}

void decode_height(uint8_t *buffer)
{
	uint8_t hauteur = buffer[1];
	uint8_t inclinaison = buffer[2];
	uint8_t null_data = (uint8_t)(0xFF);
	fill_values_param(255.0, 255.0, inclinaison,hauteur,null_data,null_data);
}

void decode_mode(mode_type_e mode)
{
	if(mode != PROTECTED)
	{
		fill_values_param(255.0, 255.0,0xFF, 0xFF,0,0);
	}
	fill_mode_param(mode);
}

void decode_DTC(uint8_t code_dtc)
{
	uint8_t null_data = (uint8_t)(0xFF);
	fill_values_param(255.0, 255.0, null_data,null_data,code_dtc,null_data);
}

static uint8_t buffer[8];
static uint16_t i = 0;

void uart_communication(void)
{
	uint8_t c=0;
	if(UART_data_ready(UART2_ID))
	{
		while(c != '\n')
		{
			if(UART_data_ready(UART2_ID))
			{
				c = UART_getc(UART2_ID);	//lecture du prochain caract�re
				buffer[i] = c;				//On m�morise le caract�re dans le tableau
				i++;
			}								//on incr�mente l'index (si < TAB_SIZE -2 !)
		}
		decode_trame(buffer);
		i = 0;
		c=0;
	}
}

