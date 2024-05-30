/*
 * ntc.c
 *
 *  Created on: 14 march 2024
 *      Author: Axel GAILLARD
 */


#include "config.h"
#include "ntc.h"
#if USE_ADC
/**
 * @brief Initializes the NTC module.
 *
 * This function initializes the ADC for NTC operations.
 */
void NTC_init()
{
	ADC_init();
}
/**
 * @brief Gets the temperature value from the ADC.
 *
 * This function reads the ADC value and converts it to a temperature value.
 *
 * @param adc The ADC identifier.
 * @return The temperature value in Celsius.
 */
float NTC_getValue(adc_id_e adc)
{
	return temp(ADC_getValue(adc));
}

// Calcule la température en degrés kevlin
/**
 * @brief Calculates the temperature in Kelvin.
 *
 * This function converts an ADC value to temperature in Kelvin.
 *
 * @param val The ADC value to convert.
 * @return The corresponding temperature in Kelvin.
 */
float temp_kelvin(int16_t val)
{
	double u = 3.3;                  // Tension d’alimentation du pont diviseur
    double r = 10000.0;              // Résistance en haut du pont
    double ut = val * u / 4096.0;    // Tension mesurée avec l’entrée analogique
    double beta = 4030.0;            // Constante de la thermistance
    double r0 = 10000.0;             // Résistance de référence de la thermistance
    double t0 = 298.0;               // Température de référence
    return (float)(1/((log(((ut*r/u)/(1-(ut/u)))/r0)/beta)+(1/t0)) - 5);
}

// Calcule la température en degrés celcius
/**
 * @brief Calculates the temperature in Celsius.
 *
 * This function converts an ADC value to temperature in Celsius.
 *
 * @param val The ADC value to convert.
 * @return The corresponding temperature in Celsius.
 */
float temp(int16_t val)
{
	return temp_kelvin(val) - (float)273;
}

#endif
