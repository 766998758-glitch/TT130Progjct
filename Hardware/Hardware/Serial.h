#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>

void Serial_SendByte(uint8_t Byte);
void Serial_Init(void);
void Serial_SendArray(uint8_t *Array, uint16_t Length);  //上面那个能传的东西太少，我们用数组来传更多数据。这里是个指针，指向待发送数组的首地址
void Serial_SendString(char *String);   //由于'\0'，所以就不用Length了
void Serial_SendNum(uint32_t Num, uint8_t Length);  //显示字符串形式的数字
void Serial_Printf(char *format, ...);
uint8_t Serial_GetRxFlag(void);
extern char Serial_RxPacket[100];   //接收缓存区
extern uint8_t Serial_RxFlag;
void SendFloatRaw(float f);



#endif
