#include "DS1302.h"
#include "LCD1602.h"
#include <reg52.h>

/*
DS1302时钟模块

*/

// 定义芯片的引脚
__sbit __at(0xB0 + 5) DS1302_CE;   // 信号  1为连通 0断开
__sbit __at(0xB0 + 6) DS1302_SCLK; // 时钟信号  1是写入
__sbit __at(0xB0 + 4) DS1302_IO;   // 前八个是配置项  后8个是日期数据

// 重声明 时分秒的寄存器 位数
#define DS1302_SECOND 0x80
#define DS1302_MINUTE 0x82
#define DS1302_HOUR 0x84
#define DS1302_DATE 0x86
#define DS1302_MONTH 0x88
#define DS1302_DAY 0x8A
#define DS1302_YEAR 0x8C
#define DS1302_WP 0x8E // 读写

// 存储时间的数组                  年、月、日、时、分、秒、星期
// 数组main里也可使用
unsigned char DS1302_Time[] = {26, 04, 16, 12, 59, 55, 4};

// 初始化时钟的参数
void DS1302_Init(void) {
  DS1302_CE = 0;
  DS1302_SCLK = 0;
}

// 写入数据
/*
byte是配置数据
Data是日期数据
*/

void DS1302_WriteByte(unsigned char byte, unsigned char Data) {
  unsigned char i;
  // 默认是写入模式
  DS1302_CE = 1; // 先通

  // 0100 0001    1000 0010   和592芯片类似  一个个输 给高压推入
  //   DS1302_IO = byte & 0x01;//第一位
  //   DS1302_SCLK = 1;
  //   DS1302_SCLK = 0;

  // 输入配置 byte
  for (i = 0; i < 8; i++) {
    DS1302_IO = byte & (0x01 << i); // 第一位
    DS1302_SCLK = 1;
    DS1302_SCLK = 0;
    // byte >>= 1;
  }

  // 输入日期数据
  for (i = 0; i < 8; i++) {
    DS1302_IO = Data & (0x01 << i); // 左移i位
    DS1302_SCLK = 1;
    DS1302_SCLK = 0;
  }

  // 将信号CE拉低
  DS1302_CE = 0;
}

// 读取日期信息  只要配置字节  返回日期的数据
unsigned char DS1302_ReadByte(unsigned char byte) {
  unsigned char i;
  unsigned char date = 0x00;
  // 默认是读取模式
  DS1302_CE = 1; // 先通
  // 输入配置 byte为读取模式  最后一位为1 0000 0001    读就是 1000 0000
  byte |= 1;
  // 读第一位

  
  

  for (i = 0; i < 8; i++) {
    DS1302_IO = byte & (0x01 << i); // 第一位

    DS1302_SCLK = 0;
    DS1302_SCLK = 1;

    // byte >>= 1;
  }

  // 赋值给date
  for (i = 0; i < 8; i++) {
    // 先给电   具体分析时序图  SCLK15个波峰
    DS1302_SCLK = 1;
    DS1302_SCLK = 0;
    if (DS1302_IO) {
      date |= (0x01 << i);
    }
  }

  DS1302_CE = 0;
  DS1302_IO = 0; // 配置为输入模式 0000 0000    1读 0写
  return date;
}

// 读取时间方法  将数据设置给时间数组  DS1302_Time
void DS1302_ReadTime(void) {

  unsigned char Temp;
  Temp = DS1302_ReadByte(DS1302_YEAR);
  DS1302_Time[0] = Temp / 16 * 10 + Temp % 16; // BCD码转十进制后读取
  // DS1302_Time[0]=Temp;
  Temp = DS1302_ReadByte(DS1302_MONTH);
  DS1302_Time[1] = Temp / 16 * 10 + Temp % 16;
  Temp = DS1302_ReadByte(DS1302_DATE);
  DS1302_Time[2] = Temp / 16 * 10 + Temp % 16;
  Temp = DS1302_ReadByte(DS1302_HOUR);
  DS1302_Time[3] = Temp / 16 * 10 + Temp % 16;
  Temp = DS1302_ReadByte(DS1302_MINUTE);
  DS1302_Time[4] = Temp / 16 * 10 + Temp % 16;
  Temp = DS1302_ReadByte(DS1302_SECOND);
  DS1302_Time[5] = Temp / 16 * 10 + Temp % 16;
  Temp = DS1302_ReadByte(DS1302_DAY);
  DS1302_Time[6] = Temp / 16 * 10 + Temp % 16;
}

// 设置时间的方法
void DS1302_SetTime(void) {
  // 设置可写
  DS1302_WriteByte(DS1302_WP, 0x00);

  // 操作数组改变日期   再读取
  DS1302_WriteByte(DS1302_YEAR, DS1302_Time[0] / 10 * 16 + DS1302_Time[0] % 10);
  DS1302_WriteByte(DS1302_MONTH,
                   DS1302_Time[1] / 10 * 16 + DS1302_Time[1] % 10);
  DS1302_WriteByte(DS1302_DATE, DS1302_Time[2] / 10 * 16 + DS1302_Time[2] % 10);
  DS1302_WriteByte(DS1302_HOUR, DS1302_Time[3] / 10 * 16 + DS1302_Time[3] % 10);
  DS1302_WriteByte(DS1302_MINUTE,
                   DS1302_Time[4] / 10 * 16 + DS1302_Time[4] % 10);
  DS1302_WriteByte(DS1302_SECOND,
                   DS1302_Time[5] / 10 * 16 + DS1302_Time[5] % 10);
  DS1302_WriteByte(DS1302_DAY, DS1302_Time[6] / 10 * 16 + DS1302_Time[6] % 10);

  DS1302_WriteByte(DS1302_WP, 0x80);
}

// unsigned char x;

// void main(){

//     LCD_Init();
//     DS1302_Init();

//     //解除写保护
//     // DS1302_WriteByte(0X8E, 0x00);

//     //设置时间
//     // DS1302_WriteByte(0X80, 0x03);

//     while(1){

//         // x=DS1302_ReadByte(0x81);//1000  0001
//         //显示秒 并且对返回的BCD 数据做处理
//         // LCD_ShowNum(2,1, x/16*10+x%16, 4);

//     }

// }