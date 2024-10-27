#include "bsp_advoltage.h"
#include "ADC.h"

static float voltage = 0;

void ADvoltage_init() {
}

void ADvoltage_scan() {
#define ADC_TIME    10
  // 2ms in
  static uint8_t i = 0;
  static uint32_t sum = 0;
  static uint16_t min = 0;
  static uint16_t max = 0;
  static uint8_t last = 0;

  uint16_t val = ADVOLTAGE_GET_VALUE();

  if(i == ADC_TIME) {
    // ÂË²¨
    sum -= min;
    sum -= max;
    uint16_t avg = sum / (ADC_TIME - 2);

    voltage = BASE_VOLTAGE * avg * ADVOLTAGE_SCALE / 4096.0;

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

float ADvoltage_get() {
  return voltage;
}