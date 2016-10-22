/* *********************************************************

 ESTUDO COMPARATIVO ENTRE TÉCNICAS DE CONTROLE: PID e LPA2v
 
             Pós-Graduação Lato Sensu
 Especialização em Sistemas Eletrônicos para Controle 
        Faculdade de Tecnologia SENAI "Anchieta"
 
               José W. R. Pereira 	
                   2016-10-02

********************************************************* */

/* ************************** Uso dos Pinos em cada PORT

PA0 : UART1_RX 
PA1 : UART1_TX
PA2 :
PA3 : 
PA4 : 
PA5 : 
PA6 :
PA7 :

PB0 : 
PB1 : 
PB2 :
PB3 : 
PB4 : 
PB5 : 
PB6 : M0PWM0
PB7 : M0PWM1

PC0 : 
PC1 : 
PC2 :
PC3 : 
PC4 : 
PC5 : 
PC6 :
PC7 :

PD0 : 
PD1 : 
PD2 :
PD3 : 
PD4 : 
PD5 : 
PD6 : UART2-RX
PD7 : UART2-TX

PE0 : 
PE1 : 
PE2 :
PE3 : 
PE4 : 
PE5 : 
PE6 :
PE7 :

PF0 : SW2
PF1 : LED_R
PF2 : LED_B
PF3 : LED_G
PF4 : SW1

PF5 : 
PF6 :
PF7 :

********************************************************* */

#include "pll.h"
#include "swleds.h"
#include "systick.h"
#include "uart.h"
#include "bluetooth.h"
#include "pwm.h"
#include "timer.h"

/**********************************************************/
//extern void DisableInterrupts(void); // Disable interrupts
//extern void EnableInterrupts(void);  // Enable interrupts
//long StartCritical (void);    // previous I bit, disable interrupts
//void EndCritical(long sr);    // restore I bit to previous value
//void WaitForInterrupt(void);  // low power mode
/**********************************************************/


void main( void )
{
  unsigned char aux;
  initSWLEDS();
  initPLL();			// 80MHz
  initSysTick( 80000 );		// 80MHz / 80 000 = 1kHz
  initUART0_80MHz_115200bps();
  initBluetooth();
  initPWM();
  initTimer(1000*80000);


  while( 1 )
  {
    //while( sysTickRun(  400 ) );  CPLLED( RED );  
    if( t1flag )
    {
	t1flag = 0;
	CLRLED( RED );
	UART_OutUDec( contaPulsos );
	UART_OutCRLF();
	contaPulsos = 0;
    } 
    if( UART_InCharAvailable()  ) 
    {
	aux = UART_InChar();
	if( aux >= '0' && aux <= '9' )
	{
           pwmSet( 10000, (aux - '0') * 1000 );
           pwmStart();
        }
        if( aux == 'P' )
	{
	  pwmStop();
	}
	if( aux == 'M' )
	{
	  pwmStart();
	}
    }
  }

}

