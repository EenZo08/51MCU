#include <reg52.h>
#include "OneWire.h"

// 单总线的配置    DS18B20 温度读取 通信 需要单总线通信
// 定义DS18B20通信引脚    主机总线
__sbit __at(0xB0 + 7) OneWire_DQ;

// 初始化  返回初始化状态信息
unsigned char OneWire_Init(void) {

  unsigned char i;
  unsigned char AckBit;

  EA=0;//和中断冲突了

  OneWire_DQ = 1;
  OneWire_DQ = 0;

  // 延时  考虑到代码也要执行时间 而单总线延时比较严格
  i = 227;
  while (--i)
    ; // 延时500us

  OneWire_DQ = 1; // 靠电阻拉高

  i = 32;
  while (--i)
    ; // Delay 70us

  AckBit = OneWire_DQ;

  i = 227;
  while (--i)
    ; // 延时500us
   EA=1;
  return AckBit;
}

// 发送一位Byte   0/1
void OneWire_SendBit(unsigned char Bit) {

  unsigned char i;

  EA=0;//和中断冲突了


  OneWire_DQ = 0;
  i = 4;
  while (--i)
    ; // Delay 10us
  OneWire_DQ = Bit;
  i = 24;
  while (--i)
    ; // Delay 50us
  OneWire_DQ = 1;
   EA=1;
}

// 接收一位字节
unsigned char OneWire_ReceiveBit(void) {
  unsigned char i;
  unsigned char Bit;
  EA=0;//和中断冲突了

  OneWire_DQ = 0;
  i = 2;
  while (--i)
    ; // Delay 5us
  OneWire_DQ = 1;
  i = 2;
  while (--i)
    ; // Delay 5us
  Bit = OneWire_DQ;
  i = 24;
  while (--i)
    ; // Delay 50us
   EA=1;
  return Bit;
}

// 返送一个字节  8位16进制
void OneWire_SendByte(unsigned char Byte) {
  unsigned char i;
  for (i = 0; i < 8; i++) {
    OneWire_SendBit(Byte & (0x01 << i));
  }
}

// 接收一位字节
unsigned char OneWire_ReceiveByte(void) { 
    unsigned char i; 
    unsigned char Byte;
    for(i=0;i<8;i++){
        if(OneWire_ReceiveBit()){
            Byte|=(0x01<<i);
        }
    }

    return Byte;


}