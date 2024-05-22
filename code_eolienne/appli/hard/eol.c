/*
 * eol.c
 *
 *  Created on: Mar 18, 2024
 *      Author: Tanguy Suteau
 */
#include "config.h"
#if USE_ADC

#include "stm32f1_adc.h"
#include "eol.h"
#include "math.h"

void EOL_init()
{
	ADC_init();
}

uint16_t EOL_getValue(adc_id_e adc)
{
	return ADC_getValue(adc);
}
//Tension en millivolt
uint16_t EOL_tension_mV(int16_t val){
	return (int16_t)((((int32_t)val)*3300)/4096);
}

uint16_t EOL_tension_V(int16_t val){
	return EOL_tension_mV(val)*1000;
}




#endif

