#include "IR.h"
#include "Timer0.h"
#include "delay.h"
#include "digital.h"
#include <reg52.h>

unsigned char Command;

// 红外遥控调整电机转速

// SDCC 启动钩子函数，在 main() 之前执行
// unsigned char _sdcc_external_startup(void) {
//   P10 = 0;  // 第一时间拉低电机引脚
//   return 0; // 返回 0 让 SDCC 继续完成全局/静态变量初始化
// }

// sdcc 要先声明
void Timer0_Routine() __interrupt(1);
void Int0_Routine(void) __interrupt(0);

// 计数值和比较值
unsigned char Counter = 0;
unsigned char Compare = 0;

unsigned char Speed = 0;

void main() {

  P10 = 0;
  // P10 = 0;
  //   Delay1ms(10);

  Timer0_Init();
  IR_Init();

  while (1) {

    if (IR_GetDataFlag() || IR_GetRepeatFlag()) // 如果收到数据帧或者收到连发帧
    {

      Command = IR_GetCommand(); // 获取遥控器命令码
      if (Command == IR_0)       // 如果遥控器VOL-按键按下
      {
        Speed = 0;
        Compare = 0;
      }
      if (Command == IR_1) // 如果遥控器VOL+按键按下
      {
        Speed = 1;
        Compare = 50;
      }

      if (Command == IR_2) // 如果遥控器VOL+按键按下
      {
        Speed = 2;
        Compare = 70;
      }
      if (Command == IR_3) // 如果遥控器VOL+按键按下
      {
        Speed = 3;
        Compare = 100;
      }
    }

    if (P31 == 0) {
      Delay1ms(20);
      while (P31 == 0)
        ;
      Delay1ms(20);

      Speed++;
      if (Speed > 3) {
        Speed = 0;
      }

      //   switch (Speed) {
      //   case 0:
      //     Compare = 0;
      //     break;
      //   case 1:
      //     Compare = 33;
      //     break;
      //   case 2:
      //     Compare = 66;
      //     break;
      //   case 3:
      //     Compare = 100;
      //     break;
      //   }

      if (Speed == 0) {
        Compare = 0;
      } else if (Speed == 1) {
        Compare = 50;
      } else if (Speed == 2) {
        Compare = 75;
      } else if (Speed == 3) {
        Compare = 100;
      }
    }
    Digital(1, Speed);
  }
}

// // // 中断函数  支线  但还是会改变全局变量   单位是1毫秒
// TF0/计数器计数溢出执行
void Timer0_Routine() __interrupt(1) {
  // 静态变量
  //   static unsigned int time = 0;
  // 重置
  //   P2 = ~P2;

  TH0 = 0XFC; // 高位
  TL0 = 0X67; // 地位
  // time++;
  Counter++;
  if (Counter >= 100) {
    Counter = 0; // 周期是100   超过100置0重新开始
  }

  if (Counter < Compare) { // 比较计数值和比较值    决定引脚的电压是1/0
    P10 = 1;
  } else {
    P10 = 0;
  }
}
