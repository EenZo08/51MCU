/**
 * CPU: 89C52
 * Freq: 12MHz
*/
#include <stdio.h>
#include <reg52.h>

void Delay1ms(unsigned int ms)	//@11.0592MHz
{
	unsigned char data i, j;

	// _nop_();
	// _nop_();
	// _nop_();

    while(ms--){
        i = 11;
        j = 190;
        do
        {
            while (--j);
        } while (--i);
    }

}


void main()
{
    //     unsigned int i;
    // uint8_t a=0x01;//0000 0001
    //     while(1){
        // P2=0XFE;//1111 1110
        // Delay1ms(100);
        // P2=0XFD;//1111 1101
        // Delay1ms(100);
        // P2=0XFB;//1111 1011
        // Delay1ms(100);
        // P2=0XF7;//1111 0111
        // Delay1ms(100);
        // P2=0XEF;//1110 1111
        // Delay1ms(100);
        // P2=0XDF;//1101 1111
        // Delay1ms(100);
        // P2=0XBF;//1011 1111
        // Delay1ms(100);
        // P2=0X7F;//0111 1111
        // Delay1ms(100);

        //取反方案  单向的
        // P2=~a;//1111 1110

        // a=a<<1;//0000 0010
        // Delay1ms(100);
        // if(a==0x00){//0000 0000 重置a的值为0000 0001
        //     a=0x01;
        // }

        //双向循环
        // a=0x01;
        // for(i=0;i<8;i++){
        //     P2=~a;//1111 1110

        //     a=a<<1;//0000 0010
        //         Delay1ms(100);
        // }

        // a=0x40;//0100 0000
        // for(i=0;i<7;i++){
        //     P2=~a;//1111 1110
        //     a=a>>1;//0000 0010
        //     Delay1ms(100);
        // }

    // }
    unsigned char led_pattern = 0x01; // 初始化LED点亮模式，从最右边的LED开始（位0）
    bit direction = 0; // 0表示向左移动，1表示向右移动

    while(1){
        // 根据当前方向更新LED模式
        if (direction == 0) { // 向左移动
            led_pattern = led_pattern << 1;
            // 如果LED模式达到最左边（位7），则改变方向
            if (led_pattern == 0x80) {
                direction = 1;
            }
            // 防止移位超出范围导致led_pattern变为0，如果发生则重置为起始位置
            // (通常在检测0x80后立即改变方向不会发生，但作为安全措施)
            if (led_pattern == 0x00) {
                 led_pattern = 0x01;
            }
        } else { // 向右移动
            led_pattern = led_pattern >> 1;
            // 如果LED模式达到最右边（位0），则改变方向
            if (led_pattern == 0x01) {
                direction = 0;
            }
            // 防止移位超出范围导致led_pattern变为0，如果发生则重置为起始位置
            // (通常在检测0x01后立即改变方向不会发生，但作为安全措施)
            if (led_pattern == 0x00) {
                 led_pattern = 0x80;
            }
        }

        // 将当前LED模式输出到P2口
        // 假设LED是共阳极连接，即P2端口输出0V时LED亮，输出5V时LED灭
        // 因此需要对led_pattern取反，使对应位为0的LED亮起
        P2 = ~led_pattern;

        // 延时一段时间，以便观察LED的变化
        Delay1ms(100);
    }
}