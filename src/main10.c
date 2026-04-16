#include "DS1302.h"
#include "LCD1602.h"
#include "delay.h"
#include <reg52.h>
#include <stdint.h>

// 定义状态   随按键进行改变
uint8_t MODE = 0;

/*
p31 按键1  选择&退出
p30  按键0  右移
p32  按键2  加时间
p33  按键3  减时间

*/

void ShowTime() {

  DS1302_ReadTime();

  // LCD_ShowNum(1,1,1,2);
  //   LCD_ShowNum(1, 1, DS1302_Time[0], 2); // 显示年
  LCD_ShowNum(1, 1, DS1302_Time[0], 2); // 显示年
  LCD_ShowNum(1, 4, DS1302_Time[1], 2); // 显示月
  LCD_ShowNum(1, 7, DS1302_Time[2], 2); // 显示日
  LCD_ShowNum(2, 1, DS1302_Time[3], 2); // 显示时
  LCD_ShowNum(2, 4, DS1302_Time[4], 2); // 显示分
  LCD_ShowNum(2, 7, DS1302_Time[5], 2); // 显示秒
}

uint8_t SELECT = 0; // 选择的时间位   下标位

void main() {

  LCD_Init();
  DS1302_Init();

  DS1302_SetTime(); // 初始化时间

  while (1) {

    if (P31 == 0) { // 进入选择/退出模式
      Delay1ms(20);
      while (P31 == 0)
        ;
      Delay1ms(20);
      MODE = !MODE;

      LCD_ShowNum(1, 14, MODE, 2);

      // switch (SELECT) {
      // case 0:
      //   LCD_ShowNum(1, 1, DS1302_Time[0], 2); // 显示年
      //   Delay1ms(1000);
      //   LCD_ShowString(1, 1, "  ", 2);        // 显示年
      //   LCD_ShowNum(1, 4, DS1302_Time[1], 2); // 显示月
      //   LCD_ShowNum(1, 7, DS1302_Time[2], 2); // 显示日
      //   LCD_ShowNum(2, 1, DS1302_Time[3], 2); // 显示时
      //   LCD_ShowNum(2, 4, DS1302_Time[4], 2); // 显示分
      //   LCD_ShowNum(2, 7, DS1302_Time[5], 2); // 显示秒
      //   break;
      // case 1:
      //   LCD_ShowNum(1, 1, DS1302_Time[0], 2); // 显示年
      //   LCD_ShowNum(1, 4, DS1302_Time[1], 2); // 显示月
      //     Delay1ms(1000);
      //     LCD_ShowString(1, 4, "  ", 2);        // 显示年
      //   LCD_ShowNum(1, 7, DS1302_Time[2], 2); // 显示日
      //   LCD_ShowNum(2, 1, DS1302_Time[3], 2); // 显示时
      //   LCD_ShowNum(2, 4, DS1302_Time[4], 2); // 显示分
      //   LCD_ShowNum(2, 7, DS1302_Time[5], 2); // 显示秒
      // }
    }

    if (MODE) { // 为1

      if (P30 == 0) { // 右移按下   选择右移
        Delay1ms(20);
        while (P30 == 0)
          ;
        Delay1ms(20);

        

        SELECT++; // 选择下标右移
        if (SELECT > 5) {
          SELECT = 0;
        }
      }

      if (P32 == 0) { // 加时间

        Delay1ms(20);
        while (P32 == 0)
          ;
        Delay1ms(20);
        // LCD_ShowNum(2, 10, 32, 2);
        switch (SELECT) {
        case 0:

          if (DS1302_Time[0] < 99) { // 年
            DS1302_Time[0]++;
          } else { // 大于就置0
            DS1302_Time[0] = 0;
          }
          break;
        case 1:
          if (DS1302_Time[1] < 12) { // 月
            DS1302_Time[1]++;
          } else {
            DS1302_Time[1] = 1;
          }
          break;
        case 2: // 日
          if (DS1302_Time[1] == 1 || DS1302_Time[1] == 3 ||
              DS1302_Time[1] == 5 || DS1302_Time[1] == 7 ||
              DS1302_Time[1] == 8 || DS1302_Time[1] == 10 ||
              DS1302_Time[1] == 12) {
            if (DS1302_Time[2] < 31) {
              DS1302_Time[2]++;
            } else {
              DS1302_Time[2] = 1;
            }
          } else if (DS1302_Time[1] == 4 || DS1302_Time[1] == 6 ||
                     DS1302_Time[1] == 9 || DS1302_Time[1] == 11) {
            if (DS1302_Time[2] < 30) {
              DS1302_Time[2]++;
            } else {
              DS1302_Time[2] = 1;
            }
          }
          if (DS1302_Time[1] == 2) {
            if (DS1302_Time[0] % 4 == 0) { // 闰年  有29天
              if (DS1302_Time[2] < 29) {
                DS1302_Time[2]++;
              } else {
                DS1302_Time[2] = 1;
              }
            } else { // 非闰年  有28天
              if (DS1302_Time[2] < 28) {
                DS1302_Time[2]++;
              } else {
                DS1302_Time[2] = 1;
              }
            }
            break;
          case 3: // 时
            if (DS1302_Time[3] < 24) {
              DS1302_Time[3]++;
            } else {
              DS1302_Time[3] = 0;
            }
            break;
          case 4: // 分
            if (DS1302_Time[4] < 59) {
              DS1302_Time[4]++;
            } else {
              DS1302_Time[4] = 0;
            }
            break;
          case 5: // 秒
            if (DS1302_Time[5] < 59) {
              DS1302_Time[5]++;
            } else {
              DS1302_Time[5] = 0;
            }
            break;
          default:
            break;
          }
        }

        DS1302_SetTime();
      }

      if (P33 == 0) { // 减时间
        Delay1ms(20);
        while (P33 == 0) {
          ;
        }
        Delay1ms(20);
        // LCD_ShowNum(2, 10, 33, 2);
        SELECT--;
        if (SELECT < 0) {
          SELECT = 5;
        }

        switch (SELECT) {
        case 0:
          if (DS1302_Time[0] > 0) { // 年
            DS1302_Time[0]--;
          } else if (DS1302_Time[0] > 100) { // 0-1=255 unsign 无符号
            DS1302_Time[0] = 99;
          }
          break;
        case 1:
          if (DS1302_Time[1] > 0) { // 月
            DS1302_Time[1]--;
          } else if (DS1302_Time[1] > 12) {
            DS1302_Time[1] = 12;
          }
          break;
        case 2:
          // 是否为闰年
          if (DS1302_Time[0] % 4 == 0) { // 闰年  有29天
            if (DS1302_Time[2] > 0) {    // 日
              DS1302_Time[2]--;
            } else if (DS1302_Time[2] > 29) {
              DS1302_Time[2] = 29;
            }
          } else {                    // 非闰年  有28天
            if (DS1302_Time[2] > 0) { // 日
              DS1302_Time[2]--;
            } else if (DS1302_Time[2] > 28) {
              DS1302_Time[2] = 28;
            }
          }

          break;
        case 3:
          if (DS1302_Time[3] > 0) { // 时
            DS1302_Time[3]--;
          } else if (DS1302_Time[3] > 23) {
            DS1302_Time[3] = 23;
          }
          break;
        case 4:
          if (DS1302_Time[4] > 0) { // 分
            DS1302_Time[4]--;
          } else if (DS1302_Time[4] > 59) {
            DS1302_Time[4] = 59;
          }
          break;
        case 5:
          if (DS1302_Time[5] > 0) { // 秒
            DS1302_Time[5]--;
          } else if (DS1302_Time[5] > 59) {
            DS1302_Time[5] = 59;
          }
          break;
        default:
          break;
        }

        DS1302_SetTime();
      }

      LCD_ShowNum(2, 10, SELECT, 2);

      LCD_ShowNum(1, 1, DS1302_Time[0], 2); // 显示年
      LCD_ShowNum(1, 4, DS1302_Time[1], 2); // 显示月
      LCD_ShowNum(1, 7, DS1302_Time[2], 2); // 显示日
      LCD_ShowNum(2, 1, DS1302_Time[3], 2); // 显示时
      LCD_ShowNum(2, 4, DS1302_Time[4], 2); // 显示分
      LCD_ShowNum(2, 7, DS1302_Time[5], 2); // 显示秒

      //重新选择
    //   SELECT = 0;
    }

    if (MODE == 0) {
      ShowTime();
    }
  }
}