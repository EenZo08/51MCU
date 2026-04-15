#include "digital.h"
#include "delay.h"
#include <reg52.h>
// 显示数字几  共阴  反过来读
// 2   0101 1011    对应顺序   dp gfedcba
unsigned char DigitalNum[] = {0X3F, 0X06, 0X5B, 0X4F, 0X66,
                              0X6D, 0X7D, 0X07, 0X7F, 0X6F};

// x：第几个亮    num：亮数字几
void Digital(uint8_t x, uint8_t num) {

  switch (x) {
  case 1: // 第1个数码管
    P22 = 0;
    P23 = 0;
    P24 = 0;
    break;
  case 2: // 第2个数码管
    P22 = 1;
    P23 = 0;
    P24 = 0;
    break;
  case 3: // 第3个数码管
    P22 = 0;
    P23 = 1;
    P24 = 0;
    break;
  case 4: // 第4个数码管
    P22 = 1;
    P23 = 1;
    P24 = 0;
    break;
  case 5: // 第5个数码管
    P22 = 0;
    P23 = 0;
    P24 = 1;
    break;
  case 6: // 第6个数码管
    P22 = 1;
    P23 = 0;
    P24 = 1;
    break;
  case 7: // 第7个数码管
    P22 = 0;
    P23 = 1;
    P24 = 1;
    break;
  case 8: // 第8个数码管
    P22 = 1;
    P23 = 1;
    P24 = 1;
    break;
  }
  P0 = DigitalNum[num];
  Delay1ms(1);
  P0 = 0X00;
}
