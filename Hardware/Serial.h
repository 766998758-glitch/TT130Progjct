#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>

void Serial_SendByte(uint8_t Byte);
void Serial_Init(void);
void Serial_SendArray(uint8_t *Array, uint16_t Length);  //�����Ǹ��ܴ��Ķ���̫�٣����������������������ݡ������Ǹ�ָ�룬ָ�������������׵�ַ
void Serial_SendString(char *String);   //����'\0'�����ԾͲ���Length��
void Serial_SendNum(uint32_t Num, uint8_t Length);  //��ʾ�ַ�����ʽ������
void Serial_Printf(char *format, ...);
uint8_t Serial_GetRxFlag(void);
extern char Serial_RxPacket[100];   //���ջ�����
extern uint8_t Serial_RxFlag;
void SendFloatRaw(float f);



#endif
