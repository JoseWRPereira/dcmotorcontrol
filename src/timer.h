#ifndef __TIMER_H__
#define __TIMER_H__

#include "tm4c123gh6pm.h"

extern unsigned char t1flag, t2flag;

void IntT1A_Handler( void );
void IntT1B_Handler( void );

void iniTimer( void );

#endif

