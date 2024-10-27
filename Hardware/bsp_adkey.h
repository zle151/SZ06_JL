#ifndef __BSP_ADKEY_H__
#define __BSP_ADKEY_H__

#include "board_config.h"

void ADkey_init();
void ADkey_scan();

uint8_t ADkey_is_down(uint8_t key);
uint8_t ADkey_is_up(uint8_t key);

extern void ADKey_on_down(uint8_t key);
extern void ADKey_on_up(uint8_t key);

#endif