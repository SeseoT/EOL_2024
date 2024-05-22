/*
 * ntc.c
 *
 *  Created on: 14 march 2024
 *      Author: Axel GAILLARD
 */


#include "config.h"
#if USE_ADC

#include "stm32f1_adc.h"
#include "ntc.h"
#include "math.h"

void NTC_init()
{
	ADC_init();
}

float NTC_getValue(adc_id_e adc)
{
	return temp(ADC_getValue(adc));
}

// Calcule la température en degrés kevlin
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
float temp(int16_t val)
{
	return temp_kelvin(val) - 273;
}

#endif
