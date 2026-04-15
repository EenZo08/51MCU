#include "MatrixKey.h"
#include "LCD1602.h"
#include "delay.h"
#include <reg52.h>

// 矩阵键盘  s1-13    1-0的lcd显示
unsigned char MatrixKeyNum() {

  unsigned char num = 0;
  // 逐列扫描
  P1 = 0xFF;
  P13 = 0;
  if (P17 == 0) {
    Delay1ms(20);
    while (P17 == 0)
      ;
    Delay1ms(20);
    num = 1;
  }
  if (P16 == 0) {
    Delay1ms(20);
    while (P16 == 0)
      ;
    Delay1ms(20);
    num= 5;
  }
  if (P15 == 0) {
    Delay1ms(20);
    while (P15 == 0)
      ;
    Delay1ms(20);
    num= 9;
  }
  if (P14 == 0) {
    Delay1ms(20);
    while (P14 == 0)
      ;
    Delay1ms(20);
    num= 13;
  }

  P1 = 0xFF;
  P12 = 0;
  if (P17 == 0) {
    Delay1ms(20);
    while (P17 == 0)
      ;
    Delay1ms(20);
    num= 2;
  }
  if (P16 == 0) {
    Delay1ms(20);
    while (P16 == 0)
      ;
    Delay1ms(20);
    num= 6;
  }
  if (P15 == 0) {
    Delay1ms(20);
    while (P15 == 0)
      ;
    Delay1ms(20);
    num= 10;
  }
  if (P14 == 0) {
    Delay1ms(20);
    while (P14 == 0)
      ;
    Delay1ms(20);
    num= 14;
  }

  P1 = 0xFF;
  P11 = 0;
  if (P17 == 0) {
    Delay1ms(20);
    while (P17 == 0)
      ;
    Delay1ms(20);
    num= 3;
  }
  if (P16 == 0) {
    Delay1ms(20);
    while (P16 == 0)
      ;
    Delay1ms(20);
    num= 7;
  }
  if (P15 == 0) {
    Delay1ms(20);
    while (P15 == 0)
      ;
    Delay1ms(20);
    num= 11;
  }
  if (P14 == 0) {
    Delay1ms(20);
    while (P14 == 0)
      ;
    Delay1ms(20);
    num= 15;
  }

  P1 = 0xFF;
  P10 = 0;
  if (P17 == 0) {
    Delay1ms(20);
    while (P17 == 0)
      ;
    Delay1ms(20);
    num= 4;
  }
  if (P16 == 0) {
    Delay1ms(20);
    while (P16 == 0)
      ;
    Delay1ms(20);
    num= 8;
  }
  if (P15 == 0) {
    Delay1ms(20);
    while (P15 == 0)
      ;
    Delay1ms(20);
    num= 12;
  }
  if (P14 == 0) {
    Delay1ms(20);
    while (P14 == 0)
      ;
    Delay1ms(20);
    num= 16;
  }

  return num;
}
