#include "Uart.h"
#include <reg52.h>
#include <stdint.h>

void Uart1_Init(void) // 4800bps@11.0592MHz   串口1模式1 可变波特率8位
{
  // 串口配置
  SCON = 0x50; // 8位数据,可变波特率   模式1

  // 定时器1的配置
  TMOD &= 0x0F; // 清零定时器1配置位（高4位）
  TMOD |= 0x20; // 定时器1工作于模式2（8位自动重装）
  TL1 = 0xFA;   // 设置定时初始值   模式确定的
  TH1 = 0xFA;   // 设置定时初始值
  TR1 = 1;      // 定时器1开始计时

  // 中断系统配置
  ET1 = 0; // 禁止定时器中断

  // 串口中断使能   中断的允许控制寄存器
  EA = 1;
  ES = 1;
}

// 发送数据
void Uart1_SendByte(unsigned char Byte) {
  SBUF = Byte; // SBUF 发送数据寄存器  写数据到SBUF 就会发送数据
               // 发送完 TI 会置1   需要重新设置为0
  while (TI == 0)
    ;
  TI = 0;
}


// 串口中断函数、接收数据模板
// void Uart1_Rountine(void) __interrupt(4) {

//   if (RI) { // RI是接收中断标志位寄存器  1  表示接受数据完成

//     // 接收到的数据在SBUF寄存器中
//     // LCD_ShowString(1, 1, SBUF);

//     P2 = ~SBUF;

//     // 发送回电脑
//     Uart1_SendByte(SBUF);

//     RI = 0; // 重新置零
//   }
// }