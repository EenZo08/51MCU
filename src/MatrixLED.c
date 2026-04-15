#include "REG52.H"
#include <reg52.h>
#include "Delay.h"
#include "MatrixLED.h"



/**
矩阵LED显示模块   74HC595 驱动


相对原理图  按行来点亮LED矩阵   
**/


// SER=P34;
// SCK=P36;
// RCK=P35;
// 重新声明 P35,P36,P34 为位变量 SDCC配置
__sbit __at(0xB0 + 4) SER;
__sbit __at(0xB0 + 5) RCK;
__sbit __at(0xB0 + 6) SCK;

//重新定义P0   可位寻址
// __sfr __at(0x80)  xxxxxx;  //P0



// 74HC595转码函数
void _74HC595_WriteByte(unsigned char Byte) {

  unsigned char i;

  // 先处理8位的   但可能有8位  有或没有  010/0100 0001   要一位一位移动进队列

  /*
  假设0100 0001

  第一位
  SER=Byte&0x80;     0100 0001 & 1000 0000 = 0000 0000   第一位是左0  值是0 SER=0
  0

  第二位
  SER=Byte&0x40;     0100 0001 & 0100 0000 = 0100 0000   第二位0x40>0   值是1 SER=1
  
  01（进队列）

  */

  for (i = 0; i < 8; i++) {

    // if(Byte&(0x80>>i)){
    //     SER=1;  //非0即1
    // }
    // else{
    //     SER=0;
    // }

    SER = Byte & (0x80 >> i);
    // 给高电平1移位
    SCK = 1;
    // 移位后重置低电平
    SCK = 0;
  }
  //到8为给高电平使能让数据推向右边的输出引脚
  RCK = 1;

  RCK = 0;
}

/**
*@brief 矩阵LED显示字节函数
*@param Byte 要显示的字节   Column 在第几行显示
*@return 无
**/

void MatrixLED_ShowByte(unsigned char Byte,unsigned char Column){
    _74HC595_WriteByte(Byte);

    P0=~(0x80>>(Column));
    // P0=~(0x08>>(Column));

    Delay1ms(1);
    //1111 1111   
    P0=0xFF;

}

/*

@brief 主函数
@param 无
@return 无
*/
void MatrixLED_Init(){

    //初始化移位和锁存寄存器
    SCK = 0;
    RCK = 0;
}


// void main(){
  
//   MatrixLED_Init();
//     while(1){
//       MatrixLED_ShowByte(0x01,1);
//       MatrixLED_ShowByte(0x02,2);
//       MatrixLED_ShowByte(0x04,3);
//       MatrixLED_ShowByte(0x08,4);
//     }

// }