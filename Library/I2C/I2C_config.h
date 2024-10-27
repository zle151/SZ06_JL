#ifndef __I2C_CONFIG_H__
#define __I2C_CONFIG_H__

#include "gd32f4xx.h"

#define USE_I2C0  1
#define USE_I2C1  0
#define USE_I2C2  1

#if USE_I2C0

#define I2C0_SOFT   		 1   // 0Ӳʵ�֣�1��ʵ��
// PB6, PB8
#define I2C0_SCL_RCU     RCU_GPIOB
#define I2C0_SCL_PORT    GPIOB
#define I2C0_SCL_PIN     GPIO_PIN_6
// PB7, PB9
#define I2C0_SDA_RCU     RCU_GPIOB
#define I2C0_SDA_PORT    GPIOB
#define I2C0_SDA_PIN     GPIO_PIN_7

#define I2C0_SPEED       400000UL

#endif

// I2C1 -------------------------------------------
#if USE_I2C0

#define I2C1_SOFT   0   // 0Ӳʵ�֣�1��ʵ��
// PB10,PF1,PH4	
#define I2C1_SCL_RCU     RCU_GPIOB
#define I2C1_SCL_PORT    GPIOB
#define I2C1_SCL_PIN     GPIO_PIN_10
// PF0,PH5,PB11,PC12,PB3
#define I2C1_SDA_RCU     RCU_GPIOB
#define I2C1_SDA_PORT    GPIOB
#define I2C1_SDA_PIN     GPIO_PIN_11
#define I2C1_SPEED       100000UL

#endif
// I2C2 -------------------------------------------
#if USE_I2C2

#define I2C2_SOFT   		 1   // 0Ӳʵ�֣�1��ʵ��
#define I2C2_SPEED       400000UL
// PA8,PH7
#define I2C2_SCL_RCU     RCU_GPIOA
#define I2C2_SCL_PORT    GPIOA
#define I2C2_SCL_PIN     GPIO_PIN_8
// PH8,PC9,PB4
#define I2C2_SDA_RCU     RCU_GPIOC
#define I2C2_SDA_PORT    GPIOC
#define I2C2_SDA_PIN     GPIO_PIN_9

#endif
#endif