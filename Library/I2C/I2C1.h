#ifndef __I2C1_H__
#define __I2C1_H__

#include "gd32f4xx.h"
#include "I2C_config.h"

void I2C1_init();

/* 写数据到I2C设备指定寄存器 */
uint8_t I2C1_write(uint8_t addr, uint8_t reg, uint8_t* dat, uint32_t len);

uint8_t I2C1_soft_write2(uint8_t addr, uint8_t reg, uint8_t* dat, uint32_t offset, uint32_t len);

/* 从I2C设备指定寄存器读取数据 */
uint8_t I2C1_read(uint8_t addr, uint8_t reg, uint8_t* dat, uint32_t len);


#endif