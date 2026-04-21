#include "LCD1602.h"
#include "delay.h"
#include <reg52.h>

//lcd 轮播


void main() {

  LCD_Init();
  LCD_ShowString(1, 1, "Hello World! Hello C!");
  while (1) {

    LCD_WriteCommand(0X18);//向左右动的指令
    Delay1ms(500);

  }
}