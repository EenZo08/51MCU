#include "LCD1602.h"
#include "XPT2046.h"
#include "delay.h"
#include <reg52.h>

//AD 模拟-数字转化 显示 可调  热敏 光敏

unsigned int ADValue;
void main() {
  LCD_Init();

  LCD_ShowString(1, 1, "ADJ  NTC  GR");
  while (1) {
    ADValue = XPT2046_ReadAD(XPT2046_XP);  //可调电阻
    LCD_ShowNum(2, 1, ADValue, 3);

    ADValue = XPT2046_ReadAD(XPT2046_YP); //热敏电阻
    LCD_ShowNum(2, 6, ADValue, 3);

    ADValue = XPT2046_ReadAD(XPT2046_VBAT);//光敏电阻
    LCD_ShowNum(2, 11, ADValue, 3);
    Delay1ms(100);
  }
}
