#ifndef __TIMER_H__
#define __TIMER_H__

#include "tm4c123gh6pm.h"


unsigned long readT1A( void );
void resetT1A( void );

void IntT1A_Handler( void );
void IntT1B_Handler( void );

void iniTimer( void );


#endif
