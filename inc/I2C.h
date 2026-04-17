#ifndef __I2C_H__
#define __I2C_H__

#include <stdint.h>

void I2C_Start(void);
void I2C_Stop(void);
void I2C_SendByte(uint8_t byte);
uint8_t I2C_ReceiveByte(void);
void I2C_SendAck(uint8_t ack);
unsigned char I2C_ReceiveAck(void);

#endif