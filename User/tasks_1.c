#include "tasks.h"
#include "App.h"

void Task_init(void) {
  /* 公共线 3V3, GND*/
  
  /* EC11旋钮输入接线：*/ 
  /* 
    A:	PD11 EXTI
    B:	PD13 EXTI
    C:	PD15 EXTI 
  */
  App_Input_init();
  
  /* LED188数码管接线：*/
  /*
  	LED1: PE15  GPIO      
  	LED2: PE13  GPIO 
  	LED3: PE11  GPIO
  	LED4: PE9   GPIO 
  	LED5: PE7   GPIO 
  */ 
  App_Display_init();
}


Task_t task_list[] = {
  // state,         count, period,  callback
  { TASK_STATE_STOP,  0,   	  2,     App_Input_EC11_task},
  { TASK_STATE_STOP,  0,   		2,  	 App_Display_LED188_task},
};

uint8_t task_cnt = sizeof(task_list) / sizeof(Task_t);