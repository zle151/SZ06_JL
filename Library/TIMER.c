#include "TIMER.h"

static void timer_gpio_config(uint32_t gpio_rcu, uint32_t gpio_port, uint32_t gpio_pin, uint32_t gpio_af) {
  rcu_periph_clock_enable(gpio_rcu);
  /* ����gpioģʽ */
  gpio_mode_set(gpio_port, GPIO_MODE_AF, GPIO_PUPD_NONE, gpio_pin);
  gpio_output_options_set(gpio_port, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, gpio_pin);
  gpio_af_set(gpio_port, gpio_af, gpio_pin);
}

static void timer_init_config(rcu_periph_enum rcu_periph, uint32_t timer_periph, uint16_t t_prescaler, uint32_t t_period) {
  rcu_periph_clock_enable(rcu_periph);
  timer_deinit(timer_periph);
  /*��ʼ������ */
  timer_parameter_struct initpara;
  /* initialize TIMER init parameter struct */
  timer_struct_para_init(&initpara);
  /* ������Ҫ����ֵ ��Ƶϵ�� ������ʵ�ָ��͵�timerƵ�ʣ� */
  initpara.prescaler 	= t_prescaler - 1;
  /* 1�����ڵļ���(period Max: 65535) Freq > 3662  */
  initpara.period		= t_period - 1;
  /* initialize TIMER counter */
  timer_init(timer_periph, &initpara);
  /* enable a TIMER */
  timer_enable(timer_periph);
  
}

static void timer_channel_config(uint32_t timer_periph, uint16_t channel, timer_oc_parameter_struct* ocpara) {
  /* ����������� configure TIMER channel output function */
  timer_channel_output_config(timer_periph, channel, ocpara);
  /* ����ͨ���������Ƚ�ģʽ configure TIMER channel output compare mode */
  timer_channel_output_mode_config(timer_periph, channel, TIMER_OC_MODE_PWM0);
}

void TIMER_init() {

  /* ����Ƶ��*/
  rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);

  /* TIMER ͨ��������� */
  timer_oc_parameter_struct ocpara;

// -----------------------------------------------------------------------TIMER_0
#if USE_TIMER_0

  timer_init_config(RCU_TIMER0, TIMER0, TM0_PRESCALER, TM0_PERIOD); // ��ͨ���޹�

  /* initialize TIMER channel output parameter struct */
  timer_channel_output_struct_para_init(&ocpara);

#ifdef	TM0_CH0
  timer_gpio_config(TM0_CH0);
  ocpara.outputstate  = (uint16_t)TIMER_CCX_ENABLE;
  timer_channel_config(TIMER0, TIMER_CH_0, &ocpara);
#endif

#ifdef	TM0_CH0_ON
  timer_gpio_config(TM0_CH0_ON);
  ocpara.outputnstate  = (uint16_t)TIMER_CCXN_ENABLE;
  timer_channel_config(TIMER0, TIMER_CH_0, &ocpara);
#endif


#ifdef	TM0_CH1
  timer_gpio_config(TM0_CH1);
  ocpara.outputstate  = (uint16_t)TIMER_CCX_ENABLE;
  timer_channel_config(TIMER0, TIMER_CH_1, &ocpara);
#endif

#ifdef	TM0_CH1_ON
  timer_gpio_config(TM0_CH1_ON);
  ocpara.outputnstate  = (uint16_t)TIMER_CCXN_ENABLE;
  timer_channel_config(TIMER0, TIMER_CH_1, &ocpara);
#endif


#ifdef	TM0_CH2
  timer_gpio_config(TM0_CH2);
  ocpara.outputstate  = (uint16_t)TIMER_CCX_ENABLE;
  timer_channel_config(TIMER0, TIMER_CH_2, &ocpara);
#endif

#ifdef	TM0_CH2_ON
  timer_gpio_config(TM0_CH2_ON);
  ocpara.outputnstate  = (uint16_t)TIMER_CCXN_ENABLE;
  timer_channel_config(TIMER0, TIMER_CH_2, &ocpara);
#endif

#ifdef	TM0_CH3
  timer_gpio_config(TM0_CH3);
  ocpara.outputstate  = (uint16_t)TIMER_CCX_ENABLE;
  timer_channel_config(TIMER0, TIMER_CH_3, &ocpara);
#endif


#endif

// -----------------------------------------------------------------------TIMER_1
#if USE_TIMER_1
  
  timer_init_config(RCU_TIMER1, TIMER1, TM1_PRESCALER, TM1_PERIOD); // ��ͨ���޹�

  /* initialize TIMER channel output parameter struct */
  timer_channel_output_struct_para_init(&ocpara);
#ifdef	TM1_CH0
  timer_gpio_config(TM1_CH0);
  ocpara.outputstate  = (uint16_t)TIMER_CCX_ENABLE;
  timer_channel_config(TIMER1, TIMER_CH_0, &ocpara);
#endif

#ifdef	TM1_CH1
  timer_gpio_config(TM1_CH1);
  ocpara.outputstate  = (uint16_t)TIMER_CCX_ENABLE;
  timer_channel_config(TIMER1, TIMER_CH_1, &ocpara);
#endif

#endif

// -----------------------------------------------------------------------TIMER_2
#if USE_TIMER_2

  timer_init_config(RCU_TIMER2, TIMER2, TM2_PRESCALER, TM2_PERIOD); // ��ͨ���޹�

  /* initialize TIMER channel output parameter struct */
  timer_channel_output_struct_para_init(&ocpara);
#ifdef	TM2_CH0
  timer_gpio_config(TM2_CH0);
  ocpara.outputstate  = (uint16_t)TIMER_CCX_ENABLE;
  timer_channel_config(TIMER2, TIMER_CH_0, &ocpara);
#endif

#ifdef	TM2_CH1
  timer_gpio_config(TM2_CH1);
  ocpara.outputstate  = (uint16_t)TIMER_CCX_ENABLE;
  timer_channel_config(TIMER2, TIMER_CH_1, &ocpara);
#endif

#ifdef	TM2_CH2
  timer_gpio_config(TM2_CH2);
  ocpara.outputstate  = (uint16_t)TIMER_CCX_ENABLE;
  timer_channel_config(TIMER2, TIMER_CH_2, &ocpara);
#endif

#endif

// -----------------------------------------------------------------------TIMER_5
#if USE_TIMER_5

  timer_init_config(RCU_TIMER5, TIMER5, TM5_PRESCALER, TM5_PERIOD); // ��ͨ���޹�

  /* �����ж����ȼ� */
  nvic_irq_enable(TIMER5_IRQn, 0, 1);
  /* �����ж� */
  timer_interrupt_enable(TIMER5, TIMER_INT_UP);
	
#endif

// -----------------------------------------------------------------------TIMER_6
#if USE_TIMER_6

  timer_init_config(RCU_TIMER6, TIMER6, TM6_PRESCALER, TM6_PERIOD); // ��ͨ���޹�

  /* �����ж����ȼ� */
  nvic_irq_enable(TIMER6_IRQn, 0, 1);
  /* �����ж� */
  timer_interrupt_enable(TIMER6, TIMER_INT_UP);
	
#endif

// -----------------------------------------------------------------------TIMER_7
#if USE_TIMER_7

  timer_init_config(RCU_TIMER7, TIMER7, TM7_PRESCALER, TM7_PERIOD); // ��ͨ���޹�

  /* initialize TIMER channel output parameter struct */
  timer_channel_output_struct_para_init(&ocpara);

#ifdef	TM7_CH0
  timer_gpio_config(TM7_CH0);
  ocpara.outputstate  = (uint16_t)TIMER_CCX_ENABLE;
  timer_channel_config(TIMER7, TIMER_CH_0, &ocpara);
#endif

#ifdef	TM7_CH0_ON
  timer_gpio_config(TM7_CH0_ON);
  ocpara.outputnstate  = (uint16_t)TIMER_CCXN_ENABLE;
  timer_channel_config(TIMER7, TIMER_CH_0, &ocpara);
#endif


#ifdef	TM7_CH1
  timer_gpio_config(TM7_CH1);
  ocpara.outputstate  = (uint16_t)TIMER_CCX_ENABLE;
  timer_channel_config(TIMER7, TIMER_CH_1, &ocpara);
#endif

#ifdef	TM7_CH1_ON
  timer_gpio_config(TM7_CH1_ON);
  ocpara.outputnstate  = (uint16_t)TIMER_CCXN_ENABLE;
  timer_channel_config(TIMER7, TIMER_CH_1, &ocpara);
#endif


#ifdef	TM7_CH2
  timer_gpio_config(TM7_CH2);
  ocpara.outputstate  = (uint16_t)TIMER_CCX_ENABLE;
  timer_channel_config(TIMER7, TIMER_CH_2, &ocpara);
#endif

#ifdef	TM7_CH2_ON
  timer_gpio_config(TM7_CH2_ON);
  ocpara.outputnstate  = (uint16_t)TIMER_CCXN_ENABLE;
  timer_channel_config(TIMER7, TIMER_CH_2, &ocpara);
#endif


#ifdef	TM7_CH3
  timer_gpio_config(TM7_CH3);
  ocpara.outputstate  = (uint16_t)TIMER_CCX_ENABLE;
  timer_channel_config(TIMER7, TIMER_CH_3, &ocpara);
#endif

#endif


#if USE_TIMER_0 || USE_TIMER_7
  // Break --------------------------------------------------
  // break ֻ��Ը߼���ʱ��TIMER0 & TIMER7���򿪻���������·
  /* TIMERͨ������������· */
  timer_break_parameter_struct breakpara;
  /* ��ʼ��TIMER break�����ṹ�� */
  timer_break_struct_para_init(&breakpara);
  /* break����ļ��� HIGH */
  breakpara.breakpolarity   = TIMER_BREAK_POLARITY_HIGH;
  /* ����Զ������� */
  breakpara.outputautostate = TIMER_OUTAUTO_ENABLE;
  /* bread���������*/
  breakpara.breakstate     = TIMER_BREAK_ENABLE;
#if USE_TIMER_0
  /* ����TIMER0 break */
  timer_break_config(TIMER0, &breakpara);
  /* ����TIMER0 break */
  timer_break_enable(TIMER0);
#endif

#if USE_TIMER_7
  /* ����TIMER7 break */
  timer_break_config(TIMER7, &breakpara);
  /* ����TIMER7 break */
  timer_break_enable(TIMER7);
#endif

#endif

}

void TIMER_period_update(uint32_t timer_periph, uint16_t t_prescaler, uint32_t t_period) {
  
  /*��ʼ������ */
  timer_parameter_struct initpara;
  /* initialize TIMER init parameter struct */
  timer_struct_para_init(&initpara);
  /* ������Ҫ����ֵ ��Ƶϵ�� ������ʵ�ָ��͵�timerƵ�ʣ� */
  initpara.prescaler 	= t_prescaler - 1;
  /* 1�����ڵļ���(period Max: 65535) Freq > 3662  */
  initpara.period		= t_period - 1;
  /* initialize TIMER counter */
  timer_init(timer_periph, &initpara);
}

void TIMER_channel_update(uint32_t timer, uint16_t channel, float duty) {
  if(duty > 100) duty = 100;
  else if(duty < 0) duty = 0;
  uint32_t pulse =  TIMER_CAR(timer) * duty / 100.0f;
  timer_channel_output_pulse_value_config(timer, channel, pulse);
}

#if USE_TIMER_5

void TIMER5_DAC_IRQHandler(void) {
    if(SET == timer_interrupt_flag_get(TIMER5, TIMER_INT_FLAG_UP)) {
        //����жϱ�־λ
        timer_interrupt_flag_clear(TIMER5,TIMER_INT_FLAG_UP);

        TIMER_5_on_interrupt(TIMER_INT_FLAG_UP);
    }
}
#endif

#if USE_TIMER_6
void TIMER6_IRQHandler(void){
    if(SET == timer_interrupt_flag_get(TIMER6, TIMER_INT_FLAG_UP)) {
        //����жϱ�־λ
        timer_interrupt_flag_clear(TIMER6,TIMER_INT_FLAG_UP);

        TIMER_6_on_interrupt(TIMER_INT_FLAG_UP);
    }
}

#endif 