#include "DS18B20.h"
#include "Delay.h"
#include "Digital.h"
#include "IR.h"
#include "Timer0.h"
#include <reg52.h>

//数码管显示温度   倒计时

/*遥控按键
0	IR_0	切换到温度模式
1	IR_1	切换到闹钟设置模式
EQ	IR_EQ	确认开始倒计时
VOL+	IR_VOL_ADD	增加秒数（设置模式）
VOL-	IR_VOL_MINUS	减少秒数（设置模式）
*/


__sbit __at(0xA0 + 5) Buzzer; // P2.5，普中A2蜂鸣器

// SDCC 中断声明
void Timer0_Routine(void) __interrupt(1);
void Int0_Routine(void) __interrupt(0);

// 系统模式定义
#define MODE_TEMP 0      // 温度显示模式
#define MODE_ALARM_SET 1 // 闹钟设置模式
#define MODE_ALARM_RUN 2 // 倒计时运行模式

unsigned char sys_mode = MODE_TEMP;
unsigned char alarm_seconds = 30; // 默认30秒
unsigned int countdown_tick = 0;  // 倒计时剩余秒数
unsigned char beep_timer = 0;     // 蜂鸣器响铃计时

// PWM 电机相关（保留，不影响新功能）
unsigned char Counter = 0, Compare = 0, Speed = 0;

// 温度变量
float temperature;
unsigned int temp_display; // 温度×10，用于数码管显示

// 闪烁控制
unsigned char blink_flag = 0;
unsigned int ms_counter = 0;

void main() {
  P10 = 0; // 电机初始停转
  Timer0_Init();
  IR_Init();
//   Digital_Init(); // 数码管初始化
//   DS18B20_Init();

  while (1) {
    // ----- 红外遥控处理（全局）-----
    if (IR_GetDataFlag() || IR_GetRepeatFlag()) {
      unsigned char cmd = IR_GetCommand();

      // 模式切换
      if (cmd == IR_0) {
        sys_mode = MODE_TEMP;
      } else if (cmd == IR_1) {
        sys_mode = MODE_ALARM_SET;
        countdown_tick = alarm_seconds; // 预设值
      } else if (cmd == IR_EQ && sys_mode == MODE_ALARM_SET) {
        sys_mode = MODE_ALARM_RUN; // 确认开始倒计时
        countdown_tick = alarm_seconds;
      }

      // 在设置模式下调整秒数
      if (sys_mode == MODE_ALARM_SET) {
        if (cmd == IR_VOL_ADD && alarm_seconds < 99) {
          alarm_seconds++;
          countdown_tick = alarm_seconds;
        }
        if (cmd == IR_VOL_MINUS && alarm_seconds > 1) {
          alarm_seconds--;
          countdown_tick = alarm_seconds;
        }
      }
    }

    // ----- 温度采集（每500ms更新一次）-----
    static unsigned int temp_timer = 0;
    if (ms_counter - temp_timer >= 500) {
      temp_timer = ms_counter;
      DS18B20_ConvertT();
      temperature = DS18B20_ReadT();
      temp_display = (unsigned int)(temperature * 10); // 保留一位小数
    }

    // ----- 显示刷新（由定时中断置位标志，这里只更新缓冲区）-----
    switch (sys_mode) {
    case MODE_TEMP:
      Digital_SetTemp(temp_display); // 显示格式：XX.X
      break;

    case MODE_ALARM_SET:
      // 闪烁显示：秒闪烁时消隐数字
      if (blink_flag || (ms_counter % 500 < 250)) {
        Digital_SetNum(countdown_tick);
      } else {
        Digital_SetBlank(); // 短暂消隐实现闪烁
      }
      break;

    case MODE_ALARM_RUN:
      Digital_SetNum(countdown_tick);
      if (countdown_tick == 0) {
        sys_mode = MODE_TEMP; // 倒计时结束返回温度模式
        beep_timer = 5;       // 蜂鸣器响5秒
      }
      break;
    }

    // 蜂鸣器控制
    if (beep_timer > 0) {
      Buzzer = (ms_counter % 500 < 250) ? 1 : 0; // 间歇响
    } else {
      Buzzer = 0;
    }

    // 独立按键备用（P31 切换模式）
    if (P31 == 0) {
      Delay1ms(20);
      while (P31 == 0)
        ;
      Delay1ms(20);
      sys_mode = (sys_mode + 1) % 3;
      if (sys_mode == MODE_ALARM_SET)
        countdown_tick = alarm_seconds;
    }

    // 电机控制（保留）
    if (P30 == 0) {
      Delay1ms(20);
      while (P30 == 0)
        ;
      Delay1ms(20);
      Speed = (Speed + 1) % 4;
      Compare = Speed * 33; // 0,33,66,99
    }
  }
}

// 1ms 定时中断（系统时基 + PWM）
void Timer0_Routine() __interrupt(1) {
  TH0 = 0xFC;
  TL0 = 0x67;

  // 系统毫秒计数
  ms_counter++;
  if (ms_counter >= 1000) {
    ms_counter = 0;
    blink_flag = !blink_flag;

    // 倒计时每秒递减
    if (sys_mode == MODE_ALARM_RUN && countdown_tick > 0) {
      countdown_tick--;
    }

    // 蜂鸣器计时
    if (beep_timer > 0) {
      beep_timer--;
    }
  }

  // PWM 电机控制
  Counter++;
  if (Counter >= 100)
    Counter = 0;
  P10 = (Counter < Compare) ? 1 : 0;

  // 数码管动态扫描（假设 Digital_Loop 在中断中调用）
  Digital_Loop();
}