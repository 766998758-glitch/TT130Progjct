#include "stm32f10x.h"                  // Device header
#include "Key.h"
#include "OLED.h"
#include "Encoder.h"

uint8_t current_line2=2;
double LED_Button2_1=0.2,LED_Button2_2=0.5,LED_Button2_3=0.8;


void Cursor_Init2(void)
{
	OLED_ShowString(current_line2, 2, ">");
	OLED_ShowFloat(2, 5, LED_Button2_1, 1, 1);
	OLED_ShowFloat(3, 5, LED_Button2_2, 1, 1);
	OLED_ShowFloat(4, 5, LED_Button2_3, 1, 1);
}

void Cursor_Tick2(void)
{
	double encoder_delta = Encoder_Get();
	uint8_t Key_Num = Key_GetNum();
	
	if(encoder_delta != 0)
    {
        if(current_line2 == 2)
        {
            LED_Button2_1 += encoder_delta;
            OLED_ShowFloat(2, 5, LED_Button2_1, 1, 1);
        }
        else if(current_line2 == 3)
        {
            LED_Button2_2 += encoder_delta;
            OLED_ShowFloat(3, 5, LED_Button2_2, 1, 1);
        }
        else if(current_line2 == 4)
        {
            LED_Button2_3 += encoder_delta;
            OLED_ShowFloat(4, 5, LED_Button2_3, 1, 1);
        }
    }
	
	
	if(Key_Num == 3)  //обрф    
    {
        OLED_ShowString(current_line2, 2, " ");
        current_line2 = ((current_line2 - 2 + 1) % 3) + 2;
        OLED_ShowString(current_line2, 2, ">");			
		}
}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
