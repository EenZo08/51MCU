#include "Key4.h"
#include "Delay.h"
#include <reg52.h>

unsigned char Key;
// 4按键模块   返回按键值  1~4

// unsigned char KEY4(void) {

//   unsigned char key = 0;

//   if (P31 == 0) {

//     Delay1ms(20);
//     while (P31 == 0)
//       ;
//     Delay1ms(20);
//     key = 1;
//   }
//   if (P30 == 0) {
//     Delay1ms(20);
//     while (P30 == 0)
//       ;
//     Delay1ms(20);
//     key = 2;
//   }
//   if (P32 == 0) {
//     Delay1ms(20);
//     while (P32 == 0)
//       ;
//     Delay1ms(20);
//     key = 3;
//   }
//   if (P33 == 0) {
//     Delay1ms(20);
//     while (P33 == 0)
//       ;
//     Delay1ms(20);
//     key = 4;
//   }

//   return key;
// }
// 返回出去
unsigned char Key4(void) {
  unsigned char temp = 0;
  temp = Key;
  Key = 0;
  return temp;
}
unsigned char Key4_GetState() {
  unsigned char KeyNum = 0;

  if (P31 == 0) {
    KeyNum = 1;
  }
  if (P30 == 0) {
    KeyNum = 2;
  }
  if (P32 == 0) {
    KeyNum = 3;
  }
  if (P33 == 0) {
    KeyNum = 4;
  }
  return KeyNum;
}

void Key4_Loop() {

  static unsigned char LastState;
  static unsigned char CurState;
  LastState = CurState;
  CurState = Key4_GetState();

  if (LastState == 1 &&
      CurState == 0) { // 检测的是松手前   上一个状态是1  当前状态是0
                       // 说明是1键松手        互换就是 按下就改变
    Key = 1;
  }
  if (LastState == 2 && CurState == 0) {
    Key = 2;
  }
  if (LastState == 3 && CurState == 0) {
    Key = 3;
  }
  if (LastState == 4 && CurState == 0) {
    Key = 4;
  }
}
