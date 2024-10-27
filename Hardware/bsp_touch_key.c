#include "bsp_touch_key.h"

#include <stdio.h>

static uint16_t key_state = 0x00;


void TK_init() {
	unsigned char databuf;
	
	// ����ͨ��������
	databuf = 0x79;
	TK_I2C_WRITE(TK_DEVICE_ADDRESS, TK_REG_SEN_SET_0, &databuf, 1);
	
	// ��������������
	databuf = 0x79;
	TK_I2C_WRITE(TK_DEVICE_ADDRESS, TK_REG_SEN_SET_COM, &databuf, 1);
	// �Ĵ�������
	// 1.��������
	// 2.����˯�ߣ����������
	// 3.��׼���Ƿ�У׼
	// 4.������ֹͣУ��
	// 5.������Ӧ�ٶ�����
	databuf = TK_SLPCYC_3R5T | TK_SLOW_TO_SLEEP | TK_HOLD | TK_KVF_50S_CORREC | TK_RTM3;
	TK_I2C_WRITE(TK_DEVICE_ADDRESS, TK_REG_CTRL_0, &databuf, 1);
	
	// ���û�׼���ݣ�15pf��
	databuf =0x0F;
	TK_I2C_WRITE(TK_DEVICE_ADDRESS, TK_REG_CTRL_1, &databuf, 1);
	
}


void TK_INT_FUNC() {
	uint8_t buf[2];
	TK_I2C_READ(TK_DEVICE_ADDRESS, TK_REG_OUTPUT, buf, 2);
	
	uint16_t current = (uint16_t)(buf[0] << 8) + buf[1];
  
	current >>= 4;
	
	uint16_t diff = key_state ^ current;
	
	for(uint8_t i = 0; i < 12; i++) {
		if((diff >> i) & 0x01) {
			if((current >> i) & 0x01) {
				// 0 -> 1: ����
				TK_on_down(11 - i);
			} else {
				// 1 -> 0: ̧��
				TK_on_up(11 - i);
			}
		}
	}
	key_state = current;
}