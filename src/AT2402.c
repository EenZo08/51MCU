#include "I2C.h"
#include <reg52.h>
#include "AT2402.h"
// #include<stdint.h>

//配置 EEPROM  AT2402 的存储 读取配置
//AT2402 设备地址  0xA0
#define AT2402_ADDR 0xA0   


/*
@brief 写入字节                                            要延时5ms 
@param addr 数据地址
@param byte 具体数据
*/
//写入字节  要延时5ms  相对于主机是接受数据    主机要发送应答给从机        
void AT2402_WriteByte(unsigned char addr, unsigned char byte) {
  I2C_Start();
  I2C_SendByte(AT2402_ADDR);//设备地址
  I2C_ReceiveAck();//从机接受应答 
  I2C_SendByte(addr);//设备内的数据地址
  I2C_ReceiveAck();
  I2C_SendByte(byte);
  I2C_ReceiveAck();
  I2C_Stop();
}

/*
@brief 读取字节
@param addr 数据地址
@return byte 具体数据
*/


//读取字节   相对于主机是发送数据    主机要接收应答从机
unsigned char AT2402_ReadByte(uint8_t addr) {
  unsigned char byte;
  I2C_Start();
  I2C_SendByte(AT2402_ADDR);//设备地址
  I2C_ReceiveAck();//从机接受应答 
  I2C_SendByte(addr);//设备内的数据地址
  I2C_ReceiveAck();
  I2C_Start();
  I2C_SendByte(AT2402_ADDR | 0x01);//设备地址  读取模式
  I2C_ReceiveAck();//从机接受应答 
  byte = I2C_ReceiveByte();//接收数据
  I2C_SendAck(1);//发送应答
  I2C_Stop();
  return byte;
}
