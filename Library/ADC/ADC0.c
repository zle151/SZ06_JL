#include "ADC0.h"

#if USE_ADC_0  


#define ADCx        ADC0
#define RCU_ADCx    RCU_ADC0

#define ADCx_RESOLUTION     ADC0_RESOLUTION
#define ADCx_DATAALIGN      ADC0_DATAALIGN

#define ADCx_CHN_TYPE       ADC0_CHN_TYPE
#define ADCx_CHNS_LEN       ADC0_CHNS_LEN
#define ADCx_CHNS           ADC0_CHNS

#define ADCx_SAMPLETIME     ADC0_SAMPLETIME


#if ADCx_CHN_TYPE == ADC_CHN_ROUTINE
#if ADCx_CHNS_LEN > 19
#error "ADC0 channel Routine len must <= 19 "
#endif
#else
#if ADCx_CHNS_LEN > 4
#error "ADC0 channel Insert len must <= 4 "
#endif
#endif

#define GP_CH0      RCU_GPIOA, GPIOA, GPIO_PIN_0
#define GP_CH1      RCU_GPIOA, GPIOA, GPIO_PIN_1
#define GP_CH2      RCU_GPIOA, GPIOA, GPIO_PIN_2
#define GP_CH3      RCU_GPIOA, GPIOA, GPIO_PIN_3
#define GP_CH4      RCU_GPIOA, GPIOA, GPIO_PIN_4
#define GP_CH5      RCU_GPIOA, GPIOA, GPIO_PIN_5
#define GP_CH6      RCU_GPIOA, GPIOA, GPIO_PIN_6
#define GP_CH7      RCU_GPIOA, GPIOA, GPIO_PIN_7
#define GP_CH8      RCU_GPIOB, GPIOB, GPIO_PIN_0
#define GP_CH9      RCU_GPIOB, GPIOB, GPIO_PIN_1
#define GP_CH10     RCU_GPIOC, GPIOC, GPIO_PIN_0
#define GP_CH11     RCU_GPIOC, GPIOC, GPIO_PIN_1
#define GP_CH12     RCU_GPIOC, GPIOC, GPIO_PIN_2
#define GP_CH13     RCU_GPIOC, GPIOC, GPIO_PIN_3
#define GP_CH14     RCU_GPIOC, GPIOC, GPIO_PIN_4
#define GP_CH15     RCU_GPIOC, GPIOC, GPIO_PIN_5


static inline void gpio_cfg(rcu_periph_enum rcu, uint32_t gpio, uint32_t pin) {
     // 1. ʱ�ӳ�ʼ��
    rcu_periph_clock_enable(rcu);
    // 2. ����GPIO �������ģʽ
    gpio_mode_set(gpio, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, pin);   
}


#if ADCx_CHN_TYPE == ADC_CHN_ROUTINE
static uint16_t adcx_result[ADCx_CHNS_LEN] = {0};
static void ADCx_DMA_config() {
    uint32_t dmax = DMA1;
    uint32_t dmax_rcu = RCU_DMA1;
    uint32_t dmax_ch = DMA_CH0;
    uint32_t damx_sub = DMA_SUBPERI0;

    uint32_t dmax_dirction = DMA_PERIPH_TO_MEMORY;

    uint32_t dmax_src = (uint32_t)(&ADC_RDATA(ADC0));
    uint32_t dmax_src_inc = DMA_PERIPH_INCREASE_DISABLE;
    uint32_t dmax_src_width = DMA_PERIPH_WIDTH_16BIT;
    uint32_t dmax_src_len = 2;

    uint32_t dmax_dst = (uint32_t)(adcx_result);
    uint32_t dmax_dst_inc = DMA_MEMORY_INCREASE_ENABLE;

    /**************** DMA p2m *******************/
    // ʱ��
    rcu_periph_clock_enable(dmax_rcu);
    // ����dma
    dma_deinit(dmax, dmax_ch);

    //////// dma ����
    dma_single_data_parameter_struct dsdps;
    dma_single_data_para_struct_init(&dsdps);
    // ����
    dsdps.direction = DMA_PERIPH_TO_MEMORY;
    // �ڴ�: dst
    dsdps.memory0_addr = dmax_dst;
    dsdps.memory_inc = dmax_dst_inc;
    // ����: src
    dsdps.periph_addr = dmax_src;
    dsdps.periph_inc = dmax_src_inc;
    // ���ݳ���
    dsdps.number = dmax_src_len;
    // dst���ݿ��
    dsdps.periph_memory_width = dmax_src_width;
    // �������ȼ�
    dsdps.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_single_data_mode_init(dmax, dmax_ch, &dsdps);

    //////// ���� dma ������
    dma_channel_subperipheral_select(dmax, dmax_ch, damx_sub);

    dma_circulation_enable(dmax, dmax_ch);

    // Ĭ�Ͽ�������
    dma_flag_clear(dmax, dmax_ch, DMA_FLAG_FTF);
    dma_channel_enable(dmax, dmax_ch);
}
#endif


void ADC0_init() {
    uint8_t adcx_chns[] = ADCx_CHNS;
    uint8_t adcx_chns_len = ADCx_CHNS_LEN;
    
     /******************** GPIO config *********************/
    for(uint8_t i = 0; i < adcx_chns_len; i++) {
        if(adcx_chns[i] == ADC_CHANNEL_0 ) {
            gpio_cfg(GP_CH0);
        } else if(adcx_chns[i] == ADC_CHANNEL_1) {
            gpio_cfg(GP_CH1);
        } else if(adcx_chns[i] == ADC_CHANNEL_2) {
            gpio_cfg(GP_CH2);
        } else if(adcx_chns[i] == ADC_CHANNEL_3) {
            gpio_cfg(GP_CH3);
        } else if(adcx_chns[i] == ADC_CHANNEL_4) {
            gpio_cfg(GP_CH4);
        } else if(adcx_chns[i] == ADC_CHANNEL_5) {
            gpio_cfg(GP_CH5);
        } else if(adcx_chns[i] == ADC_CHANNEL_6) {
            gpio_cfg(GP_CH6);
        } else if(adcx_chns[i] == ADC_CHANNEL_7) {
            gpio_cfg(GP_CH7);
        } else if(adcx_chns[i] == ADC_CHANNEL_8) {
            gpio_cfg(GP_CH8);
        } else if(adcx_chns[i] == ADC_CHANNEL_9) {
            gpio_cfg(GP_CH9);
        } else if(adcx_chns[i] == ADC_CHANNEL_10) {
            gpio_cfg(GP_CH10);
        } else if(adcx_chns[i] == ADC_CHANNEL_11) {
            gpio_cfg(GP_CH11);
        } else if(adcx_chns[i] == ADC_CHANNEL_12) {
            gpio_cfg(GP_CH12);
        } else if(adcx_chns[i] == ADC_CHANNEL_13) {
            gpio_cfg(GP_CH13);
        } else if(adcx_chns[i] == ADC_CHANNEL_14) {
            gpio_cfg(GP_CH14);
        } else if(adcx_chns[i] == ADC_CHANNEL_15) {
            gpio_cfg(GP_CH15);
        }
    }

    /******************** ADC config **********************/
    // ����ʱ��
    rcu_periph_clock_enable(RCU_ADCx);
    
    ////////// ��������
    adc_resolution_config(ADCx, ADCx_RESOLUTION); // �ֱ���
    adc_data_alignment_config(ADCx, ADCx_DATAALIGN);// �����Ҷ���

    //////////// ��������
#if ADCx_CHN_TYPE == ADC_CHN_ROUTINE
    adc_channel_length_config(ADCx, ADC_ROUTINE_CHANNEL, adcx_chns_len); // ����
    
    for(uint8_t i = 0; i < adcx_chns_len; i++) {
        adc_routine_channel_config(ADCx, i, adcx_chns[i], ADCx_SAMPLETIME);
        
        if(adcx_chns[i] >= ADC_CHANNEL_16) {
            adc_channel_16_to_18(ADC_TEMP_VREF_CHANNEL_SWITCH, ENABLE);
            adc_channel_16_to_18(ADC_VBAT_CHANNEL_SWITCH, ENABLE);
        }
    }
#else    
    adc_channel_length_config(ADCx, ADC_INSERTED_CHANNEL, adcx_chns_len); // ����
    
    for(uint8_t i = 0; i < adcx_chns_len; i++) {
        adc_inserted_channel_config(ADCx, i, adcx_chns[i], ADCx_SAMPLETIME);
        
        if(adcx_chns[i] >= ADC_CHANNEL_16) {
            adc_channel_16_to_18(ADC_TEMP_VREF_CHANNEL_SWITCH, ENABLE);
            adc_channel_16_to_18(ADC_VBAT_CHANNEL_SWITCH, ENABLE);
        }
    }
#endif    
 
    /////////// ģʽ����
    adc_special_function_config(ADCx, ADC_CONTINUOUS_MODE, ENABLE);//����ģʽ
    adc_special_function_config(ADCx, ADC_SCAN_MODE, ENABLE);// ɨ��ģʽ

#if ADCx_CHN_TYPE == ADC_CHN_ROUTINE   
    /// DMA 
    adc_dma_request_after_last_enable(ADCx);
    adc_dma_mode_enable(ADCx);
		
    ADCx_DMA_config();
#else
    adc_special_function_config(ADCx, ADC_INSERTED_CHANNEL_AUTO, ENABLE);
#endif

    // ��ADC
    adc_enable(ADCx);

    /* �ȴ�ADC�ȶ� wait for ADC stability */
    delay_1ms(1);
    // У׼ADC
    adc_calibration_enable(ADCx);
    
    
#if ADCx_CHN_TYPE == ADC_CHN_ROUTINE    
    // ��������
    adc_software_trigger_enable(ADCx, ADC_ROUTINE_CHANNEL);
#else
    // ��������
    adc_software_trigger_enable(ADCx, ADC_INSERTED_CHANNEL);
    
	// /* �ȴ��ɼ���ϣ������ǣ���ȡ��� */
	// while(adc_flag_get(ADC0, ADC_FLAG_EOIC) == RESET);	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// adc_flag_clear(ADC0, ADC_FLAG_EOIC);	

#endif
}


uint16_t ADC0_get(uint8_t chn) {
#if ADCx_CHN_TYPE == ADC_CHN_ROUTINE  
    uint8_t idx = 100;
    uint8_t adcx_chns[] = ADCx_CHNS;
    uint8_t adcx_chns_len = ADCx_CHNS_LEN;
    for(uint8_t i = 0; i < adcx_chns_len; i++) {
        if(adcx_chns[i] == chn) {
            idx = i;
            break;
        }
    }
    if(idx != 100) return adcx_result[idx];
    return 0;
#else
    return adc_inserted_data_read(ADCx, chn);

#endif    

}



#endif