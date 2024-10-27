#include "bsp_ws2812.h"
#include <string.h>

/*
ʱ�������					��Сֵ	����ֵ			���ֵ	��λ
Timeline name				Min.	Typical value	Max.	Unit
T 	��Ԫ���� 				0.89 	--- 			--- 	��s 
TOH 0�룬�ߵ�ƽʱ�� 			0.2 	0.295 			0.35 	��s 
TOL 0�룬�͵�ƽʱ�� 			0.55 	0.595 			1.2 	��s 
T1H 1�룬�ߵ�ƽʱ�� 			0.55 	0.595 			1.2 	��s 
T1L 1�룬�͵�ƽʱ�� 			0.2 	0.295 			0.35 	��s 
TRST Reset�룬�͵�ƽʱ��		80 		--- 			--- 	��s
*/
/*
4��Ƶ��42MHZ/4 = 10.5MHZ��0.096us 1������

0�룬 3�����ڣ���288��+ 7����672��
1�룬 7�����ڣ���672��+ 3����288��
RST,  1000�����ڣ��ͣ�

RGB: 3byte = 24bit => 24 * 10���� = 240������ = 30byte
RGB >> SPI: 3byte -> 30 byte

1byte=0x00:	(3�����ڣ��ߣ�+ 7���ͣ�) * 8 + (7�����ڣ��ߣ�+ 3���ͣ�) * 0 = {0xE0, 0x38, 0x0E, 0x03, 0x80,0xE0, 0x38, 0x0E, 0x03, 0x80}
1byte=0x01:	(3�����ڣ��ߣ�+ 7���ͣ�) * 7 + (7�����ڣ��ߣ�+ 3���ͣ�) * 1 = {0xE0, 0x38, 0x0E, 0x03, 0x80,0xE0, 0x38, 0x0E, 0x03, 0xF8}
1byte=0x02:	(3�����ڣ��ߣ�+ 7���ͣ�) * 6 + (7�����ڣ��ߣ�+ 3���ͣ�) * 1 = {0xE0, 0x38, 0x0E, 0x03, 0x80,0xE0, 0x38, 0x0E, 0x03, 0xF8}

*/

#define RGB_LEN     30

static uint8_t data[WS2812_NUM][RGB_LEN] = {0};


// ������ұ���ǰ�洢 0 �� 1 ��ӳ����
static const uint16_t lookup_table[2] = {0b1110000000, 0b1111111000};

// ��1��byteת��Ϊ10��byte
static void convert_byte_to_10bytes(uint8_t byte, uint8_t output[10]) {
	uint8_t temp[80];  // ��ʱ�洢80λ
	
	// Step 1: ʹ�ò��ұ�1��byteת��Ϊ80λ
	for (int i = 0; i < 8; i++) {
		// ʹ�ò��ұ�ֱ�Ӳ���ӳ��
		uint16_t mapping = lookup_table[(byte >> (7 - i)) & 1];
		
		// ��10λ��ӳ��ֵ����temp����
		for (int j = 0; j < 10; j++) {
			temp[i * 10 + j] = (mapping >> (9 - j)) & 1;
		}
	}
	
	// Step 2: ��80λ�����ư�8λһ�飬�����10��byte
	for (int i = 0; i < 10; i++) {
		output[i] = 0;  // ��ʼ�������ÿ��byte
		for (int j = 0; j < 8; j++) {
			// ��ÿ��8λ��������ϳ�һ��byte
			output[i] |= (temp[i * 8 + j] << (7 - j));
		}
	}
}

void ws2812_init()
{
}

void ws2812_off() 
{
	memset(data, 0, WS2812_NUM * RGB_LEN);
}

void ws2812_set_color(uint8_t id, uint32_t rgb) 
{
	if(id >= WS2812_NUM) return;
	
	uint8_t r = (rgb >> 16) & 0xFF;
	uint8_t g = (rgb >> 8)  & 0xFF;
	uint8_t b = (rgb >> 0)  & 0xFF;
	
	uint8_t tmp[10];
	// G
	convert_byte_to_10bytes(g, tmp);
	memcpy(data[id], tmp, 10);
	// R
	convert_byte_to_10bytes(r, tmp);
	memcpy(data[id] + 10, tmp, 10);
	// B
	convert_byte_to_10bytes(b, tmp);
	memcpy(data[id] + 20, tmp, 10);
}


void ws2812_display() {
//	for(uint8_t i = 0; i < WS2812_NUM; i++) {
//		for(uint8_t j = 0; j < 30; j++) {
//			WS2812_SPI_WRITE(data[i][j]);
//		}
//	}
//	// reset
//	WS2812_SPI_WRITE(0xFF);
//	for(uint8_t i = 0; i < 100; i++) {
//		WS2812_SPI_WRITE(0x00);
//	}
    
    SPI1_dma_tx_start(data[0], WS2812_NUM * RGB_LEN, 1);
}

void SPI1_dma_tx_on_finish() {
    static uint8_t state = 1;
    static uint8_t tmp = 0x00;
    if(state == 0) {
        SPI1_dma_tx_start(data[0], WS2812_NUM * RGB_LEN, 1);
//        printf("0\r\n");
    } else if(state == 1) {
        tmp = 0xFF;
        SPI1_dma_tx_start(&tmp, 1, 0);
//        printf("1\r\n");
    } else if(state == 2) {
        tmp = 0x00;
        SPI1_dma_tx_start(&tmp, 100, 0);
//        printf("2\r\n");
    }
    state++;
    if(state > 2) {
        state = 0;
    }
}

