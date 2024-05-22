
#ifndef INTERFACE_TFT_H_
#define INTERFACE_TFT_H_

#include "config.h"
#if USE_SCREEN_TFT_ILI9341


#define NBR_OF_BUTTON 				4
#define NBR_OF_BUTTON_CALIB 		3
#define NBR_OF_BUTTON_MODE  		3
#define NBR_OF_BUTTON_CALIB_AUTO  	1

#define NBR_OF_POT 					2
#define NBR_OF_TAB 					2

#define BUTTON_HOME 				0
#define BUTTON_MANUAL 				1
#define BUTTON_AUTOMATIC 			2
#define BUTTON_COLOR     			3

#define BUTTON_SP 					0
#define BUTTON_SM 					1
#define BUTTON_P 					2
#define BUTTON_M     				3

#define TAB_M 						0
#define TAB_A 						1

#define SIZE_TAB 					20
#define DELAY_B 					200
#define DFLT_T_CALIBRAGE    		200


typedef enum
{
	HOME,
	AUTOMATICAL_MODE,
	MANUAL_MODE,
	PROTECTED_MODE,
	RESEARCH_EOL
}state_interface_e;

typedef struct
{
    uint16_t x1;
    uint16_t x2;
    uint16_t y1;
    uint16_t y2;
    uint32_t time;
    char text;
    uint16_t color;
}button_t;

typedef struct
{
    uint16_t x1;
    uint16_t x2;
    uint16_t y1;
    uint16_t y2;
    uint16_t height;
}pot_t;

typedef struct
{
    uint16_t x1;
    uint16_t x2;
    uint16_t y1;
    uint16_t y2;
}acq_tab_t;

typedef struct
{
    float temp;
    float voltage;
    uint8_t hauteur;
    uint8_t inclinaison;
    uint8_t DTC_nbr;
    uint8_t DTC;
    uint32_t T_calibrage;
    state_interface_e mode;
    bool_e flag;
}param_t;

int16_t search_button(int16_t x, int16_t y, button_t *button_tab, int8_t nb_button);

int16_t search_pot(int16_t x, int16_t y, pot_t *pot_tab);

void change_pot(pot_t *pot, int16_t y);

void pressed_button(button_t *button, uint32_t absolute_t);

void fill_button(button_t *button);

void fill_pot(pot_t *pot);

void fill_param(button_t *button_calib_tab,button_t *button_mode_tab);

void fill_temp_volt(state_interface_e state);

void fill_DTC(void);

void fill_acquisition_tab(acq_tab_t *tab, uint16_t background_color, uint16_t text_color);

void fill_values(state_interface_e state , param_t *param,uint16_t background_color,uint16_t text_color);

void fill_t_calibrage(param_t *param,uint16_t background_color,uint16_t text_color);

void fill_mode(param_t *param,uint16_t background_color,uint16_t text_color);

void fill_DTC_str(param_t *param,uint16_t background_color,uint16_t text_color);

void print_dot(int16_t x, int16_t y);

void search_time_over(uint32_t absolute_t,button_t *button_tab, int8_t nb_button);


//button 1
#define BUTTON_1_X1 5
#define BUTTON_1_X2 45
#define BUTTON_1_Y1 5
#define BUTTON_1_Y2 45
#define BUTTON_1_text 'H'
#define BUTTON_1_color ILI9341_COLOR_RED

//button 2
#define BUTTON_2_X1 50
#define BUTTON_2_X2 90
#define BUTTON_2_Y1 5
#define BUTTON_2_Y2 45
#define BUTTON_2_text 'M'
#define BUTTON_2_color ILI9341_COLOR_RED

//button 3
#define BUTTON_3_X1 95
#define BUTTON_3_X2 135
#define BUTTON_3_Y1 5
#define BUTTON_3_Y2 45
#define BUTTON_3_text 'A'
#define BUTTON_3_color ILI9341_COLOR_BLUE

//button color
#define BUTTON_C_X1 270
#define BUTTON_C_X2 310
#define BUTTON_C_Y1 5
#define BUTTON_C_Y2 45
#define BUTTON_C_text 'C'
#define BUTTON_C_color ILI9341_COLOR_RED

//menu bar
#define MENU_X1 4
#define MENU_X2 311
#define MENU_Y1 4
#define MENU_Y2 46

//Potentiometer 1
#define POT_1_X1 200
#define POT_1_X2 250
#define POT_1_Y1 50
#define POT_1_Y2 230

//Potentiometer 2
#define POT_2_X1 260
#define POT_2_X2 310
#define POT_2_Y1 50
#define POT_2_Y2 230

//Acquisition tab manual
#define ACQ_M_X1 5
#define ACQ_M_X2 190
#define ACQ_M_Y1 50
#define ACQ_M_Y2 170

//Acquisition tab auto
#define ACQ_A_X1 5
#define ACQ_A_X2 205
#define ACQ_A_Y1 50
#define ACQ_A_Y2 230

//Param rec Home
#define P_R_X1 5
#define P_R_X2 205
#define P_R_Y1 50
#define P_R_Y2 170

//Param rec menu
#define P_R_0_X1 5
#define P_R_0_X2 205
#define P_R_0_Y1 110
#define P_R_0_Y2 130

//Param rec int
#define P_R_1_X1 6
#define P_R_1_X2 204
#define P_R_1_Y1 51
#define P_R_1_Y2 120

//Param rec value
#define P_R_2_X1 5
#define P_R_2_X2 205
#define P_R_2_Y1 121
#define P_R_2_Y2 169


//T calibrage affichage
#define P_1_X1 10
#define P_1_X2 95
#define P_1_Y1 135
#define P_1_Y2 165

//button +
#define BUTTON_P_X1 135
#define BUTTON_P_X2 165
#define BUTTON_P_Y1 135
#define BUTTON_P_Y2 165
#define BUTTON_P_text '+'
#define BUTTON_P_color ILI9341_COLOR_RED

//button -
#define BUTTON_M_X1 100
#define BUTTON_M_X2 130
#define BUTTON_M_Y1 135
#define BUTTON_M_Y2 165
#define BUTTON_M_text '-'
#define BUTTON_M_color ILI9341_COLOR_RED

//button S +
#define BUTTON_V_X1 170
#define BUTTON_V_X2 200
#define BUTTON_V_Y1 135
#define BUTTON_V_Y2 165
#define BUTTON_V_text 'V'
#define BUTTON_V_color ILI9341_COLOR_GREEN

//MODE menu
#define M_0_X1 5
#define M_0_X2 205
#define M_0_Y1 50
#define M_0_Y2 70

//MODE affichage
#define M_1_X1 10
#define M_1_X2 95
#define M_1_Y1 75
#define M_1_Y2 105

//button AUTO
#define BUTTON_AM_X1 135
#define BUTTON_AM_X2 165
#define BUTTON_AM_Y1 75
#define BUTTON_AM_Y2 105
#define BUTTON_AM_text 'A'
#define BUTTON_AM_color ILI9341_COLOR_RED

//button MANUEL
#define BUTTON_MM_X1 100
#define BUTTON_MM_X2 130
#define BUTTON_MM_Y1 75
#define BUTTON_MM_Y2 105
#define BUTTON_MM_text 'M'
#define BUTTON_MM_color ILI9341_COLOR_RED

//button CALIBRAGE AUTO
#define BUTTON_C_A_X1 170
#define BUTTON_C_A_X2 200
#define BUTTON_C_A_Y1 75
#define BUTTON_C_A_Y2 105
#define BUTTON_C_A_text 'C'
#define BUTTON_C_A_color ILI9341_COLOR_RED


//button CALIBRAGE AUTO ON MODE AUTO
#define BUTTON_C_A_A_X1 160
#define BUTTON_C_A_A_X2 200
#define BUTTON_C_A_A_Y1 55
#define BUTTON_C_A_A_Y2 95
#define BUTTON_C_A_A_text 'C'
#define BUTTON_C_A_A_color ILI9341_COLOR_RED

//Value rec Home
#define V_R_X1 210
#define V_R_X2 310
#define V_R_Y1 50
#define V_R_Y2 170

//Value rec Temp
#define V_R_T_X1 210
#define V_R_T_X2 310
#define V_R_T_Y1 50
#define V_R_T_Y2 70

//Value rec Temp
#define V_R_TV_X1 210
#define V_R_TV_X2 310
#define V_R_TV_Y1 70
#define V_R_TV_Y2 110

//Value rec Volts
#define V_R_V_X1 210
#define V_R_V_X2 310
#define V_R_V_Y1 110
#define V_R_V_Y2 130

//Value rec Volts
#define V_R_VV_X1 210
#define V_R_VV_X2 310
#define V_R_VV_Y1 130
#define V_R_VV_Y2 170

//Value rec MAN Temp
#define V_MAN_T_X1 5
#define V_MAN_T_X2 65
#define V_MAN_T_Y1 175
#define V_MAN_T_Y2 230

//Value rec MAN Volts
#define V_MAN_V_X1 65
#define V_MAN_V_X2 125
#define V_MAN_V_Y1 175
#define V_MAN_V_Y2 230

//Value rec MAN DTC
#define V_MAN_D_X1 125
#define V_MAN_D_X2 190
#define V_MAN_D_Y1 175
#define V_MAN_D_Y2 230

//DTC HOME
#define DTC_X1 5
#define DTC_X2 210
#define DTC_Y1 175
#define DTC_Y2 230

//DTC NBR
#define DTC_N_X1 211
#define DTC_N_X2 310
#define DTC_N_Y1 175
#define DTC_N_Y2 230


#define STR_T_MAX  "la temperature mon reuf"
#define STR_T_MIN  "la temperature mon keuf"
#define STR_V_MAX  "la tension mon reuf    "
#define STR_NO_EOL "AUCUNE EOLIENNE TROUVE "

#define STR_SIZE 24

#endif
#endif /* INTERFACE_TFT_H_ */

