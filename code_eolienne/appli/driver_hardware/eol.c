/*
 * eol.c
 *
 *  Created on: Mar 18, 2024
 *      Author: Tanguy Suteau
 */
#include "config.h"
#if USE_ADC

#include "eol.h"

/**
 * @brief Initializes the EOL module.
 *
 * This function initializes the ADC for EOL operations.
 */
void EOL_init()
{
	ADC_init();
}

/**
 * @brief Gets the ADC value for the WindTurbine with a given ADC identifier.
 *
 * @param adc The ADC identifier.
 * @return The ADC value.
 */
uint16_t EOL_getValue(adc_id_e adc)
{
	return ADC_getValue(adc);
}

/**
 * @brief Converts an ADC value to voltage in millivolts.
 *
 * @param val The ADC value to convert.
 * @return The corresponding voltage in millivolts.
 */
uint16_t EOL_tension_mV(int16_t val){
	return (int16_t)((((int32_t)val)*3300)/4096);
}

/**
 * @brief Converts an ADC value to voltage in volts.
 *
 * @param val The ADC value to convert.
 * @return The corresponding voltage in volts.
 */
uint16_t EOL_tension_V(int16_t val){
	return EOL_tension_mV(val)*1000;
}




#endif

