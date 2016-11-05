#ifndef __PWM_H__
#define __PWM_H__

#include "tm4c123gh6pm.h"

extern unsigned long dutyCycleA;

void initPWM( unsigned long freq, unsigned long dutyc );
void IntPWM0_Handler( void );
void pwmSet( unsigned long freq, unsigned long dutyc );
void pwmStart( void );
void pwmStop( void );
//void SysTick_Handler( void );
//void dutycycle( unsigned char porcento );

#endif

