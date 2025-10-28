#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>

char Serial_RxPacket[100];   //���ջ�����
uint8_t Serial_RxFlag;  //�յ����ݰ�����Flag


void Serial_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;     //Tx�����룬����������Rx���������Rx��֮
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF_PP; 
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPU; 
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_10;      //TX������Pin9��RX������Pin10
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);	
	USART_InitTypeDef USART_InitStructure;          //�����Init�����ڲ�������Զ����9600��Ӧ�ķ�Ƶϵ����Ȼ�����BRR�Ĵ�����
	
	USART_InitStructure.USART_BaudRate = 9600;      //�����ʣ�����ͨ״̬�£����Ǳ����ʣ���λʱ���ڴ���ı�������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;           //Ӳ��������
	USART_InitStructure.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;        //����ģʽ�������Ƿ���ģʽ��Rx�ǽ���ģʽ
	USART_InitStructure.USART_Parity = USART_Parity_No;         //У��λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;      //ֹͣλ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;        //�ֳ�
	USART_Init(USART1, &USART_InitStructure);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART1, ENABLE); 
}


void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART1, Byte); //�Ӷ������������Byte������Data��֮��Data&1FF�����޹ظ�λ���㣬Ȼ���ٸ�ֵ��DR�Ĵ���
	//������д��DR���������ͨ��TDR���������ݼĴ�������TDR�ٴ���������λ�Ĵ�����Ȼ��һλһλ�İ������Ƴ�TX���ţ�������ݷ���
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);     //����Ҫ�ȴ�TXE��1�����Լ��˸�while.����SET��ͣ��
	
}
                                     //���������޷��ж��Ƿ�����������ٴ���Length
void Serial_SendArray(uint8_t *Array, uint16_t Length)  //�����Ǹ��ܴ��Ķ���̫�٣����������������������ݡ������Ǹ�ָ�룬ָ�������������׵�ַ�������������ô��
{
	uint16_t i;
	for(i = 0 ; i< Length ; i++)
	{
		Serial_SendByte(Array[i]);   //����ȡ�������ÿһ�Ȼ����SendByte����
	}
}

void Serial_SendString(char *String)   //����'\0'�����ԾͲ���Length�ˡ������ַ���
{
	uint8_t i;
	for(i = 0; String[i] != '\0'; i++)
	{
		Serial_SendByte(String[i]);
	}
}


uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while(Y --)
	{
		Result *= X;
	}
	return Result;
}

                                                 //ȡxλ���ǣ�����/10^x%10��ע�Ⱑ����λx=0
void Serial_SendNum(uint32_t Num, uint8_t Length)  //��ʾ�ַ�����ʽ������
{
	uint8_t i;
	for(i = 0; i < Length ;i++)
	{
		Serial_SendByte(Num / Serial_Pow(10,Length - i - 1) % 10 + 0x30);  //�������ᰴ��ʮ���ƴӸ�λ����λ���η���,Ȼ�����ƫ��
	}
}

int fputc(int ch, FILE *f)     //printfֻ����ʾ����Ļ�ϣ�OLED�ϲ��У��������ǵ��ض���һ�µ�����
{                              //fputc��printf�ĵײ㣬stdio���ڵ���printf��ʱ��ʵ���Ͼ��ǲ��ϵ���fputcһ������ӡ��
	Serial_SendByte(ch);
	return ch;
	
}

//void Serial_Printf(char *format, ...)  //�������main��ĵ������������ǳ������ݣ��Ҳ���֪�����ɱ��������ʵ�ַ���
//{
//	char String[100];
//	va_list arg;  //����һ�������б����
//	va_start(arg, format);   //��formatλ�ÿ�ʼ���ղ���������arg����
//	vsprintf(String, format, arg);   //��ӡString,��ʽ���ַ�����format,��������arg
//	va_end(arg);    //�ͷŲ�����
//	Serial_SendString(String);
//}

//void SendFloatRaw(float f)
//{
//    uint8_t *data = (uint8_t*)&f;
//    Serial_SendByte(data[0]);
//    Serial_SendByte(data[1]);
//    Serial_SendByte(data[2]);
//    Serial_SendByte(data[3]);
//}



//���������������ݺ������NVIC��������ɡ��жϽ��ա����ܵġ����ڽ���һ������ģʽ��һ���ǲ�ѯ��һ�����жϡ����߹�����ͬ���ǳ�����ͬ
//uint8_t Serial_GetRxFlag(void)
//{
//	if(Serial_RxFlag == 1)
//	{
//		Serial_RxFlag = 0;
//		return 1;
//	}
//	else{return 0;}
//}  //�����ע�͵�������ΪFlag�������ˣ��Ҿ��Ȳ���0��



void USART1_IRQHandler(void)   //ʹ���ж�ģʽ���������ݰ�����Ȼ����ѯģʽҲ���Խ��գ����Ǹ�״̬��
{                               //������ж�ģʽ����+�ɴ�����������
	static uint8_t RxState = 0;
	static uint8_t pRxPacket = 0;  //��ʾ�����յ���һ���ˡ�
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		
		char RxData = USART_ReceiveData(USART1);
		
		if(RxState == 0)  //�ȴ���ͷ
		{
			if(RxData == '@' && Serial_RxFlag == 0)   //�ڶ��������Ƿ�ֹ���ݰ�����̫�죬���´���ġ������ٷ���һ��
			{
				RxState = 1;    //���ַ�������ֱ���ڸ���״̬��ת�ƣ�������
				pRxPacket = 0;
			}
		}
		else if(RxState == 1)  //��������
		{
			if(RxData == '\r')
			{
				RxState = 2;
			}
			else
			{
				Serial_RxPacket[pRxPacket] = RxData;
				pRxPacket++;
			}
		}
		else if(RxState == 2)   //�ȴ���β
		{
			if(RxData == '\n')
			{
				RxState = 0;
				Serial_RxPacket[pRxPacket] = '\0';   //�����Ϊ���������ַ�����ʱ����ж�ʲôʱ�����
				Serial_RxFlag = 1;
			}
		}
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);  //�����־λ�õ�
	}
	
}



















