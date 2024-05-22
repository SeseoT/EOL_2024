/*
 * com.c
 *
 *  Created on: 18 mars 2024
 *      Author: Tanguy Suteau
 */

#include "stm32f1_uart.h"
#include "com.h"

void initCOMs(uart_id_e uart_id, uint32_t baudrate){
	UART_init(uart_id, baudrate);
}

bool_e comIRQ(uart_id_e uart_id){//IRQ a changer
	return UART_data_ready(UART2_ID);
}

//fonction qui remplie la structure d'une trame
void  getTrame(trame_struct * trame){
	static uint8_t TAB_trame[TRAME_TOTAL_SIZE];
	static uint16_t index = 0;
	uint8_t c=0;
	if(UART_data_ready(UART2_ID)){
		while(c != '\n')
			{
				if(UART_data_ready(UART2_ID)){
					c = UART_getc(UART2_ID);			//lecture du prochain caract�re
					//printf("%c",c);
					TAB_trame[index] = c;						//On m�morise le caract�re dans le tableau
					index++;
				}
										//on incr�mente l'index (si < TAB_SIZE -2 !)
			}
	}

	index = 0;
	printf("%d",TAB_trame[0]);
	trame->type = TAB_trame[0];
	trame->data[0] = TAB_trame[1];
	trame->data[1] = TAB_trame[2];
	printf("%d",trame->type);
	printf("%d",trame->data[0]);
	printf("%d",trame->data[1]);

}

void putTrame(trame_struct * trame){
	//UART_putc(UART2_ID,trame->data[0]);
	uint8_t tramTotale[TRAME_TOTAL_SIZE];
	//Entête type
	tramTotale[0] = trame->type;
	//Corps de la trame
	for(int i =1;i<TRAME_TAB_DATA_SIZE;i++){
		tramTotale[i] = trame->data[i-1];
	}
	//Fin de trame
	tramTotale[TRAME_TOTAL_SIZE-2]=13;
	tramTotale[TRAME_TOTAL_SIZE-1]=10;
	for(int o=0;o<TRAME_TOTAL_SIZE;o++){
		UART_putc(UART2_ID,tramTotale[o]);
		UART_putc(UART3_ID,tramTotale[o]);
	}


}
