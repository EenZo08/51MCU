#ifndef __MATRIXLED_H__
#define __MATRIXLED_H__

void _74HC595_WriteByte(unsigned char Byte);
void MatrixLED_ShowByte(unsigned char Byte,unsigned char Column);
void MatrixLED_Init(void);

#endif