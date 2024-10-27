#include "bsp_dot_matrix.h"


void write_byte(uint8_t dat);
void write(uint8_t address, uint8_t dat);


static void DM_GPIO_Init(void)
{
	DM_SINGLE_PIN_INIT(DM_CLK_RCU,DM_CLK_PORT,DM_CLK_PIN);
	DM_SINGLE_PIN_INIT(DM_CS_RCU,DM_CS_PORT,DM_CS_PIN);
	DM_SINGLE_PIN_INIT(DM_DIN_RCU,DM_DIN_PORT,DM_DIN_PIN);
}

static void Write_DM_byte(uint8_t Data)
{
	uint8_t i;
	for(i=8;i>=1;i--)
	{
//		delay_1ms(3);
		DM_CLK_L();
		if(Data&0x80)//高位先行，Data把最高位复制给DM_DIN引脚
		{
			DM_DIN_H();
		}
		else
		{
			DM_DIN_L();
		}
		Data=Data<<1;//向左移1位
		DM_CLK_H();
	}
}
 
static void DM_Write_Command(uint8_t Address,uint8_t Data)
{		
	DM_CS_L();
	Write_DM_byte(Address);
	Write_DM_byte(Data);
	DM_CS_H();
}

void DM_init() {
	DM_GPIO_Init();
	DM_Write_Command(0x09,0x00);//编码格式/译码格式
	DM_Write_Command(0x0A,0x0D);//亮度控制，可自行更改 （0x00 - 0x0F)
	DM_Write_Command(0x0B,0x07);//扫描控制，需扫描一个字节（8位）（0x00 - 0x07)
	DM_Write_Command(0x0C,0x01);//掉电模式，屏幕开关（0x00关，0x01开）
	DM_Write_Command(0x0F,0x00);//显示检测，0x00正常模式，0x01检测模式
}
//展示一屏
void DM_display(uint8_t arr[8]) {
	for(int i=1;i<9;i++){
		DM_Write_Command(i,arr[i-1]);
	}
}

void DM_off_all(void)
{
	for(int i=1;i<9;i++){
		DM_Write_Command(i,0x00);
	}
}