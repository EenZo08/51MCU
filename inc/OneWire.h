#ifndef __ONEWIRE_H_
#define __ONEWIRE_H_

unsigned char OneWire_Init(void);
void OneWire_SendBit(unsigned char Bit);
unsigned char OneWire_ReceiveBit(void);
void OneWire_SendByte(unsigned char Byte);
unsigned char OneWire_ReceiveByte(void) ;

#endif