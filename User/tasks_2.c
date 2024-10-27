#include "tasks.h"
#include "App.h"

void Task_init(void) {
  /* 公共线 3V3, GND*/
  /* ADKey按键输入接线：*/ 
  /* 
    ADC: PA2 ADC0
  */
  App_Input_init();
	
  /* I2C OLED输出接线：*/ 
  /* 
    I2C2: PA8 SCL
    I2C2: PC9 SDA
  */
  App_Display_init();
}


Task_t task_list[] = {
  // state,         count, period,  callback
  { TASK_STATE_STOP,  0,   	 10,     App_Input_adkey_task},
  { TASK_STATE_STOP,  0,   	100,  	 App_Display_OLED_task},
};

uint8_t task_cnt = sizeof(task_list) / sizeof(Task_t);