#ifndef __BSP_ADCURRENT_H__
#define __BSP_ADCURRENT_H__

#include "board_config.h"

void ADcurrent_init();
void ADcurrent_scan();

float ADcurrent_get();

#endif
