#include "bsp_led_188.h"



#define ALL_OFF()   LED188_1_HZ(); \
                    LED188_2_HZ(); \
                    LED188_3_HZ(); \
                    LED188_4_HZ(); \
                    LED188_5_HZ(); \


#define OUT1(val)   LED188_1_PP();LED188_1(val)
#define OUT2(val)   LED188_2_PP();LED188_2(val)
#define OUT3(val)   LED188_3_PP();LED188_3(val)
#define OUT4(val)   LED188_4_PP();LED188_4(val)
#define OUT5(val)   LED188_5_PP();LED188_5(val)


#define A3  (1 << 0)
#define B3  (1 << 1)
#define C3  (1 << 2)
#define D3  (1 << 3)
#define E3  (1 << 4)
#define F3  (1 << 5)
#define G3  (1 << 6)

#define A2  (1 << 8)
#define B2  (1 << 9)
#define C2  (1 << 10)
#define D2  (1 << 11)
#define E2  (1 << 12)
#define F2  (1 << 13)
#define G2  (1 << 14)

#define B1  (1 << 16)
#define C1  (1 << 17)

#define K1  (1 << 18)
#define K2  (1 << 19)

///////////////////////////////////////////////////////////////////////////////////////////
//  19  18      17  16      15  13  12  11  10   9   8   7      7   6   5   4   3   2   1   0   //
//  K2  K1  |   C1  B1  |   XX  G2  F2  E2  D2  C2  B2  A2  |  XX  G3  F3  E3  D3  C3  B3  A3   //
///////////////////////////////////////////////////////////////////////////////////////////
static uint32_t states = 0x00;
static uint8_t num_table[] = {
  0x3F,   // 0
  0x06,   // 1
  0x5B,   // 2
  0x4F,   // 3
  0x66,   // 4
  0x6D,   // 5
  0x7D,   // 6
  0x07,   // 7
  0x7F,   // 8
  0x6F,   // 9
};

static void GPIO_config(rcu_periph_enum rcu, uint32_t port, uint32_t pin) {
  // 1. 时钟初始化
  rcu_periph_clock_enable(rcu);
  // 2. 配置GPIO 输入输出模式
  gpio_mode_set(port, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, pin);
  // 3. 配置GPIO 输出选项
  gpio_output_options_set(port, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, pin);
  // 4. 默认输出电平
  gpio_bit_write(port, pin, RESET);
}

void LED188_init() {
  GPIO_config(LED188_1_RCC, LED188_1_PORT, LED188_1_PIN);
  GPIO_config(LED188_2_RCC, LED188_2_PORT, LED188_2_PIN);
  GPIO_config(LED188_3_RCC, LED188_3_PORT, LED188_3_PIN);
  GPIO_config(LED188_4_RCC, LED188_4_PORT, LED188_4_PIN);
  GPIO_config(LED188_5_RCC, LED188_5_PORT, LED188_5_PIN);
}

void LED188_set_num(uint8_t num) {
  static uint8_t num_cache = 255;

  if(num == num_cache) return;

  num_cache = num;

  if(num > 199) num = 199;

  // 百位
  if(num >= 100) {
    states |=  ((uint32_t)0x30000);
  } else {
    states &= ~((uint32_t)0x30000);
  }

  // 十位
  uint8_t shi = num % 100 / 10;
  /// 先清空所有
  states &= ~((uint32_t)0xFF00);
  states |= (uint32_t)(num_table[shi] << 8);

  // 个位
  uint8_t ge = num % 10;
  /// 先清空所有
  states &= ~((uint32_t)0xFF);
  states |= num_table[ge];
}
void LED188_clear_num() {
  states &= ~0xFFFFFF;
}
void LED188_set_charge(uint8_t enable) {
  if(enable) {
    states |= (0x01 << 18);
  } else {
    states &= ~(0x01 << 18);
  }
}
void LED188_set_percent(uint8_t enable) {
  if(enable) {
    states |= (0x01 << 19);
  } else {
    states &= ~(0x01 << 19);
  }
}

void LED188_display() {
  static uint8_t i = 1;

///// 方案一
//////////////////////////////////////////////////
/// 高/低 |   1   |   2   |   3   |   4   |   5
///   1   |   X   |  A3   |  C3   |   E3  |   X
///   2   |   B3  |   X   |  A2   |   C1  |   K2
///   3   |   D3  |  B2   |  X    |   B1  |   K1
///   4   |   F3  |  D2   |  C2   |   X   |   X
///   5   |   G3  |  E2   |  F2   |   G2  |   X
//////////////////////////////////////////////////
  ALL_OFF();
  switch(i++) {
  case 1:
    OUT1(1);
    if(states & A3) OUT2(0); //A3
    if(states & C3) OUT3(0); //C3
    if(states & E3) OUT4(0); //E3
    // NC           
    break;          
  case 2:           
    OUT2(1);        
    if(states & B3) OUT1(0); //B3
    if(states & A2) OUT3(0); //A2
    if(states & C1) OUT4(0); //C1
    if(states & K2) OUT5(0); //K2
    break;          
  case 3:           
    OUT3(1);        
    if(states & D3) OUT1(0); //D3
    if(states & B2) OUT2(0); //B2
    if(states & B1) OUT4(0); //B1
    if(states & K1) OUT5(0); //K1
    break;          
  case 4:           
    OUT4(1);        
    if(states & F3) OUT1(0); //F3
    if(states & D2) OUT2(0); //D2
    if(states & C2) OUT3(0); //C2
    // NC           
    break;          
  case 5:           
    OUT5(1);        
    if(states & G3) OUT1(0); //G3
    if(states & E2) OUT2(0); //E2
    if(states & F2) OUT3(0); //F2
    if(states & G2) OUT4(0); //G2
    i = 1;
    break;
  }

///// 方案二
//////////////////////////////////////////////////
/// 低/高 |   1   |   2   |   3   |   4   |   5
///   1   |   X   |  B3   |  D3   |   F3  |   G3
///   2   |   A3  |   X   |  B2   |   D2  |   E2
///   3   |   C3  |  A2   |  X    |   C2  |   F2
///   4   |   E3  |  C1   |  B1   |   X   |   G2
///   5   |   X   |  K2   |  K1   |   X   |   X
//////////////////////////////////////////////////
//    ALL_OFF();
//    switch(i++) {
//    case 1:
//        OUT1(0);
//        if(states & B3) {OUT2(1);}    //B3
//        if(states & D3) {OUT3(1);}    //D3
//        if(states & F3) {OUT4(1);}    //F3
//        if(states & G3) {OUT5(1);}    //G3
//        break;
//    case 2:
//        OUT2(0);
//        if(states & A3) {OUT1(1);}    //A3
//        if(states & B2) {OUT3(1);}    //B2
//        if(states & D2) {OUT4(1);}    //D2
//        if(states & E2) {OUT5(1);}    //E2
//        break;
//    case 3:
//        OUT3(0);
//        if(states & C3) {OUT1(1);}    //C3
//        if(states & A2) {OUT2(1);}    //A2
//        if(states & C2) {OUT4(1);}    //C2
//        if(states & F2) {OUT5(1);}    //F2
//        break;
//    case 4:
//        OUT4(0);
//        if(states & E3) {OUT1(1);}    //E3
//        if(states & C1) {OUT2(1);}    //C1
//        if(states & B1) {OUT3(1);}    //B1
//        if(states & G2) {OUT5(1);}    //G2
//        break;
//    case 5:
//        OUT5(0);
//        //NC
//        if(states & K2) {OUT2(1);}    //K2
//        if(states & K1) {OUT3(1);}    //K1
//        //NC
//        i = 1;
//        break;
//    }

}