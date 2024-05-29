/**
  ******************************************************************************
  * @file    interface_tft.c
  * @author  GAILLARD Axel
  * @date    april-2024
  * @brief   draw specifics shapes on tft screen functions.
  ******************************************************************************
*/

#include "config.h"
#if USE_SCREEN_TFT_ILI9341
#include "interface_tft.h"

/**
 * @brief draw a button on a tft screen
 * 
 * @param button 
 */
void fill_button(button_t *button)
{
	ILI9341_DrawFilledRectangle(button->x1,button->y1,button->x2,button->y2,ILI9341_COLOR_BLUE);
	ILI9341_DrawFilledRectangle((uint16_t)(button->x1+(button->x2-button->x1)/4),(uint16_t)(button->y1+(button->y2-button->y1)/4),(uint16_t)(button->x2-(button->x2-button->x1)/4),(uint16_t)(button->y2-(button->y2-button->y1)/4),button->color);
	ILI9341_Putc((uint16_t)(button->x1+(button->x2-button->x1)/4 + 7),(uint16_t)(button->y1+(button->y2-button->y1)/4 + 5),button->text,&Font_7x10,ILI9341_COLOR_WHITE,button->color);
}

/**
 * @brief draw a potentiometer on a tft screen
 * 
 * @param pot 
 */
void fill_pot(pot_t *pot)
{
	ILI9341_DrawFilledRectangle(pot->x1,pot->y1,pot->x2,pot->y2,ILI9341_COLOR_BLUE);
	ILI9341_DrawFilledRectangle((uint16_t)(pot->x1+(pot->x2-pot->x1)/4),(uint16_t)(pot->y1+(pot->y2-pot->y1)/10),(uint16_t)(pot->x2-(pot->x2-pot->x1)/4),(uint16_t)(pot->y2-(pot->y2-pot->y1)/10),ILI9341_COLOR_BLUE2);

	uint16_t pot_size = (uint16_t)(((pot->y2-(pot->y2-pot->y1)/10) - (pot->y1+(pot->y2-pot->y1)/10))-30);
	uint16_t y1 = (uint16_t)((pot->height*pot_size)/100 + (pot->y1+(pot->y2-pot->y1)/10));
	ILI9341_DrawFilledRectangle((uint16_t)(pot->x1+(pot->x2-pot->x1)/4),y1,(uint16_t)(pot->x2-(pot->x2-pot->x1)/4),(uint16_t)(y1 + 30),ILI9341_COLOR_GRAY);
}

/**
 * @brief draw an acquisition table on a tft screen
 * 
 * @param tab 
 * @param background_color 
 * @param text_color 
 */
void fill_acquisition_tab(acq_tab_t *tab, uint16_t background_color, uint16_t text_color)
{
	ILI9341_DrawRectangle(tab->x1, tab->y1, tab->x2, tab->y2, ILI9341_COLOR_BLUE2);

	ILI9341_Puts((uint16_t)(tab->x1+5),(uint16_t)(tab->y1+5), "Volts acquisition", &Font_7x10, text_color, background_color);

	ILI9341_DrawCircle((int16_t)(tab->x1+5),(int16_t)(tab->y2-5),5,ILI9341_COLOR_BLUE);

	ILI9341_DrawLine((uint16_t)(tab->x1+5),(uint16_t)(tab->y1+(tab->y2-tab->y1)*0.25),(uint16_t)(tab->x1+5),(uint16_t)(tab->y2-5),ILI9341_COLOR_RED);
	ILI9341_DrawLine((uint16_t)(tab->x1+5),(uint16_t)(tab->y2-5),(uint16_t)(tab->x1+(tab->x2-tab->x1)*0.80),(uint16_t)(tab->y2-5),ILI9341_COLOR_RED);

	ILI9341_Puts((uint16_t)(tab->x1+2),(uint16_t)(tab->y1+(tab->y2-tab->y1)*0.25-3),"U(v)",&Font_7x10,text_color,background_color);
	ILI9341_Puts((uint16_t)(tab->x1+(tab->x2-tab->x1)*0.80),(uint16_t)(tab->y2-12),"T(s)",&Font_7x10,text_color,background_color);
}

/**
 * @brief draw a 3 pixels radius dot on a tft screen
 * 
 * @param x coordinate of center circle point
 * @param y coordinate of center circle point
 */
void print_dot(int16_t x, int16_t y)
{
	ILI9341_DrawCircle(x,y,3,ILI9341_COLOR_RED);
}

/**
 * @brief draw the parameter area on a tft screen
 * 
 * @param button_calib_tab 
 * @param button_mode_tab 
 */
void fill_param(button_t *button_calib_tab,button_t *button_mode_tab)
{
	ILI9341_DrawRectangle(P_R_X1, P_R_Y1, P_R_X2, P_R_Y2, ILI9341_COLOR_BLUE2);
	ILI9341_DrawRectangle(P_R_1_X1, P_R_1_Y1, P_R_1_X2, P_R_1_Y2, ILI9341_COLOR_BLUE2);
	ILI9341_DrawRectangle(P_R_2_X1, P_R_2_Y1, P_R_2_X2, P_R_2_Y2, ILI9341_COLOR_BLUE2);
	ILI9341_DrawFilledRectangle(P_R_0_X1, P_R_0_Y1, P_R_0_X2, P_R_0_Y2, ILI9341_COLOR_BLUE2);
	ILI9341_Puts(P_R_0_X1+7,P_R_0_Y1+5, "Temps de calibrage auto", &Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLUE2);
	ILI9341_DrawRectangle(P_1_X1, P_1_Y1, P_1_X2, P_1_Y2, ILI9341_COLOR_BLUE2);

	ILI9341_DrawFilledRectangle(M_0_X1, M_0_Y1, M_0_X2, M_0_Y2, ILI9341_COLOR_BLUE2);
	ILI9341_Puts(M_0_X1+7,M_0_Y1+5, "mode et calibrage", &Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLUE2);
	ILI9341_DrawRectangle(M_1_X1, M_1_Y1, M_1_X2, M_1_Y2, ILI9341_COLOR_BLUE2);

	for(int16_t i = 0;i<NBR_OF_BUTTON_CALIB; i++ )
	{
		fill_button(&button_calib_tab[i]);
	}

	for(int16_t i = 0;i<NBR_OF_BUTTON_MODE; i++ )
	{
		fill_button(&button_mode_tab[i]);
	}

}

/**
 * @brief draw the values area on a tft screen
 * 
 * @param state 
 * @param param 
 * @param background_color 
 * @param text_color 
 */
void fill_values(state_interface_e state , param_t *param,uint16_t background_color,uint16_t text_color)
{
	char str[10];
	if(state == MANUAL_MODE)
	{
		ILI9341_DrawFilledRectangle(V_MAN_T_X1+1, V_MAN_T_Y1+21, V_MAN_T_X2-1, V_MAN_T_Y2-1, background_color);
		ILI9341_DrawFilledRectangle(V_MAN_V_X1+1, V_MAN_V_Y1+21, V_MAN_V_X2-1, V_MAN_V_Y2-1, background_color);
		ILI9341_DrawFilledRectangle(V_MAN_D_X1+1, V_MAN_D_Y1+21, V_MAN_D_X2-1, V_MAN_D_Y2-1, background_color);

        sprintf(str, "%2.1f", param->temp);

		ILI9341_Puts(V_MAN_T_X1+7,V_MAN_T_Y1+25, str, &Font_7x10, text_color, background_color);
		sprintf(str, "%2.1f", param->voltage);

		ILI9341_Puts(V_MAN_V_X1+7,V_MAN_V_Y1+25, str, &Font_7x10, text_color, background_color);

		sprintf(str, "%d", param->DTC_nbr);

		ILI9341_Puts(V_MAN_D_X1+7,V_MAN_D_Y1+25, str, &Font_7x10, text_color, background_color);
	}
	else if(state == AUTOMATICAL_MODE || state == HOME)
	{
		ILI9341_DrawFilledRectangle(V_R_TV_X1+1, V_R_TV_Y1+1, V_R_TV_X2-1, V_R_TV_Y2-1, background_color);
		ILI9341_DrawFilledRectangle(V_R_VV_X1+1, V_R_VV_Y1+1, V_R_VV_X2-1, V_R_VV_Y2-1, background_color);
        sprintf(str, "%2.1fc", param->temp);
		ILI9341_Puts(V_R_T_X1+7,V_R_T_Y1+25, str, &Font_7x10, text_color, background_color);
		sprintf(str, "%2.1f V", param->voltage);
		ILI9341_Puts(V_R_V_X1+7,V_R_V_Y1+25, str, &Font_7x10, text_color, background_color);

		if(state == AUTOMATICAL_MODE)
		{
			ILI9341_DrawFilledRectangle(V_R_V_X1+1, DTC_N_Y1+21, DTC_N_X2-1, DTC_N_Y2-1, background_color);
			sprintf(str, "%d", param->DTC_nbr);
			ILI9341_Puts(V_R_V_X1+7,DTC_N_Y1+25, str, &Font_7x10, text_color, background_color);
		}
		else
		{
			sprintf(str, "%d", param->DTC_nbr);
			ILI9341_DrawFilledRectangle(DTC_N_X1+1, DTC_N_Y1+21, DTC_N_X2-1, DTC_N_Y2-1, background_color);
			ILI9341_Puts(DTC_N_X1+7,DTC_N_Y1+25, str, &Font_7x10, text_color, background_color);
		}
	}
}

/**
 * @brief draw the right string of the calibrage time on a tft screen
 * 
 * @param param 
 * @param background_color 
 * @param text_color 
 */
void fill_t_calibrage(param_t *param,uint16_t background_color,uint16_t text_color)
{
	char str[10];
	sprintf(str, "%d s   ", (int)param->T_calibrage);
	ILI9341_Puts(P_1_X1+7,P_1_Y1+10, str, &Font_7x10, text_color, background_color);
}

/**
 * @brief draw the mode string on a param area on a tft screen
 * 
 * @param param 
 * @param background_color 
 * @param text_color 
 */
void fill_mode(param_t *param,uint16_t background_color,uint16_t text_color)
{
	char str[11];
	if(param->mode == MANUAL_MODE)
		sprintf(str, "manuel    ");
	else if(param->mode == AUTOMATICAL_MODE)
		sprintf(str, "auto      ");
	else if(param->mode == PROTECTED_MODE)
	{
		sprintf(str, "protection");
		text_color = ILI9341_COLOR_WHITE;
		background_color = ILI9341_COLOR_RED;
	}
	else if(param->mode == RESEARCH_MODE)
	{
		sprintf(str, "recherche ");
		text_color = ILI9341_COLOR_WHITE;
		background_color = ILI9341_COLOR_RED;
	}
	else
		sprintf(str, "          ");
	ILI9341_DrawFilledRectangle(M_1_X1+1, M_1_Y1+1, M_1_X2-1, M_1_Y2-1, background_color);
	ILI9341_Puts(M_1_X1+7,M_1_Y1+10, str, &Font_7x10, text_color, background_color);
}

/**
 * @brief draw the error string on a error area on a tft screen
 * 
 * @param param 
 * @param background_color 
 * @param text_color 
 */
void fill_DTC_str(param_t *param,uint16_t background_color,uint16_t text_color)
{
	char str[STR_SIZE];

	if(param->DTC)
		{
			param->DTC_nbr++;
			background_color = ILI9341_COLOR_RED;
			text_color = ILI9341_COLOR_WHITE;
		}

	if(param->DTC_nbr)
	{
		switch(param->DTC)
		{
			case 0:
				param->DTC = 0;
				param->DTC_nbr = 0;
				sprintf(str, "                       ");
				break;
			case 1:
				sprintf(str, STR_T_MAX);
				break;
			case 2:
				sprintf(str, STR_T_MIN);
				break;
			case 3:
				sprintf(str, STR_V_MAX);
				break;
			default:
				sprintf(str, "ERROR");
				break;
		}
	}
	else
	{
		sprintf(str, "                       ");
	}

	ILI9341_DrawFilledRectangle(DTC_X1+1, DTC_Y1+21, DTC_X2-1, DTC_Y2-1, background_color);
	ILI9341_Puts(DTC_X1+7,DTC_Y1+25, str, &Font_7x10, text_color, background_color);
}

/**
 * @brief draw the temperature and voltage on a values area on a tft screen
 * 
 * @param state 
 */
void fill_temp_volt(state_interface_e state)
{
	if(state == AUTOMATICAL_MODE || state == HOME)
	{
		ILI9341_DrawRectangle(V_R_X1, V_R_Y1, V_R_X2, V_R_Y2, ILI9341_COLOR_BLUE2);

		ILI9341_DrawFilledRectangle(V_R_T_X1, V_R_T_Y1, V_R_T_X2, V_R_T_Y2, ILI9341_COLOR_BLUE2);
		ILI9341_DrawRectangle(V_R_TV_X1, V_R_TV_Y1, V_R_TV_X2, V_R_TV_Y2, ILI9341_COLOR_BLUE2);
		ILI9341_Puts(V_R_T_X1+7,V_R_T_Y1+5, "Temperature", &Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLUE2);

		ILI9341_DrawFilledRectangle(V_R_V_X1, V_R_V_Y1, V_R_V_X2, V_R_V_Y2, ILI9341_COLOR_BLUE2);
		ILI9341_DrawRectangle(V_R_VV_X1, V_R_VV_Y1, V_R_VV_X2, V_R_VV_Y2, ILI9341_COLOR_BLUE2);
		ILI9341_Puts(V_R_V_X1+7,V_R_V_Y1+5, "Voltage", &Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLUE2);
	}
	if(state == MANUAL_MODE)
	{
		ILI9341_DrawRectangle(V_MAN_T_X1, V_MAN_T_Y1, V_MAN_T_X2, V_MAN_T_Y2, ILI9341_COLOR_BLUE2);
		ILI9341_DrawFilledRectangle(V_MAN_T_X1, V_MAN_T_Y1, V_MAN_T_X2, V_MAN_T_Y1+20, ILI9341_COLOR_BLUE2);
		ILI9341_Puts(V_MAN_T_X1+7,V_MAN_T_Y1+5, "Temp", &Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLUE2);

		ILI9341_DrawRectangle(V_MAN_V_X1, V_MAN_V_Y1, V_MAN_V_X2, V_MAN_V_Y2, ILI9341_COLOR_BLUE2);
		ILI9341_DrawFilledRectangle(V_MAN_V_X1, V_MAN_V_Y1, V_MAN_V_X2, V_MAN_V_Y1+20, ILI9341_COLOR_BLUE2);
		ILI9341_Puts(V_MAN_V_X1+7,V_MAN_V_Y1+5, "Volt", &Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLUE2);

		ILI9341_DrawRectangle(V_MAN_D_X1, V_MAN_D_Y1, V_MAN_D_X2, V_MAN_D_Y2, ILI9341_COLOR_BLUE2);
		ILI9341_DrawFilledRectangle(V_MAN_D_X1, V_MAN_D_Y1, V_MAN_D_X2, V_MAN_D_Y1+20, ILI9341_COLOR_BLUE2);
		ILI9341_Puts(V_MAN_D_X1+7,V_MAN_D_Y1+5, "DTC", &Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLUE2);
	}
	if(state == AUTOMATICAL_MODE)
	{
		ILI9341_DrawRectangle(V_R_V_X1, DTC_N_Y1, DTC_N_X2, DTC_N_Y2, ILI9341_COLOR_BLUE2);
		ILI9341_DrawFilledRectangle(V_R_V_X1, DTC_N_Y1, DTC_N_X2, DTC_N_Y1+20, ILI9341_COLOR_BLUE2);
		ILI9341_Puts(V_R_V_X1+7,DTC_N_Y1+5, "TOTAL DTC", &Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLUE2);
	}
}

/**
 * @brief draw the error area on a tft screen
 * 
 */
void fill_DTC(void)
{
	ILI9341_DrawRectangle(DTC_X1, DTC_Y1, DTC_X2, DTC_Y2, ILI9341_COLOR_BLUE2);
	ILI9341_DrawRectangle(DTC_N_X1, DTC_N_Y1, DTC_N_X2, DTC_N_Y2, ILI9341_COLOR_BLUE2);

	ILI9341_DrawFilledRectangle(DTC_X1, DTC_Y1, DTC_X2, DTC_Y1+20, ILI9341_COLOR_BLUE2);
	ILI9341_DrawFilledRectangle(DTC_N_X1, DTC_N_Y1, DTC_N_X2, DTC_N_Y1+20, ILI9341_COLOR_BLUE2);

	ILI9341_Puts(DTC_X1+7,DTC_Y1+5, "DTC :", &Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLUE2);
	ILI9341_Puts(DTC_N_X1+7,DTC_N_Y1+5, "TOTAL", &Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLUE2);
}

/**
 * @brief change a potentiometer position on a tft screen
 * 
 * @param pot 
 * @param y 
 */
void change_pot(pot_t *pot, int16_t y)
{
	uint16_t y2, y1, pot_size;
	if(y+30 >= (pot->y2-(pot->y2-pot->y1)/10))
	{
		y2 = (uint16_t)(pot->y2-(pot->y2-pot->y1)/10);
		y1 = (uint16_t)(y2-30);
	}
	else if(y<=(uint16_t)(pot->y1+(pot->y2-pot->y1)/10))
	{
		y1 = (uint16_t)(pot->y1+(pot->y2-pot->y1)/10);
		y2 = (uint16_t)(y1+30);
	}
	else
	{
		y2 = (uint16_t)(y + 30);
		y1 = (uint16_t)y;
	}
	ILI9341_DrawFilledRectangle((uint16_t)(pot->x1+(pot->x2-pot->x1)/4),(uint16_t)(pot->y1+(pot->y2-pot->y1)/10),(uint16_t)(pot->x2-(pot->x2-pot->x1)/4),(uint16_t)(pot->y2-(pot->y2-pot->y1)/10),ILI9341_COLOR_BLUE2);
	ILI9341_DrawFilledRectangle((uint16_t)(pot->x1+(pot->x2-pot->x1)/4),y1,(uint16_t)(pot->x2-(pot->x2-pot->x1)/4), y2,ILI9341_COLOR_GRAY);

	pot_size = (uint16_t)(((pot->y2-(pot->y2-pot->y1)/10) - (pot->y1+(pot->y2-pot->y1)/10))-30);
	pot->height = (uint16_t)((100*(y1-(pot->y1+(pot->y2-pot->y1)/10)))/pot_size);
}

/**
 * @brief search if a button was pressed and wich is pressed
 * 
 * @param x 
 * @param y 
 * @param button_tab 
 * @param nb_button 
 * @return int16_t the right button pressed
 */
int16_t search_button(int16_t x, int16_t y, button_t *button_tab, int8_t nb_button)
{
	int16_t j = nb_button;
	for(int16_t i = 0;i<nb_button; i++ )
	{
		if(x >= button_tab[i].x1 && x<= button_tab[i].x2 && y >= button_tab[i].y1 && y<= button_tab[i].y2 )
			j = i;
	}
	return j;
}

/**
 * @brief search if a potentiometer was pressed and wich is pressed
 * 
 * @param x 
 * @param y 
 * @param pot_tab 
 * @return int16_t the right potentiometer pressed
 */
int16_t search_pot(int16_t x, int16_t y, pot_t *pot_tab)
{
	int16_t j = NBR_OF_POT;
	for(int16_t i = 0;i<NBR_OF_POT; i++ )
	{
		if(x >= pot_tab[i].x1 && x<= pot_tab[i].x2 && y >= pot_tab[i].y1 && y<= pot_tab[i].y2 )
			j = i;
	}
	return j;
}

/**
 * @brief animation of a pressed button
 * 
 * @param button 
 * @param absolute_t 
 */
void pressed_button(button_t *button, uint32_t absolute_t)
{
	ILI9341_DrawFilledRectangle(button->x1,button->y1,button->x2,button->y2,ILI9341_COLOR_BLUE);
	ILI9341_DrawFilledRectangle((uint16_t)(button->x1+(button->x2-button->x1)/4),(uint16_t)(button->y1+(button->y2-button->y1)/4),(uint16_t)(button->x2-(button->x2-button->x1)/4),(uint16_t)(button->y2-(button->y2-button->y1)/4),ILI9341_COLOR_WHITE);
	button->time = absolute_t + DELAY_B;
}

/**
 * @brief search if an animation is over
 * 
 * @param absolute_t 
 * @param button_tab 
 * @param nb_button 
 */
void search_time_over(uint32_t absolute_t,button_t *button_tab, int8_t nb_button)
{
	for(int16_t i = 0;i<nb_button; i++ )
	{
		if(button_tab[i].time <= absolute_t && button_tab[i].time !=0)
		{
			button_tab[i].time = 0;
			fill_button(&button_tab[i]);
		}

	}
}

#endif
