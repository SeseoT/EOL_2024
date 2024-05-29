/**
  ******************************************************************************
  * @file    state_machine.c
  * @author  GAILLARD Axel
  * @date    July-2024
  * @brief   principal state machine function.
  ******************************************************************************
*/
#include "state_machine.h"

#define DELAY_RESEARCH 2000

static void STATE_MACHINE_process_ms();

static volatile uint32_t t = DELAY_RESEARCH;

/**
 * @brief principal state machine
 * 
 */
void state_machine(void)
{
	static state_e state = INIT;
	static state_interface_e * mode_interface;

	while(1)
	{
		switch(state)
		{
		case INIT:
			Systick_add_callback_function(&STATE_MACHINE_process_ms);
			BUTTON_init();
			LED_init();
			mode_interface = INTERFACE_TFT_init();
			state = INTERFACE;
			break;
		case RECEIVE_TRAME:
			uart_communication();
			if(!t && *mode_interface == RESEARCH_MODE)
				state = RESEARCH;
			else
				state = INTERFACE;
			break;
		case INTERFACE:
			interface_state_machine();
			if(!t && *mode_interface == RESEARCH_MODE)
				state = RESEARCH;
			else
				state = RECEIVE_TRAME;
			break;
		case RESEARCH:
			transmit_trame(uart,research, dtc_none, mode_none, pot_none,0);
			t = DELAY_RESEARCH;
			state = RECEIVE_TRAME;
			break;
		default:
			break;
		}
		if(BUTTON_state_machine() == BUTTON_EVENT_SHORT_PRESS)
		{
			state = RESEARCH;
			decode_mode(RESEARCH_EOL);
		}


		if(*mode_interface == PROTECTED_MODE)
			LED_set(LED_FLASH);
		else if(*mode_interface == RESEARCH_MODE)
			LED_set(LED_RESEARCH);
		else
			LED_set(LED_OFF);
	}
}

/**
 * @brief a ms timer for the principal state machine
 * 
 */
static void STATE_MACHINE_process_ms(void)
{
	if(t)
		t--;
}
