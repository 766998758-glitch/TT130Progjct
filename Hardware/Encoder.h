#ifndef __ENCODER_H
#define __ENCODER_H

double Encoder_Get(void);
void Encoder_Init(void);
void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);



#endif