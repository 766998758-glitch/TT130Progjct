#include "stm32f10x.h"                  // Device header


void PWM_Init()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	
	GPIO_InitTypeDef GPIO_InitStructure1;
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;  
	GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure1);
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure11;
	GPIO_InitStructure11.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure11.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure11.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure11);
	
	TIM_InternalClockConfig(TIM2);
		
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 32 - 1;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;		//CCR
	
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);
	
	NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_Init(&NVIC_InitStructure);
	TIM_Cmd(TIM2, ENABLE);
		
}

void Set_PWMCompare1(uint16_t compare)
{
	TIM_SetCompare3(TIM2, compare);
}
void Set_PWMCompare2(uint16_t compare)
{
	TIM_SetCompare4(TIM2, compare);
}

void Forward1(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_12);
  GPIO_ResetBits(GPIOB, GPIO_Pin_13);
}

void Backward1(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_13);
  GPIO_ResetBits(GPIOB, GPIO_Pin_12);
}

void Forward2(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_14);
  GPIO_ResetBits(GPIOB, GPIO_Pin_15);
}

void Backward2(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_15);
  GPIO_ResetBits(GPIOB, GPIO_Pin_14);
}

