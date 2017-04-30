#ifndef __TIMER_H__
#define __TIMER_H__

#include "tm4c123gh6pm.h"

extern unsigned long seg1;
extern unsigned char print;

unsigned long readT1A( void );
void resetT1A( void );

void IntT1A_Handler( void );
void IntT1B_Handler( void );

void IntT2A_Handler( void );
void IntT2B_Handler( void );

void iniTimer( unsigned long time, unsigned long timeAquis );


#endif

