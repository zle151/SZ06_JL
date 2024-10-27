#include "bsp_adcurrent.h"

static float current = 0;

void ADcurrent_init() {
	
}
#define ADC_TIME    10

void ADcurrent_scan() {
    // 2ms in
    static uint8_t i = 0;
    static uint32_t sum = 0;
    static uint16_t min = 0;
    static uint16_t max = 0;
    static uint8_t last = 0;
	
//	uint16_t val = ADCURRENT_GET_VALUE();
  uint16_t val = 	ADC0_get(ADC_CHANNEL_1);
	
	if(i == ADC_TIME) {
		// ÂË²¨
        sum -= min;
        sum -= max;
        uint16_t avg = sum / (ADC_TIME - 2);
		
		current = BASE_VOLTAGE * avg * 10 / ADVOLTAGE_SCALE / 4096.0;
		
		sum = 0;
        min = val;
        max = val;
        i = 0;
	} else {
		sum += val;
        i++;
        if(min > val) min = val;
        if(max < val) max = val;
	}
}

float ADcurrent_get() {
	return current;
}