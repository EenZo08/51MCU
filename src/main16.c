#include "delay.h"
#include <reg52.h>

// led呼吸灯

void Delay(unsigned int i) {
  while (i--)
    ;
}

void main() {

  unsigned char i, j;
  while (1) {
    // 周期100  先越来越亮
    for (i = 0; i < 100; i++) {
      for (j = 0; j < 30; j++) {
        P20 = 0;
        Delay(i);
        P20 = 1;
        Delay(100 - i);
      }
    }

    for (i = 100; i > 0; i--) {
      for (j = 0; j < 30; j++) {
        P20 = 0;
        Delay(i);
        P20 = 1;
        Delay(100 - i);
      }
    }

    // P20 = 1;
    // Delay(100);
  }
}