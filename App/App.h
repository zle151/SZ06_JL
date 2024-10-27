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
    /* ����timer��tick��� */                          \
    if (task_timer_get_tick() < tick) tick = 0;        \
                                                       \
    if (task_timer_get_tick() - tick < interval){      \
      return;                                          \
    }                                                  \
    /*��������� >= TASK_INTERVAL*/                   \
    tick = task_timer_get_tick();                      \

#define CLIP_VALUE(value, min, max)                  \
    if (value < min) value = min;                     \
    else if (value > max) value = max;                \


/////////////// ����ҵ�� ///////////////////
    
void Test_init();
    
void Test_ws2812(); // ������ʾ
    
void Test_dot_matrix(); // ������ʾ
    
/////////////// ��ʽҵ�� ///////////////////
// --------------------- ������ʾ
extern int g_LED188_num_temperature;
extern int g_LED188_num_time;
extern uint8_t g_run_mode ; //0����ģʽ�� 1ʱ������ģʽ

extern uint8_t g_motor_mode ;// Mode 1, Mode 2, Mode 3
extern uint8_t g_motor_intensity ;// 0: Low, 1: Medium, 2: High
extern uint8_t g_motor_working ;// 0: Stop, 1: Working
extern uint8_t g_oled_refresh_need ;

void App_Display_init();  
void App_Display_LED188_task(); 
void App_Display_OLED_task();
void App_Display_OLED_task1();

// --------------------- �û�����
void App_Input_init();  
void App_Input_EC11_task();   
void App_Input_adkey_task();

// --------------------- ������
typedef enum {
  LockState_Default = 0, // Ĭ��״̬
  LockState_Input, // ��������״̬
  LockState_Unlock, // ����״̬
  LockState_Error, // �������״̬
  LockState_TryAgain, // ����
  LockState_Locked, // ����״̬
} LockState;

extern LockState g_lock_state;
extern char g_password[7]; // �洢�ַ�����֧��������������
extern uint16_t g_lock_time; // ����ʱ��, ��λ��
void App_Password_on_input(uint8_t i, char c);

// ---------------------����������
void App_Display_DM_task();
#endif