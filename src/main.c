/* *********************************************************

 ESTUDO COMPARATIVO ENTRE TÉCNICAS DE CONTROLE: PID e LPA2v
 
             Pós-Graduação Lato Sensu
 Especialização em Sistemas Eletrônicos para Controle 
        Faculdade de Tecnologia SENAI "Anchieta"
 
               José W. R. Pereira 	
                   2016-10-02

********************************************************* */

/* ************************** Uso dos Pinos em cada PORT

PA0 : UART0_RX 
PA1 : UART0_TX
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
PD2 : WT3A - Capture Mode
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
#include "controlador.h"


void main( void )
{
  unsigned char aux='0';

  initPLL();				// 80MHz
  initSWLEDS();
  initUART0_80MHz_115200bps();
  initBluetooth();
  initPWM( 500, 50 ); // FreqPWM = 500Hz, DutyCicle = 50%
//  pwmStop();
  initTimer(1000*80000, 10*80000);
  UART_OutChar('#');
  UART_OutChar('>');

//  pwmSet(PWM_FREQ, ((int)(pwmDz+pwmUn)*PWM_FREQ)/100 );
  while( 1 )
  {

    if( print )
    {
      print = 0;
      SETLED( RED );
      UART_OutUDec( rpsB/10 );
      UART_OutChar( '.' );
      UART_OutUDec(rpsB%10);
      UART_OutChar( ' ' );
      UART_OutUDec( Cout );
      UART_OutCRLF();
      CLRLED( RED );
    }
    if( UART_InCharAvailable() )
    {
      aux = UART_InChar();
      
      switch( aux )
      {
 	case 13: // ENTER  	
		  if( !habilita )
		     pwmSet( PWM_FREQ, 99 );
	          habilita = !habilita;
		break;
      }
    }
  } // while(1)
} // void main(void)

