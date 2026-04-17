#include "delay.h"
#include <reg52.h>

#define _nop_() __asm nop __endasm

void Delay500us(void) //@11.0592MHz
{
  unsigned char data i;

  _nop_();
  i = 227;
  while (--i)
    ;
}

uint8_t state = 0;
void main() {

  while (1) {

    if (P31 == 0) {
      Delay1ms(20);
      while (P31 == 0)
        ;
      unsigned int i;
      for (i = 0; i < 200; i++) {
        P25 = !P25;
        Delay500us(); // 震动需要时间 频率
      }
    }
  }
}