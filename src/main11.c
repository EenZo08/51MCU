#include "AT2402.h"
#include "Delay.h"
#include "Key4.h"
#include <LCD1602.h>
#include <reg52.h>


// EEPROM的 AT2402 模块存储测试
unsigned char num = 0;

void main() {

  LCD_Init();

  uint8_t key = 0;
  while (1) {
    key = KEY4();
    if (key == 1) { // 第一个按键num自增
      num++;
      LCD_ShowNum(1, 1, num, 4);
    }

    if (key == 2) { // 第二个按键num自减
      num--;
      LCD_ShowNum(1, 1, num, 4);
    }

    if (key == 3) { // 第三个按键num存储 写入
      // 存num的低位  存在0地址
      AT2402_WriteByte(0, num % 256);
      // 存num的高位   存在1地址
      Delay1ms(5);
      AT2402_WriteByte(1, num / 256);
      Delay1ms(5);

      LCD_ShowString(2, 1, "Num Written!");
      Delay1ms(1000);
      LCD_ShowString(2, 1, "             ");
    }

    if (key == 4) { // 第四个按键num读取  从0地址开始读取2字节
      // 读取num的低位
      num = AT2402_ReadByte(0);
      // 读取num的高位 |=有才读
      num |= AT2402_ReadByte(1) << 8;
      LCD_ShowNum(1, 1, num, 4);

      LCD_ShowString(2, 1, "Num Read!");
      Delay1ms(1000);
      LCD_ShowString(2, 1, "             ");
    }
  }
}
