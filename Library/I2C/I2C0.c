#include "I2C0.h"

#if USE_I2C0

#if I2C0_SOFT

#include "systick.h"

#if I2C0_SPEED >= 400000UL
  #define DELAY()   delay_1us(1)
#else
  #define DELAY()   delay_1us(5)
#endif

#define SDA(bit)  gpio_bit_write(I2C0_SDA_PORT, I2C0_SDA_PIN, (bit) ? SET : RESET)
#define SCL(bit)  gpio_bit_write(I2C0_SCL_PORT, I2C0_SCL_PIN, (bit) ? SET : RESET)

#define I2C0_SDA_OUT() gpio_mode_set(I2C0_SDA_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, I2C0_SDA_PIN);
#define I2C0_SDA_IN()  gpio_mode_set(I2C0_SDA_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, I2C0_SDA_PIN);
// ��ȡSDA��ƽ
#define I2C0_SDA_STATE() gpio_input_bit_get(I2C0_SDA_PORT, I2C0_SDA_PIN)

// ��ʼ��I2C����������GPIOΪ��©���
void I2C0_init(){
  // ��ʵ�֣�CPU����GPIO
  // Ӳʵ�֣�CPU����I2C�ļĴ�����AF���ã���ʼ������
  
  // ��׼ģʽ 100Kbits/s, ����ģʽ 400Kbits/s
  // 100Kbits/s -> 100 000 bit / 1 000 000us -> 1bit/10us -> 5us
  // 400Kbits/s -> 400 000 bit / 1 000 000us -> 4bit/10us -> 1us+
  
  // ����ʱ��1us -> 1bit��Ҫ2us  -> 4bit��Ҫ8us
  
  // PB6 - SCL
  rcu_periph_clock_enable(I2C0_SCL_RCU);
  gpio_mode_set(I2C0_SCL_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, I2C0_SCL_PIN);
  gpio_output_options_set(I2C0_SCL_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_MAX, I2C0_SCL_PIN);
  // PB7 - SDA
  rcu_periph_clock_enable(I2C0_SDA_RCU);
  gpio_mode_set(I2C0_SDA_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, I2C0_SDA_PIN);
  gpio_output_options_set(I2C0_SDA_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_MAX, I2C0_SDA_PIN);
}

// ��ʼ�ź�
static void start();
// �豸��ַ���Ĵ�����ַ������
static void send(uint8_t dat);
// ����1���ֽ�
static uint8_t recv();
// ����ack��Ӧ
static void send_ack();
// ����nack��Ӧ
static void send_nack();

// �ȴ���Ӧ������ֵ > 0, �����쳣
static uint8_t wait_ack();
// ֹͣ�ź�
static void stop();


/**********************************************************
 * @brief д���ݵ�I2C�豸ָ���Ĵ���
 * @param  addr �豸��ַ (д��ַ��ǰ7λ)
 * @param  reg  �Ĵ�����ַ
 * @param  dat  �ֽ�����
 * @param  len  ���ݳ���
 * @return 0�ɹ���1 �豸������, 2 �Ĵ���������, 3 ��������Ӧ
 **********************************************************/
uint8_t I2C0_write(uint8_t addr, uint8_t reg, uint8_t* dat, uint32_t len){
  
  // ��ʼ
  start();
  
  // �����豸��ַ (д��ַ)
  send((addr << 1) | 0);
  // �ȴ���Ӧ
  if(wait_ack()) return 1;
  
  // ���ͼĴ�����ַ
  send(reg);
  // �ȴ���Ӧ
  if(wait_ack()) return 2;
  
  // ѭ���������е��ֽ�
  for(uint32_t i = 0; i < len; i++){
    // ��������
    send(dat[i]);
    // �ȴ���Ӧ
    if(wait_ack()) return 3;
  }
  
  // ֹͣ
  stop();
  
  return 0;
}

uint8_t I2C0_write2(uint8_t addr, uint8_t reg, uint8_t* dat, uint32_t offset, uint32_t len){
  
  // ��ʼ
  start();
  
  // �����豸��ַ (д��ַ)
  send((addr << 1) | 0);
  // �ȴ���Ӧ
  if(wait_ack()) return 1;
  
  // ���ͼĴ�����ַ
  send(reg);
  // �ȴ���Ӧ
  if(wait_ack()) return 2;
  
  // ѭ���������е��ֽ�
//  for(uint32_t i = 0; i < len; i++){
//    // ��������
//    send(dat[i * offset]);
//    // �ȴ���Ӧ
//    if(wait_ack()) return 3;
//  }
  while(len--){
    send(*dat);
    dat += offset;
    // �ȴ���Ӧ
    if(wait_ack()) return 3;
  }
  
  // ֹͣ
  stop();
  
  return 0;
}

/**********************************************************
 * @brief ��I2C�豸ָ���Ĵ�����ȡ����
 * @param  addr �豸��ַ  (д��ַ��ǰ7λ)   0x51
                д��ַ (addr << 1) | 0     0xA2
                ����ַ (addr << 1) | 1     0xA3
 * @param  reg  �Ĵ�����ַ
 * @param  dat  �ֽ�����
 * @param  len  ���ݳ���
 * @return 0�ɹ���1 �豸������, 2 �Ĵ���������, 3 ��������Ӧ
 * @return 
 **********************************************************/
uint8_t I2C0_read(uint8_t addr, uint8_t reg, uint8_t* dat, uint32_t len){
  uint8_t addr_write = (addr << 1) | 0;  
  uint8_t addr_read  = (addr << 1) | 1;
  
  // ��ʼ
  start();
  
  // �����豸��ַ (д��ַ)
  send(addr_write);
  // �ȴ���Ӧ
  if(wait_ack()) return 1;
  
  // ���ͼĴ�����ַ
  send(reg);
  // �ȴ���Ӧ
  if(wait_ack()) return 2;
  
  // ��ʼ
  start();
  // �����豸��ַ (����ַ)
  send(addr_read);
  // �ȴ���Ӧ
  if(wait_ack()) return 3;
  
  /******************* ѭ���������� *****************/
  for(uint32_t i = 0; i < len; i++){
    // ����1���ֽ�(8bit)
    dat[i] = recv();
    
    if(i != len - 1){
      // ������ӦACK
      send_ack();
    }else {
      // ���Ϳ���ӦNACK
      send_nack();
    }
  }
  
  /**************************************************/
  
  // ֹͣ
  stop();
  
  return 0;
}


// ��ʼ�ź�
static void start(){
  I2C0_SDA_OUT();
  // ����SDA
  SDA(1);
  DELAY();
  
  // ����SCL
  SCL(1);
  DELAY();
  
  // ����SDA
  SDA(0);
  DELAY();
  
  // ����SCL
  SCL(0);
  DELAY();
}

// �豸��ַ���Ĵ�����ַ������
static void send(uint8_t dat){
  I2C0_SDA_OUT();
  // 8bit �ȷ���λ
  // 1101 0010
  // 101 0010  ����1λ
  // 01 0010   ����1λ
  for(uint8_t i = 0; i < 8; i++){
    if(dat & 0x80){
      SDA(1); // �ߵ�ƽ
    }else {
      SDA(0); // �͵�ƽ
    }
    DELAY();
    
    SCL(1);
    DELAY();
    SCL(0);
    DELAY();
    
    // ������������1λ
    dat <<= 1;
  }
}

// ����1���ֽ�
static uint8_t recv(){
  // �ͷ�SDA����Ȩ
  I2C0_SDA_IN();
  uint8_t cnt = 8; // 1byte = 8bit
  uint8_t byte = 0x00; // ��������������
  
  while(cnt--){   // ����һ��bit�����ո�λ��
    // SCL����
    SCL(0); 
    DELAY(); // �ȴ����豸׼������
    
    SCL(1);  // ����������Ч��
    // 0000 0000 -> 1001 1011
    // 0000 0001    8
    // 0000 0010    7
    // 0000 0100    6
    // 0000 1001    5
    // 0001 0011    4
    // 0010 0110    3
    // 0100 1101    2
    // 1001 1011    1
    
    byte <<= 1;
    
    if(I2C0_SDA_STATE()) byte++;
    
    // SCL�ڸߵ�ƽDelayһ�����������һ��ѭ��
    DELAY();
  }
  // SCL�ָ��͵�ƽ, ������
  SCL(0);
  
  return byte;
}

// ����ack��Ӧ
static void send_ack(){
  // ������ȡSDA����Ȩ
  I2C0_SDA_OUT();
  // SDA����
  SDA(0);
  DELAY();
  
  // SCL����
  SCL(1);
  DELAY();
  
  // SCL����
  SCL(0);
  DELAY();
  
}
// ����nack��Ӧ
static void send_nack(){
  // ������ȡSDA����Ȩ
  I2C0_SDA_OUT();
  // SDA����
  SDA(1);
  DELAY();
  
  // SCL����
  SCL(1);
  DELAY();
  
  // SCL����
  SCL(0);
  DELAY();

}

// �ȴ���Ӧ������ֵ > 0, �����쳣
static uint8_t wait_ack(){
  I2C0_SDA_OUT();
  // ����SDA
  SDA(1);
  DELAY();
  
  // ����SCL�� ת������Ȩ��SDAתΪ����ģʽ��
  SCL(1);
  I2C0_SDA_IN();
  DELAY();
  
  // �ж�SDA��ƽ״̬
  if(I2C0_SDA_STATE() == RESET){
    // ���豸������SDA����Ӧ�ɹ�, ��ȡSDAȨ��
    SCL(0);
    I2C0_SDA_OUT();
  } else {
    // ����Ӧ��, ��ȡSDAȨ�ޣ�ֱ�ӽ���
    stop();
    return 1;
  }
  
  return 0;
}
// ֹͣ�ź�
static void stop(){
  I2C0_SDA_OUT();
  
  // ����SCL
  SCL(0);
  // ����SDA
  SDA(0);
  DELAY();
  
  // ����SCL
  SCL(1);
  DELAY();
  
  // ����SDA
  SDA(1);
  DELAY();
}


#else

#define I2C0RCU     RCU_I2C0
#define i2cx        I2C0
// PB6 - SCL
// PB7 - SDA
void I2C0_init(){

  // ��ʵ�֣�CPU����GPIO
  // Ӳʵ�֣�CPU����I2C�ļĴ�����AF���ã���ʼ������
  // ��׼ģʽ 100Kbits/s, ����ģʽ 400Kbits/s
  // 100Kbits/s -> 100 000 bit / 1 000 000us -> 1bit/10us -> 5us
  // 400Kbits/s -> 400 000 bit / 1 000 000us -> 4bit/10us -> 1us+
  
  // GPIO -----------------------------------------
  // SCL
  rcu_periph_clock_enable(I2C0_SCL_RCU);
  gpio_mode_set(I2C0_SCL_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, I2C0_SCL_PIN);
  gpio_output_options_set(I2C0_SCL_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_MAX, I2C0_SCL_PIN);
  gpio_af_set(I2C0_SCL_PORT, GPIO_AF_4, I2C0_SCL_PIN);
  // SDA
  rcu_periph_clock_enable(I2C0_SDA_RCU);
  gpio_mode_set(I2C0_SDA_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, I2C0_SDA_PIN);
  gpio_output_options_set(I2C0_SDA_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_MAX, I2C0_SDA_PIN);
  gpio_af_set(I2C0_SDA_PORT, GPIO_AF_4, I2C0_SDA_PIN);
  
  // I2C  ------------------------------------------
  rcu_periph_clock_enable(I2C0RCU);
  /* ���� reset I2C */
  i2c_deinit(i2cx);
  /* ����I2C���ٶȣ��͸ߵ�ƽ�ı��� configure I2C clock */
  i2c_clock_config(i2cx, I2C0_SPEED, I2C_DTCY_2);
  /* ����I2C��ַ���� configure I2C address */
  i2c_mode_addr_config(i2cx, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, 0x00);
  /* ���� enable I2C */
  i2c_enable(i2cx);
  
  /* �յ������Զ��ظ�ACK  whether or not to send an ACK */
  i2c_ack_config(i2cx, I2C_ACK_ENABLE);

}
#define	TIMEOUT	50000

// �ȴ����flag��ΪSET���ɹ�return 0
static uint8_t I2C_wait(uint32_t flag) {
    uint16_t cnt = 0;

    while(!i2c_flag_get(i2cx, flag)) {
        cnt++;
        if(cnt > TIMEOUT) return 1;
    }
    return 0;
}

// �ȴ����flag��ΪRESET, �ɹ�return 0
static uint8_t I2C_waitn(uint32_t flag) {
    uint16_t cnt = 0;

    while(i2c_flag_get(i2cx, flag)) {
        cnt++;
        if(cnt > TIMEOUT) return 1;
    }
		return 0;
}

/* д���ݵ�I2C�豸ָ���Ĵ��� */
uint8_t I2C0_write(uint8_t addr, uint8_t reg, uint8_t* dat, uint32_t len){
  
  uint8_t write_addr = addr << 1;
  /************* start ***********************/
  // �ȴ�I2C����
  if(I2C_waitn(I2C_FLAG_I2CBSY)) return 1;
  // start
  i2c_start_on_bus(i2cx);
  // �ȴ�I2C���豸�ɹ�������ʼ�ź�
  if(I2C_wait(I2C_FLAG_SBSEND)) return 2;
  
  /************* device address **************/
  // �����豸��ַ
  i2c_master_addressing(i2cx, write_addr, I2C_TRANSMITTER);
  // �ȴ���ַ�������
  if(I2C_wait(I2C_FLAG_ADDSEND)) return 3;
  i2c_flag_clear(i2cx, I2C_FLAG_ADDSEND);
  
  /************ register address ************/
  // �Ĵ�����ַ
  // �ȴ��������ݻ�����Ϊ�� SET
  if(I2C_wait(I2C_FLAG_TBE)) return 4;
  // ��������
  i2c_data_transmit(i2cx, reg);
  // �ȴ����ݷ������
  if(I2C_wait(I2C_FLAG_BTC)) return 5;
  
  /***************** data ******************/
  // ��������
  uint32_t i;
  for(i = 0; i < len; i++) {
      uint32_t d = dat[i];
      // �ȴ��������ݻ�����Ϊ��
      if(I2C_wait(I2C_FLAG_TBE)) return 6;
      // ��������
      i2c_data_transmit(i2cx, d);
      // �ȴ����ݷ������
      if(I2C_wait(I2C_FLAG_BTC)) return 7;
  }
  /***************** stop ********************/
  // stop
  i2c_stop_on_bus(i2cx);

  if(I2C_waitn(I2C_CTL0(i2cx)&I2C_CTL0_STOP)) return 8;
  
  return 0;
}


/* ��I2C�豸ָ���Ĵ�����ȡ���� */
uint8_t I2C0_read(uint8_t addr, uint8_t reg, uint8_t* dat, uint32_t len){
  uint8_t write_addr = (addr << 1) | 0;
  uint8_t  read_addr = (addr << 1) | 1;
  
  /************* start ***********************/
  // �ȴ�I2C����
  if(I2C_waitn(I2C_FLAG_I2CBSY)) return 1;
  // ���������ź�
  i2c_start_on_bus(i2cx);
  // �ȴ�I2C���豸�ɹ�������ʼ�ź�
  if(I2C_wait(I2C_FLAG_SBSEND)) return 2;
  
  /************* device address **************/
  // ���ʹ��豸��ַ��д��ַ��
  i2c_master_addressing(i2cx, write_addr, I2C_TRANSMITTER);

  if(I2C_wait(I2C_FLAG_ADDSEND)) return 3;
  i2c_flag_clear(i2cx, I2C_FLAG_ADDSEND);
  
  /********** register address **************/
  // �ȴ����ͻ�����Ϊ��
  if(I2C_wait(I2C_FLAG_TBE)) return 4;
  // ���ͼĴ�����ַ
  i2c_data_transmit(i2cx, reg);
  // �ȴ������������	
  if(I2C_wait(I2C_FLAG_BTC)) return 5;
  
  /************* start ***********************/
  // �����������ź�
  i2c_start_on_bus(i2cx);
  if(I2C_wait(I2C_FLAG_SBSEND)) return 7;
  
  /************* device address **************/
  // ���ʹ��豸��ַ������ַ��
  i2c_master_addressing(i2cx, read_addr, I2C_RECEIVER);
  if(I2C_wait(I2C_FLAG_ADDSEND)) return 8;
  i2c_flag_clear(i2cx, I2C_FLAG_ADDSEND);
  
  /************* data **************/
  // ack
  i2c_ack_config(i2cx, I2C_ACK_ENABLE);
  // ����һ�����ݺ��Զ�����ACK
  i2c_ackpos_config(i2cx, I2C_ACKPOS_CURRENT);
  // ȷ��ACK������
  if(I2C_wait(I2C_CTL0(i2cx) & I2C_CTL0_ACKEN)) return 9;

  // ��ȡ����
  uint32_t i;
  for (i = 0; i < len; i++) {
      if (i == len - 1) {
          // �ڶ�ȡ���һ���ֽ�֮ǰ������ACK������Ϊ�Զ�NACK
          i2c_ack_config(i2cx, I2C_ACK_DISABLE);
      }

      // �ȴ����ջ�������Ϊ��
      if(I2C_wait(I2C_FLAG_RBNE)) return 10;
      
      dat[i] = i2c_data_receive(i2cx);
  }
  
  /***************** stop ********************/
  i2c_stop_on_bus(i2cx);
  
  if(I2C_waitn(I2C_CTL0(i2cx)&I2C_CTL0_STOP)) return 11;
  
  return 0;
}

#endif

#endif

