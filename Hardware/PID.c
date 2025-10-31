#include "stm32f10x.h"       // Device header
#include "Serial.h"
#include "PWM.h"
#include "TT130.h"
#include <string.h>
#include "OLED.h"


float CNT,Out,Target;
float error0,error1,ErrorInt;
float kp,ki,kd;

int PID_state = 0;

void PID_Mode(void)
{
				if(state == 1)
				{
					//Î»ÖÃÊ½PID¿ØËÙ
					kp = 0.6;
					ki = 0;
					kd = 4;
					static uint32_t count1 = 0;
					count1++;
					if(count1 >= 12)
					{
						CNT = GetEncoderCount_Tick1();
						count1 = 0;
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
				}
	
}
