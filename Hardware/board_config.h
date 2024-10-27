#ifndef __BOARD_CONFIG_H__
#define __BOARD_CONFIG_H__

#include "gd32f4xx.h"
#include "TIMER.h"
#include "I2C.h"
#include "SPI.h"
#include "ADC.h"


///////////////// 引脚对应关系 ////////////////////
//////////////////////////////////////////////////
//	触摸按键		I2C			SCL: PB6            //
//							SDA: PB7            //
//////////////////////////////////////////////////
//							CLK: PA5            //
//	点阵屏		SPI			MOSI:PA7            //
//							CS:	 PA6            //
//////////////////////////////////////////////////
//	WS2812		SPI			MOSI:PB15           //
//  						CLK: PB10 (NC)      //
//////////////////////////////////////////////////
// 							LED1: PE15          //
//							LED2: PE13          //
//	LED188		GPIO		LED3: PE11          //
//							LED4: PE9           //
//							LED5: PE7           //
//////////////////////////////////////////////////
//	ADKey		ADC			ADC0: PA2           //
//////////////////////////////////////////////////
//							A:	PD11            //
//	EC11		EXTI		B:	PD13            //
//							D:	PD15            //
//////////////////////////////////////////////////
//	OLED		I2C			SCL: PA8            //
//							SDA: PC8            //
//////////////////////////////////////////////////
//  电流检测		ADC			ADC0: PA1           //
//////////////////////////////////////////////////
//  电压检测		ADC			ADC0: PA4           //
//////////////////////////////////////////////////


/////////////// 触摸按键 ///////////////////
// PB8 int pin
// PB6 scl
// PB7 sda
#define TK_DEVICE_ADDRESS			    0x40
#define TK_INT_FUNC						    EXTI8_on_trig
#define TK_I2C_WRITE(a, r, d, l)	I2C0_write(a, r, d, l)
#define TK_I2C_READ(a, r, d, l)		I2C0_read(a, r, d, l)


/////////////// 点阵屏 ///////////////////
#define DM_NUM					3

// clk:  PA5
// mosi: PA7
// cs: 	 PA6

#define DM_CLK_RCU  RCU_GPIOA
#define DM_CLK_PORT GPIOA
#define DM_CLK_PIN  GPIO_PIN_5

#define DM_CS_RCU  RCU_GPIOA
#define DM_CS_PORT GPIOA
#define DM_CS_PIN  GPIO_PIN_6

#define DM_DIN_RCU  RCU_GPIOA
#define DM_DIN_PORT GPIOA
#define DM_DIN_PIN  GPIO_PIN_7

//#define DM_SPI_WRITE(dat)		SPI0_write(dat)

//#define DM_CS_RCC				  RCU_GPIOA
//#define DM_CS_PORT				GPIOA
//#define DM_CS_PIN				  GPIO_PIN_6
//#define DM_CS(val)				gpio_bit_write(DM_CS_PORT, DM_CS_PIN, (val) ? SET : RESET)

//#define DM_GPIO_INIT()			rcu_periph_clock_enable(DM_CS_RCC);	\
//								gpio_mode_set(DM_CS_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, DM_CS_PIN);	\
//								gpio_output_options_set(DM_CS_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, DM_CS_PIN);

///////////////// ws2812 //////////////////////////////
// spi2，4分频，42MHZ/4 = 10.5MHZ
// PB15 mosi
// PB10 clk(not use)
#define WS2812_NUM	4

#define WS2812_SPI_WRITE(dat)		lib_spi1_write(dat)



////////////////// LED188 //////////////////////////////
#define LED188_5_RCC     RCU_GPIOE
#define LED188_5_PORT    GPIOE
#define LED188_5_PIN     GPIO_PIN_7

#define LED188_4_RCC     RCU_GPIOE
#define LED188_4_PORT    GPIOE
#define LED188_4_PIN     GPIO_PIN_9

#define LED188_3_RCC     RCU_GPIOE
#define LED188_3_PORT    GPIOE
#define LED188_3_PIN     GPIO_PIN_11

#define LED188_2_RCC     RCU_GPIOE
#define LED188_2_PORT    GPIOE
#define LED188_2_PIN     GPIO_PIN_13

#define LED188_1_RCC     RCU_GPIOE
#define LED188_1_PORT    GPIOE
#define LED188_1_PIN     GPIO_PIN_15

#define LED188_1_PP()   gpio_mode_set(LED188_1_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, LED188_1_PIN)
#define LED188_2_PP()   gpio_mode_set(LED188_2_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, LED188_2_PIN)
#define LED188_3_PP()   gpio_mode_set(LED188_3_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, LED188_3_PIN)
#define LED188_4_PP()   gpio_mode_set(LED188_4_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, LED188_4_PIN)
#define LED188_5_PP()   gpio_mode_set(LED188_5_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, LED188_5_PIN)

#define LED188_1_HZ()   gpio_mode_set(LED188_1_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, LED188_1_PIN);
#define LED188_2_HZ()   gpio_mode_set(LED188_2_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, LED188_2_PIN);
#define LED188_3_HZ()   gpio_mode_set(LED188_3_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, LED188_3_PIN);
#define LED188_4_HZ()   gpio_mode_set(LED188_4_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, LED188_4_PIN);
#define LED188_5_HZ()   gpio_mode_set(LED188_5_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, LED188_5_PIN);

#define LED188_1(val)   gpio_bit_write(LED188_1_PORT, LED188_1_PIN, (val) ? SET : RESET)
#define LED188_2(val)   gpio_bit_write(LED188_2_PORT, LED188_2_PIN, (val) ? SET : RESET)
#define LED188_3(val)   gpio_bit_write(LED188_3_PORT, LED188_3_PIN, (val) ? SET : RESET)
#define LED188_4(val)   gpio_bit_write(LED188_4_PORT, LED188_4_PIN, (val) ? SET : RESET)
#define LED188_5(val)   gpio_bit_write(LED188_5_PORT, LED188_5_PIN, (val) ? SET : RESET)


///////////////////////// ADC 按键 ///////////////////////////////
#define ADKEY
// PA2 ADC0 IN2 
#if ADC0_CHN_TYPE == ADC_CHN_ROUTINE
#define ADKEY_GET_VALUE()      ADC0_get(ADC_CHANNEL_2)
#else
#define ADKEY_GET_VALUE()      ADC0_get(ADC_INSERTED_CHANNEL_0)
#endif

//////////////////////// EC11 旋钮 ///////////////////////////////
//// EXTI 11 interrupt
//   A: PD11
//   B: PD13
//   D: PD15
#define EC11_A_RCC		      RCU_GPIOD
#define EC11_A_PORT		      GPIOD
#define EC11_A_PIN		      GPIO_PIN_11
#define EC11_A_INT_FUNC	    EXTI11_on_trig

#define EC11_B_RCC		      RCU_GPIOD
#define EC11_B_PORT		      GPIOD
#define EC11_B_PIN		      GPIO_PIN_13
#define EC11_B_INT_FUNC	    EXTI13_on_trig

#define EC11_C_RCC		      RCU_GPIOD
#define EC11_C_PORT		      GPIOD
#define EC11_C_PIN		      GPIO_PIN_15
#define EC11_C_INT_FUNC	    EXTI15_on_trig

//#define EC11_GPIO_INIT()	rcu_periph_clock_enable(EC11_B_RCC);	\
//							gpio_mode_set(EC11_B_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, EC11_B_PIN);
#define EC11_GPIO_INIT()

#define EC11_A_STATE()		gpio_input_bit_get(EC11_A_PORT, EC11_A_PIN)
#define EC11_B_STATE()		gpio_input_bit_get(EC11_B_PORT, EC11_B_PIN)
#define EC11_C_STATE()		gpio_input_bit_get(EC11_C_PORT, EC11_C_PIN)

/////////////////////////////// OLED //////////////////////////////////
// PA8: scl
// PC9: sda
#define OLED_I2C_WR(a,r,d,l)  I2C2_write(a,r,d,l)


#define BASE_VOLTAGE	((float)3.3)
//////////////////// 电流检测 ////////////////////////
// PA1: ADC0 IN1
#define ADCURRENT_SCALE				50
#define ADCURRENT_GET_VALUE()      	ADC0_get(ADC_CHANNEL_1)


//////////////////// 电压检测 ////////////////////////
// PA4: ADC0 IN4
#define ADVOLTAGE_SCALE				16
#define ADVOLTAGE_GET_VALUE()      	ADC0_get(ADC_CHANNEL_4)

#endif