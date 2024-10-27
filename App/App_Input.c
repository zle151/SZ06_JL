#include "App.h"
#include "bsp_ec11.h"
#include "bsp_adkey.h"
#include "bsp_led_188.h"
#include "bsp_touch_key.h"

void App_Input_init() {
  
#if BUSINESS == BUSINESS_1
  // EC11 Test
  EC11_init();
#elif BUSINESS == BUSINESS_2
  // ADKey Test
  ADkey_init();
#elif BUSINESS == BUSINESS_3
  // Touch Key Test
  TK_init();  // SDA: PB7 SCL: PB6  INT: PB8
#elif BUSINESS == BUSINESS_4
  // Touch Key Test
  TK_init();  // SDA: PB7 SCL: PB6  INT: PB8
#endif
}

static uint32_t debounce_key_ticks = 0;
void EC11_on_down() {
  INTERVAL_CHECK(debounce_key_ticks, 100);

  printf("EC11 down\r\n");
  g_run_mode = !g_run_mode;

  LED188_set_charge(!g_run_mode);
  LED188_set_percent(g_run_mode);
  LED188_set_num(g_run_mode ? g_LED188_num_time : g_LED188_num_temperature);
}

void EC11_on_up() { printf("EC11 up\r\n"); }

void EC11_on_turn(uint8_t is_clockwise) {
  if (g_run_mode == 0) {
    // 待机模式，修改温度
    if (is_clockwise) {
      g_LED188_num_temperature++;
    } else {
      g_LED188_num_temperature--;
    }
    CLIP_VALUE(g_LED188_num_temperature, 80, 199);
    LED188_set_num(g_LED188_num_temperature);

  } else if (g_run_mode == 1) {
    // 修改时间模式
    if (is_clockwise) {
      g_LED188_num_time++;
    } else {
      g_LED188_num_time--;
    }
    CLIP_VALUE(g_LED188_num_time, 1, 60);
    LED188_set_num(g_LED188_num_time);
  }
}

static uint32_t debounce_turn_ticks = 0;
void App_Input_EC11_task() {

  // 是0，说明没检测到中断（没旋转）
  if (ec11_cur_state == 0) return;

  // 5ms内的连续变化，不予理会，防止抖动
  //  INTERVAL_CHECK(debounce_turn_ticks, 5);

  if (ec11_cur_state == 1) {  // A边缘产生中断
    // 一定要清零
    ec11_cur_state = 0;

    if (EC11_A_STATE()) {  // A上升沿

      if (EC11_B_STATE()) {
        printf("CCW: 1\n");
        EC11_on_turn(0);
      } else {
        printf("CW: 1\n");
        EC11_on_turn(1);
      }
    } else {  // A下降沿
      if (EC11_B_STATE()) {
        printf("CW: 2\n");
        EC11_on_turn(1);
      } else {
        printf("CCW: 2\n");
        EC11_on_turn(0);
      }
    }
  }
#if 0
  else if(ec11_cur_state == 2) { // B边缘产生中断
    // 一定要清零
    ec11_cur_state = 0;

    if(EC11_B_STATE()) { // B上升沿

      if(EC11_A_STATE()) {
        printf("CW: 3\n");
      } else {
        printf("CCW: 3\n");
      }

    } else {	// B下降沿
      if(EC11_A_STATE()) {
        printf("CCW: 4\n");
      } else {
        printf("CW: 4\n");
      }
    }
  }
#endif

}


void App_Input_adkey_task() { 
  ADkey_scan(); 
}
// uint8_t g_motor_mode = 0;// Mode 1, Mode 2, Mode 3
// uint8_t g_motor_intensity = 0;// 0: Low, 1: Medium, 2: High
// uint8_t g_motor_working = 0;// 0: Stop, 1: Working
// uint8_t g_oled_refresh_need = 1;
void ADKey_on_down(uint8_t key) { 
  printf("adkey %d down\r\n", key + 1);
  switch (key)
  {
  case 0:
    g_motor_mode = (g_motor_mode + 1) % 3;
    break;
  case 1:
    g_motor_intensity = (g_motor_intensity + 1) % 3;
    break;
  case 2:
    g_motor_working = (g_motor_working + 1) % 2;
    break;
  default:
    break;
  }
  g_oled_refresh_need = 1;
}

void ADKey_on_up(uint8_t key) { 
  printf("adkey %d up\r\n", key + 1);
}


void TK_on_up(uint8_t i) {
  char c = ' ';
  if (i > 8) {
    if (i == 9) c = '*';
    if (i == 10) c = '0';
    if (i == 11) c = '#';
  } else {
    c = '1' + i;
  }
  printf("tk %c up\r\n", c);
}
void TK_on_down(uint8_t i) {
  char c = ' ';
  if (i > 8) {
    if (i == 9) c = '*';
    if (i == 10) c = '0';
    if (i == 11) c = '#';
  } else {
    c = '1' + i;
  }
  printf("tk %c down\r\n", c);
  App_Password_on_input(i, c);
}
