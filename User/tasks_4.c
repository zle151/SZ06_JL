#include "tasks.h"
#include "App.h"

void Task_init(void) {
  /* 公共线 3V3, GND*/
  
  /* 触摸按键输入接线：*/ 
  /*   
		I2C0:PB6  SCL
		I2C0:PB7	SDA
		INT: PB8
  */
  App_Input_init();
  
  /* 输出接线：*/ 
  /* 
	I2C OLED：
    I2C2: PA8 SCL
    I2C2: PC9 SDA
	
	SPI
	CLK:  PA5
	MOSI: PA7
	CS: 	PA6
  */
  App_Display_init();
}


Task_t task_list[] = {
  // state,         count, period,  callback
  { TASK_STATE_STOP,  0,   	100,  	 App_Display_OLED_task1},
	{ TASK_STATE_STOP,  0,    500,  	 App_Display_DM_task},
};

uint8_t task_cnt = sizeof(task_list) / sizeof(Task_t);