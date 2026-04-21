#include<reg52.h>
#include "Int0.h"



//外部中断0测试   中断引脚在p32 按键3和红外接收  p32电压有变化就触发外部中断
void main() {
    Int0_Init();
  while (1) {
  }
}


//外部中断0中断函数模板
void INT0_Routine(void) __interrupt(0)
{
	P2 = ~P2;
}