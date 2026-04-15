// #include "LCD1602.h"
#include "Delay.h"
#include "Timer0.h"
#include "reg52.h"
#include "stdint.h"

// 记录led状态
unsigned char x = 0xFE;
// 记录led模式
unsigned char mode = 0;

// 方法二：用纯C内联函数实现循环移位（可选，更清晰）
static inline unsigned char _crol_(unsigned char val, unsigned char n) {
  n &= 0x07; // 确保移位位数不超过7
  return (val << n) | (val >> (8 - n));
}

static inline unsigned char _cror_(unsigned char val, unsigned char n) {
  n &= 0x07;
  return (val >> n) | (val << (8 - n));
}

// void Key(){

//     if(P31==0){
//         Delay1ms(20);
//         while(P31==0);
//         x=_crol_(x,1);
//         P2=x;
//         Delay1ms(20);
//     }
//     if(P32==0){
//         Delay1ms(20);
//         while(P32==0);
//          x=_cror_(x,1);
//         P2=x;
//         Delay1ms(20);
//     }

// }

void main() {
  // 先初始化定时器
    Timer0_Init();

  while (1) {
    if (P31 == 0) {
      Delay1ms(20);
      while (P31 == 0)
        ;
      mode = 0;
    // x=0xFE;
    // P2=x;
      Delay1ms(20);
    }
    if (P30 == 0) {
      Delay1ms(20);
      while (P30 == 0)
        ;
      mode = 1;
    //   x=0xFF;
    //   P2=x;
      Delay1ms(20);
    }
  }
}
unsigned int time = 0;
// // 中断函数  支线  但还是会改变全局变量   单位是1毫秒  TF0/计数器计数溢出执行
void Timer0_Routine() __interrupt(1) {


  // 重置
  TH0 = 0XFC; // 高位
  TL0 = 0X67; // 地位
  time++;
  if (time >= 500) {
    if (mode) {
      x = _crol_(x, 1);
    //   x=0xFE;
      P2 = x;
    }else{
        x = _cror_(x, 1);
        // x=0x00;
        P2 = x;
    }

    time = 0;
  }
}