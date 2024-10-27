#ifndef __BSP_TOUCH_KEY_H__
#define __BSP_TOUCH_KEY_H__


#include "board_config.h"


//////// Register ADDR ///////////////////////////////////
#define TK_REG_SEN_SET_0		0x00 	//CIN4 ͨ�������ȵ����õ�ַ
#define TK_REG_SEN_SET_COM	 	0x01 	//����ͨ�������ȵ����õ�ַ
#define TK_REG_CTRL_0	 		0x02 	//CTRL0 ���ƼĴ������õ�ַ
#define TK_REG_CTRL_1	 		0x03 	//CTRL1 ���ƼĴ������õ�ַ
#define TK_REG_OUTPUT	 		0x08	//����״̬�Ĵ��������ַ
#define TK_REG_SAMP		 		0x0A 	//��������ֵ���������ַ



#define TK_RTM0 0 	//3 ������������Ч��1 �����������ж���Ч
#define TK_RTM1 1 	//4 ������������Ч��2 �����������ж���Ч
#define TK_RTM2 2 	//5 ������������Ч��3 �����������ж���Ч
#define TK_RTM3 3 	//6 ������������Ч��4 �����������ж���Ч

#define TK_KVF_STOP_CORREC 	(1u<<2) // ������Ч��������У׼
#define TK_KVF_50S_CORREC 	(0u<<2) // ������Ч��50S ��ʼУ׼

#define TK_HOLD 	(1u<<3) 	//���߱��ֲ�У׼
#define TK_NOTHOLD 	(0u<<3) 	//���߳���У׼

#define TK_SLPCYC_LGT 	(0u<<5) //�����
#define TK_SLPCYC_0R5T 	(1u<<5) //���ߺ������� 60MS
#define TK_SLPCYC_1R5T 	(2u<<5) //���ߺ������� 180MS
#define TK_SLPCYC_2R5T 	(3u<<5) //���ߺ������� 300MS
#define TK_SLPCYC_3R5T 	(4u<<5) //���ߺ������� 420MS
#define TK_SLPCYC_4R5T 	(5u<<5) //���ߺ������� 540MS
#define TK_SLPCYC_5R5T 	(6u<<5) //���ߺ������� 660MS
#define TK_SLPCYC_6R5T 	(7u<<5) //���ߺ������� 780MS

#define TK_FAST_TO_SLEEP (1u<<4) //���ٽ�������
#define TK_SLOW_TO_SLEEP (0u<<4) // 75S ��������


void TK_init();

extern void TK_on_up(uint8_t i);
extern void TK_on_down(uint8_t i);

#endif