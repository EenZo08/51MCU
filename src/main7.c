#include "Delay.h"
#include "LCD1602.h"
#include "Uart.h"
#include <reg52.h>
#include <stdint.h>


unsigned char x = 0;

void main() {
  Uart1_Init();
  LCD_Init();
  while (1) {
    // 电脑发数据
    //  Uart1_SendByte(x);
    //  x++;
    //  Delay1ms(1000);
  }
}

// 串口中断函数、接收数据
void Uart1_Rountine(void) __interrupt(4) {

  if (RI) { // RI是接收中断标志位寄存器  1  表示接受数据完成

    // 接收到的数据在SBUF寄存器中
    // LCD_ShowString(1, 1, SBUF);
    P2 = ~SBUF;
    // 发送回电脑
    Uart1_SendByte(SBUF);

    RI = 0; // 重新置零
  }
}