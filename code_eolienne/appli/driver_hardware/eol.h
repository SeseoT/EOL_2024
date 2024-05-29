/*
 * eol.h
 *
 *  Created on: Mar 18, 2024
 *      Author: Tanguy Suteau
 */

#ifndef EOL_H_
#define EOL_H_
#include "config.h"
#include "macro_types.h"
#include "config.h"
#include "stm32f1_gpio.h"
#include "stm32f1_adc.h"
#if USE_ADC

void EOL_init();

uint16_t EOL_getValue(adc_id_e adc);

uint16_t EOL_tension_mV(int16_t val);

uint16_t EOL_tension_V(int16_t val);

#endif
#endif /* EOL_H_ */
