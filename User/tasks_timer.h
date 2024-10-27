#ifndef __TASKS_TIMER_H__
#define __TASKS_TIMER_H__

#include "gd32f4xx.h"

extern volatile uint32_t task_tick;

void task_timer_init();

uint32_t task_timer_get_tick();

#endif