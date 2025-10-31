#include "stm32f10x.h"                  // Device header
#include "Delay.h"

uint8_t Key_Num;
uint8_t Key_LongPress = 0;           
uint8_t Key_LongPress_Active = 0;    

void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

}

uint8_t Key_GetNum(void)    
{
	uint8_t temp;  //�Ҳ���ֱ�ӷ���Key_Nun,Ҫ��Ȼ��һ��������ֺ��һֱ�Ƿ��㣬����mode��������ƾͻ����л�ģʽ��˸
	temp = Key_Num;
	Key_Num = 0;
	return temp;
}
uint8_t Key_GetLongPress(void) //�������Ǹ���ȡ��ͨ�����������ȡ��������
{
    uint8_t temp;
    temp = Key_LongPress;
    Key_LongPress = 0;
    return temp;
}


uint8_t Key_GetState(void) //��ȡ��ǰ����״̬
{
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2) == 0)  
	{
		return 1;
	}
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 0)  
	{
		return 2;
	}
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)  
	{
		return 3;
	}
	if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) == 0)  
	{
		return 4;
	}
	
		return 0;   //���û���κ�һ����������
}

void Key_Tick(void){         //��ĵ�һ����״̬�������������TIM�ж�����������
	static uint16_t count;
	static uint8_t CurrState, PrevState;   //static��ʼֵ��0
	static uint16_t long_press_count = 0;     //������ʱ
  static uint16_t repeat_count = 0;       //�ظ�������ʱ
	static uint8_t long_press_key = 0;         //��ǰ�����İ���
	
	count++;
	
	if(count>=20)       //���20ָ20ms�������õ���20ms�ж�һ��
	{
		count=0;
		PrevState = CurrState;
		CurrState = Key_GetState();
		
		if(CurrState == 0 && PrevState != 0)  //��׽��һ�ΰ�������������
		{
			if(long_press_count < 1000)
			{
				Key_Num = PrevState;     
			}
					long_press_count = 0;
          repeat_count = 0;
          Key_LongPress_Active = 0;
          long_press_key = 0;
		}
		
		if(CurrState == 1 || CurrState == 2)  
        {
            long_press_count += 20;  
            
            if(long_press_count >= 1000 && long_press_key == 0)
            {
                Key_LongPress = CurrState;  
                Key_LongPress_Active = 1;   
                long_press_key = CurrState; 
                repeat_count = 0;           
            }
        }
        else
        {
            long_press_count = 0;
            Key_LongPress_Active = 0;
            long_press_key = 0;
        }
        
        if(Key_LongPress_Active && (CurrState == 1 || CurrState == 2))
        {
            repeat_count += 20;
            if(repeat_count >= 100)  
            {
                Key_LongPress = long_press_key;  
                repeat_count = 0;                
            }
        }
    }
}
		
		
	
	























