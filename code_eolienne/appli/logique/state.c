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
static trame_struct inputTrame;
static state_e state = INIT;
static adc_id_e ADC_EOL = ADC_9;
static adc_id_e ADC_Tempe = ADC_4;

void test_ms(void)
{
	if(comIRQ(UART2_ID)){
			//getTrame(&inputTrame);
			state = MODE_Receive;
		}
}


void state_machine(uint32_t *t, uint32_t *tCalibms,uint32_t *tModeSecu)
{
	static state_e state = INIT;
	static state_e previous_state = INIT;
	static eolienne_t eolienne;
	static servo_t servoHauteur;
	static servo_t servoOrientation;
	bool_e entrance = (state!=previous_state)?TRUE:FALSE;
	previous_state = state;
	static trame_struct outputTrameSERVOX;//Contient hauteur et orientation eolienne
	static trame_struct outputTrameDATA;//Contient tension eolienne et température
	static trame_struct outputTrameDEFAULT;//Contient les défaults de l'éolienne
	static trame_struct outputTrameMODE;
	static modeEolienne_t oldMode;
	switch(state)
		{
	case INIT:
		printf("INIT");
		//Systick_add_callback_function(&test_ms);
		//On initialise tous les modules:
		EOL_init();
		SERVO_init();
		BUTTON_init();
		servoHauteur.servo_num = SERVO_0;
		servoOrientation.servo_num = SERVO_1;
		SERVO_set_position(&servoOrientation, 0);
		SERVO_set_position(&servoHauteur, 0);
		state = MODE_StandBy;
		//On demmare l'eolienne en mode auto et on initalise tous ces parametre avec les valeurs de référence
		eolienne.paramEolienne.mode = MODE_Manuel;
		oldMode = MODE_Manuel;
		eolienne.paramEolienne.csgMaxTempe = 27;
		eolienne.paramEolienne.csgMinTempe = 5;
		eolienne.paramEolienne.tmpsPeriodeCalibrage = 100;
		eolienne.paramEolienne.tmpsSecurite = 100;
		eolienne.paramEolienne.tensionVentTropFort = 200;
		break;
	case MODE_Receive:
		printf("Mode_Receive");
		getTrame(&inputTrame);
		switch(inputTrame.type)//Navigation parmi les types de trame
			{
		case MODE:
			oldMode = eolienne.paramEolienne.mode;
			if(inputTrame.data[0] == 1){
				eolienne.paramEolienne.mode = MODE_Manuel;
			}else if(inputTrame.data[0]==2){
				eolienne.paramEolienne.mode = MODE_Securite;
			}else{
				eolienne.paramEolienne.mode = MODE_Auto;
				*tCalibms = 0;//Remise a 0 du temps en calibrage
			}
			state = MODE_StandBy;
			break;
		case SERVOX:
			if(inputTrame.data[0] == 0){
				SERVO_set_position(&servoHauteur,inputTrame.data[1]);
				eolienne.currentHauteur = SERVO_get_position(servoHauteur);
			}else{
				SERVO_set_position(&servoOrientation,inputTrame.data[1]);
				eolienne.currentOrientation = SERVO_get_position(servoOrientation);
			}
			state = MODE_StandBy;
			break;
		case PARAM:
			printf("DANS PARAM\n");
			//updateParam(&eolienne, &inputTrame);
			//eolienne.paramEolienne.tmpsPeriodeCalibrage = inputTrame.data[0];
			//calibrage(&eolienne, &servoHauteur, &servoOrientation);
			eolienne.paramEolienne.tmpsPeriodeCalibrage =(inputTrame.data[0]<<8)+(inputTrame.data[1]);
			*tCalibms = 0;
			state = MODE_StandBy;
			break;
		case CALIBRAGE:
			printf("DANS PARAM\n");
			//updateParam(&eolienne, &inputTrame);
			//eolienne.paramEolienne.tmpsPeriodeCalibrage = inputTrame.data[0];
			calibrage(&eolienne, &servoHauteur, &servoOrientation);
			state = MODE_StandBy;
			break;
		case RECHERCHE:
			outputTrameMODE.type = RECHERCHE;
			outputTrameDATA.data[0] = eolienne.paramEolienne.mode;
			putTrame(&outputTrameMODE);
			state = MODE_StandBy;
			break;
		default:
			state = MODE_StandBy;
		}
		break;
	case MODE_StandBy://On attend trame de l'IHM et envois l'etat de l'eol
		//On vérifie si on a recu une trame
		if(entrance){
			printf("Entre standby");
		}

		if(comIRQ(UART2_ID)){
			printf("Comme prete");
			state = MODE_Receive;
			break;
		}
		//BUTTON_state_machine();
		if(BUTTON_state_machine() == BUTTON_EVENT_SHORT_PRESS){
			calibrage(&eolienne, &servoHauteur, &servoOrientation);
		}

		if(*t<TIMER_MSG-100 && eolienne.paramEolienne.mode != MODE_Securite){
			if(eolienne.paramEolienne.mode == MODE_Manuel){
				//Led Rouge = mode manuel
				if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_14)){
					HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,0);
				}
			}else if(eolienne.paramEolienne.mode == MODE_Auto){
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

				eolienne.tension = EOL_tension_V(EOL_getValue(ADC_EOL))/1000;//EOL_getValue(ADC_EOL);//(float)35.6;//
				eolienne.temperature = NTC_getValue(ADC_Tempe)*10;//(float)219.0;//EOL_getValue(ADC_Tempe);

				outputTrameDATA.type = DATA;
				outputTrameDATA.data[1] = (uint16_t)eolienne.temperature & 0xFF;//tabTension[0];
				outputTrameDATA.data[0] = (uint8_t)((uint16_t)eolienne.temperature >>8);//tabTension[1];
				outputTrameDATA.data[3] = (uint16_t)eolienne.tension & 0xFF;//tabTempe[0];
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
				calibrage(&eolienne, &servoHauteur, &servoOrientation);
				*tCalibms = 0;
			}
			//Action mode auto : EOL imobile et tt les minutes recherches meilleur spot, si aucun mieux sport on bouge pas ou peu

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

	/*static trame_struct testTrame;
	if(comIRQ(UART2_ID)){
		getTrame(&testTrame);
	}*/

}
//gestion des défaut:

//Alocation des parametre de l'eolienne
void updateParam(eolienne_t *eolienne,trame_struct *trame){
	switch(trame->data[0]){//Navigation parmi les differents type de parametre
	case EOL_PARAM_MODE:
		printf("Update Mode\n");
		//param  = &eolienne->paramEolienne.mode;
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
	//param = trame->data[1];
}
//004 000 001 013 010
//001 056 013 010
//002 070 013 010


void calibrage(eolienne_t *eolienne,servo_t *servoH, servo_t *servoO ){
	//On cadrille une zonne de 10*10
	//printf("Entrée en calibrage\n");
	uint8_t hRef = SERVO_get_position(*servoH);
	uint8_t oRef = SERVO_get_position(*servoO);
	uint8_t vRef = eolienne->tension;//EOL_getValue(ADC_9);
	uint8_t h = 0;
	uint8_t o = 0;
	uint8_t vTemp = 0;

	for(o=0 ; o<=2 ; o++){
		//printf("O++\n");
		SERVO_set_position(servoO, o*10);
		for(h=0 ; h<=10 ; h++){
			//printf("H++\n");
			SERVO_set_position(servoH, h*10);
			vTemp = EOL_tension_V(EOL_getValue(ADC_EOL))/1000;//EOL_getValue(ADC_EOL);
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
	SERVO_set_position(&servoH, hRef);
	eolienne->currentOrientation = oRef;
	SERVO_set_position(&servoH, oRef);
	eolienne->tension = vRef;
	videUart();

}
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

uint8_t float_separateur_virgule(float num){//result[0] = parti entiere, result[1] = parti decimal
	uint8_t result[2];
	uint8_t pEntiere = (uint8_t)num;
	uint8_t pDecimal = 0;
	result[0] = pEntiere;
	float tempDecimal = (num - pEntiere)*100;
	pDecimal = (uint8_t)tempDecimal;
	result[1] = pDecimal;
	return result;
}


