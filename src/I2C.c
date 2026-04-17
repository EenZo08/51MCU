#include <reg52.h>
#include "I2C.h"
/*
I2C 协议 的基础配置

*/

// 重声明双总线   EEPROM储存器要I2C 协议配置    它的引脚SCL和SDA对应p21和p20
// __sbit__ SCL = 0x90;
// __sbit__ SDA = 0x91;
__sbit __at(0xA0 + 0) I2C_SDA; // EEPROM_SDA P20
__sbit __at(0xA0 + 1) I2C_SCL; // EEPROM_SCL P21

// 起始信号   信号间隔小
void I2C_Start(void) {
  I2C_SDA = 1;
  I2C_SCL = 1;
  I2C_SDA = 0;
  I2C_SCL = 0;
}

// 终止信号
void I2C_Stop(void) {
  I2C_SDA = 0;
  I2C_SCL = 1;
  I2C_SDA = 1;
}
 
// 发送一个字节   先发地址 从机发应答 再发数据 从机发应答
void I2C_SendByte(unsigned char byte) {

  // 读  反方向
  // 第一个是地址   0000 0001  读R  0000 0001   &0x80   1000 0000   &0x40 0100
  // 0000
  unsigned char i;
  for (int i = 0; i < 8; i++) {
    I2C_SDA = byte & (0x80>>i);
    I2C_SCL = 1;
    I2C_SCL = 0;
  }
}


// 接收数据
unsigned char I2C_ReceiveByte(void) {

  unsigned char i, byte = 0;

// 拉高SDA  主机接受数据   时序图
  I2C_SDA = 1;


  // 数据在I2C_SDA    1/0   有数据就是1 没就是0
  for (i = 0; i < 8; i++) {

    I2C_SCL = 1;    //拉高SCL  读取
    if (I2C_SDA) {
      byte |= (0x80>>i);
    }
    I2C_SCL = 0;//读完拉低

  }
  return byte;
}

/*
@brief 发送应答位
@param ack 应答位 0 应答 1 非应答
*/

// 发送应答位  发送数据 从机发送     接收数据  主机发送         地址  从机发应答  从机发数据  主机发应答
void I2C_SendAck(unsigned char ack) {

  I2C_SDA = ack;
  I2C_SCL = 1;
  I2C_SCL = 0;
}



//接受应答
unsigned char I2C_ReceiveAck(void) {

    unsigned char ack;
    I2C_SDA = 1;
    I2C_SCL = 1;

    //获取引脚P20的状态
    ack = I2C_SDA;

    I2C_SCL = 0;
    return ack;

}