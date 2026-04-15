#include <reg52.h>
#include <stdint.h>
//按钮点灯  左右循环  呼吸灯  二进制 
#include "delay.h"

void main() {
  // 点灭点亮
  // while (1) {
  //     if(P31==0){//按键按下p31==0   松开p31==1
  //         P20=0;
  //     }
  //     else{
  //         P20=1;
  //     }
  // }

  // while(1){
  //     P20=0;
  //     Delay1ms(1000);
  //     P20=1;
  //     Delay1ms(1000);
  // }
  // P20=0;

  // 点灭点亮
  // while (1) {
  //   if (P31 == 0) {
  //     Delay1ms(20);
  //     while (P31 == 0);
  //     Delay1ms(20);
  //     P20 =~P20;
  //   }

  // }
  // 一次点亮一次点灭
  // uint8_t a = 0;
  // while (1) {
  //   if (P31 == 0) { // 按键按下
  //     Delay1ms(20); // 消抖
  //     while (P31 == 0);           // 等待释放
  //     Delay1ms(20); // 释放消抖
  //     a = ~a; // 第一次 ~a=255  P20=255 所以不会亮  没有消抖 会乱 时消时不消
  //     P20 = a; // LED状态翻转    按一下取反   P20类型  sdcc有关系
  //   }
  // }

  // 二进制表示
  //  uint8_t a = 0;
  //  while(1){
  //        if (P31 == 0) { // 按键按下
  //      Delay1ms(20); // 消抖
  //      while (P31 == 0);           // 等待释放
  //      Delay1ms(20); // 释放消抖
  //      a ++; // 第一次 ~a=255  P20=255 所以不会亮  没有消抖 会乱 时消时不消
  //      P2 = a; // LED状态翻转    按一下取反   P20类型  sdcc有关系
  //      if(a==8){
  //        a=0;
  //      }
  //  }
  //  }

  // 循环    向左向右移动
  unsigned char a = 0X01;
  P20 = 0;
  while (1) {
    if (P31 == 0) { // 按键按下  向右移动
      Delay1ms(20); // 消抖
      while (P31 == 0)
        ;           // 等待释放
      Delay1ms(20); // 释放消抖
      if (a == 0x80) {
        a = 0X01;
      } else {
        a <<= 1;
      }
      P2 = ~a;
    }

    if (P30 == 0) {
      Delay1ms(20); // 消抖
      while (P30 == 0);// 等待释放
      Delay1ms(20); // 释放消抖

      if (a == 0x01) {
        a = 0x80;
      } else {
        a >>= 1;
      }
      P2 = ~a;
    }
  }
}