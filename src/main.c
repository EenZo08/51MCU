#include "AT2402.h"
#include "DS18B20.h"
#include "Key4.h"
#include "LCD1602.h"
#include "Timer0.h"
#include "delay.h"
#include <limits.h>
#include <reg52.h>

#include "IR.h"

// DS18B20 读取温度  显示在LCD1602上并设置上下限温度最高低值   THigh    TLow

// 红外遥控切换   功能  切换要状态机

// 计时  闹钟功能    30           开始计时

// 计时到蜂鸣器提醒
__sbit __at(0xA0 + 5) Buzzer; // P2.5
// unsigned char beep_timer = 0; // 蜂鸣器响铃剩余秒数

void Timer0_Routine() __interrupt(1);
void Int0_Routine(void) __interrupt(0);

float T; //-55 到 +125
signed char THigh;
signed char TLow; //-128~127
unsigned char key;

unsigned char eeprom_dirty = 0; // 标记是否需要写入 EEPROM 判断什么时候才写入

unsigned char Command;
// unsigned char Address;

unsigned char MCU_State = 0; //  标记当前显示状态    0:显示温度    1红外  解决p32引脚冲突


unsigned char MODE = 0; // 状态  0显示温度  1是显示闹钟
unsigned char Action = 0;

unsigned char HOU = 0;
unsigned char MIN = 0;
unsigned char SEC = 30;

unsigned int i;

unsigned int beep_timer = 0; // 剩余响铃时间（毫秒）
// unsigned int last_beep_tick = 0;      // 上一次翻转时的系统毫秒计数
// volatile unsigned int g_sys_tick = 0; // 系统毫秒计数

#define _nop_() __asm nop __endasm

void Delay500us(void) //@11.0592MHz
{
  unsigned char data i;

  _nop_();
  i = 227;
  while (--i)
    ;
}

void main() {
  DS18B20_ConvertT();
  Delay1ms(1000); // 延时等默认值刷新掉

  LCD_Init();
  Timer0_Init();

  IR_Init(); // 定时器1计数

  LCD_ShowString(1, 1, "Temp:");

  // 从E2PROM中读取配置
  THigh = AT2402_ReadByte(0);
  TLow = AT2402_ReadByte(1);

  //   // 判断存储的值是否合法
  //   if (THigh > 125 || TLow < -55 || THigh <= TLow) {
  //     THigh = 20;
  //     TLow = 15;
  //   }

  // ✅ 修正：校验 EEPROM 数据合法性
  if (THigh > 125 || THigh < -55 || TLow > 125 || TLow < -55 || THigh <= TLow) {
    THigh = 20;
    TLow = 15;
    eeprom_dirty = 1; // 标记需要写入
  }

  // 显示
  LCD_ShowString(2, 1, "TH:");
  LCD_ShowSignedNum(2, 4, THigh, 3);
  LCD_ShowString(2, 10, "TL:");
  LCD_ShowSignedNum(2, 13, TLow, 3);
  while (1) {

    key = Key4();
    // 红外遥控
    if (IR_GetDataFlag() || IR_GetRepeatFlag()) // 如果收到数据帧或者收到连发帧
    {
      // Address = IR_GetAddress(); // 获取遥控器地址码
      Command = IR_GetCommand(); // 获取遥控器命令码

      // LCD_ShowHexNum(2, 1, Address, 2); // 显示遥控器地址码
      // LCD_ShowHexNum(2, 7, Command, 2); // 显示遥控器命令码

      if (Command == IR_EQ) // 如果遥控器VOL-按键按下
      {
        MODE = !MODE;
        MCU_State = 1;
        Action = 0;
      }
      if (Command == IR_POWER) { // 确认开始倒计时 0设置  1是计时
        Action = !Action;
      }

      if (MODE) {
        if (Command == IR_VOL_ADD) {
          SEC++;
          if (SEC >= 60) {
            SEC = 0;
            MIN++;
            if (MIN >= 60) {
              MIN = 0;
              HOU++;
              if (HOU >= 24)
                HOU = 0;
            }
          }
        }

        if (Command == IR_VOL_MINUS) {
          // 减少秒（连带借位）
          if (SEC > 0) {
            SEC--;
          } else {
            if (MIN > 0) {
              MIN--;
              SEC = 59;
            } else if (HOU > 0) {
              HOU--;
              MIN = 59;
              SEC = 59;
            }
          }
        }
      }
    }

    // lcd显示温度
    if (MODE == 0) {
      // 转化温度
      DS18B20_ConvertT();
      // 读取温度
      T = DS18B20_ReadT();
      if (T < 0) {
        LCD_ShowChar(1, 6, '-');
        T = -T; // 转为正数  -正数
      } else {
        LCD_ShowChar(1, 6, '+');
      }

      LCD_ShowString(1, 1, "Temp:");
      // 显示温度
      LCD_ShowNum(1, 7, T, 3);
      // 显示小数点
      LCD_ShowChar(1, 10, '.');
      // 显示小数部分
      LCD_ShowNum(1, 11, (unsigned long)(T * 10000) % 10000, 4);

      if (key) {
        MCU_State = 0;
        eeprom_dirty = 1; // 有按键操作，标记需要保存

        if (key == 1) { // T -55 +125  DS18B20 可识别的温度范围
          THigh++;
          if (THigh > 125) {
            THigh = 125;
          }
        }
        if (key == 2) {
          if (THigh > TLow + 1)
            THigh--;
          // THigh--;
          // if (THigh <= TLow) { // 或者 THigh=TLow+1；   做最低值的判断
          //   THigh++;
          // }
        }

        if (key == 3) { // 外部中断和p32硬件是一起的  每次接收红外

          if (MCU_State == 0) {
            TLow++;
            if (TLow >= THigh) {
              TLow--;
            }
          }
        }

        if (key == 4) {

          if (TLow > -55) {
            TLow--;
          }
        }

        // 显示

        LCD_ShowString(2, 1, "TH:");
        LCD_ShowSignedNum(2, 4, THigh, 3);
        LCD_ShowString(2, 8, "  ");
        LCD_ShowString(2, 10, "TL:");
        LCD_ShowSignedNum(2, 13, TLow, 3);

        // // 存储到E2PROM中
        // AT2402_WriteByte(0, THigh);
        // Delay1ms(5);
        // AT2402_WriteByte(1, TLow);
        // Delay1ms(5);

        // ✅ 只在值变化时写入 EEPROM
        if (eeprom_dirty) {
          AT2402_WriteByte(0, THigh);
          Delay1ms(5);
          AT2402_WriteByte(1, TLow);
          Delay1ms(5);
          eeprom_dirty = 0;
        }

        // 做判断提示         1是蜂鸣器  2lcd显示提示
        if (T >= THigh) {
          LCD_ShowString(1, 16, "H");
        } else if (T <= TLow) {
          LCD_ShowString(1, 16, "L");
        } else {
          LCD_ShowString(1, 16, " ");
        }
      }

      LCD_ShowString(2, 1, "TH:");
      LCD_ShowSignedNum(2, 4, THigh, 3);
      LCD_ShowString(2, 8, "  ");
      LCD_ShowString(2, 10, "TL:");
      LCD_ShowSignedNum(2, 13, TLow, 3);
    }

    if (MODE == 1) { // 显示闹钟    0设置时间  1开始计时

      LCD_ShowString(1, 1, "CLOCK:          ");
      LCD_ShowNum(2, 1, HOU, 2);
      LCD_ShowString(2, 3, ":");
      LCD_ShowNum(2, 4, MIN, 2);
      LCD_ShowString(2, 6, ":");
      LCD_ShowNum(2, 7, SEC, 2);
      LCD_ShowString(2, 9, "        ");

      // 蜂鸣器 for循环会阻塞主线程2000x500us=1s 阻塞就会影响lcd的显示、按键键盘
      //  在定时器0的中断函数     不能用for循环的   所有其他中断都被阻塞
      if (beep_timer == 1) {
        beep_timer = 0;
        for (i = 0; i < 2000; i++) {
          Buzzer = !Buzzer;
          Delay500us(); // 震动需要时间 频率
        }
      } else {
        Buzzer = 0;
      }

      // if (beep_timer > 0) {
      //   // 检查是否到了翻转时刻（每 500μs 翻转一次）
      //   if (g_sys_tick - last_beep_tick >= 1) { // 1ms 翻转 → 500Hz
      //     // 若要 1kHz，需要每 500μs 翻转，但 g_sys_tick 精度只有 1ms
      //     // 所以这里用 1ms 翻转，产生 500Hz 方波，也能响
      //     last_beep_tick = g_sys_tick;
      //     Buzzer = !Buzzer;
      //     beep_timer--; // 每翻转一次减 1ms，5000 次就是 5 秒
      //   }
      // } else {
      //   Buzzer = 0;
      // }
    }
  }
}

// 中断函数  支线  但还是会改变全局变量   单位是1毫秒  TF0/计数器计数溢出执行
void Timer0_Routine() __interrupt(1) {
  // 静态变量
  static unsigned int time = 0;
  static unsigned int time1 = 0;
  // 重置
  TH0 = 0XFC; // 高位
  TL0 = 0X67; // 地位

  time++;
  time1++;
  // g_sys_tick++; // 每 1ms 加 1

  if (time >= 20) {
    Key4_Loop();
    time = 0;
  }

  if (time1 >= 1000) {
    time1 = 0;
    if (MODE == 1 && Action == 1) {
      if (SEC > 0) {
        SEC--;
      } else {
        if (MIN > 0) {
          MIN--;
          SEC = 59;
        } else if (HOU > 0) {
          HOU--;
          MIN = 59;
          SEC = 59;
        } else {
          // 倒计时结束
          Action = 0;
          MODE = 0; // 返回温度模式
          MCU_State = 0;
          // 启动蜂鸣器（你需要在主循环或中断中处理蜂鸣器响铃）
          beep_timer = 1;
        }
      }
    }
  }

  // 倒计时递减逻辑（每秒触发）
  // static unsigned int last_sec_tick = 0;
  // if (g_sys_tick - last_sec_tick >= 1000) {
  //   last_sec_tick = g_sys_tick;

  //   if (MODE == 1 && Action == 1) {
  //     if (SEC > 0)
  //       SEC--;
  //     else if (MIN > 0) {
  //       MIN--;
  //       SEC = 59;
  //     } else if (HOU > 0) {
  //       HOU--;
  //       MIN = 59;
  //       SEC = 59;
  //     } else {
  //       Action = 0;
  //       MODE = 0;
  //       beep_timer = 5000; // 响铃 5000ms = 5 秒
  //     }
  //   }
  // }
}