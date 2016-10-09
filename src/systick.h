#ifndef __SYSTICK_H__
#define __SYSTICK_H__

#include "tm4c123gh6pm.h"

void initSysTickms( unsigned long baseTempo_ms );
void initSysTickus( unsigned long baseTempo_us );
unsigned char sysTickRun( void );
void sysTickWaitBusy( unsigned long time );

#endif //__SYSTICK_H__
