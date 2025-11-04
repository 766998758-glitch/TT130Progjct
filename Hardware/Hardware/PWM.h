#ifndef __PWM_H
#define __PWM_H

void PWM_Init();
void Set_PWMCompare1(uint16_t compare);
void Set_PWMCompare2(uint16_t compare);
void Forward1(void);
void Backward1(void);
void Forward2(void);
void Backward2(void);

#endif
