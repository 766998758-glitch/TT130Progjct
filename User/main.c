#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Key.h"
#include "Timer.h"
//#include "Cursor2.h"
//#include "Encoder.h"
#include "PWM.h"
#include "TT130.h"
#include "Serial.h"  
#include<math.h>
#include<stdio.h>
#include<string.h>


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
			printf("%f,%f,%f\n",Target,CNT,Out/1.4);
			
			if(strcmp(Serial_RxPacket, "TwoHundred") == 0)
				{
					Target = 200;
				}
				else if(strcmp(Serial_RxPacket, "mTwoHundred") == 0)
				{
					Target = -200;
				}
				Serial_RxFlag = 0;
		}
}


void TIM2_IRQHandler(void)    
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		kp = 0.8;
		ki = 0;
		kd = 3;
		
		static uint32_t count = 0;
		count++;
		
		if(count >= 200)
		{
			CNT = GetEncoderCount_Tick1();
			count = 0;
		}
		
		
		error1 = error0;   
		error0 = Target - CNT;
	
	
		ErrorInt += error0;
		
		
		Out = kp * error0 + ki * ErrorInt + kd * (error0 - error1); 
		
		
		if(Out > 200){Out = 200;}
		if(Out < -200){Out = -200;}
		
		
		if(Out > 0)
		{
			Forward1();
			Set_PWMCompare1((uint16_t)Out);  
		}
		else
		{
			Backward1();
			Set_PWMCompare1((uint16_t)(-Out));
		}
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}