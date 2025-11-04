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
				//增量式PID	
        kp = 0.15;
        ki = 0.01;
        kd = 0.8;
        
        static uint32_t count1 = 0;
        static float loc_error0 = 0, loc_error1 = 0, loc_error2 = 0;
        static float loc_last_out = 0;
        
        count1++;
        if(count1 >= 15)
        {
            CNT = GetEncoderCount_Tick1();
            count1 = 0;
        }
        
        loc_error1 = loc_error0;
        loc_error0 = Target - CNT;
        
        // 增量式PID使用局部变量(我就应该再写一套变量的）
        float delta_out = kp * (loc_error0 - loc_error1) + 
                         ki * loc_error0 + 
                         kd * (loc_error0 - 2*loc_error1 + loc_error2);
        
        Out = loc_last_out + delta_out;
        loc_last_out = Out;
        loc_error2 = loc_error1;  // 更新error2
					if(Out > 200){Out = 200;loc_last_out = 200;}
					if(Out < -200){Out = -200;loc_last_out = -200;}
					
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
					kp = 0.22; ki = 0; kd = 2;
        
        static float last_error = 0;
        static float error_sum = 0;
				static float dynamic_offset = 0;  //初始误差
        
        float master_position = GetEncoderCount_Tick1()+6.5;
        float follow_position = GetEncoderCount_Tick2();
        
        // 目标值 = 初始位置 + (电机1当前位置 - 初始位置)
        // 这样初始误差为0
        float follow_target = master_position;        
        float error = follow_target - follow_position;
					
					if(fabs(error_sum) > 15.0f) {  // 当积分累积到一定程度
            dynamic_offset += error_sum * 0.01f;  // 缓慢调整偏移
            error_sum = 0;  // 重置积分避免饱和
        }
        
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
