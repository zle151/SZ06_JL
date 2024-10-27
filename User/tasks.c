#include "tasks.h"



// �����ִ��״̬�л� timer 1msִ��һ��
void Task_switch_handler(void) {
  for(uint8_t i = 0; i < task_cnt; i++) {
    // �ж���������ǹ���״̬�������л��ж�
    if (task_list[i].state == TASK_STATE_SUSPEND) {
      continue;
    }

    if(task_list[i].count) task_list[i].count--;

    if (task_list[i].count == 0) {
      // ����״̬state�л�
      task_list[i].state = TASK_STATE_RUN;
      // ����count����ֵ
      task_list[i].count = task_list[i].period;
    }
  }
}

// ����ִ�еĴ�����
void Task_exec_handler(void) {
  // ��״̬Ϊ TASK_STATE_RUN ������������ǻָ���STOP
  for (uint8_t i = 0; i < task_cnt; i++) {
    if (task_list[i].state == TASK_STATE_RUN) {
      task_list[i].callback();
      task_list[i].state = TASK_STATE_STOP;
    }
  }
}

// �����������������Ҷ�Ӧλ��
int Task_get_task_id(Task_callback func_name) {
  for (uint8_t i = 0; i < task_cnt; i++) {
    if(task_list[i].callback == func_name) {
      return i;
    }
  }
  return -1;
}

// ����������������ȡ����״̬
uint8_t Task_get_task_state(uint8_t index) {
  if(index >= task_cnt) return TASK_STATE_STOP;
  return task_list[index].state;
}

// ��ָͣ������
void Task_suspend(uint8_t index) {
  if(index >= task_cnt) return;

  task_list[index].state = TASK_STATE_SUSPEND;

  // ����ص���������NULL����ִ��
  if (task_list[index].callback_suspend != NULL) {
    task_list[index].callback_suspend();
  }
}

// �ָ�ָ������
void Task_resume(uint8_t index) {
  if(index >= task_cnt) return;

  // ����count����ֵ
  task_list[index].count = task_list[index].period;
  task_list[index].state = TASK_STATE_RUN;

}