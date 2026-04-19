#include "DS18B20.h"
#include <reg52.h>
#include"LCD1602.h"
#include "delay.h"



//DS18B20 读取温度  显示在LCD1602上


float T;//-55 到 +125

void main(){
    DS180B20_ConvertT();
    Delay1ms(1000);   //延时等默认值刷新掉



    LCD_Init();
    LCD_ShowString(1, 1, "Temp:");
    
    while(1){

        //转化温度
        DS180B20_ConvertT();
        //读取温度
        T=DS18B20_ReadT(); 
        if(T<0){
            LCD_ShowChar(2,1,'-');
            T=-T;//转为正数  -正数
        }else{
            LCD_ShowChar(2,1,'+');
        }
        //显示温度
        LCD_ShowNum(2,2,T,3);
        //显示小数点
        LCD_ShowChar(2,5,'.');
        //显示小数部分
        LCD_ShowNum(2,6,
            (unsigned long)(T*10000)%10000,4);




    }

}