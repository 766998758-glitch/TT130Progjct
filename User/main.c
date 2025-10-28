#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Key.h"
#include "Timer.h"
#include "Cursor2.h"
#include "Encoder.h"
#include "PWM.h"
#include "TT130.h"
#include "Serial.h"  //��������ȱ��õġ��ȵ���е�����˵��ֱ����packet[2]����Target���С�
#include<math.h>
#include<stdio.h>


//uint8_t menu_state = 0;
float CNT,Out,Target;
float error0,error1,ErrorInt;
float kp = 0.6,ki = 0.2,kd = 0.7;
uint32_t last_time = 0;
uint32_t current_time = 0;


int main(void)
{
	OLED_Init();
	Key_Init();
	Timer_Init();
	Encoder_Init();
	PWM_Init();
	Serial_Init();
	MotorEncoder_Init();
	
	
//		OLED_ShowString(1, 1, "PID");
//		OLED_ShowString(2, 3, "KP");
//		OLED_ShowString(3, 3, "KI");
//		OLED_ShowString(4, 3, "KD");
//		Cursor_Init2();
		
	 while (1)
    {
			Target = 20;			
			
			Forward();
			Set_PWMCompare(Out);
			printf("%f,%f,%f\n",Target,CNT,Out);
			
			OLED_ShowString(1, 1, "Encoder Test:");
    
    
    
        // ??1:????TIM3->CNT,???
        int16_t encoder_raw = (int16_t)TIM3->CNT;
        OLED_ShowNum(2, 1, encoder_raw, 5);
        
        // ??2:???????
        int16_t encoder_tick = GetEncoderCount_Tick();
        OLED_ShowNum(3, 1, encoder_tick, 5);
        
        // ??????,????????
        if(encoder_raw != 0)
        {
            OLED_ShowString(4, 1, "Encoder OK!   ");
        }
        else
        {
            OLED_ShowString(4, 1, "Encoder ERROR!");
        }
        
        Delay_ms(200);
    
			
		}
}

void TIM2_IRQHandler(void)     //����������ڰ���һ����ԽСԽ�á����ǻ��ܵ�Ӳ��������Լ�����������õĻ���1ms�ж�һ��
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		Key_Tick();
		Cursor_Tick2();
		
//		kp = LED_Button2_1;
//		ki = LED_Button2_2;
//		kd = LED_Button2_3;
		
		static uint8_t count = 0;
		count++;
		
		if(count >= 10)
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
		if(Out > 100){Out = 100;}
		if(Out < -100){Out = -100;}
		
		/*ִ�п���*/
//		if(Out > 0)
//		{
//			Forward();
//			Set_PWMCompare((uint16_t)Out);   //��������ض��󣬶��ڵ����˵������PWM
//		}
//		else
//		{
//			Backward();
//			Set_PWMCompare((uint16_t)(-Out));
//		}
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}