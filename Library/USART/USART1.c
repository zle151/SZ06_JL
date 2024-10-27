
#include "USART1.h"
#include <string.h>

#if USART1_PRINTF

#include <stdio.h>
// printf ����
int fputc(int ch, FILE *f) {
  USART1_send_byte((uint8_t) ch);
  return ch;
}

#endif

// static ֻ�е�ǰ�ļ��ɼ�������������
static uint8_t 		rx_buffer[USART1_RX_BUFFER_LEN];
static uint32_t 	rx_cnt = 0;

// DMA�õ���USART�����շ��Ĵ���
#define USART1_DATA_ADDR						((uint32_t)&USART_DATA(USART1))

#if USART1_TX_DMA_ENABLE
static void USART1_TX_DMA_init() {

  // TX���ݷ��ͣ� �ڴ� -> USART1����
  // SRC����Դ����ַ���������ȴ���
  // DSTĿ��	��USART_DATA_ADDR

  // RCU -------------------------------------
  rcu_periph_clock_enable(USART1_TX_DMA_RCU);

  // DMA -------------------------------------
  // ����DMA_ USART1_TX
  dma_deinit(USART1_TX_DMA_PERIPH_CH);
  dma_single_data_parameter_struct init_struct;
  /* initialize the DMA single data mode parameters struct with the default values */
  dma_single_data_para_struct_init(&init_struct);
  // �������� (�ڴ浽����)
  init_struct.direction           = DMA_MEMORY_TO_PERIPH;

  // SRCԴͷ �ڴ� 				  -------
//	init_struct.memory0_addr        = (uint32_t)dst;
  init_struct.memory_inc          = DMA_MEMORY_INCREASE_ENABLE;
  // ������������
//	init_struct.number              = sizeof(src) / sizeof(src[0]);
  // ÿ�����ݵĴ�С(BIT����)
  init_struct.periph_memory_width = DMA_MEMORY_WIDTH_8BIT;

  // DSTĿ�� ����	periph	 -------
//	uint32_t a = USART_DATA(USART1);
  init_struct.periph_addr         = USART1_DATA_ADDR;
  init_struct.periph_inc          = DMA_PERIPH_INCREASE_DISABLE;

  /* ��ʼ�� DMA single data mode initialize */
  dma_single_data_mode_init(USART1_TX_DMA_PERIPH_CH, &init_struct);

  /* ѡ��ָ��ͨ���Ӽ� DMA channel peripheral select** */
  dma_channel_subperipheral_select(USART1_TX_DMA_PERIPH_CH, USART1_TX_DMA_CH_SUB);

  // NVIC ------------------------------------��������жϣ���ѡ��
//	nvic_irq_enable(DMA1_Channel7_IRQn, 2, 2);
  /* enable DMA interrupt */
//	dma_interrupt_enable(DMA_PERIPH_CH, DMA_CHXCTL_FTFIE);
}
#endif

#if USART1_RX_DMA_ENABLE
static void USART1_RX_DMA_init() {

  // RX���ݽ��գ�USART1���� -> �ڴ�
  // SRC����Դ�� USART1���ݼĴ���USART1_DATA_ADDR
  // DSTĿ��	�� rx_buffer

  // RCU -------------------------------------
  rcu_periph_clock_enable(USART1_RX_DMA_RCU);

  // DMA -------------------------------------
  // ����DMA_ USART1_TX
  dma_deinit(USART1_RX_DMA_PERIPH_CH);
  dma_single_data_parameter_struct init_struct;
  /* initialize the DMA single data mode parameters struct with the default values */
  dma_single_data_para_struct_init(&init_struct);
  // �������� (�ڴ浽����)
  init_struct.direction           = DMA_PERIPH_TO_MEMORY;

  // SRCԴͷ ����	periph	 -------
  init_struct.periph_addr         = USART1_DATA_ADDR;
  init_struct.periph_inc          = DMA_PERIPH_INCREASE_DISABLE;
  // ������������
  init_struct.number            	= USART1_RX_BUFFER_LEN;
  // ÿ�����ݵĴ�С(BIT����)
  init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;

  // DSTĿ�� �ڴ� 				  -------
  init_struct.memory0_addr        = (uint32_t)rx_buffer;
  init_struct.memory_inc          = DMA_MEMORY_INCREASE_ENABLE;

  /* ��ʼ�� DMA single data mode initialize */
  dma_single_data_mode_init(USART1_RX_DMA_PERIPH_CH, &init_struct);

  /* ѡ��ָ��ͨ���Ӽ� DMA channel peripheral select** */
  dma_channel_subperipheral_select(USART1_RX_DMA_PERIPH_CH, USART1_RX_DMA_CH_SUB);

  // ֪ͨDMA�������ݣ�����������
  dma_channel_enable(USART1_RX_DMA_PERIPH_CH);

}
#endif

void USART1_init() {

#if USART1_TX_DMA_ENABLE
  USART1_TX_DMA_init();
#endif

#if USART1_RX_DMA_ENABLE
  USART1_RX_DMA_init();
#endif


  // GPIO ---------------------------------------------------
  // PA9: TX   PA10: RX
  // RCUʱ��
  rcu_periph_clock_enable(USART1_TX_RCU);
  // ��ʼ��GPIOģʽ, ����ģʽ
  gpio_mode_set(USART1_TX_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, USART1_TX_PIN);
  // �������ѡ��
  gpio_output_options_set(USART1_TX_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, USART1_TX_PIN);
  // ����GPIO���Ÿ��ù��� USART1
  gpio_af_set(USART1_TX_PORT, USART1_AF, USART1_TX_PIN);

  // RCUʱ��
  rcu_periph_clock_enable(USART1_RX_RCU); // �ϱ�RCU_GPIOA��ʼ�������������ʡ��
  // ��ʼ��GPIOģʽ, ����ģʽ
  gpio_mode_set(USART1_RX_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, USART1_RX_PIN);
  // �������ѡ��
  gpio_output_options_set(USART1_RX_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, USART1_RX_PIN);
  // ����GPIO���Ÿ��ù��� USART1
  gpio_af_set(USART1_RX_PORT, USART1_AF, USART1_RX_PIN);

  // USART ---------------------------------------------------
  // RCUʱ��
  rcu_periph_clock_enable(RCU_USART1);

  // (����)
  usart_deinit(USART1);
  // ����usart����: �����ʣ����ݸ�����У��λ��ֹͣλ
  usart_baudrate_set(USART1, USART1_BAUDRATE);						// �����ʣ���ѡ
  usart_word_length_set(USART1, USART_WL_8BIT); // ���ݸ���, Ĭ��8bit
  usart_parity_config(USART1, USART_PM_NONE);    // ��żУ�飬 Ĭ����У��λ
  usart_stop_bit_set(USART1, USART_STB_1BIT);		// ֹͣλ��Ĭ��1bit

  usart_data_first_config(USART1, USART_MSBF_LSB); // ��С��ģʽ�� Ĭ��С��, �ȷ���λ USART_MSBF_LSB

#if USART1_TX_DMA_ENABLE
  // ����DMA���͹���
  usart_dma_transmit_config(USART1, USART_TRANSMIT_DMA_ENABLE);
#endif

#if USART1_RX_DMA_ENABLE
  // ����DMA���չ���
  usart_dma_receive_config(USART1, USART_RECEIVE_DMA_ENABLE);
#endif

  // �������ݷ��͹���
  usart_transmit_config(USART1, USART_TRANSMIT_ENABLE);
  // �������ݽ��չ���
  usart_receive_config(USART1, USART_RECEIVE_ENABLE);

  // NVIC�ж�����-----------------------------------------------
  nvic_irq_enable(USART1_IRQn, USART1_IRQ_PRIORIY);

#if !USART1_RX_DMA_ENABLE
  // �����ж� (������buffer��Ϊ���ж�)
  usart_interrupt_enable(USART1, USART_INT_RBNE);
#endif

  // ���ÿ����ж�
  usart_interrupt_enable(USART1, USART_INT_IDLE);

  // ����usart
  usart_enable(USART1);

//	printf("usart_data_addr: 0x%X \n", (uint32_t)(USART_DATA(USART1)));
}

// Hello123

// ����1�ֽ�����
void USART1_send_byte(uint8_t data) {

#if USART1_TX_DMA_ENABLE
  // ʹ��DMA����
  USART1_send_data(&data, 1);
#else
  //ͨ��USART����
  usart_data_transmit(USART1, data);
  // ����ʽ�жϻ������Ƿ��Ѿ�����
  while(RESET == usart_flag_get(USART1, USART_FLAG_TBE));
#endif

}

// �����ֽ�����
void USART1_send_data(uint8_t* data, uint32_t len) {
  // ͨ��DMAʵ�����ݵķ���

#if USART1_TX_DMA_ENABLE
  // Դ��ַ
  dma_memory_address_config(USART1_TX_DMA_PERIPH_CH, DMA_MEMORY_0, (uint32_t)data);
  // ���ݳ���
  dma_transfer_number_config(USART1_TX_DMA_PERIPH_CH, len);
  // ֪ͨDMA�������ݣ�����������
  dma_channel_enable(USART1_TX_DMA_PERIPH_CH);

  // �ȴ����ݴ������
  while(RESET == dma_flag_get(USART1_TX_DMA_PERIPH_CH, DMA_FLAG_FTF));
  // �����ǣ������´��޷�����
  dma_flag_clear(USART1_TX_DMA_PERIPH_CH, DMA_FLAG_FTF);

#else

  while(data && len) {
    // ����һ���ֽ�����
    USART1_send_byte((uint8_t)(*data));
    // ���ַ�ָ��������һλ��ָ����һ���ַ�
    data++;
    // ���ȼ�һ
    len--;
  }
#endif
}


// �����ַ���
void USART1_send_string(char *data) {

#if USART1_TX_DMA_ENABLE
  // ʹ��DMA����
  USART1_send_data((uint8_t *)data, strlen(data));

#else

  //����ֹͣ��0����
  // 1. data��Ϊ�գ� 2. ���ݲ���ֹͣ��'\0'
  while(data && *data) {
    USART1_send_byte((uint8_t)(*data));
    // ���ַ�ָ��������һλ��ָ����һ���ַ�
    data++;
  }
#endif

}


/*
1. USART1���жϺ���ֻ��һ��
2. �����жϵ�ԭ���ж��
3. ��Ҫ����flag������ʲôԭ�򴥷���
*/
void USART1_IRQHandler() {

#if USART1_RX_DMA_ENABLE

//	printf("USART1_IRQHandler\n");
  if (SET == usart_interrupt_flag_get(USART1, USART_INT_FLAG_IDLE)) {
    // ������
    // ��һ�μĴ�������������ֵû���ã�����IDLE��flagһֱ����
    usart_data_receive(USART1);


    // ��ȡDMA���������ݸ��� 1024 byte
    /* ʣ����������� get the number of remaining data to be transferred by the DMA */
    rx_cnt = USART1_RX_BUFFER_LEN - dma_transfer_number_get(USART1_RX_DMA_PERIPH_CH);

    // ��ѡ������ַ���������ǣ������ӡ����
    rx_buffer[rx_cnt] = '\0'; // ��ͬ��0


#if USART1_RECV_CALLBACK
    USART1_on_recv(rx_buffer, rx_cnt);
#endif

    // ֹͣ����
    dma_channel_disable(USART1_RX_DMA_PERIPH_CH);
    // ����FTF���ݴ�����ϱ��
    dma_flag_clear(USART1_RX_DMA_PERIPH_CH, DMA_FLAG_FTF);
    // ���ü�����(����Ҫ��
//		dma_transfer_number_config(USART1_RX_DMA_PERIPH_CH, RX_BUFFER_LEN);
    // ���ð���
    dma_channel_enable(USART1_RX_DMA_PERIPH_CH);

  }

#else

  /***************** ��ͳ�����ݽ���CPU������� **************/

  // �ж�flag���
  if (SET == usart_interrupt_flag_get(USART1, USART_INT_FLAG_RBNE)) {
    // ���յ�����
    //		printf("FLAG_RBNE: %d\n", rx_cnt);
    // �����ǣ����㴮�ڵ�·������һ���ֽ�����
    usart_interrupt_flag_clear(USART1, USART_INT_FLAG_RBNE);

    // ��ȡ�Ĵ����������
    uint8_t data = usart_data_receive(USART1);

    // ���浽buffer
    rx_buffer[rx_cnt++] = data;
    if (rx_cnt >= USART1_RX_BUFFER_LEN) {
      rx_cnt = 0;	// ��ֹ���������
    }
    // ԭ�����أ����ԣ�
//    USART1_send_byte(data);
		
#if USART1_RECV_BYTE_CALLBACK
    USART1_on_byte_recv(data);
#endif
  }

  if (SET == usart_interrupt_flag_get(USART1, USART_INT_FLAG_IDLE)) {
    // ������
    //		printf(">IDLE<: %d\n", rx_cnt);
    // ��һ�μĴ�������������ֵû���ã�����IDLE��flagһֱ����
    usart_data_receive(USART1);

    // ��ѡ������ַ���������ǣ������ӡ����
    rx_buffer[rx_cnt] = '\0'; // ��ͬ��0
    //		rx_buffer[rx_cnt] = 0;

    // "abc123"0	�ַ�����ӡʱ������0�����Զ�ֹͣ
    // "hhh"0
#if USART1_RECV_CALLBACK
    USART1_on_recv(rx_buffer, rx_cnt);
#endif

    // ��ѡ�����û�����������ȫ��Ϊ��
    memset(rx_buffer, 0, rx_cnt);
    // ��������
    rx_cnt = 0;

  }

#endif
}
