#include "swleds.h"

unsigned long contaPulsos;

void initSWLEDS( void )
{ 
  volatile unsigned long delay;
  SYSCTL_RCGCGPIO_R	|= SYSCTL_RCGC2_GPIOF;
  SYSCTL_RCGC2_R 	|= SYSCTL_RCGC2_GPIOF;
  delay = SYSCTL_RCGC2_R;
  GPIO_PORTF_LOCK_R	= GPIO_LOCK_KEY;	// Unlock PF[0]  
  GPIO_PORTF_CR_R	|=  0x1F;		// Allow GPIO port F
  GPIO_PORTF_DIR_R	&= ~0x11;		// Set Input
  GPIO_PORTF_DIR_R	|=  0x0E;		// Set Output
  GPIO_PORTF_AFSEL_R	&= ~0x1F;		// GPIO, Alt function off
  GPIO_PORTF_PCTL_R	&= ~GPIO_PCTL_PF4_M;	// PF4 mask
  GPIO_PORTF_PUR_R	|=  0x11;		// Set Pull-Up 
  GPIO_PORTF_DEN_R	|=  0x1F;		// Digital Enable
  GPIO_PORTF_IM_R	&= ~0x11;		// Interrupt Mask
  GPIO_PORTF_IBE_R 	&=  0x11;		// Interrupt Both Edge
  GPIO_PORTF_IEV_R	|=  0x11; 		// Set Rising Edge
  GPIO_PORTF_IS_R	&= ~0x11;		// Interrupt Sense	
  GPIO_PORTF_IM_R       |=  0x11;		// Interrupt Mask
  GPIO_PORTF_ICR_R	|=  0x11;		// Clear Interrupt Reg

  NVIC_PRI7_R		 = (NVIC_PRI7_R & 0xFF00FFFF)|0x00400000;
  NVIC_EN0_R		 = 0x40000000; 
}


void GPIOPortF_Handler(void)
{ 
  if(GPIO_PORTF_RIS_R&0x01)
  {  // SW2 touch
    GPIO_PORTF_ICR_R = 0x01;  // acknowledge flag0
    ++contaPulsos;
  }
  if(GPIO_PORTF_RIS_R&0x10)
  {  // SW1 touch
    GPIO_PORTF_ICR_R = 0x10;  // acknowledge flag4
    CPLLED( RED ); 
  }
}

