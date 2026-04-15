#include "Delay.h"
#include "MatrixLED.h"
#include "REG52.H"
#include "Timer0.h"
#include <reg52.h>


// LED取模数组   动态显示
unsigned char code Animation[] = {
    0x3C, 0x42, 0xA9, 0x85, 0x85, 0xA9, 0x42, 0x3C, 
    0x3C, 0x42, 0xA1, 0x85,0x85, 0xA1, 0x42, 0x3C,
    0x3C, 0x42, 0xA5, 0x89,0x89, 0xA5, 0x42, 0x3C,
};

// 假设取模后共4帧，每帧8字节
// unsigned char code anim[4][8] = {
//     {0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00}, // 第1帧：立正
//     {0x00,0x00,0x00,0x18,0x24,0x00,0x00,0x00}, // 第2帧：提肩/蓄力
//     {0x00,0x00,0x00,0x24,0x18,0x00,0x00,0x00}, // 第3帧：靠肩/发力
//     {0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00}  // 第4帧：回正
// };

unsigned char i, Offset = 0, Count = 0;

void main() {

  void Timer0_Init();

  MatrixLED_Init();
  while (1) {
        for (i = 0; i < 8; i++) {
        MatrixLED_ShowByte(Animation[i + Offset], i);
    }

    Count++;
    if (Count > 15) {
      Count = 0;
      Offset += 8; // 偏移+8，切换下一帧画面
      if (Offset > 16) {
        Offset = 0;
      }
    }

  }
}

// 中断函数  支线  但还是会改变全局变量   单位是1毫秒  TF0/计数器计数溢出执行
// void Timer0_Routine() __interrupt(1) {
//   // 静态变量
//   static unsigned int time = 0;
//   // 重置
//   TH0 = 0XFC; // 高位
//   TL0 = 0X67; // 地位
//   time++;
//   if (time >= 1000) {

//     time = 0;
//   }
// }