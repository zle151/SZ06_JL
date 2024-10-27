#include "bsp_ec11.h"
#include <stdio.h>

uint8_t ec11_cur_state = 0; // 1. A�ж� 2. B�ж�

void EC11_init() {
  EC11_GPIO_INIT();

  printf("EC11 init\r\n");
}

void EC11_A_INT_FUNC() {
	ec11_cur_state = 1;
}

void EC11_B_INT_FUNC() {
	ec11_cur_state = 2;
}

void EC11_C_INT_FUNC() {
  // printf("EC11 C int\r\n");
  if(EC11_C_STATE()) {
    // �ߵ�ƽ
    EC11_on_up();
  } else {
    // �͵�ƽ
    EC11_on_down();
  }
}