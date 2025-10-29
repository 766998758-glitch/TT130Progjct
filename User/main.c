#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Key.h"
#include "Timer.h"
//#include "Cursor2.h"
//#include "Encoder.h"
#include "PWM.h"
#include "TT130.h"
#include "Serial.h"  //��������ȱ��õġ��ȵ���е�����˵��ֱ����packet[2]����Target���С�
#include<math.h>
#include<stdio.h>


//uint8_t menu_state = 0;
float CNT,Out,Target;
float error0,error1,ErrorInt;
float kp,ki,kd;
//uint32_t last_time = 0;
//uint32_t current_time = 0;


int main(void)
{
	OLED_Init();
	Key_Init();
	Timer_Init();
	PWM_Init();
	Serial_Init();
	MotorEncoder_Init();
	
		
	 while (1)
    {
			Target = 200;			
			OLED_ShowNum(1, 1, Out, 5);
			
			printf("%f,%f,%f\n",Target,CNT,Out/1.4);
		}
}

void TIM2_IRQHandler(void)     //����������ڰ���һ����ԽСԽ�á����ǻ��ܵ�Ӳ��������Լ�����������õĻ���1ms�ж�һ��
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		Key_Tick();
		
		kp = 0.8;
		ki = 0;
		kd = 3;
		
		static uint8_t count = 0;
		count++;
		
		if(count >= 200)
	{
		/**********λ��ʽPID����***********/
		/*��ȡʵ��ֵ*/
		CNT = GetEncoderCount_Tick();
		count = 0;
	}
		
		/*��ñ��������ϴ����*/
		error1 = error0;   //�ϴ��������ϴεı������
		error0 = Target - CNT;
		
		/*����*/
		ErrorInt += error0;
		
		/*PID����*/
		Out = kp * error0 + ki * ErrorInt + kd * (error0 - error1); 
		
		/*����޷�*/
		if(Out > 200){Out = 200;}
		if(Out < -200){Out = -200;}
		
		
		/*ִ�п���*/
		if(Out > 0)
		{
			Forward();
			Set_PWMCompare((uint16_t)Out);   //��������ض��󣬶��ڵ����˵������PWM
		}
		else
		{
			Backward();
			Set_PWMCompare((uint16_t)(-Out));
		}
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}