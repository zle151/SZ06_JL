#include "App.h"

char* UNLOCK_PASSWORD = "123456";

//i=0~11, c='1'~'9', '*', '0' '#'
void App_Password_on_input(uint8_t i, char c){

  if (g_lock_state == LockState_Default) { // Ĭ��״̬
    g_lock_state = 1;
    // �������
    memset(g_password, 0, sizeof(g_password));
    // �������������֣���ӵ�������
    if (i < 9) {
      g_password[0] = c;
      // �л�����������״̬
      g_lock_state =  LockState_Input;
    }
    return;
  }

  if (g_lock_state == LockState_Input) { // ��������״̬
    // �������������֣���ӵ�������
    if (i < 9 || i == 10) {
      if (strlen(g_password) < 6) {
        g_password[strlen(g_password)] = c;
      }
    } else if (i == 9) {  // ����������*���������
      memset(g_password, 0, sizeof(g_password));
    } else if (i == 11) { // ����������#����֤����
      // �������û���룬��������
      if (strlen(g_password) == 0) {
        return;
      }

      if (strcmp(g_password, UNLOCK_PASSWORD) == 0) {
        g_lock_state = LockState_Unlock;
				memset(g_password, 0, sizeof(g_password));
      } else {
        g_lock_state = LockState_TryAgain;
      }
    }
    return;
  }

  // ����״̬
  if (g_lock_state == LockState_Locked) {
    g_password[0] = c;
    return;
  }
	
	// ����״̬
	if (g_lock_state == LockState_Unlock){
		g_lock_state = LockState_Default;
		return;
	}

}