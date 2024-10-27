#ifndef __TIMER_H__
#define __TIMER_H__

#include "gd32f4xx.h"
#include "TIMER_config.h"

void TIMER_init();

/**********************************************************
 * @brief ����ָ��Timer�ķ�Ƶϵ��������
 * @param t_prescaler ��Ƶϵ��[1, 65536]
 * @param t_period    ���ڼ���ֵ[1, 65536]
 * @return 
 **********************************************************/
void TIMER_period_update(uint32_t timer, uint16_t t_prescaler, uint32_t t_period);

/**********************************************************
 * @brief ����PWMָ��ͨ����ռ�ձ�
 * @param duty ռ�ձ���ֵ [0, 100]
 **********************************************************/
void TIMER_channel_update(uint32_t timer, uint16_t channel, float duty);

#if USE_TIMER_5
extern void TIMER_5_on_interrupt(uint32_t interrupt);
#endif

#if USE_TIMER_6
extern void TIMER_6_on_interrupt(uint32_t interrupt);
#endif

#endif