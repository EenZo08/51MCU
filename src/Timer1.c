#include <reg52.h>
#include "Timer1.h"
/**
  * @brief  定时器0初始化
  * @param  无
  * @retval 无
  */
void Timer1_Init(void)
{
	TMOD &= 0x0F;		//设置定时器模式
	TMOD |= 0x10;		//设置定时器模式
	TL1 = 0;		//设置定时初值
	TH1 = 0;		//设置定时初值
	TF1 = 0;		//清除TF0标志
	TR1 = 0;		//定时器0不计时
}

/**
  * @brief  定时器0设置计数器值
  * @param  Value，要设置的计数器值，范围：0~65535
  * @retval 无
  */
void Timer1_SetCounter(unsigned int Value)
{
	TH1=Value/256;
	TL1=Value%256;
}

/**
  * @brief  定时器0获取计数器值
  * @param  无
  * @retval 计数器值，范围：0~65535
  */
unsigned int Timer1_GetCounter(void)
{
	return (TH1<<8)|TL1;
}

/**
  * @brief  定时器0启动停止控制
  * @param  Flag 启动停止标志，1为启动，0为停止
  * @retval 无
  */
void Timer1_Run(unsigned char Flag)
{
	TR1=Flag;
}
