#include "AT2402.h"
#include "Delay.h"
#include "Key4.h"
// #include "LCD1602.h"
#include "Timer0.h"
#include "delay.h"
#include "digital.h"
#include <reg52.h>


// 定时器扫描   4按键  数码管  +存储AT2402模块

unsigned char num;
unsigned char Min, Sec, MiniSec;

unsigned char Runflag;

void main() {
//   LCD_Init();
  Timer0_Init();
  unsigned char key = 0;
  while (1) {
    key = Key4();
    if (key == 1) { // 切换模式
      Runflag = !Runflag;
    //   if (Runflag) {
    //     LCD_ShowString(1, 1, "Run ");
    //   } else {
    //     LCD_ShowString(1, 1, "Stop");
    //   }
    }

    if (key == 2) { // 数码管清零
      Min = Sec = MiniSec = 0;
    }

    if (key == 3) { // 第三个按键存储 写入
      AT2402_WriteByte(0, Min);
      Delay1ms(5);
      AT2402_WriteByte(1, Sec);
      Delay1ms(5);
      AT2402_WriteByte(2, MiniSec);
      Delay1ms(5);
    }

    if (key == 4) { // 第四个按键num读取  从0地址开始读取2字节
      // 读取num的低位
      Min = AT2402_ReadByte(0);

      Sec = AT2402_ReadByte(1);

      MiniSec = AT2402_ReadByte(2);
    }

    // 把数码管的缓存数组更改
    Digital_Set(1, Min / 10);
    Digital_Set(2, Min % 10);
    Digital_Set(3, 11); // 显示 -
    Digital_Set(4, Sec / 10);
    Digital_Set(5, Sec % 10);
    Digital_Set(6, 11); // 显示 -
    Digital_Set(7, MiniSec / 10);
    Digital_Set(8, MiniSec % 10);
  }
}

void Sec_loop() { // 时间
  if (Runflag) {
    MiniSec++;
    if (MiniSec >= 100) {
      MiniSec = 0;
      Sec++;
      if (Sec >= 60) {
        Sec = 0;
        Min++;
        if (Min >= 60) {
          Min = 0;
        }
      }
    }
  }
}

// 中断函数  支线  但还是会改变全局变量   单位是1毫秒  TF0/计数器计数溢出执行
void Timer0_Routine() __interrupt(1) {
  // 静态变量
  static unsigned int time0, time1, time2;
  // 重置
  TH0 = 0XFC; // 高位
  TL0 = 0X67; // 地位
  time0++;
  if (time0 >= 20) { // 20ms扫描按键
    time0 = 0;
    Key4_Loop();
  }

  time1++;
  if (time1 >= 2) { // 扫描数码管
    time1 = 0;
    Digital_Loop();
  }

  time2++;
  if (time2 >= 10) {
    time2 = 0;
    Sec_loop();
  }
}