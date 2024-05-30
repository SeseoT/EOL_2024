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


/**
 * @brief Initializes the servo module.
 *
 * This function initializes and starts the timer with a period of 10 ms, and
 * enables PWM signals on the timer channels.
 */
void SERVO_init(void){
	//initialisation et lancement du timer1 à une période de 10 ms
	TIMER_run_us(TIMER4_ID, PERIOD_TIMER*1000, FALSE); //10000us = 10ms
	//activation du signal PWM sur le canal 1 du timer 1 (broche PA8)
	TIMER_enable_PWM(TIMER4_ID, TIM_CHANNEL_1, 150, FALSE, FALSE);
	TIMER_enable_PWM(TIMER4_ID, TIM_CHANNEL_2, 150, FALSE, FALSE);
	//rapport cyclique reglé pour une position servo de 50%

}
/**
 * @brief Sets the position of a specified servo.
 *
 * This function sets the position of the given servo by adjusting the PWM duty cycle.
 *
 * @param servo_X Pointer to the servo structure.
 * @param position The desired position (0 to 100).
 * @return TRUE if the position is set successfully, FALSE otherwise.
 */
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

/**
 * @brief Gets the current position of the specified servo.
 *
 * This function returns the current position of the given servo.
 *
 * @param servo_X The servo structure.
 * @return The current position of the servo.
 */
uint16_t SERVO_get_position(servo_t servo_X){
	return servo_X.currentPosition;
}

/**
 * @brief Test function for servo control.
 *
 * This function is used for testing the servo control by adjusting the position
 * based on button presses.
 *
 * @param servo The servo structure to test.
 */
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






