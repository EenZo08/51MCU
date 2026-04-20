#include <reg52.h>
#include "XPT2046.h"

__sbit __at(0xB0 + 4) XPT2046_DIN;  // 34
__sbit __at(0xB0 + 5) XPT2046_CS;   // 35
__sbit __at(0xB0 + 6) XPT2046_DCLK; // 36
__sbit __at(0xB0 + 7) XPT2046_DOUT; // 37




// 读取AD的值   模拟数字转换器
unsigned int XPT2046_ReadAD(unsigned char Commend) {
  unsigned char i;
  unsigned int ADValue=0;

  XPT2046_CS = 0;
  XPT2046_DCLK = 0;

  for (i = 0; i < 8; i++) {//先输命令  看手册
    XPT2046_DIN = Commend & (0x80 >> i);
    XPT2046_DCLK = 1;
    XPT2046_DCLK = 0;
  }


/*
输出的值有8位  12位    一共是16进制 16个数  不够后面全是0填充 要处理
*/
  for(i=0;i<16;i++){

    XPT2046_DCLK=1;
    XPT2046_DCLK=0;

    
    if(XPT2046_DOUT){
      ADValue|=(0x8000>>i);
    }
  }

    XPT2046_CS = 1;
    return ADValue>>8;

}
