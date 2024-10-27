#include "tasks_timer.h"
#include "tasks.h"

// Timer6 按照 1000hz 执行任务
volatile uint32_t task_tick = 0;

void task_timer_init() {
}

void TIMER_6_on_interrupt() {
    Task_switch_handler();
    task_tick++;
}

uint32_t task_timer_get_tick() {
    return task_tick;
}