#include "DS18B20.h"
#include "OneWire.h"
#include <reg52.h>

// 定义DS18B20指令
#define DS18B20_SKIP_ROM 0xCC
#define DS18B20_CONVERT_T 0x44
#define DS18B20_READ_SCRATCHPAD 0xBE

// 开始温度变换
void DS18B20_ConvertT(void) {
  OneWire_Init();
  OneWire_SendByte(DS18B20_SKIP_ROM);
  OneWire_SendByte(DS18B20_CONVERT_T);
}

// 读取温度    转化看表
float DS18B20_ReadT(void) {
  unsigned char TLSB, TMSB;
  int Temp;
  float T;
  OneWire_Init();
  OneWire_SendByte(DS18B20_SKIP_ROM);
  OneWire_SendByte(DS18B20_READ_SCRATCHPAD);
  TLSB = OneWire_ReceiveByte();
  TMSB = OneWire_ReceiveByte();
  Temp = (TMSB << 8) | TLSB;
  T = Temp / 16.0;
  return T;
}