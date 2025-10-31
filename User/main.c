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
#include "PID.h"


uint8_t state = 0;

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
			float CNT,Out,Target;
			
			printf("%f,%f,%f\n",Target,CNT,Out/1.3);
			
			if(Serial_RxFlag == 1)
			{
				if(strcmp(Serial_RxPacket, "ModeOne") == 0 )
				{
					OLED_ShowString(1, 1, "SingleRound");
					state = 1;
				}
				else if(state == 1){
					if(strcmp(Serial_RxPacket, "TwoHundred") == 0)
					{
						Target = 300;
					}
					else if(strcmp(Serial_RxPacket, "mTwoHundred") == 0)
					{
						Target = -300;
					}
					else if(strcmp(Serial_RxPacket, "Zero") == 0)
					{
						Target = 0;
					}
				}
				Serial_RxFlag = 0;
			}
		}
}


void TIM1_UP_IRQHandler(void)    
{
	
	if(TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
	{
			void PID_Mode();

		
		
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	}
}