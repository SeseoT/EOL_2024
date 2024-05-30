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
	//Initialisation de la couche logicielle HAL (Hardware Abstraction Layer)
	//Cette ligne doit rester la premi�re �tape de la fonction main().
	HAL_Init();


	//Initialisation de l'UART2 � la vitesse de 115200 bauds/secondes (92kbits/s) PA2 : Tx  | PA3 : Rx.
		//Attention, les pins PA2 et PA3 ne sont pas reli�es jusqu'au connecteur de la Nucleo.
		//Ces broches sont redirig�es vers la sonde de d�bogage, la liaison UART �tant ensuite encapsul�e sur l'USB vers le PC de d�veloppement.
	UART_init(UART2_ID,115200);
	UART_init(UART3_ID,115200);

	//"Indique que les printf sortent vers le p�riph�rique UART2."
	//SYS_set_std_usart(UART2_ID, UART2_ID, UART2_ID);

	//Initialisation du port de la led Verte (carte Nucleo)
	BSP_GPIO_PinCfg(LED_GREEN_GPIO, LED_GREEN_PIN, GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);
	BSP_GPIO_PinCfg(GPIOB, GPIO_PIN_14, GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);
	HAL_GPIO_WritePin(LED_GREEN_GPIO,LED_GREEN_PIN,1);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,0);


	//Initialisation du port du bouton bleu (carte Nucleo) et autre bouton
	BSP_GPIO_PinCfg(BLUE_BUTTON_GPIO, BLUE_BUTTON_PIN, GPIO_MODE_INPUT,GPIO_PULLUP,GPIO_SPEED_FREQ_HIGH);
	//BSP_GPIO_PinCfg(GPIOB, GPIO_PIN_13, GPIO_MODE_INPUT,GPIO_PULLUP,GPIO_SPEED_FREQ_HIGH);

	//On ajoute la fonction process_ms � la liste des fonctions appel�es automatiquement chaque ms par la routine d'interruption du p�riph�rique SYSTICK
	Systick_add_callback_function(&process_ms);
	Systick_add_callback_function(&test_ms);

	printf("GO");
	t=100;//On envoies la trame tt les 1000ms
	
	while(1)	//boucle de t�che de fond
	{
		state_machine(&t,&tCalibms,&tModeSecu);
	}
}
