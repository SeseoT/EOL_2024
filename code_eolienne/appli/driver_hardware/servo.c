/*
 * servo.c
 *
 *  Created on: 9 févr. 2024
 *      Author: Tanguy Suteau
 */
#include "stm32f1_timer.h"
#include "macro_types.h"
#include "config.h"
#include "stm32f1_gpio.h"
#include "servo.h"

#define PERIOD_TIMER 10 //ms

//static uint16_t current_positionS0;
//static uint16_t current_positionS1;




void SERVO_init(void){
	//initialisation et lancement du timer1 à une période de 10 ms
	TIMER_run_us(TIMER4_ID, PERIOD_TIMER*1000, FALSE); //10000us = 10ms
	//activation du signal PWM sur le canal 1 du timer 1 (broche PA8)
	TIMER_enable_PWM(TIMER4_ID, TIM_CHANNEL_1, 150, FALSE, FALSE);
	TIMER_enable_PWM(TIMER4_ID, TIM_CHANNEL_2, 150, FALSE, FALSE);
	//rapport cyclique reglé pour une position servo de 50%

}

bool_e SERVO_set_position(servo_t *servo_X ,uint16_t position)
{
	switch(servo_X->servo_num)
	{
	case(SERVO_0):
		if(position > 100){
			position = 100;
			return FALSE;
		}//chanel 1
		TIMER_set_duty(TIMER4_ID, TIM_CHANNEL_1, 100+position);
		servo_X->currentPosition = position;
		return TRUE;

	case(SERVO_1):
		if(position > 100){
			position = 100;
			return FALSE;
		}//chanel 2
		TIMER_set_duty(TIMER4_ID, TIM_CHANNEL_2, 100+position);
		servo_X->currentPosition = position;
		return TRUE;

	default:
		return FALSE;

	}
}


uint16_t SERVO_get_position(servo_t servo_X){
	return servo_X.currentPosition;
}


void SERVO_process_test(servo_t servo){
	static uint16_t position = 50;
	static bool_e previous_button = FALSE;
	bool_e current_button;
	//lecture du bouton bleu
	current_button = !HAL_GPIO_ReadPin(BLUE_BUTTON_GPIO, BLUE_BUTTON_PIN);


	if(current_button && !previous_button) //si appui bouton
	{
	position = (position > 99)?0:(position+5); //de 0 à 100%, par pas de 5%
	if(SERVO_set_position(&servo,position)){
		printf("Position SET\n");
	}
	}
	printf("Position %d.\n",SERVO_get_position(servo));
	previous_button = current_button; //sauvegarde pour le prochain passage
	HAL_Delay(10); //anti-rebond "de fortune" en cadencant la lecture du bouton
}






