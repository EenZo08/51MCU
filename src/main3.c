#include "LCD1602.h"
#include "MatrixKey.h"
#include "delay.h"
#include <reg52.h>
#include <stdint.h>

unsigned char count = 0;

//封装MatrixKey函数 显示按键对应的数字
void main() {
  LCD_Init();
      LCD_ShowString(1, 1, "MatrixKeyNum");
      while (1) {
      count = MatrixKeyNum();
      if(count){
        LCD_ShowNum(2, 1, count, 2);
      }
    }




}
