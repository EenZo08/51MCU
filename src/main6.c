#include "Delay.h"
#include "LCD1602.h"
#include "Timer0.h"
#include "reg52.h"
#include "stdint.h"

// 定时器的时钟计数案例    状态机+中断
//  记录秒数
unsigned int Sce = 0;
// 记录分钟数
unsigned int Min = 0;
// 记录小时数
unsigned int Hou = 0;

// 暂停标志位 设置参数中断 中断函数
unsigned char Pause = 0;

void main() {

  // 先初始化定时器
  Timer0_Init();
  // 初始化LCD
  LCD_Init();

  while (1) {
    // 暂停计数按键  P30 第二个按键
    if (P30 == 0) {
      Delay1ms(20);
      // 暂停
      Pause = !Pause;
      while (P30 == 0);
    }

    // 重置功能
    if (P31 == 0) {
      Delay1ms(20);

      while (P31 == 0);
      // 重置时间
      Sce = 0;
      Min = 0;
      Hou = 0;

      // 并且设置暂停
      Pause = 1;
      // 显示暂停

      Delay1ms(20);
    }

    if (Pause) {
      LCD_ShowString(1, 12, "Pause");
    } else {
      LCD_ShowString(1, 12, "Count");
    }

    // 实时刷新时分秒的状态
    LCD_ShowString(1, 1, "CLOCK:");
    // P2=0;
    LCD_ShowNum(2, 7, Sce, 2);
    LCD_ShowNum(2, 4, Min, 2);
    LCD_ShowNum(2, 1, Hou, 2);
    LCD_ShowString(2, 3, ":");
    LCD_ShowString(2, 6, ":");
  }
}

// // 中断函数  支线  但还是会改变全局变量   单位是1毫秒  TF0/计数器计数溢出执行
void Timer0_Routine() __interrupt(1) {
  static unsigned int time = 0;
  // 重置
  TH0 = 0XFC; // 高位
  TL0 = 0X67; // 地位
  if (!Pause) {
    time++;
    if (time >= 1000) { // 按秒计数   链接main函数的变量  sec min hou
                        // 主函数和中断函数的关系

      Sce++;
      if (Sce >= 60) {
        Sce = 0;
        Min++;
        if (Min >= 60) {
          Min = 0;
          Hou++;
          if (Hou >= 24) {
            Hou = 0;
          }
        }
      }

      time = 0;
    }
  }
}

// 简单版本
//  #include "Delay.h"
//  #include "LCD1602.h"
//  #include "Timer0.h"
//  #include "reg52.h"
//  #include "stdint.h"

// // 定时器的时钟计数案例
// //  记录秒数
// unsigned int Sce = 0;
// // 记录分钟数
// unsigned int Min = 0;
// // 记录小时数
// unsigned int Hou = 0;

// void main() {

//   // 先初始化定时器
//   Timer0_Init();
//   // 初始化LCD
//   LCD_Init();

//   while (1) {

//     // 重置功能
//     if (P31 == 0) {
//       Delay1ms(20);
//       while (P31 == 0)
//         ;
//       // 重置时间
//       Sce = 0;
//       Min = 0;
//       Hou = 0;

//       Delay1ms(20);
//     }

//     // 实时刷新时分秒的状态
//     LCD_ShowString(1, 1, "CLOCK:");
//     // P2=0;
//     LCD_ShowNum(2, 7, Sce, 2);
//     LCD_ShowNum(2, 4, Min, 2);
//     LCD_ShowNum(2, 1, Hou, 2);
//     LCD_ShowString(2, 3, ":");
//     LCD_ShowString(2, 6, ":");
//   }
// }

// // // 中断函数  支线  但还是会改变全局变量   单位是1毫秒
// TF0/计数器计数溢出执行 void Timer0_Routine() __interrupt(1) {
//   static unsigned int time = 0;
//   // 重置
//   TH0 = 0XFC; // 高位
//   TL0 = 0X67; // 地位
//     time++;
//     if (time >= 1000) { // 按秒计数   链接main函数的变量  sec min
//     hou主函数和中断函数的关系
//       Sce++;
//       if (Sce >= 60) {
//         Sce = 0;
//         Min++;
//         if (Min >= 60) {
//           Min = 0;
//           Hou++;
//           if (Hou >= 24) {
//             Hou = 0;
//           }
//         }
//       }
//       time = 0;
//     }
// }