/**
  ******************************************************************************
  * @file    interface.c
  * @author  GAILLARD Axel
  * @date    may-2024
  * @brief   interface screen functions.
  ******************************************************************************
*/
#include "interface.h"

static volatile uint32_t absolute_t = 0;

button_t button_tab[NBR_OF_BUTTON] = {{BUTTON_1_X1,BUTTON_1_X2,BUTTON_1_Y1,BUTTON_1_Y2,0,BUTTON_1_text,BUTTON_1_color},{BUTTON_2_X1,BUTTON_2_X2,BUTTON_2_Y1,BUTTON_2_Y2,0,BUTTON_2_text,BUTTON_2_color},{BUTTON_3_X1,BUTTON_3_X2,BUTTON_3_Y1,BUTTON_3_Y2,0,BUTTON_3_text,BUTTON_3_color},{BUTTON_C_X1,BUTTON_C_X2,BUTTON_C_Y1,BUTTON_C_Y2,0,BUTTON_C_text,BUTTON_C_color}};

button_t button_param_tab[NBR_OF_BUTTON_CALIB] = {{BUTTON_P_X1,BUTTON_P_X2,BUTTON_P_Y1,BUTTON_P_Y2,0,BUTTON_P_text,BUTTON_P_color},{BUTTON_M_X1,BUTTON_M_X2,BUTTON_M_Y1,BUTTON_M_Y2,0,BUTTON_M_text,BUTTON_M_color},{BUTTON_V_X1,BUTTON_V_X2,BUTTON_V_Y1,BUTTON_V_Y2,0,BUTTON_V_text,BUTTON_V_color}};

button_t button_mode_tab[NBR_OF_BUTTON_MODE] = {{BUTTON_AM_X1,BUTTON_AM_X2,BUTTON_AM_Y1,BUTTON_AM_Y2,0,BUTTON_AM_text,BUTTON_AM_color},{BUTTON_MM_X1,BUTTON_MM_X2,BUTTON_MM_Y1,BUTTON_MM_Y2,0,BUTTON_MM_text,BUTTON_MM_color},{BUTTON_C_A_X1,BUTTON_C_A_X2,BUTTON_C_A_Y1,BUTTON_C_A_Y2,0,BUTTON_C_A_text,BUTTON_C_A_color}};

button_t button_calib_auto_tab[NBR_OF_BUTTON_CALIB_AUTO] = {{BUTTON_C_A_A_X1,BUTTON_C_A_A_X2,BUTTON_C_A_A_Y1,BUTTON_C_A_A_Y2,0,BUTTON_C_A_A_text,BUTTON_C_A_A_color}};
pot_t pot_tab[NBR_OF_POT] = {{POT_1_X1,POT_1_X2,POT_1_Y1,POT_1_Y2,0},{POT_2_X1,POT_2_X2,POT_2_Y1,POT_2_Y2,0}};

acq_tab_t acq_tab[NBR_OF_TAB] = {{ACQ_M_X1,ACQ_M_X2,ACQ_M_Y1,ACQ_M_Y2},{ACQ_A_X1,ACQ_A_X2,ACQ_A_Y1,ACQ_A_Y2}};

param_t param = {(float)0.0, (float)0.0 , 10 , 100, 0,0,DFLT_T_CALIBRAGE,RESEARCH_MODE, FALSE};

uint8_t tab_values_volt[SIZE_TAB];

static uint16_t background_color	    = ILI9341_COLOR_WHITE;
static uint16_t text_color       	    = ILI9341_COLOR_BLACK;
static state_interface_e state          = HOME;
static state_interface_e previous_state = HOME;

static void INTERFACE_TFT_process_ms();

/**
 * @brief 
 * 
 * @param new_mode 
 */
void fill_mode_param(state_interface_e new_mode)
{
	param.mode = new_mode;
	param.flag = TRUE;
}

/**
 * @brief 
 * 
 * @param temperature 
 * @param voltage 
 * @param inclinaison 
 * @param hauteur 
 * @param DTC 
 * @param DTC_nbr 
 */
void fill_values_param(float temperature, float voltage, uint8_t inclinaison, uint8_t hauteur, uint8_t DTC, uint8_t DTC_nbr)
{
	if(hauteur != 0xFF)
		param.hauteur = hauteur;
	if(inclinaison != 0xFF)
		param.inclinaison = inclinaison;
	if(DTC_nbr != 0xFF)
		param.DTC_nbr = DTC_nbr;
	if(DTC != 0xFF)
		param.DTC = DTC;
	if(temperature != 255.0)
		param.temp = temperature;
	if(voltage != 255.0)
	{
		param.voltage = voltage;
		fill_voltage_tab((uint8_t)voltage);
	}
	param.flag = TRUE;
}

/**
 * @brief 
 * 
 * @param voltage 
 */
void fill_voltage_tab(uint8_t voltage)
{
	uint8_t i = 0;
	for(i = 0; i<SIZE_TAB; i++)
	{
		if(tab_values_volt[i] == 0xFF)
		{
			tab_values_volt[i] = voltage;
			break;
		}
	}
	if(i == SIZE_TAB)
	{
		for(i = 0; i<SIZE_TAB; i++)
		{
			if(i != SIZE_TAB-1)
			{
				tab_values_volt[i] = tab_values_volt[i+1];
			}
			else
			{
				tab_values_volt[i] = voltage;
			}
		}
	}
}

/**
 * @brief 
 * 
 * @param tab 
 * @param background_color 
 */
void fill_values_acquisition_tab(acq_tab_t *tab,uint16_t background_color)
{
	int16_t hauteur = (int16_t)((int16_t)(tab->y2-5) - (int16_t)(tab->y1+(tab->y2-tab->y1)*0.25));
	int16_t largeur = (int16_t)((int16_t)(tab->x1+(tab->x2-tab->x1)*0.80) - (int16_t)(tab->x1+5));
	int16_t largeur_entre = (int16_t)(largeur/SIZE_TAB);
	int16_t paddingX = (int16_t)(tab->x1+9);
	int16_t paddingY = (int16_t)(tab->y2-5);

	ILI9341_DrawFilledRectangle((uint16_t)(paddingX-3),(uint16_t)(paddingY-hauteur+5),(uint16_t)(paddingX+largeur-5),(uint16_t)(paddingY-2),background_color);
	int16_t lastdotX;
	int16_t lastdotY;
	int16_t dotX;
	int16_t dotY;
	for(int8_t i=0; i<SIZE_TAB;i++)
	{
		if(tab_values_volt[i] != 0xFF)
		{
			dotX = (int16_t)(largeur_entre*i + paddingX);
			dotY = (int16_t)(paddingY - (int16_t)(hauteur*tab_values_volt[i]/12));
			//print_dot(dotX,dotY);
			if(i!=0)
			{
				ILI9341_DrawLine((uint16_t)lastdotX,(uint16_t)lastdotY,(uint16_t)dotX,(uint16_t)dotY,ILI9341_COLOR_RED);
				HAL_Delay(1);
			}
			lastdotX = dotX;
			lastdotY = dotY;
		}

	}
}

/**
 * @brief 
 * 
 * @param button_param 
 * @param background_color 
 * @param text_color 
 */
void change_calib(button_t *button_param,uint16_t background_color,uint16_t text_color)
{
	if(button_param->text == '-')
	{
		param.T_calibrage = param.T_calibrage - 1;
	}
	else if (button_param->text == '+')
	{
		param.T_calibrage = param.T_calibrage + 1;
	}
	else if (button_param->text == 'V')
	{
		transmit_trame(uart,T_calibrage, dtc_none, mode_none, pot_none,(uint32_t)param.T_calibrage);
	}
	fill_t_calibrage(&param,background_color,text_color);
}

/**
 * @brief 
 * 
 * @param button_tab 
 * @param button_mode 
 * @param background_color 
 * @param text_color 
 */
void change_mode(button_t *button_tab,button_t *button_mode,uint16_t background_color,uint16_t text_color)
{
	if(button_mode->text == 'M')
	{
		param.mode = MANUAL_MODE;
		button_tab[2].color = ILI9341_COLOR_BLUE;
		button_tab[1].color = ILI9341_COLOR_RED;
		fill_button(&button_tab[2]);
		fill_button(&button_tab[1]);
		transmit_trame(uart,mode, dtc_none, MANUAL, pot_none,0);

	}
	else if (button_mode->text == 'A')
	{
		param.mode = AUTOMATICAL_MODE;
		button_tab[1].color = ILI9341_COLOR_BLUE;
		button_tab[2].color = ILI9341_COLOR_RED;
		fill_button(&button_tab[2]);
		fill_button(&button_tab[1]);
		transmit_trame(uart,mode, dtc_none, AUTO, pot_none,0);
	}
	else if (button_mode->text == 'C')
	{
		if(param.mode == AUTOMATICAL_MODE)
			transmit_trame(uart,calibrage, dtc_none, mode_none, pot_none,1);
	}
	if(state == HOME)
	{
		fill_mode(&param,background_color,text_color);
	}
}

/**
 * @brief 
 * 
 */
void change_mode_protected(void)
{

	if (param.mode == PROTECTED_MODE || param.mode == RESEARCH_MODE)
	{
		state = HOME;
		button_tab[1].color = ILI9341_COLOR_BLUE;
		button_tab[2].color = ILI9341_COLOR_BLUE;
	}
	if (param.mode == AUTOMATICAL_MODE)
	{
		button_tab[1].color = ILI9341_COLOR_BLUE;
		button_tab[2].color = ILI9341_COLOR_RED;
	}
	if (param.mode == MANUAL_MODE)
	{
		button_tab[1].color = ILI9341_COLOR_RED;
		button_tab[2].color = ILI9341_COLOR_BLUE;
	}
	fill_button(&button_tab[2]);
	fill_button(&button_tab[1]);
}

/**
 * @brief 
 * 
 * @return state_interface_e* 
 */
state_interface_e * INTERFACE_TFT_init(void)
{
	Systick_add_callback_function(&INTERFACE_TFT_process_ms);

	ILI9341_Init();	//initialisation de l'�cran TFT
	ILI9341_Rotate(ILI9341_Orientation_Landscape_2);

	//change_mode(button_tab,&button_mode_tab[1],background_color,text_color);

	INTERFACE_TFT_init_home(background_color,text_color);

	XPT2046_init();	//initialisation du tactile

	for(uint8_t i = 0; i<SIZE_TAB; i++)
	{
		tab_values_volt[i] = 0xFF;
	}

	return &param.mode;
}

/**
 * @brief 
 * 
 * @param background_color 
 */
void INTERFACE_TFT_init_page(uint16_t background_color)
{
	ILI9341_Fill(background_color);
	ILI9341_DrawFilledRectangle(MENU_X1,MENU_Y1,MENU_X2,MENU_Y2,ILI9341_COLOR_BLUE2);

	for(int16_t i = 0;i<NBR_OF_BUTTON; i++ )
	{
		fill_button(&button_tab[i]);
	}
}

/**
 * @brief 
 * 
 * @param i 
 * @param background_color 
 * @param text_color 
 */
void INTERFACE_TFT_init_automatical_page(int16_t i,uint16_t background_color,uint16_t text_color)
{
	INTERFACE_TFT_init_page(background_color);
	ILI9341_Puts(140,15, "AUTO PAGE", &Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLUE2);
	fill_acquisition_tab(&acq_tab[i], background_color, text_color);
	fill_temp_volt(AUTOMATICAL_MODE);
	fill_values(state, &param, background_color, text_color);
	fill_values_acquisition_tab(&acq_tab[1],background_color);
	for(int16_t i = 0;i<NBR_OF_BUTTON_CALIB_AUTO; i++ )
	{
		fill_button(&button_calib_auto_tab[i]);
	}
}

/**
 * @brief 
 * 
 * @param background_color 
 * @param text_color 
 */
void INTERFACE_TFT_init_home(uint16_t background_color,uint16_t text_color)
{
	INTERFACE_TFT_init_page(background_color);
	ILI9341_Puts(140,15, "HOME PAGE", &Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLUE2);

	fill_param(button_param_tab,button_mode_tab);
	fill_t_calibrage(&param,background_color,text_color);
	fill_mode(&param,background_color,text_color);
	fill_temp_volt(HOME);
	fill_DTC();
	fill_values(state, &param, background_color, text_color);
	if(param.DTC_nbr)
	{
		fill_DTC_str(&param, background_color, text_color);
	}
}

/**
 * @brief 
 * 
 * @param i 
 * @param background_color 
 * @param text_color 
 */
void INTERFACE_TFT_init_manual_page(int16_t i,uint16_t background_color,uint16_t text_color)
{
	INTERFACE_TFT_init_page(background_color);
	ILI9341_Puts(140,15, "MAN PAGE", &Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLUE2);

	for(int16_t i = 0;i<NBR_OF_POT; i++ )
	{
		fill_pot(&pot_tab[i]);
	}
	fill_acquisition_tab(&acq_tab[i],background_color,text_color);
	fill_temp_volt(MANUAL_MODE);
	fill_values(state, &param, background_color, text_color);
	fill_values_acquisition_tab(&acq_tab[0],background_color);
}

/**
 * @brief 
 * 
 */
void interface_state_machine()
{
	bool_e entrance = (state!=previous_state)?TRUE:FALSE;

		previous_state = state;
		int16_t x, y;
		int16_t i = NBR_OF_BUTTON;
		int16_t d = NBR_OF_BUTTON_CALIB;
		int16_t e = NBR_OF_BUTTON_MODE;
		int16_t f = NBR_OF_BUTTON_CALIB_AUTO;
		int16_t j = NBR_OF_POT;

		if(param.flag)
		{
			if(state == MANUAL_MODE)
			{
				fill_values_acquisition_tab(&acq_tab[0],background_color);
			}
			else if(state == AUTOMATICAL_MODE)
			{
				fill_values_acquisition_tab(&acq_tab[1],background_color);
			}
			if(state == HOME)
			{
				fill_mode(&param,background_color,text_color);
				fill_DTC_str(&param, background_color, text_color);
			}
			if(param.mode == PROTECTED_MODE || param.mode == RESEARCH_MODE)
			{
				change_mode_protected();
			}
			fill_values(state, &param, background_color, text_color);
			param.flag = FALSE;
		}

		switch(state)
		{
			case HOME:
				if(entrance)
				{
					INTERFACE_TFT_init_home(background_color,text_color);
				}
				if(XPT2046_getMedianCoordinates(&x, &y, XPT2046_COORDINATE_SCREEN_RELATIVE))
				{
					//bouton principaux
					i = search_button( x, y,button_tab,NBR_OF_BUTTON);
					if(i !=NBR_OF_BUTTON)
					{
						pressed_button(&button_tab[i],absolute_t);
					}
					//bouton calib t
					d = search_button( x, y,button_param_tab,NBR_OF_BUTTON_CALIB);
					if(d !=NBR_OF_BUTTON_CALIB)
					{
						pressed_button(&button_param_tab[d],absolute_t);
						change_calib(&button_param_tab[d],background_color,text_color);
					}
					//bouton mode
					e = search_button( x, y,button_mode_tab,NBR_OF_BUTTON_MODE);
					if(e !=NBR_OF_BUTTON_MODE)
					{
						pressed_button(&button_mode_tab[e],absolute_t);
						change_mode(button_tab,&button_mode_tab[e],background_color,text_color);
					}
				}
				search_time_over(absolute_t,button_tab,NBR_OF_BUTTON);
				search_time_over(absolute_t,button_param_tab,NBR_OF_BUTTON_CALIB);
				search_time_over(absolute_t,button_mode_tab,NBR_OF_BUTTON_MODE);
				if(i == BUTTON_COLOR)
				{
					uint16_t temp = background_color;
					background_color = text_color;
					text_color = temp;
					INTERFACE_TFT_init_home(background_color,text_color);
					state = HOME;
				}
				if(i == BUTTON_MANUAL && param.mode == MANUAL_MODE)
					state = MANUAL_MODE;
				if(i == BUTTON_AUTOMATIC && param.mode == AUTOMATICAL_MODE)
					state = AUTOMATICAL_MODE;
				break;
			case MANUAL_MODE:
				if(entrance)
				{
					INTERFACE_TFT_init_manual_page(TAB_M,background_color,text_color);
				}
				if(XPT2046_getMedianCoordinates(&x, &y, XPT2046_COORDINATE_SCREEN_RELATIVE))
				{
					i = search_button( x, y,button_tab,NBR_OF_BUTTON);
					if(i !=NBR_OF_BUTTON)
						pressed_button(&button_tab[i],absolute_t);
					j = search_pot( x, y,pot_tab);
					if(j !=NBR_OF_POT)
					{
						change_pot(&pot_tab[j], y);
						if(pot_tab[j].x1 == POT_1_X1)
						{
							param.hauteur = (uint8_t)(pot_tab[j].height);
							transmit_trame(uart,height, dtc_none, mode_none, hauteur,(uint32_t)param.hauteur);
						}
						if(pot_tab[j].x1 == POT_2_X1)
						{
							param.inclinaison = (uint8_t)(pot_tab[j].height);
							transmit_trame(uart,height, dtc_none, mode_none, inclinaison,(uint32_t)param.inclinaison);
						}
					}
				}
				search_time_over(absolute_t,button_tab,NBR_OF_BUTTON);
				if(i == BUTTON_COLOR)
				{
					uint16_t temp = background_color;
					background_color = text_color;
					text_color = temp;
					INTERFACE_TFT_init_manual_page(TAB_M, background_color, text_color);
					state = MANUAL_MODE;
				}
				if(i == BUTTON_HOME)
					state = HOME;
				if(i == BUTTON_AUTOMATIC && param.mode == AUTOMATICAL_MODE)
					state = AUTOMATICAL_MODE;
				break;
			case AUTOMATICAL_MODE:
				if(entrance)
				{
					INTERFACE_TFT_init_automatical_page(TAB_A, background_color, text_color);
				}
				if(XPT2046_getMedianCoordinates(&x, &y, XPT2046_COORDINATE_SCREEN_RELATIVE))
				{
					i = search_button( x, y,button_tab,NBR_OF_BUTTON);
					if(i !=NBR_OF_BUTTON)
						pressed_button(&button_tab[i],absolute_t);
					//bouton mode
					f = search_button( x, y,button_calib_auto_tab,NBR_OF_BUTTON_CALIB_AUTO);
					if(f !=NBR_OF_BUTTON_CALIB_AUTO)
					{
						pressed_button(&button_calib_auto_tab[f],absolute_t);
						change_mode(button_tab,&button_calib_auto_tab[f],background_color,text_color);
					}
				}
				search_time_over(absolute_t,button_tab,NBR_OF_BUTTON);
				search_time_over(absolute_t,button_calib_auto_tab,NBR_OF_BUTTON_CALIB_AUTO);
				if(i == BUTTON_COLOR)
				{
					uint16_t temp = background_color;
					background_color = text_color;
					text_color = temp;
					INTERFACE_TFT_init_automatical_page(TAB_A, background_color, text_color);
					state = AUTOMATICAL_MODE;
				}
				if(i == BUTTON_HOME)
					state = HOME;
				if(i == BUTTON_MANUAL && param.mode == MANUAL_MODE)
					state = MANUAL_MODE;
				break;
			default:
				break;
		}
}

/**
 * @brief 
 * 
 */
static void INTERFACE_TFT_process_ms(void)
{
	absolute_t++;
	if(absolute_t >= 0xFFFFFF00)
		absolute_t = 0;
}

