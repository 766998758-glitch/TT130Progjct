#include "stm32f10x.h"       // Device header
#include "Serial.h"
#include "PWM.h"
#include "TT130.h"
#include <string.h>
#include <math.h>
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
					kp = 0.7;
					ki = 0;
					kd = 2;
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
					
					//老实了，还是用位置式吧
					kp = 0.2; ki = 0; kd = 2;
        
        static float initial_master_pos = 0;
        static uint8_t first_enter = 1;
        static float last_error = 0;
        static float error_sum = 0;
        
        // 首次进入时记录电机1的初始位置
        if(first_enter) {
            first_enter = 0;
            last_error = 0;
            error_sum = 0;
            Out = 0;  // 重置输出
        }
        
        float master_position = GetEncoderCount_Tick1()+5;
        float follow_position = GetEncoderCount_Tick2();
        
        // 目标值 = 初始位置 + (电机1当前位置 - 初始位置)
        // 这样初始误差为0
        float follow_target = master_position;        
        float error = follow_target - follow_position;
        
        // 死区控制
        if(fabs(error) < 3.0f) {
            error = 0;
            error_sum = 0;
        }
        
        error_sum += error;
        float derivative = error - last_error;
        last_error = error;
        
        Out = kp * error + ki * error_sum + kd * derivative;
        
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
