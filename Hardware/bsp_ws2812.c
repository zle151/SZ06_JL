#include "bsp_ws2812.h"
#include <string.h>

/*
时序表名称					最小值	典型值			最大值	单位
Timeline name				Min.	Typical value	Max.	Unit
T 	码元周期 				0.89 	--- 			--- 	μs 
TOH 0码，高电平时间 			0.2 	0.295 			0.35 	μs 
TOL 0码，低电平时间 			0.55 	0.595 			1.2 	μs 
T1H 1码，高电平时间 			0.55 	0.595 			1.2 	μs 
T1L 1码，低电平时间 			0.2 	0.295 			0.35 	μs 
TRST Reset码，低电平时间		80 		--- 			--- 	μs
*/
/*
4分频，42MHZ/4 = 10.5MHZ，0.096us 1个周期

0码， 3个周期（高288）+ 7（低672）
1码， 7个周期（高672）+ 3（低288）
RST,  1000个周期（低）

RGB: 3byte = 24bit => 24 * 10周期 = 240个周期 = 30byte
RGB >> SPI: 3byte -> 30 byte

1byte=0x00:	(3个周期（高）+ 7（低）) * 8 + (7个周期（高）+ 3（低）) * 0 = {0xE0, 0x38, 0x0E, 0x03, 0x80,0xE0, 0x38, 0x0E, 0x03, 0x80}
1byte=0x01:	(3个周期（高）+ 7（低）) * 7 + (7个周期（高）+ 3（低）) * 1 = {0xE0, 0x38, 0x0E, 0x03, 0x80,0xE0, 0x38, 0x0E, 0x03, 0xF8}
1byte=0x02:	(3个周期（高）+ 7（低）) * 6 + (7个周期（高）+ 3（低）) * 1 = {0xE0, 0x38, 0x0E, 0x03, 0x80,0xE0, 0x38, 0x0E, 0x03, 0xF8}

*/

#define RGB_LEN     30

static uint8_t data[WS2812_NUM][RGB_LEN] = {0};


// 定义查找表，提前存储 0 和 1 的映射结果
static const uint16_t lookup_table[2] = {0b1110000000, 0b1111111000};

// 将1个byte转换为10个byte
static void convert_byte_to_10bytes(uint8_t byte, uint8_t output[10]) {
	uint8_t temp[80];  // 临时存储80位
	
	// Step 1: 使用查找表将1个byte转换为80位
	for (int i = 0; i < 8; i++) {
		// 使用查找表直接查找映射
		uint16_t mapping = lookup_table[(byte >> (7 - i)) & 1];
		
		// 将10位的映射值存入temp数组
		for (int j = 0; j < 10; j++) {
			temp[i * 10 + j] = (mapping >> (9 - j)) & 1;
		}
	}
	
	// Step 2: 将80位二进制按8位一组，打包成10个byte
	for (int i = 0; i < 10; i++) {
		output[i] = 0;  // 初始化输出的每个byte
		for (int j = 0; j < 8; j++) {
			// 将每组8位二进制组合成一个byte
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

