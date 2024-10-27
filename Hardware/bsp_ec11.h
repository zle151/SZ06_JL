#ifndef __BSP_EC11_H__
#define __BSP_EC11_H__

#include "board_config.h"

extern uint8_t ec11_cur_state;

void EC11_init();

extern void EC11_on_down();
extern void EC11_on_up();
extern void EC11_on_turn(uint8_t is_clockwise);

#endif