#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Key.h"
#include "Timer.h"
#include "PWM.h"
#include "TT130.h"
#include "Serial.h"  
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "PID.h"


uint8_t state = 0;
float CNT,Out,Target;
int PID_state;


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
			printf("%f,%f,%f\n",Target,CNT,Out);
			
			if(Serial_RxFlag == 1)
			{
				if(strcmp(Serial_RxPacket, "ModeOne") == 0 )
				{
					OLED_Clear();
					OLED_ShowString(1, 1, "SingleRound");
					state = 1;
					PID_state = 1;
				}
				else if(strcmp(Serial_RxPacket, "ModeTwo") == 0)
				{
					OLED_Clear();
					OLED_ShowString(1, 1, "Follow");
					state = 2;
					PID_state = 2;
				}

				
				if(state == 1){
					if(strcmp(Serial_RxPacket, "TwoHundred") == 0)
					{
						Target = 80;
					}
					else if(strcmp(Serial_RxPacket, "mTwoHundred") == 0)
					{
						Target = -80;
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



void TIM2_IRQHandler(void)    
{
	
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		Key_Tick();
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}



void TIM1_UP_IRQHandler(void)    
{
	
	if(TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
	{
			switch(PID_state)
			{
				case 1:{PID_Mode1();
								break;}
				case 2:{PID_Mode2();
				        break;}
				default:{Set_PWMCompare1(0);
								 Set_PWMCompare2(0);
								break;}
				
			}
		
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	}
}




