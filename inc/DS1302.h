#ifndef __DS1302_H__
#define __DS1302_H__

// 外部声明时间数组，供其他文件使用
extern unsigned char DS1302_Time[];

// 函数原型
void DS1302_Init(void);
void DS1302_WriteByte(unsigned char byte, unsigned char Data);
unsigned char DS1302_ReadByte(unsigned char byte);
void DS1302_ReadTime(void);
void DS1302_SetTime(void);

#endif
