/**
 * CPU: 89C52
 * Freq: 12MHz
*/

#include <reg52.h>

void delay_100ms(void) //@12MHz
{
    unsigned char i, j;

    i = 195;
    j = 138;

    do
    {
        while (--j)
            ;
    } while (--i);
}

void main(void)
{
    // while (1)
    // {
    //     P20 = 1-P20;
    //     delay_100ms();
        
    // }

   // P2=0XFE;//11111110  二进制不支持  只亮p20
    // P2=0x55;//01010101 一亮一灭
    // P2=0X01;//00000001 只有p20不亮
    // P2=2;
    P2=1;
    delay_100ms();

}
