#include "digital.h"
#include "delay.h"
#include <reg52.h>
// 显示数字几  共阴  反过来读
// 2   0101 1011    对应顺序   dp gfedcba
// unsigned char DigitalNum[] = {0X3F, 0X06, 0X5B, 0X4F, 0X66,
//                               0X6D, 0X7D, 0X07, 0X7F, 0X6F};

// // x：第几个亮    num：亮数字几
// void Digital(unsigned char x, unsigned char num) {

//   switch (x) {
//   case 1: // 第1个数码管
//     P22 = 0;
//     P23 = 0;
//     P24 = 0;
//     break;
//   case 2: // 第2个数码管
//     P22 = 1;
//     P23 = 0;
//     P24 = 0;
//     break;
//   case 3: // 第3个数码管
//     P22 = 0;
//     P23 = 1;
//     P24 = 0;
//     break;
//   case 4: // 第4个数码管
//     P22 = 1;
//     P23 = 1;
//     P24 = 0;
//     break;
//   case 5: // 第5个数码管
//     P22 = 0;
//     P23 = 0;
//     P24 = 1;
//     break;
//   case 6: // 第6个数码管
//     P22 = 1;
//     P23 = 0;
//     P24 = 1;
//     break;
//   case 7: // 第7个数码管
//     P22 = 0;
//     P23 = 1;
//     P24 = 1;
//     break;
//   case 8: // 第8个数码管
//     P22 = 1;
//     P23 = 1;
//     P24 = 1;
//     break;
//   }
//   P0 = DigitalNum[num];
//   Delay1ms(1);
//   P0 = 0X00;
// }



// =========================================中断函数用============================================================



// 缓存数组   用来显示数据              10是不显示的
unsigned char DigitalCache[] = {0, 10, 10, 10, 10, 10, 10, 10, 10};

// 2   0101 1011    对应顺序   dp gfedcba      0x00:0     0x40 :-
unsigned char DigitalNum[] = {0X3F, 0X06, 0X5B, 0X4F, 0X66, 0X6D,
                              0X7D, 0X07, 0X7F, 0X6F, 0x00, 0x40};

void Digital_Set(unsigned char x,unsigned char num){
  DigitalCache[x]=num;
}




// x：第几个亮    num：亮数字几
void Digital(unsigned char x, unsigned char num) {

  P0 = 0X00; // 段选消影

  switch (x) { // 位选
  case 8:      // 第1个数码管
    P22 = 0;
    P23 = 0;
    P24 = 0;
    break;
  case 7: // 第2个数码管
    P22 = 1;
    P23 = 0;
    P24 = 0;
    break;
  case 6: // 第3个数码管
    P22 = 0;
    P23 = 1;
    P24 = 0;
    break;
  case 5: // 第4个数码管
    P22 = 1;
    P23 = 1;
    P24 = 0;
    break;
  case 4: // 第5个数码管
    P22 = 0;
    P23 = 0;
    P24 = 1;
    break;
  case 3: // 第6个数码管
    P22 = 1;
    P23 = 0;
    P24 = 1;
    break;
  case 2: // 第7个数码管
    P22 = 0;
    P23 = 1;
    P24 = 1;
    break;
  case 1: // 第8个数码管
    P22 = 1;
    P23 = 1;
    P24 = 1;
    break;
  }
  P0 = DigitalNum[num];
}

void Digital_Loop() {
  static unsigned char i=1;

  Digital(i, DigitalCache[i]);//从下标1开始显示
  i++;
  if (i >= 9)i = 1;//显示数组的原因
}