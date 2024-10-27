#ifndef __I2C2_H__
#define __I2C2_H__

#include "gd32f4xx.h"
#include "I2C_config.h"

void I2C2_init();

/* д���ݵ�I2C�豸ָ���Ĵ��� */
uint8_t I2C2_write(uint8_t addr, uint8_t reg, uint8_t* dat, uint32_t len);

uint8_t I2C2_soft_write2(uint8_t addr, uint8_t reg, uint8_t* dat, uint32_t offset, uint32_t len);

/* ��I2C�豸ָ���Ĵ�����ȡ���� */
uint8_t I2C2_read(uint8_t addr, uint8_t reg, uint8_t* dat, uint32_t len);


#endif