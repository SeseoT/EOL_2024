/*
 * led.c
 *
 *  Created on: 26 juin 2019
 *      Author: Nirgal
 */


#include "led.h"

#define LED_BLINK_PERIOD	500
#define LED_FLASH_PERIOD	500
#define LED_FLASH_DURATION	50

#define LED_R_PERIOD	    2000
#define LED_R_DURATION	    100

static void LED_process_ms(void);
static volatile led_state_e led_state;
static volatile uint32_t t = 0;

/**
 * @brief init the led state machine
 * 
 */
void LED_init(void)
{
	BSP_GPIO_PinCfg(GPIOB, GPIO_PIN_7, GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);
	LED_set(LED_OFF);
	Systick_add_callback_function(&LED_process_ms);
}

/**
 * @brief set the state of a led
 * 
 * @param state 
 */
void LED_set(led_state_e state)
{
	if(led_state != state)
		t = 0;			//si on change de mode, on reset le compteur.
	led_state = state;	//on enregistre l'�tat demand�
}


/**
 * @brief callback of the led state machine (called at each ms)
 * 
 */
static void LED_process_ms(void)
{
	switch(led_state)
	{
		case LED_BLINK:
			t++;
			if(t>LED_BLINK_PERIOD)
			{
				HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);
				t = 0;
			}
			break;
		case LED_FLASH:
			t++;
			if(t==LED_FLASH_DURATION)
			{
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, FALSE);
			}
			else if(t>LED_FLASH_PERIOD)
			{
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, TRUE);
				t = 0;
			}
			break;
		case LED_RESEARCH:
			t++;
			if(t==LED_R_DURATION)
			{
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, FALSE);
			}
			else if(t>LED_R_PERIOD)
			{
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, TRUE);
				t = 0;
			}
			break;
		case LED_OFF:
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, FALSE);
			break;
		case LED_ON:
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, TRUE);
			break;
	}
}
