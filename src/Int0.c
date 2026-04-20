#include <reg52.h>
#include "Int0.h"
// 外部中断0
void Int0_Init(void) {

  IT0 = 1; // 下降沿时触发1                  按键松手才触发
           //   低电平触发0                  一直按就触发

  IE0 = 0;

  EX0 = 1; // 使能外部中断0
  EA = 1;  // 使能总中断

  PX0 = 1; // 优先级最高
}

/*


*/
