/*
 * state_machine.h
 *
 *  Created on: 20 mai 2024
 *      Author: axel GAILLARD
 */

#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_

#include "interface.h"
#include "communication.h"
#include "button.h"
#include "led.h"
#include "flash.h"

void state_machine(void);

typedef enum   //type de trame de remonter de default
{
	INIT,
	RECEIVE_TRAME,
	INTERFACE,
	RESEARCH,
}state_e;


#endif /* STATE_MACHINE_H_ */
