#ifndef __BSP_DOT_MATRIX_H__
#define __BSP_DOT_MATRIX_H__

#include "board_config.h"

#define DM_SINGLE_PIN_INIT(rcu,port,pin) rcu_periph_clock_enable(rcu);\
														gpio_mode_set(port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, pin);\
														gpio_output_options_set(port, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, pin);\

											
#define DM_CLK_L()  gpio_bit_reset(DM_CLK_PORT, DM_CLK_PIN)	
#define DM_CLK_H()  gpio_bit_set(DM_CLK_PORT, DM_CLK_PIN)	

#define DM_CS_L()  gpio_bit_reset(DM_CS_PORT, DM_CS_PIN)	
#define DM_CS_H()  gpio_bit_set(DM_CS_PORT, DM_CS_PIN)

#define DM_DIN_L()  gpio_bit_reset(DM_DIN_PORT, DM_DIN_PIN)	
#define DM_DIN_H()  gpio_bit_set(DM_DIN_PORT, DM_DIN_PIN)	

//点阵屏行
#define ROW 8

void DM_init();
//展示一屏画面
void DM_display(uint8_t arr[8]);
void DM_off_all();


#endif