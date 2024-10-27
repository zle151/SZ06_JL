#ifndef __TASKS_H__
#define __TASKS_H__

#include "gd32f4xx.h"

#define TASK_STATE_STOP       0 // ����ֹͣ
#define TASK_STATE_RUN        1 // ���������
#define TASK_STATE_SUSPEND    2 // �������(��ͣ)

// ���庯��ָ������
typedef void (*Task_callback)(void);


typedef struct {
  
  uint8_t     state;      // ����״̬
  uint16_t    count;      // �����������������0���޸�����״̬
  uint16_t   period;      // �������ڣ�������msִ��һ�Σ��̶���
//  void (*callback)(void);
  Task_callback callback;        // ����ִ�лص�
  Task_callback callback_suspend; // �������ص�
  
} Task_t;

extern Task_t task_list[];
extern uint8_t task_cnt;

void Task_init(void);

// �����ִ��״̬�л� timer 1ms
void Task_switch_handler(void);

// ����ִ�еĴ�����
void Task_exec_handler(void);

// ��������������ȡ����
int Task_get_task_id(Task_callback func_name);

// ����������������ȡ����״̬
uint8_t Task_get_task_state(uint8_t index);

// ��ָͣ������
void Task_suspend(uint8_t index);

// �ָ�ָ������
void Task_resume(uint8_t index);


#endif