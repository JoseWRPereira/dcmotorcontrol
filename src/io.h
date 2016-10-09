#ifndef __IO_H__
#define __IO_H__

#include "tm4c123gh6pm.h"

#define PORTA                GPIO_PORTA_DATA_R
#define PORTB                GPIO_PORTB_DATA_R
#define PORTC                GPIO_PORTC_DATA_R
#define PORTD                GPIO_PORTD_DATA_R
#define PORTE                GPIO_PORTE_DATA_R
#define PORTF                GPIO_PORTF_DATA_R

#define PORTA_BASE           0x40004000
#define PORTB_BASE           0x40005000
#define PORTC_BASE           0x40006000
#define PORTD_BASE           0x40007000
#define PORTE_BASE           0x40024000
#define PORTF_BASE           0x40025000

#define PORTbit0             0x00000004 
#define PORTbit1             0x00000008 
#define PORTbit2             0x00000010 
#define PORTbit3             0x00000020 
#define PORTbit4             0x00000040  
#define PORTbit5             0x00000080 
#define PORTbit6             0x00000100 
#define PORTbit7             0x00000200 


void PortF_Init(void);       // PortF: MSB xxxIOOOI LSB
void PortE_Init(void);       // PortE: MSB xxxxxxII LSB
void PortD_Init(void);	     // PortD: 
void PortC_Init(void);	     // PortC: 

#endif

