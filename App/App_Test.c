#include "App.h"


#include "bsp_adkey.h"
#include "bsp_dot_matrix.h"
#include "bsp_touch_key.h"
#include "bsp_ws2812.h"
#include "bsp_dot_matrix.h"

void Test_init() {

  // dot matrix  // clk:  PA5     mosi: PA7  cs: pa6

  //    DM_init();
  //    DM_off_all();

  // ws2812  PB15 mosi
  ws2812_init();
  ws2812_set_color(0, 0x110000);
  ws2812_set_color(1, 0x001100);
  ws2812_set_color(2, 0x000011);
  ws2812_set_color(3, 0x111111);
  ws2812_display();
	
	//µãÕóÆÁ³õÊ¼»¯
	DM_init();
  printf("Test start\r\n");
}

void Test_ws2812() {
  static uint8_t i = 0;

  if (i == 0) {
    ws2812_set_color(0, 0x220000);
    ws2812_set_color(1, 0x002200);
    ws2812_set_color(2, 0x000022);
    ws2812_set_color(3, 0x222222);
  } else if (i == 1) {
    ws2812_set_color(0, 0x222222);
    ws2812_set_color(1, 0x220000);
    ws2812_set_color(2, 0x002200);
    ws2812_set_color(3, 0x000022);
  } else if (i == 2) {
    ws2812_set_color(0, 0x000022);
    ws2812_set_color(1, 0x222222);
    ws2812_set_color(2, 0x220000);
    ws2812_set_color(3, 0x002200);
  } else if (i == 3) {
    ws2812_set_color(0, 0x002200);
    ws2812_set_color(1, 0x000022);
    ws2812_set_color(2, 0x222222);
    ws2812_set_color(3, 0x220000);
  }
  i++;
  if (i >= 4) {
    i = 0;
  }
}

void Test_dot_matrix() {}

