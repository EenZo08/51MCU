#include "AT2402.h"
#include "DS18B20.h"
#include "Key4.h"
#include "LCD1602.h"
#include "Timer0.h"
#include "delay.h"
#include <reg52.h>

// DS18B20 读取温度  显示在LCD1602上并设置上下限温度最高低值   THigh    TLow

// 红外遥控切换   功能  切换要状态机

// 计时  闹钟功能    30           开始计时           计时到蜂鸣器提醒

void Timer0_Routine() __interrupt(1);

float T; //-55 到 +125
signed char THigh;
signed char TLow; //-128~127
unsigned char key;

unsigned char eeprom_dirty = 0; // 标记是否需要写入 EEPROM 判断什么时候才写入

void main() {
  DS180B20_ConvertT();
  Delay1ms(1000); // 延时等默认值刷新掉

  LCD_Init();
  Timer0_Init();
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

    // 转化温度
    DS180B20_ConvertT();
    // 读取温度
    T = DS18B20_ReadT();
    if (T < 0) {
      LCD_ShowChar(1, 6, '-');
      T = -T; // 转为正数  -正数
    } else {
      LCD_ShowChar(1, 6, '+');
    }
    // 显示温度
    LCD_ShowNum(1, 7, T, 3);
    // 显示小数点
    LCD_ShowChar(1, 10, '.');
    // 显示小数部分
    LCD_ShowNum(1, 11, (unsigned long)(T * 10000) % 10000, 4);

    if (key) {

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
      if (key == 3) {
        TLow++;
        if (TLow >= THigh) {
          TLow--;
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
  }
}

// 中断函数  支线  但还是会改变全局变量   单位是1毫秒  TF0/计数器计数溢出执行
void Timer0_Routine() __interrupt(1) {
  // 静态变量
  static unsigned int time = 0;
  // 重置
  TH0 = 0XFC; // 高位
  TL0 = 0X67; // 地位
  time++;
  if (time >= 20) {
    Key4_Loop();
    time = 0;
  }
}