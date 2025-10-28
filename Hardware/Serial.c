#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>

char Serial_RxPacket[100];   //接收缓存区
uint8_t Serial_RxFlag;  //收到数据包就置Flag


void Serial_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;     //Tx是输入，将数据流向Rx（输出），Rx则反之
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF_PP; 
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPU; 
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_10;      //TX配置在Pin9，RX配置在Pin10
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);	
	USART_InitTypeDef USART_InitStructure;          //下面的Init函数内部会帮我自动算好9600对应的分频系数，然后放在BRR寄存器里
	
	USART_InitStructure.USART_BaudRate = 9600;      //波特率（在普通状态下，就是比特率，单位时间内传输的比特数）
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;           //硬件流控制
	USART_InitStructure.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;        //串口模式，这里是发送模式。Rx是接收模式
	USART_InitStructure.USART_Parity = USART_Parity_No;         //校验位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;      //停止位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;        //字长
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
	USART_SendData(USART1, Byte); //从定义来看，这个Byte传给了Data，之后Data&1FF，把无关高位清零，然后再赋值给DR寄存器
	//由于是写入DR，数据最后通向TDR（发送数据寄存器），TDR再传给发送移位寄存器，然后一位一位的把数据移出TX引脚，完成数据发送
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);     //我们要等待TXE置1，所以加了个while.等于SET就停了
	
}
                                     //由于数组无法判断是否结束，我们再传个Length
void Serial_SendArray(uint8_t *Array, uint16_t Length)  //上面那个能传的东西太少，我们用数组来传更多数据。这里是个指针，指向待发送数组的首地址，传数组可以这么干
{
	uint16_t i;
	for(i = 0 ; i< Length ; i++)
	{
		Serial_SendByte(Array[i]);   //依次取出数组的每一项，然后用SendByte发送
	}
}

void Serial_SendString(char *String)   //由于'\0'，所以就不用Length了。发送字符串
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

                                                 //取x位就是：数字/10^x%10。注意啊，个位x=0
void Serial_SendNum(uint32_t Num, uint8_t Length)  //显示字符串形式的数字
{
	uint8_t i;
	for(i = 0; i < Length ;i++)
	{
		Serial_SendByte(Num / Serial_Pow(10,Length - i - 1) % 10 + 0x30);  //参数将会按照十进制从高位到地位依次发送,然后进行偏移
	}
}

int fputc(int ch, FILE *f)     //printf只能显示在屏幕上，OLED上不行，所以我们得重定向一下到串口
{                              //fputc是printf的底层，stdio库在调用printf的时候，实际上就是不断调用fputc一个个打印的
	Serial_SendByte(ch);
	return ch;
	
}

//void Serial_Printf(char *format, ...)  //这个对于main里的第三个方法，是超纲内容，我并不知道“可变参数”的实现方法
//{
//	char String[100];
//	va_list arg;  //定义一个参数列表变量
//	va_start(arg, format);   //从format位置开始接收参数表，放在arg里面
//	vsprintf(String, format, arg);   //打印String,格式化字符串是format,参数表是arg
//	va_end(arg);    //释放参数表
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



//这个下面的所有内容和上面的NVIC是用来达成”中断接收“功能的。串口接收一般两种模式，一个是查询，一个是中断。二者功能相同但是场景不同
//uint8_t Serial_GetRxFlag(void)
//{
//	if(Serial_RxFlag == 1)
//	{
//		Serial_RxFlag = 0;
//		return 1;
//	}
//	else{return 0;}
//}  //这个先注释掉啊，因为Flag又有用了，我就先不清0了



void USART1_IRQHandler(void)   //使用中断模式来接收数据包（当然，查询模式也可以接收）这是个状态机
{                               //最好用中断模式，快+可处理数据量大
	static uint8_t RxState = 0;
	static uint8_t pRxPacket = 0;  //表示“接收到哪一个了”
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		
		char RxData = USART_ReceiveData(USART1);
		
		if(RxState == 0)  //等待包头
		{
			if(RxData == '@' && Serial_RxFlag == 0)   //第二个条件是防止数据包传递太快，导致错误的。传完再发下一个
			{
				RxState = 1;    //这种方法可以直接在各个状态里转移，更方便
				pRxPacket = 0;
			}
		}
		else if(RxState == 1)  //接收数据
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
		else if(RxState == 2)   //等待包尾
		{
			if(RxData == '\n')
			{
				RxState = 0;
				Serial_RxPacket[pRxPacket] = '\0';   //这个是为了让输入字符串的时候好判断什么时候结束
				Serial_RxFlag = 1;
			}
		}
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);  //清除标志位用的
	}
	
}



















