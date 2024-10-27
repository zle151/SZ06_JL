#ifndef __BSP_WS2812_H__
#define __BSP_WS2812_H__

#include "board_config.h"

void ws2812_init();

void ws2812_off();
void ws2812_set_color(uint8_t id, uint32_t rgb);

void ws2812_display();



#endif