#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include <string.h>
#include "USART.h"
#include "TIMER.h"
#include "EXTI.h"
#include "ADC.h"
#include "SPI.h"
#include "I2C.h"

#include "tasks_timer.h"
#include "tasks.h"

/***********************
任务：模板工程
************************/
void USART0_on_recv(uint8_t* data, uint32_t len) {
  printf("recv[%d]:%s\n", len, data);
}

int main(void) {
  // 配置全局中断分组
  nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
  // 初始化系统嘀嗒定时器
  systick_config();

  // 初始化USART
  USART_init();
  TIMER_init();
  EXTI_init();
  ADC_init();
  SPI_init();
  I2C_init();
	printf("init1 Peripheral\n");
	
  task_timer_init();
	Task_init();
	printf("init2 Task\n");
	
  while(1) {
    Task_exec_handler();
  }
}
