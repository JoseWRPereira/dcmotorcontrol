#include "swleds.h"
#include "timer.h"
#include "uart.h"
#include "controlador.h"
#include "pwm.h"

unsigned int  setpoint;
unsigned char habilita;

unsigned long fila[TAMFILA];
unsigned long somaTempo;
unsigned long aquis;
unsigned int indiceFila=0;
unsigned long rpsA;
unsigned long rpsB;
unsigned int contB;

unsigned char send = 0;

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


  for( indiceFila=0; indiceFila < TAMFILA; indiceFila++ )
  {
    fila[indiceFila] = 0;//1000001;
    somaTempo += fila[indiceFila];
  }
  indiceFila = 0;
  habilita = 0;
}


void addZero_off( void )
{
    aquis = 80000001;
    somaTempo -= fila[indiceFila];
    fila[indiceFila] = aquis;
    indiceFila = ((++indiceFila) & (TAMFILA-1) );
    somaTempo += aquis;
    rpsA = SYSTEM_CLOCK/(somaTempo>>POTN2);
    contB = 0;
}

void GPIOPortF_Handler_off(void)
{ 
  if(GPIO_PORTF_RIS_R&0x01)
  {  // SW2 touch
    GPIO_PORTF_ICR_R = 0x01;  // acknowledge flag0
//    aquis = 80000000-readT1A() ;
    aquis = (0xFFFFFFFF-readT1A())/1000 ;
    resetT1A();
    somaTempo -= fila[indiceFila];
    fila[indiceFila] = aquis;
    //indiceFila = ((++indiceFila) % TAMFILA );
    indiceFila = ((++indiceFila) & (TAMFILA-1) );
    somaTempo += aquis;
    rpsA = SYSTEM_CLOCK/(somaTempo>>POTN2);
    contB = 0;
  }
  if(GPIO_PORTF_RIS_R&0x10)
  {  // SW1 touch
    GPIO_PORTF_ICR_R = 0x10;  // acknowledge flag4
  }
}

void addZero( void )
{
    aquis = 100000000;
    aquis = 0xFFFFFFFF-readT1A();
    resetT1A();
    somaTempo -= fila[indiceFila];
    fila[indiceFila] = aquis;
    indiceFila = ((++indiceFila) & (TAMFILA-1) );
    somaTempo += aquis;
    rpsA = 100000000/aquis;
    rpsB = 800000000/somaTempo;
    contB = 0;
}

void GPIOPortF_Handler(void)
{ 
  if(GPIO_PORTF_RIS_R&0x01)
  {  // SW2 touch
    GPIO_PORTF_ICR_R = 0x01;  // acknowledge flag0
    aquis = 0xFFFFFFFF-readT1A();
    resetT1A();
    aquis >>= 3;
    somaTempo -= fila[indiceFila];
    fila[indiceFila] = aquis;
    indiceFila = ((++indiceFila) & (TAMFILA-1) );
    somaTempo += aquis;
    rpsA = 100000000/aquis;
    rpsB = 800000000/somaTempo;
    contB = 0;

    if( habilita )   
    {
      pwmSet( PWM_FREQ, controlador( setpoint, 1000, rpsB ) );
    }
    else
    {
      pwmSet( PWM_FREQ, 0 );
    }
  }
  if(GPIO_PORTF_RIS_R&0x10)
  {  // SW1 touch
    GPIO_PORTF_ICR_R = 0x10;  // acknowledge flag4
  }
}

