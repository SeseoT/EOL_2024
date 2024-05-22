
#ifndef NTC_H_
#define NTC_H_

#include "config.h"
#if USE_ADC

	void  NTC_init();

	float NTC_getValue(adc_id_e adc);

	float temp_kelvin(int16_t val);

	float temp(int16_t val);

#endif
#endif /* NTC_H_ */
