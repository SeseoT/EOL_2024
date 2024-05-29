/*
 * interface.h
 *
 *  Created on: 2 mai 2024
 *      Author: axel GAILLARD
 */

#ifndef APPLICATION_INTERFACE_H_
#define APPLICATION_INTERFACE_H_

#include "config.h"

#include "stm32f1_xpt2046.h"
#include "stm32f1xx_hal.h"
#include "stm32f1_ili9341.h"
#include "interface_tft.h"

#include "stm32f1_fonts.h"
#include "stm32f1_uart.h"
#include "communication.h"
#include "systick.h"


state_interface_e * INTERFACE_TFT_init(void);

void interface_state_machine();

void INTERFACE_TFT_init_home(uint16_t background_color,uint16_t text_color);
void INTERFACE_TFT_init_manual_page(int16_t i, uint16_t background_color, uint16_t text_color);
void INTERFACE_TFT_init_automatical_page(int16_t i, uint16_t background_color, uint16_t text_color);
void INTERFACE_TFT_init_page(uint16_t background_color);

void fill_values_param(float temperature, float voltage, uint8_t inclinaison, uint8_t hauteur, uint8_t DTC, uint8_t DTC_nbr);

void fill_mode_param(state_interface_e new_mode);

void fill_voltage_tab(uint8_t voltage);

void fill_values_acquisition_tab(acq_tab_t *tab,uint16_t background_color);

void change_calib(button_t *button_param,uint16_t background_color,uint16_t text_color);

void change_mode(button_t *button_tab,button_t *button_mode,uint16_t background_color,uint16_t text_color);

void change_mode_protected(void);

#endif /* APPLICATION_INTERFACE_H_ */
