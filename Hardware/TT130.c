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
//	TIM_TimeBaseStructure.TIM_RepetitionCounter = ;   //�ò��ϣ�TIM3���Ǹ߼���ʱ��
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	//��һ����˵���ġ���ת����������Ҫ��������NVIC��Ȼ��д����ת�����������ġ�����ֱ��������ͺ��ˡ�
  //���˫�����TI12Ҳ����˵���ģ�˵�����ģ���Ҳ�����

	TIM_Cmd(TIM3, ENABLE);
}


//��ȡ����������
int16_t GetEncoderCount_Tick(void)
{
		int16_t result = (int16_t)TIM3->CNT;
		TIM3->CNT = 0;
		return result;
}







