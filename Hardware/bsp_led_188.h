#ifndef __BSP_LED_188_H__
#define __BSP_LED_188_H__

#include "board_config.h"


void LED188_init();

void LED188_set_num(uint8_t num);
void LED188_clear_num();
void LED188_set_charge(uint8_t enable);
void LED188_set_percent(uint8_t enable);

void LED188_display();

#endif
