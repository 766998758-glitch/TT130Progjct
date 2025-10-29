#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Key.h"
#include "Timer.h"
//#include "Cursor2.h"
//#include "Encoder.h"
#include "PWM.h"
#include "TT130.h"
#include "Serial.h"  //这个东西先别用的。等电池有电了再说。直接让packet[2]等于Target就行。
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

void TIM2_IRQHandler(void)     //这个调控周期啊，一般是越小越好。但是会受到硬件配置制约。这里我们用的还是1ms中断一次
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
		/**********位置式PID调控***********/
		/*获取实际值*/
		CNT = GetEncoderCount_Tick();
		count = 0;
	}
		
		/*获得本次误差和上次误差*/
		error1 = error0;   //上次误差就是上次的本次误差
		error0 = Target - CNT;
		
		/*积分*/
		ErrorInt += error0;
		
		/*PID计算*/
		Out = kp * error0 + ki * ErrorInt + kd * (error0 - error1); 
		
		/*输出限幅*/
		if(Out > 200){Out = 200;}
		if(Out < -200){Out = -200;}
		
		
		/*执行控制*/
		if(Out > 0)
		{
			Forward();
			Set_PWMCompare((uint16_t)Out);   //输出至被控对象，对于电机来说，便是PWM
		}
		else
		{
			Backward();
			Set_PWMCompare((uint16_t)(-Out));
		}
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}