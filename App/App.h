#ifndef __APP_H__
#define __APP_H__

#include "gd32f4xx.h"
#include "tasks_timer.h"
#include "systick.h"
#include <stdio.h>
#include <string.h>


#define BUSINESS_1      1
#define BUSINESS_2      2
#define BUSINESS_3      3
#define BUSINESS_4      4
#define BUSINESS_TEST   10

#ifndef BUSINESS
#define BUSINESS        BUSINESS_1
#endif

#define INTERVAL_CHECK(tick, interval)                 \
    /* 处理timer的tick溢出 */                          \
    if (task_timer_get_tick() < tick) tick = 0;        \
                                                       \
    if (task_timer_get_tick() - tick < interval){      \
      return;                                          \
    }                                                  \
    /*检查任务间隔 >= TASK_INTERVAL*/                   \
    tick = task_timer_get_tick();                      \

#define CLIP_VALUE(value, min, max)                  \
    if (value < min) value = min;                     \
    else if (value > max) value = max;                \


/////////////// 测试业务 ///////////////////
    
void Test_init();
    
void Test_ws2812(); // 测试显示
    
void Test_dot_matrix(); // 测试显示
    
/////////////// 正式业务 ///////////////////
// --------------------- 界面显示
extern int g_LED188_num_temperature;
extern int g_LED188_num_time;
extern uint8_t g_run_mode ; //0待机模式， 1时间设置模式

extern uint8_t g_motor_mode ;// Mode 1, Mode 2, Mode 3
extern uint8_t g_motor_intensity ;// 0: Low, 1: Medium, 2: High
extern uint8_t g_motor_working ;// 0: Stop, 1: Working
extern uint8_t g_oled_refresh_need ;

void App_Display_init();  
void App_Display_LED188_task(); 
void App_Display_OLED_task();
void App_Display_OLED_task1();

// --------------------- 用户输入
void App_Input_init();  
void App_Input_EC11_task();   
void App_Input_adkey_task();

// --------------------- 密码锁
typedef enum {
  LockState_Default = 0, // 默认状态
  LockState_Input, // 输入密码状态
  LockState_Unlock, // 解锁状态
  LockState_Error, // 密码错误状态
  LockState_TryAgain, // 重试
  LockState_Locked, // 锁定状态
} LockState;

extern LockState g_lock_state;
extern char g_password[7]; // 存储字符，以支持任意内容密码
extern uint16_t g_lock_time; // 锁定时间, 单位秒
void App_Password_on_input(uint8_t i, char c);

// ---------------------点阵屏任务
void App_Display_DM_task();
#endif