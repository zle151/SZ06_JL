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
		if(Data&0x80)//��λ���У�Data�����λ���Ƹ�DM_DIN����
		{
			DM_DIN_H();
		}
		else
		{
			DM_DIN_L();
		}
		Data=Data<<1;//������1λ
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
	DM_Write_Command(0x09,0x00);//�����ʽ/�����ʽ
	DM_Write_Command(0x0A,0x0D);//���ȿ��ƣ������и��� ��0x00 - 0x0F)
	DM_Write_Command(0x0B,0x07);//ɨ����ƣ���ɨ��һ���ֽڣ�8λ����0x00 - 0x07)
	DM_Write_Command(0x0C,0x01);//����ģʽ����Ļ���أ�0x00�أ�0x01����
	DM_Write_Command(0x0F,0x00);//��ʾ��⣬0x00����ģʽ��0x01���ģʽ
}
//չʾһ��
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