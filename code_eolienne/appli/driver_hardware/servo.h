/*
 * servo.h
 *
 *  Created on: 9 févr. 2024
 *      Author: Tanguy Suteau
 */

#ifndef SERVO_H_
#define SERVO_H_
#include "macro_types.h"
#include "config.h"
#include "stm32f1_gpio.h"
typedef enum{
		SERVO_0 = 0,
		SERVO_1,
		SERVO_2,
		SERVO_3,
		SERVO_4,
		SERVO_5,
		SERVO_6,
		SERVO_7,
		SERVO_8,
		SERVO_9,
		SERVO_NB
}servo_nb;


typedef struct
{
	servo_nb servo_num;
	uint16_t currentPosition;
}servo_t;

void SERVO_init(void);
bool_e SERVO_set_position(servo_t *servo_X ,uint16_t position);
uint16_t SERVO_get_position(servo_t servo_X);
void SERVO_process_test(servo_t servo);


#endif /* SERVO_H_ */
