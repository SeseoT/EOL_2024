/*
 * state.h
 *
 *  Created on: 26 mars 2024
 *      Author: Tanguy Suteau
 */

#ifndef LOGIC_STATE_H_
#define LOGIC_STATE_H_

#define TIMER_MSG 1000 //Temps entre chaque envois de trames
#define TIMER_BLINK_MSG 100 //Temps de clignotement de la led après un envois de trames
#define TIMER_MODE_SECU 2000 //Temps en mode sécu après un défautl

void state_machine(uint32_t *t,uint32_t *tCalibms,uint32_t *tModeSecu);
void test_ms(void);

typedef enum
	{
		INIT,
		MODE_Receive,
		MODE_StandBy,
	}state_e;

typedef enum
	{
		MODE_Auto,
		MODE_Manuel,
		MODE_Securite,
}modeEolienne_t;

typedef enum
	{
	EOL_DEF_SECU_VTF,//Vent trop puissant détecté donc passage en mode sécurité
	EOL_DEF_SECU,//En mode secu
	EOL_DEF_TEMP_H,//Température trop élevé
	EOL_DEF_TEMP_L,//Température trop basse
}defaut_list_t;

typedef enum
	{
	EOL_PARAM_MODE,	//Mode de l'eolienne
	EOL_PARAM_TMPS_M_AUTO,	//Temps en mode auto entre chaque période de recherche
	EOL_PARAM_TMPS_SECU,	//Temps en sécurité
	EOL_PARAM_TEMP_MAX,	//Consigne MAX Temp
	EOL_PARAM_TEMP_MIN,	//Consigne MIN Temp
}parametre_list_t;


typedef struct//Structure param courant de l'éolienne
	{
		modeEolienne_t mode;
		uint32_t tmpsPeriodeCalibrage;
		uint16_t tmpsSecurite;
		uint16_t csgMaxTempe;
		uint16_t csgMinTempe;
		float tensionVentTropFort;
}param_eolienne_s;

typedef struct//Structure defaut courant de l'éolienne,1=Defaut,0=OK
	{
		uint8_t secu_VTF;
		uint8_t mode_secu_ON;
		uint8_t tempe_H;
		uint8_t tempe_L;
}defaut_eolienne_s;

typedef struct//Structure etat courant de l'éolienne
	{
		uint16_t tension;
		uint16_t currentHauteur;
		uint16_t currentOrientation;
		param_eolienne_s paramEolienne;
		defaut_eolienne_s defautEolienne;
		float temperature;
}eolienne_t;






#endif /* LOGIC_STATE_H_ */
