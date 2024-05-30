/**
  ******************************************************************************
  * @file    main.c
  * @author  Nirgal
  * @date    03-July-2019
  * @brief   Default main function.
  ******************************************************************************
*/
#include "stm32f1xx_hal.h"
#include "stm32f1_adc.h"
#include "stm32f1_uart.h"
#include "stm32f1_sys.h"
#include "stm32f1_gpio.h"
#include "macro_types.h"
#include "systick.h"
#include "state.h"
#include "ntc.h"


static volatile uint32_t t = 0;
static volatile uint32_t tCalibms = 0;
static volatile uint32_t tModeSecu = 0;

/**
 * @brief Callback function for processing each millisecond.
 *
 * This function decrements the time variables and processes state machine tasks.
 */
void process_ms(void)
{
	if(t){
		t--;
	}
	tCalibms++;
	if(tCalibms>= 0xFFFFFF00){
		tCalibms = 0;
	}
	if(tModeSecu){
		tModeSecu--;
		}

}

/**
 * @brief Main function.
 *
 * This is the entry point of the program. It initializes hardware peripherals and enters the main loop.
 */
int main(void)
{

	HAL_Init();


	UART_init(UART2_ID,115200);
	UART_init(UART3_ID,115200);


	BSP_GPIO_PinCfg(LED_GREEN_GPIO, LED_GREEN_PIN, GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);
	BSP_GPIO_PinCfg(GPIOB, GPIO_PIN_14, GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);
	HAL_GPIO_WritePin(LED_GREEN_GPIO,LED_GREEN_PIN,1);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,0);




	Systick_add_callback_function(&process_ms);
	

	printf("GO");
	t=100;//On envoies la trame tt les 1000ms
	
	while(1)	
	{
		state_machine(&t,&tCalibms,&tModeSecu);
	}
}
