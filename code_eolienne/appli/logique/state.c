/*
 * state.c
 *
 *  Created on: 26 mars 2024
 *      Author: Tanguy Suteau
 */
#include "led.h"
#include "eol.h"
#include "servo.h"
#include "com.h"
#include "stm32f1_uart.h"
#include "state.h"
#include "systick.h"
#include "ntc.h"
#include "button.h"
#include "flash.h"


static state_e state = INIT;
static adc_id_e ADC_EOL = ADC_9;
static adc_id_e ADC_Tempe = ADC_4;

void receiveModeStateMachine(modeEolienne_t * oldMode,eolienne_t * eolienne,servo_t * servoHauteur,servo_t * servoOrientation,bool_e * okCalib,trame_struct * inputTrame,uint32_t *tCalibms,trame_struct *outputTrameMODE);


/**
 * @brief Initializes the state machine.
 *
 * This function initializes the state machine and sets initial values for eolienne, servo, and calibration parameters.
 *
 * @param eolienne Pointer to the eolienne structure.
 * @param servoHauteur Pointer to the servo structure for controlling height.
 * @param servoOrientation Pointer to the servo structure for controlling orientation.
 * @param okCalib Pointer to the calibration status flag.
 * @param oldMode Pointer to store the previous mode of the eolienne.
 */
void initStateMachine(eolienne_t * eolienne,servo_t * servoHauteur,servo_t * servoOrientation,bool_e * okCalib,modeEolienne_t * oldMode);
/**
 * @brief Performs calibration for the eolienne system.
 *
 * This function performs calibration for the eolienne system by adjusting servo positions and recording the
 * highest voltage value.
 *
 * @param eolienne Pointer to the eolienne structure.
 * @param servoH Pointer to the servo structure for controlling height.
 * @param servoO Pointer to the servo structure for controlling orientation.
 * @param okCalib Pointer to the calibration status flag.
 */
void calibrage(eolienne_t *eolienne,servo_t *servoH, servo_t *servoO, bool_e *okCalib );


/**
 * @brief State machine for controlling the eolienne system.
 *
 * This function implements the state machine for controlling the eolienne system, including handling different modes,
 * receiving and processing commands, managing calibration, safety, and fault conditions.
 *
 * @param t Pointer to the timer variable for message timing.
 * @param tCalibms Pointer to the timer variable for calibration timing.
 * @param tModeSecu Pointer to the timer variable for safety mode timing.
 */
void state_machine(uint32_t *t, uint32_t *tCalibms,uint32_t *tModeSecu)
{
	static state_e state = INIT;
	static state_e previous_state = INIT;
	static eolienne_t eolienne;
	static servo_t servoHauteur;
	static servo_t servoOrientation;
	bool_e entrance = (state!=previous_state)?TRUE:FALSE;
	previous_state = state;
	static trame_struct inputTrame;
	static trame_struct outputTrameSERVOX;//Contient hauteur et orientation eolienne
	static trame_struct outputTrameDATA;//Contient tension eolienne et température
	static trame_struct outputTrameDEFAULT;//Contient les défaults de l'éolienne
	static trame_struct outputTrameMODE;
	static modeEolienne_t oldMode;
	static bool_e okCalib = TRUE;
	switch(state)
		{
	case INIT:
		initStateMachine(&eolienne, &servoHauteur, &servoOrientation, &okCalib, &oldMode);
		state = MODE_StandBy;
		break;
	case MODE_Receive:

		receiveModeStateMachine(&oldMode, &eolienne, &servoHauteur, &servoOrientation, &okCalib, &inputTrame, tCalibms, &outputTrameMODE);
		state = MODE_StandBy;
		break;
	case MODE_StandBy://On attend trame de l'IHM et envois l'etat de l'eol

		if(comIRQ(UART2_ID)){
			printf("Comme prete");
			state = MODE_Receive;
			break;
		}
		
		if(BUTTON_state_machine() == BUTTON_EVENT_SHORT_PRESS){
			flash_write_values(eolienne.memoireFlash, SIZE_BUFFER_MEMOIRE_FLASH);
			flash_read_values(eolienne.memoireFlash, SIZE_BUFFER_MEMOIRE_FLASH);

		}


		if(*t<TIMER_MSG-100 && eolienne.paramEolienne.mode != MODE_Securite){
			if(eolienne.paramEolienne.mode == MODE_Manuel){
				//Led verte eteinte
				HAL_GPIO_WritePin(LED_GREEN_GPIO,LED_GREEN_PIN,1);
				//Led Rouge = mode manuel
				if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_14)){
					HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,0);
				}
			}else if(eolienne.paramEolienne.mode == MODE_Auto){
				//Led rouge éteinte
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,0);
				//Led Verte = mode auto
				if(!HAL_GPIO_ReadPin(LED_GREEN_GPIO,LED_GREEN_PIN)){
					HAL_GPIO_WritePin(LED_GREEN_GPIO,LED_GREEN_PIN,1);
				}

			}

		}

		//envois trames d'etats
		if(!*t){

			//On envoi mis a jours les positions de l'eol que en mode auto
			if(eolienne.paramEolienne.mode == MODE_Auto){
				eolienne.currentHauteur = SERVO_get_position(servoHauteur);
				eolienne.currentOrientation = SERVO_get_position(servoOrientation);

				outputTrameSERVOX.type = SERVOX;
				//Trame servo hauteur
				outputTrameSERVOX.data[0] = 0;
				outputTrameSERVOX.data[1] = 23;//eolienne.currentHauteur;
				putTrame(&outputTrameSERVOX);
				//Trame servo orientation
				outputTrameSERVOX.data[0] = 1;
				outputTrameSERVOX.data[1] = 56;//eolienne.currentOrientation;
				putTrame(&outputTrameSERVOX);
				//eolienne_print_param(&eolienne);
			}
			//Seul le défaut en mode sécurité est gérer, on envoi a l'IHM le défaut
			else if(eolienne.paramEolienne.mode == MODE_Securite){
				outputTrameDEFAULT.type = DEFAULT;
				outputTrameDEFAULT.data[0] = EOL_DEF_TEMP_H;
				outputTrameDEFAULT.data[1] = eolienne.defautEolienne.tempe_H;
				putTrame(&outputTrameDEFAULT);
				*t = TIMER_MSG;
			}


			//Mis a jour des variable de notre eolienne
			if(eolienne.paramEolienne.mode != MODE_Securite){
				//On autorise un calibrage venant de l'eolienne a chaque envoi de donné
				okCalib = TRUE;
				

				eolienne.tension = EOL_tension_V(EOL_getValue(ADC_EOL))/1000;
				eolienne.temperature = NTC_getValue(ADC_Tempe)*10;

				outputTrameDATA.type = DATA;
				outputTrameDATA.data[1] = (uint16_t)eolienne.temperature & 0xFF;
				outputTrameDATA.data[0] = (uint8_t)((uint16_t)eolienne.temperature >>8);
				outputTrameDATA.data[3] = (uint16_t)eolienne.tension & 0xFF;
				outputTrameDATA.data[2] = (uint8_t)((uint16_t)eolienne.tension >>8);
				putTrame(&outputTrameDATA);
				//En mode securité la led reste verte

				*t = TIMER_MSG;
				if(eolienne.paramEolienne.mode == MODE_Manuel){
					HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,1);//Led Rouge = mode manuel
				}else if(eolienne.paramEolienne.mode == MODE_Auto){
					HAL_GPIO_WritePin(LED_GREEN_GPIO,LED_GREEN_PIN,0);//Led Verte = mode auto
				}

			}

		}

		//Commande de l'eolienne selon son mode actuel
		if(eolienne.paramEolienne.mode == MODE_Auto ){
			//On att la periode de calibration
			if(*tCalibms >= eolienne.paramEolienne.tmpsPeriodeCalibrage*1000){//*6000 -> Converstion des secondes en ms
				calibrage(&eolienne, &servoHauteur, &servoOrientation, &okCalib);
				*tCalibms = 0;
			}
			//Action mode auto : EOL imobile et tt les X minutes recherches meilleur spot, si aucun mieux sport on bouge pas ou peu

		}else if(eolienne.paramEolienne.mode == MODE_Manuel){
			//Mode manuel on prend les valeurs envoyé par l'IHM
			//On met a jour les position des servo selon ce qui est stocké dans la strucuture de l'eoliene
			SERVO_set_position(&servoOrientation, eolienne.currentOrientation);
			SERVO_set_position(&servoHauteur, eolienne.currentHauteur);

		}else if (eolienne.paramEolienne.mode == MODE_Securite){
			SERVO_set_position(&servoOrientation, 0);//Position de securite
			SERVO_set_position(&servoHauteur, 0);//Position de securite

			//SORTIE MODE SECU
			if(!*tModeSecu){
				eolienne.paramEolienne.mode = oldMode;
				outputTrameMODE.type = MODE;
				outputTrameMODE.data[0] =eolienne.paramEolienne.mode;
				putTrame(&outputTrameMODE);
				HAL_GPIO_WritePin(LED_GREEN_GPIO,LED_GREEN_PIN,1);//On etteint la led
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,0);
				//On reset les défaut a 0
				eolienne.defautEolienne.secu_VTF = 0;
				eolienne.defautEolienne.mode_secu_ON = 0;
				eolienne.defautEolienne.tempe_H = 0;

			}

		}


		//Gestion des défauts
		eolienne.tension = EOL_tension_V(EOL_getValue(ADC_EOL))/1000;
		eolienne.temperature = NTC_getValue(ADC_Tempe);


		//Defaut tension trop haute( Vent Trop fort) PASSAGE DE L'EOLIENNE EN SECURITE
		if(eolienne.tension >= eolienne.paramEolienne.tensionVentTropFort && eolienne.paramEolienne.mode != MODE_Securite){
			oldMode = eolienne.paramEolienne.mode;
			eolienne.paramEolienne.mode = MODE_Securite;
			eolienne.defautEolienne.secu_VTF = 1;
			eolienne.defautEolienne.mode_secu_ON = 1;
			*tModeSecu = TIMER_MODE_SECU;
			HAL_GPIO_WritePin(LED_GREEN_GPIO,LED_GREEN_PIN,0);//On allume la led
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,1);
		}
		//Defaut tension temperature trop haute PASSAGE DE L'EOLIENNE EN SECURITE
		if(eolienne.temperature >= eolienne.paramEolienne.csgMaxTempe && eolienne.paramEolienne.mode != MODE_Securite){
			oldMode = eolienne.paramEolienne.mode;
			eolienne.paramEolienne.mode = MODE_Securite;
			eolienne.defautEolienne.tempe_H = 1;
			eolienne.defautEolienne.mode_secu_ON = 1;
			*tModeSecu = TIMER_MODE_SECU;
			HAL_GPIO_WritePin(LED_GREEN_GPIO,LED_GREEN_PIN,0);//On allume la led
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,1);
		}

		break;
	default:
		break;
		}

}


//Alocation des parametre de l'eolienne
/**
 * @brief Updates eolienne parameters based on received commands.
 *
 * This function updates eolienne parameters based on received commands from an external interface.
 *
 * @param eolienne Pointer to the eolienne structure.
 * @param trame Pointer to the received command frame.
 */
void updateParam(eolienne_t *eolienne,trame_struct *trame){
	switch(trame->data[0]){//Navigation parmi les differents type de parametre
	case EOL_PARAM_MODE:
		printf("Update Mode\n");
		eolienne->paramEolienne.mode = trame->data[1];
		break;
	case EOL_PARAM_TMPS_M_AUTO:
		printf("Update temps calibrage\n");
		eolienne->paramEolienne.tmpsPeriodeCalibrage = trame->data[1];
		break;
	case EOL_PARAM_TEMP_MAX:
		printf("Update Tempe max\n");
		eolienne->paramEolienne.csgMaxTempe = trame->data[1];
		break;
	case EOL_PARAM_TEMP_MIN:
		printf("Update tempe min\n");
		eolienne->paramEolienne.csgMinTempe = trame->data[1];
		break;
	case EOL_PARAM_TMPS_SECU:
		printf("Update temps Secu\n");
		eolienne->paramEolienne.tmpsSecurite = trame->data[1];
		break;
	default:
		break;
	}
	
}



void calibrage(eolienne_t *eolienne,servo_t *servoH, servo_t *servoO, bool_e *okCalib ){
	//On cadrille une zonne de 10*10

	uint16_t hRef = SERVO_get_position(*servoH);
	uint16_t oRef = SERVO_get_position(*servoO);
	uint16_t vRef = eolienne->tension;
	uint8_t h = 0;
	uint8_t o = 0;
	uint16_t vTemp = 0;

	for(o=0 ; o<=2 ; o++){
	
		SERVO_set_position(servoO, (uint16_t)o*30);
		for(h=0 ; h<=10 ; h++){
			SERVO_set_position(servoH, h*20);
			vTemp = EOL_tension_V(EOL_getValue(ADC_EOL))/1000;
			HAL_Delay(500);
			//On compare si la tension est plus eleve
			if(vTemp > vRef){
				vRef = vTemp;
				hRef = SERVO_get_position(*servoH);
				oRef = SERVO_get_position(*servoO);
			}

		}
	}
	//mis a jour final
	eolienne->currentHauteur = hRef;
	SERVO_set_position(servoH, hRef);
	eolienne->currentOrientation = oRef;
	SERVO_set_position(servoO, oRef);
	eolienne->tension = vRef;
	*okCalib = FALSE;

}

/**
 * @brief Prints eolienne parameters for debugging.
 *
 * This function prints eolienne parameters for debugging purposes.
 *
 * @param eolienne Pointer to the eolienne structure.
 */
void eolienne_print_param(eolienne_t * eolienne){
	printf("Envoi trame mode Eol: %d\n",eolienne->paramEolienne.mode);
	printf("Envoi trame mode csgMaxTempe: %d\n",eolienne->paramEolienne.csgMaxTempe);
	printf("Envoi trame mode csgMinTempe: %d\n",eolienne->paramEolienne.csgMinTempe);
	printf("Envoi trame mode tmpsPeriodeCalibrage: %d\n",eolienne->paramEolienne.tmpsPeriodeCalibrage);
	printf("Envoi trame mode tmpsSecurite: %d\n",eolienne->paramEolienne.tmpsSecurite);
	printf("Envoi trame currentHauteur: %d\n",eolienne->currentHauteur);//
	printf("Envoi trame currentOrientation: %d\n",eolienne->currentOrientation);
	printf("Envoi trame tension: %f\n",eolienne->tension);
}

/**
 * @brief Converts a float number into two separate bytes (integer and decimal parts).
 *
 * This function converts a float number into two separate bytes, representing the integer and decimal parts.
 *
 * @param num The float number to convert.
 * @return The two separate bytes representing the integer and decimal parts.
 */
uint8_t float_separateur_virgule(float num){
	uint8_t result[2];
	uint8_t pEntiere = (uint8_t)num;
	uint8_t pDecimal = 0;
	result[0] = pEntiere;
	float tempDecimal = (num - pEntiere)*100;
	pDecimal = (uint8_t)tempDecimal;
	result[1] = pDecimal;
	return result;
}


		
//On initialise tous les modules:
void initStateMachine(eolienne_t * eolienne,servo_t * servoHauteur,servo_t * servoOrientation,bool_e * okCalib,modeEolienne_t * oldMode){
	EOL_init();
	SERVO_init();
	BUTTON_init();
	NTC_init();
	flash_read_values(eolienne->memoireFlash, SIZE_BUFFER_MEMOIRE_FLASH);
	servoHauteur->servo_num = SERVO_0;
	servoOrientation->servo_num = SERVO_1;

	SERVO_set_position(servoOrientation,eolienne->memoireFlash[0]);
	SERVO_set_position(servoHauteur, eolienne->memoireFlash[1]);
	eolienne->paramEolienne.mode = eolienne->memoireFlash[2];
	*oldMode = eolienne->paramEolienne.mode;

	//On demmare l'eolienne en mode auto et on initalise tous ces parametre avec les valeurs de référence

	eolienne->paramEolienne.csgMaxTempe = 27;
	eolienne->paramEolienne.csgMinTempe = 5;
	eolienne->paramEolienne.tmpsPeriodeCalibrage = 100;
	eolienne->paramEolienne.tmpsSecurite = 100;
	eolienne->paramEolienne.tensionVentTropFort = 200;
	*okCalib = TRUE;

}

void receiveModeStateMachine(modeEolienne_t * oldMode,eolienne_t * eolienne,servo_t * servoHauteur,servo_t * servoOrientation,bool_e * okCalib,trame_struct * inputTrame,uint32_t *tCalibms,trame_struct *outputTrameMODE){
	getTrame(inputTrame);
	switch(inputTrame->type)//Navigation parmi les types de trame
		{
	case MODE:
		*oldMode = eolienne->paramEolienne.mode;

		if(inputTrame->data[0] == 1){
			eolienne->paramEolienne.mode = MODE_Manuel;
			eolienne->memoireFlash[2] = MODE_Manuel;
		}else if(inputTrame->data[0]==2){
			eolienne->paramEolienne.mode = MODE_Securite;
		}else{
			eolienne->paramEolienne.mode = MODE_Auto;
			eolienne->memoireFlash[2] = MODE_Auto;
			*tCalibms = 0;//Remise a 0 du temps en calibrage
		}
		state = MODE_StandBy;
		break;
	case SERVOX:
		if(inputTrame->data[0] == 0){
			SERVO_set_position(servoHauteur,inputTrame->data[1]);
			eolienne->currentHauteur = SERVO_get_position(*servoHauteur);
		}else{
			SERVO_set_position(servoOrientation,inputTrame->data[1]);
			eolienne->currentOrientation = SERVO_get_position(*servoOrientation);
		}
		state = MODE_StandBy;
		break;
	case PARAM:
		eolienne->paramEolienne.tmpsPeriodeCalibrage =(uint32_t)(inputTrame->data[0]<<8)+(inputTrame->data[1]);
		*tCalibms = 0;
		state = MODE_StandBy;
		break;
	case CALIBRAGE:
		if(*okCalib){
			calibrage(eolienne, servoHauteur, servoOrientation,okCalib);
		}
		eolienne->memoireFlash[0] = (uint8_t)SERVO_get_position(*servoOrientation);
		eolienne->memoireFlash[1] = (uint8_t)SERVO_get_position(*servoHauteur);
		state = MODE_StandBy;
		break;
	case RECHERCHE:
		outputTrameMODE->type = MODE;
		outputTrameMODE->data[0] = eolienne->paramEolienne.mode;
		putTrame(outputTrameMODE);
		state = MODE_StandBy;
		break;
	default:
		state = MODE_StandBy;
	}
}












