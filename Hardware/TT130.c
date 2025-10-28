#include "stm32f10x.h"                  // Device header

int32_t Motor_EncoderCount = 0;

void MotorEncoder_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 65535;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
//	TIM_TimeBaseStructure.TIM_RepetitionCounter = ;   //用不上，TIM3不是高级计时器
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	//这一行有说法的。旋转编码器那里要配置两次NVIC，然后写左右转动增减函数的。这里直接用这个就好了。
  //这个双相计数TI12也是有说法的，说法在哪，我也不清楚

	TIM_Cmd(TIM3, ENABLE);
}


//获取编码器计数
int16_t GetEncoderCount_Tick(void)
{
		int16_t result = (int16_t)TIM3->CNT;
		TIM3->CNT = 0;
		return result;
}







