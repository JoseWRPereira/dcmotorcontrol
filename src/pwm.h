#ifndef __PWM_H__
#define __PWM_H__

#include "tm4c123gh6pm.h"

void initPWM( void );
void IntPWM0Handler( void );
void SysTick_Handler( void );
void dutycycle( unsigned char porcento );

#endif

