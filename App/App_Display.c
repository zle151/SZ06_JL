#include "App.h"

#include "bsp_led_188.h"
#include "bsp_dot_matrix.h"
#include "oled.h"

// �¶�
int g_LED188_num_temperature = 100;
// ʱ��
int g_LED188_num_time = 10;
// 0����ģʽ�� 1ʱ������ģʽ
uint8_t g_run_mode = 0; 

void App_Display_init(){
#if BUSINESS == BUSINESS_1
  // led188  1:c5 2:a7 3:a5 4:a3 5:a1
  LED188_init();
  LED188_set_charge(1);
  LED188_set_percent(0);
  LED188_set_num(g_LED188_num_temperature);

#elif BUSINESS == BUSINESS_2
  // oled
  OLED_Init();				//��ʼ��OLED

#elif BUSINESS == BUSINESS_3
  // oled
  OLED_Init();				//��ʼ��OLED
#elif BUSINESS == BUSINESS_4
  // oled
  OLED_Init();				//��ʼ��OLED
	//��������ʼ��
	DM_init();
#endif
	
}

//  LED188_set_num(i++);

void App_Display_LED188_task(){
  
  LED188_display();
}

uint8_t g_oled_refresh_need = 1;

// �����ַ�������
char* motor_mode_arr[] = {"1", "2", "3"}; 
char* force_arr[] = {"Low", "Medium", "High"}; 
char* working_arr[] = {"Stop", "Working"}; 

uint8_t g_motor_mode = 0;// Mode 1, Mode 2, Mode 3
uint8_t g_motor_intensity = 0;// 0: Low, 1: Medium, 2: High
uint8_t g_motor_working = 0;// 0: Stop, 1: Working

void App_Display_OLED_task(){

  if(!g_oled_refresh_need) return;
  g_oled_refresh_need = 0;
  
  OLED_Clear(0);

  // ��ʾ��ǰģʽ
  OLED_ShowString(0, 0, "Mode:", 16, 1);
  OLED_ShowString(72, 0, motor_mode_arr[g_motor_mode], 16, 1);

  // ��ʾ��ǰǿ��
  OLED_ShowString(0, 20, "Force:", 16, 1);
  OLED_ShowString(72, 20, force_arr[g_motor_intensity], 16, 1);
  
  // ��ʾ��ǰ����״̬
  OLED_ShowString(0, 40, "Status:", 16, 1);
  OLED_ShowString(72, 40, working_arr[g_motor_working], 16, 1);

  OLED_Refresh();
}

/*****************
1.Ĭ��  
Locked

2. ��������
�û��������ְ�����0-9����ʼ�������룬ÿ����һ�ΰ�������Ļ�ϻ���ʾ��һ��*,���磺
Locked
Enter PIN: `******`

3. ����������󣬰���`#��`����ȷ�ϡ�ϵͳ�����Ԥ������������֤������`*��`�������ǰ����

- ������ȷ����Ļ��ʾ Unlocked!
  ������������������һ��ʱ�䣬Ȼ��ص�����״̬��
- ���������Ļ��ʾ Try Again
- ����3���������������Ļ��ʾ Locked for 10 sec
  ��������״̬����Ļ��ʾ�ȴ�һ��ʱ����ٳ���
- ����״̬��Ļ����ʾ����ʱ��ϢTry Again in: 9 sec
*****************/
// 0: Ĭ��״̬, 1: ��������״̬, 2: ����״̬, 3: �������״̬, 4: ����״̬
LockState g_lock_state = 0;
char g_password[7] = {0}; // �洢�ַ�����֧��������������
uint16_t g_lock_time = 10; // ����ʱ��, ��λ��
uint8_t g_try_chances = 3; // ���Դ���

char str_buff[30] = {0};
void App_Display_OLED_task1(){
  // if(!g_oled_refresh_need) return;
  // g_oled_refresh_need = 0;
  static uint32_t g_lock_time_start = 0;
  static uint32_t delay_tick_start = 0;
  static uint32_t delay_tick = 0;
	int32_t time_left;
	
  if(delay_tick > 0){
    // ������Ҫ��ʱ�������ж��Ƿ���Ҫˢ��
    INTERVAL_CHECK(delay_tick_start, delay_tick);
		delay_tick = 0;
  }
  
  OLED_Clear(0);
	
	switch (g_lock_state)
  {
  case LockState_Default:
    OLED_ShowString(0, 0, "Locked", 16, 1);
    break;
  case LockState_Input:
    OLED_ShowString(0, 0, "Locked", 16, 1);
    OLED_ShowString(0, 20, "Enter PIN:", 16, 1);
    // ����g_password�ĳ��ȣ� ����n��*�ŵ��ַ�������ʾ*��
    char password_show[6] = {0};
    for(int i = 0; i < strlen(g_password); i++){
      password_show[i] = '*';
    }
    OLED_ShowString(0, 40, password_show, 16, 1);
    break;
  case LockState_Unlock:
    OLED_ShowString(0, 0, "Unlocked!", 16, 1);
	
    delay_tick_start = task_timer_get_tick();
    delay_tick = 1000;
	
    break;
  case LockState_TryAgain:
    if(g_try_chances == 0){
      g_lock_state = LockState_Locked;
      // �������
      memset(g_password, 0, sizeof(g_password));
      // ���ó��Դ���
      g_try_chances = 3;
      // ��¼��ʼʱ��
      g_lock_time_start = task_timer_get_tick();
    }else {
      OLED_ShowString(0, 0, "Try Again", 16, 1);
      // ��ʾʣ����� chances
      sprintf(str_buff, "%d chances to try.", g_try_chances);
      OLED_ShowString(0, 20, str_buff, 16, 1);
			g_try_chances--;

      // 1.5��֮������ΪInputģʽ
      delay_tick_start = task_timer_get_tick();
      delay_tick = 1500;
      
      g_lock_state = LockState_Input;
      // �������
      memset(g_password, 0, sizeof(g_password));
    }
    break;
  case LockState_Locked:
    // ��������ʱ���ʣ��ʱ��
    time_left = g_lock_time - (task_timer_get_tick() - g_lock_time_start) / 1000;
    if (time_left <= 0) {
      g_lock_state = LockState_Default;
      // �������
      memset(g_password, 0, sizeof(g_password));
      return;
    }

    if(strlen(g_password) == 0){
      sprintf(str_buff, "Locked for %d sec", time_left);
      OLED_ShowString(0, 0, str_buff, 16, 1);
    }else {
      sprintf(str_buff, "Try After: %d sec", time_left);
      OLED_ShowString(0, 0, str_buff, 16, 1);
    }
    break;
  default:
    break;
  }
	
  OLED_Refresh();
}
uint8_t dm_arr[]={	
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0xfe,0x10,0x10,0x10,0xfe,0x00,0x38,0x44,
	0x82,0xA2,0x44,0xB8,0x00,0xF2,0x92,0x92,
	0x92,0x9E,0x00,0x02,0x02,0xFE,0x02,0x02,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};//��������

uint32_t len = sizeof(dm_arr)/sizeof(uint8_t);
uint32_t cnt = sizeof(dm_arr)/sizeof(uint8_t) / ROW;
//����������
void App_Display_DM_task(){
	static int j = 0;

	DM_display(&dm_arr[j*8]);
	
//	printf("display len: %d, cnt: %d\n", len, cnt);

	if(++j>=cnt){
		j = 0;
	}
}