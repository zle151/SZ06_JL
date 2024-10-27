#include "App.h"

char* UNLOCK_PASSWORD = "123456";

//i=0~11, c='1'~'9', '*', '0' '#'
void App_Password_on_input(uint8_t i, char c){

  if (g_lock_state == LockState_Default) { // 默认状态
    g_lock_state = 1;
    // 清空密码
    memset(g_password, 0, sizeof(g_password));
    // 如果输入的是数字，添加到密码中
    if (i < 9) {
      g_password[0] = c;
      // 切换到输入密码状态
      g_lock_state =  LockState_Input;
    }
    return;
  }

  if (g_lock_state == LockState_Input) { // 输入密码状态
    // 如果输入的是数字，添加到密码中
    if (i < 9 || i == 10) {
      if (strlen(g_password) < 6) {
        g_password[strlen(g_password)] = c;
      }
    } else if (i == 9) {  // 如果输入的是*，清空密码
      memset(g_password, 0, sizeof(g_password));
    } else if (i == 11) { // 如果输入的是#，验证密码
      // 如果密码没输入，不做处理
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

  // 锁定状态
  if (g_lock_state == LockState_Locked) {
    g_password[0] = c;
    return;
  }
	
	// 解锁状态
	if (g_lock_state == LockState_Unlock){
		g_lock_state = LockState_Default;
		return;
	}

}