#include "bsp_adkey.h"
#include "ADC0.h"
#include <stdio.h>

//#define ADK_VAL_KEY_1_2_3   730
//#define ADK_VAL_KEY_1_2     770
//#define ADK_VAL_KEY_1_3     880
//#define ADK_VAL_KEY_1       950
//#define ADK_VAL_KEY_2_3     1780
//#define ADK_VAL_KEY_2       2050
//#define ADK_VAL_KEY_3       3140
//#define ADK_VAL_UP          4050

#define ADK_VAL_KEY_1_2_3   9999
#define ADK_VAL_KEY_1_2     9999
#define ADK_VAL_KEY_1_3     9999
#define ADK_VAL_KEY_1       950
#define ADK_VAL_KEY_2_3     9999
#define ADK_VAL_KEY_2       2050
#define ADK_VAL_KEY_3       3140
#define ADK_VAL_UP          4050

#define FIX_RATIO         (1.01)  

#define IS_CATCH(val, def) ((val > (def - 50)) && (val < (def + 50)))

#define KEY1 (1 << 0)
#define KEY2 (1 << 1)
#define KEY3 (1 << 2)
/////// 7 6 5 4 3   2     1     0
/////// x x x x x   key3  key2  key1
static uint8_t key_state = 0;


uint8_t ADkey_is_down(uint8_t key) { return key_state & (1 << key); }
uint8_t ADkey_is_up(uint8_t key) { return !ADkey_is_down(key); }

#define ADC_COUNT 12

uint16_t recent_values[ADC_COUNT];
uint8_t recent_index = 0;

// 取最近的N次采样值平均值（均值滤波）
// 去掉最大最小值，防止干扰
uint16_t calc_recent_avg() {
  uint32_t sum = 0;
  uint16_t min = 4096;
  uint16_t max = 0;
  for (int i = 0; i < ADC_COUNT; i++) {
    uint16_t v = recent_values[i];
    if (v < min) min = v;
    if (v > max) max = v;
    sum += v;
  }
  sum -= min;
  sum -= max;
  return sum / (ADC_COUNT - 2); 
}
#include <string.h>
void ADkey_init() {
  // 将recent_values数组初始化为0
  memset(recent_values, 0, sizeof(recent_values));
}

void ADkey_scan() {
  // 2ms in
  static uint8_t last_state = 0;
  static uint8_t enable = 0;
  static uint16_t pre_avg = 0;
  static uint16_t new_avg = 0;
  static uint8_t new_avg_repeat = 0;

  uint16_t val = ADKEY_GET_VALUE();
  recent_values[recent_index] = val;
  recent_index = (recent_index + 1) % ADC_COUNT;

  // 攒够ADC_COUNT后，再把enable置1，开始计算
  if(!enable){
    if(recent_index == ADC_COUNT - 1){
      enable = 1;
    }
    return;
  }

  uint16_t avg = calc_recent_avg();

  // 新的avg连续出现3次，认为是稳定状态, 才可放行
  if(new_avg_repeat < 3){
    // 1. 新的avg与pre偏差20以上, 记录为new_avg
    if (abs(avg - pre_avg) > 20){
      // 2. 新的avg与new_avg相差不大，计数器+1
      if(abs(avg - new_avg) < 10){
        new_avg_repeat++;
      }
      new_avg = avg;
    }
    return;
  }
  new_avg_repeat = 0;

  // 根据真实芯片输入电压，对采样值进行修正
  avg = (uint16_t)(avg * FIX_RATIO); // 保证在默认状态下，读到的是4050左右

  if (avg > (ADK_VAL_UP - 200)) {
    key_state = 0;
  } else if (IS_CATCH(avg, ADK_VAL_KEY_3)) {
    key_state |= KEY3;
  } else if (IS_CATCH(avg, ADK_VAL_KEY_2)) {
    key_state |= KEY2;
  } else if (IS_CATCH(avg, ADK_VAL_KEY_2_3)) {
    key_state |= KEY2;
    key_state |= KEY3;
  } else if (IS_CATCH(avg, ADK_VAL_KEY_1)) {
    key_state |= KEY1;
  } else if (IS_CATCH(avg, ADK_VAL_KEY_1_3)) {
    key_state |= KEY1;
    key_state |= KEY3;
  } else if (IS_CATCH(avg, ADK_VAL_KEY_1_2)) {
    key_state |= KEY1;
    key_state |= KEY2;
  } else if (IS_CATCH(avg, ADK_VAL_KEY_1_2_3)) {
    key_state |= KEY1;
    key_state |= KEY2;
    key_state |= KEY3;
  }

  // printf("avg: %d, vol: %.2fV state: 0x%02X\n", avg, avg * 3.3 / 4096 , key_state);
  printf("avg: %d\n", avg);

  // 比较差异
  uint8_t diff = last_state ^ key_state;
  
  if (diff == 0) return;    // 无变化
  
  // 记录当前状态
  last_state = key_state;

  // 根据差异调用回调
  //            printf("adk: %d %d %d 0x%02X 0x%02X 0x%02X\r\n", avg, val, sum,
  //            diff, last, key_state);
  if (diff & KEY1) {
    if (key_state & KEY1) {
      // 按下
      ADKey_on_down(0);
    } else {
      // 抬起
      ADKey_on_up(0);
    }
  }
  if (diff & KEY2) {
    if (key_state & KEY2) {
      // 按下
      ADKey_on_down(1);
    } else {
      // 抬起
      ADKey_on_up(1);
    }
  }
  if (diff & KEY3) {
    if (key_state & KEY3) {
      // 按下
      ADKey_on_down(2);
    } else {
      // 抬起
      ADKey_on_up(2);
    }
  }
}