
#include "state_machine.h"


void state_machine(void)
{
	static state_e state = INIT;
	while(1)
	{
		switch(state)
		{
		case INIT:
			INTERFACE_TFT_init();
			state = INTERFACE;
			break;
		case RECEIVE_TRAME:
			uart_communication();
			state = INTERFACE;
			break;
		case INTERFACE:
			interface_state_machine();
			state = RECEIVE_TRAME;
			break;
		case RESEARCH:

			state = RECEIVE_TRAME;
			break;
		default:
			break;
		}
	}
}
