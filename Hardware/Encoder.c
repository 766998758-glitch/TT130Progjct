#include "stm32f10x.h"                  // Device header
#include "Cursor2.h"
#include "OLED.h"

double Encoder_Count=0;

void Encoder_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	GPIO_InitTypeDef GIOP_InitStrcuture;
	GIOP_InitStrcuture.GPIO_Mode= GPIO_Mode_IPU;        
	GIOP_InitStrcuture.GPIO_Pin= GPIO_Pin_0 | GPIO_Pin_1;  
	GIOP_InitStrcuture.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GIOP_InitStrcuture);
		
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);   
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);   

	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line0 | EXTI_Line1;       
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;                    
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;           
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;         
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);   

	NVIC_InitTypeDef NVIC_InitSturecture;
	NVIC_InitSturecture.NVIC_IRQChannel = EXTI0_IRQn;           
	NVIC_InitSturecture.NVIC_IRQChannelCmd = ENABLE;            
	NVIC_InitSturecture.NVIC_IRQChannelPreemptionPriority = 1;  
	NVIC_InitSturecture.NVIC_IRQChannelSubPriority = 1;        
	NVIC_Init(&NVIC_InitSturecture);

	NVIC_InitSturecture.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitSturecture.NVIC_IRQChannelCmd = ENABLE;  
	NVIC_InitSturecture.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitSturecture.NVIC_IRQChannelSubPriority = 2;     
	NVIC_Init(&NVIC_InitSturecture);
}

double Encoder_Get(void)
{	
		double temp;
		temp = Encoder_Count;
		Encoder_Count = 0;
		return temp;
}



void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) == SET){
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0){
			Encoder_Count += 0.1f;
		}
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}
void EXTI1_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line1) == SET){
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0){
			Encoder_Count -= 0.1f;
		}
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}





















