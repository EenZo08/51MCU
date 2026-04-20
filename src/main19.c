#include "Timer0.h"
#include "delay.h"
#include <reg52.h>


// PWM模拟DA 数字-模拟转换     P21亮灭 闪   和p21=0是GPIO输出 只有0或1   DAC是平滑电压  会平滑的变动 闪才是DAC输出

// 计数值和比较值
unsigned char Counter = 0;
unsigned char Compare = 0;

unsigned char i;

void main() {

  Timer0_Init();
  while (1) {

    for (i = 0; i < 100; i++) {
      Compare = i;
      Delay1ms(10);
    }
    for (i = 100; i > 0; i--) {
      Compare = i;
      Delay1ms(10);
    }
  }
}

// // // 中断函数  支线  但还是会改变全局变量   单位是1毫秒
// TF0/计数器计数溢出执行
void Timer0_Routine() __interrupt(1) {
  // 静态变量
  //   static unsigned int time = 0;
  // 重置
  TH0 = 0XFC; // 高位
  TL0 = 0X67; // 地位
  // time++;
  Counter++;
  if (Counter >= 100)
    Counter = 0; // 周期是100   超过100置0重新开始

  if (Counter < Compare) { // 比较计数值和比较值    决定引脚的电压是1/0
    P21 = 1;
  } else {
    P21 = 0;
  }
}
