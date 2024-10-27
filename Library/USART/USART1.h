#ifndef __USART1_H__
#define __USART1_H__

#include "gd32f4xx.h"
#include "USART_config.h"

void USART1_init();

// ����1�ֽ�����
void USART1_send_byte(uint8_t data);

// �����ֽ�����
void USART1_send_data(uint8_t* data, uint32_t len);

// �����ַ���
void USART1_send_string(char *data);

#if USART1_RECV_CALLBACK
extern void USART1_on_recv(uint8_t* data, uint32_t len);
#endif

#if USART1_RECV_BYTE_CALLBACK
extern void USART1_on_byte_recv(uint8_t data);
#endif


#endif