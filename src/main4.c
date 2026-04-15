#include "LCD1602.h"
#include "MatrixKey.h"
#include "delay.h"
#include <reg52.h>
#include <stdint.h>

unsigned char count[4] = {0};
unsigned char a = 0;
unsigned char password[] = {2, 3, 4, 5};
unsigned char i = 0; // 数组下标
unsigned char key;

//密码验证


// 数组清零
void Back() {
  for (a = 0; a < 4; a++) {
    count[a] = 0;
  }
  i = 0;
}

// 验证密码
uint8_t Check() {
  for (a = 0; a < 4; a++) {
    if (count[a] != password[a]) {
      return 0;
    }
  }
  return 1;
}

void main() {
  LCD_Init();
  LCD_ShowString(1, 1, "PASSWORD");

  while (1) {
    key = MatrixKeyNum();
    // LCD_ShowString(1, 12, "   ");
    if (key) {//判断是否输入了
      if (i<4&&key<11) {
        LCD_ShowString(1, 12, "   ");
        count[i] = key%10;//余10 把10设置为0
        i++;
      }
        
      if (key == 11) {//确认键
        unsigned char y = Check();
        if (y) {
          LCD_ShowString(1, 12, "okk");
          Back();
        } else {
          LCD_ShowString(1, 12, "err");
          Back();
        }
      }
      if(key==12){//取消键
        Back();
      }
    }

    for(a=0;a<4;a++){//显示当前已输入的密码   再封装显示
        LCD_ShowNum(2, 1+a, count[a], 1);   
    }
    // LCD_ShowNum(2, 1, count[0], 1);
    // LCD_ShowNum(2, 2, count[1], 1);
    // LCD_ShowNum(2, 3, count[2], 1);
    // LCD_ShowNum(2, 4, count[3], 1);
    
}
}



// 再封装后的版本
// // 显示初始化界面    
// void Display_Init(void) {
//     LCD_ShowString(1, 1, "PASSWORD:");
//     LCD_ShowString(2, 1, "----");
// }

// // 显示当前已输入的密码（第二行）
// void Display_Input(void) {
//     unsigned char j;
//     for(j = 0; j < 4; j++) {
//         if(j < i) {
//             LCD_ShowNum(2, j+1, count[j], 1);  // 已输入的数字
//         } else {
//             LCD_ShowChar(2, j+1, '-');          // 未输入显示横线
//         }
//     }
// }

// // 显示单个数字（输入时实时更新）
// void Display_SingleNum(unsigned char pos, unsigned char num) {
//     LCD_ShowNum(2, pos+1, num, 1);
// }

// // 显示提示信息（第一行右侧）
// void Display_Message(char *msg) {
//     LCD_ShowString(1, 12, "   ");  // 先清除
//     LCD_ShowString(1, 12, msg);
// }

// // 显示验证成功
// void Display_OK(void) {
//     Display_Message("OK ");
// }

// // 显示验证失败
// void Display_ERR(void) {
//     Display_Message("ERR");
// }

// // 清除提示信息
// void Display_ClearMsg(void) {
//     LCD_ShowString(1, 12, "   ");
// }

// // 清空输入显示
// void Display_ClearInput(void) {
//     LCD_ShowString(2, 1, "----");
// }

// // ==================== 业务逻辑方法 ====================

// // 数组清零
// void Back(void) {
//     for(a = 0; a < 4; a++) {
//         count[a] = 0;
//     }
//     i = 0;
//     Display_ClearInput();  // 同时清屏
//     Display_ClearMsg();    // 清除提示
// }

// // 验证密码
// uint8_t Check(void) {
//     for(a = 0; a < 4; a++) {
//         if(count[a] != password[a]) {
//             return 0;
//         }
//     }
//     return 1;
// }

// // 处理数字键输入
// void Handle_NumberKey(unsigned char key_val) {
//     if(i < 4) {
//         count[i] = key_val % 10;      // key=10 变成 0
//         Display_SingleNum(i, count[i]); // 实时显示
//         i++;
//     }
// }

// // 处理确认键
// void Handle_ConfirmKey(void) {
//     if(i == 4) {  // 必须输入满 4 位
//         if(Check()) {
//             Display_OK();
//         } else {
//             Display_ERR();
//         }
//         Delay1ms(1500);  // 显示结果 1.5 秒
//         Back();
//     }
// }

// // 处理取消键
// void Handle_CancelKey(void) {
//     Back();
// }

// // ==================== 主函数 ====================

// void main() {
//     LCD_Init();
//     Display_Init();

//     while(1) {
//         key = MatrixKeyNum();
        
//         if(key) {  // 有按键按下
//             if(key < 11) {           // 数字键 1~10
//                 Handle_NumberKey(key);
//             }
//             else if(key == 11) {     // 确认键
//                 Handle_ConfirmKey();
//             }
//             else if(key == 12) {     // 取消键
//                 Handle_CancelKey();
//             }
//         }
        
//         Display_Input();  // 持续刷新显示
//     }
// }