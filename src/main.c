#include<reg52.h>
#include"LCD1602.h"
#include"DS1302.h"



//设置时间 DS1302
unsigned char x;

void main(){

    LCD_Init();
    DS1302_Init();

    //解除写保护
    // DS1302_WriteByte(0X8E, 0x00);

    //设置时间
    // DS1302_WriteByte(0X80, 0x03);
    DS1302_SetTime();//设置时间


    while(1){

        DS1302_ReadTime();

        // LCD_ShowNum(1,1,1,2);
        LCD_ShowNum(1,1,DS1302_Time[0],2);//显示年
        LCD_ShowNum(1,1,DS1302_Time[0],2);//显示年
		LCD_ShowNum(1,4,DS1302_Time[1],2);//显示月
		LCD_ShowNum(1,7,DS1302_Time[2],2);//显示日
		LCD_ShowNum(2,1,DS1302_Time[3],2);//显示时
		LCD_ShowNum(2,4,DS1302_Time[4],2);//显示分
		LCD_ShowNum(2,7,DS1302_Time[5],2);//显示秒

    }


}