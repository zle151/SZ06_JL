#include "ADC.h"

void ADC_init() {
  // ����
  adc_deinit();
  // ������Ƶ
  adc_clock_config(ADC_ADCCK_PCLK2_DIV4);

#if USE_ADC_0
  ADC0_init();
#endif
}