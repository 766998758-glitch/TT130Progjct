#include "stm32f10x.h"       // Device header
#include "Serial.h"
#include "PWM.h"
#include "TT130.h"
#include <string.h>
#include "OLED.h"


extern float CNT,Out,Target;
float error0,error1,ErrorInt;
float kp,ki,kd;
float  error2,last_output;

extern int PID_state;


void PID_Mode1(void)
{
				if(PID_state == 1)
				{
					//位置式PID控速
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


void PID_Mode2(void)
{
				if(PID_state == 2)
				{
					//增量式PID控位置
					//注意这个代码架构啊。电机1是不转的，电机2的目标值是电机1的实际值
					kp = 0.6;
					ki = 0;
					kd = 4;
					float master_position = GetEncoderCount_Tick1();
					float follow_position = GetEncoderCount_Tick2();

					float follow_target = master_position;
					error2 = error1;
					error1 = error0;
					error0 = follow_target - follow_position;
					float delta_out = kp*(error0 - error1) +
                         		ki* error0 +
                         	  kd*(error0 - 2*error1 + error2);
        		    Out = last_output + delta_out;
        		    last_output = Out;
					if(Out >= 200){Out = 200;}
					if(Out <= -200){Out = -200;}
					if(Out > 0){
						Forward2();
						Set_PWMCompare2((uint16_t)Out);
					}
					else{
						Backward2();
						Set_PWMCompare2((uint16_t)(-Out));
					}
					Set_PWMCompare1(0);
				}
	
	
}
