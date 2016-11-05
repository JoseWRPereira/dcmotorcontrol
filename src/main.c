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

/**********************************************************/
#define PWM_FREQ	500
unsigned char pwmUn, pwmDz;
/**********************************************************/


void main( void )
{
  unsigned char aux='0';

  initPLL();				// 80MHz
  initSWLEDS();
  initUART0_80MHz_115200bps();
  initBluetooth();
  initPWM( PWM_FREQ, PWM_FREQ/10 );
  pwmStop();
  initTimer(1000*80000);
  UART_OutChar('#');
  UART_OutChar('>');
  while( 1 )
  {
    if( UART_InCharAvailable() )
    {
      aux = UART_InChar();
      UART_OutChar('#');
      UART_OutChar( aux );
      UART_OutCRLF();
      
      switch( aux )
      {
	case '0':  pwmUn = 0;	 	break;
	case '1':  pwmUn = 1; 		break;
	case '2':  pwmUn = 2;	 	break;
	case '3':  pwmUn = 3;		break;
	case '4':  pwmUn = 4;		break;
	case '5':  pwmUn = 5;		break;
	case '6':  pwmUn = 6;		break;
	case '7':  pwmUn = 7;		break;
	case '8':  pwmUn = 8;		break;
	case '9':  pwmUn = 9;		break;
	case 'q':  pwmDz = 10;	 	break;
	case 'w':  pwmDz = 20;	 	break;
	case 'e':  pwmDz = 30;	 	break;
	case 'r':  pwmDz = 40;	 	break;
	case 't':  pwmDz = 50;	 	break;
	case 'y':  pwmDz = 60;	 	break;
	case 'u':  pwmDz = 70;	 	break;
	case 'i':  pwmDz = 80;	 	break;
	case 'o':  pwmDz = 90;	 	break;
        case 'p':  pwmDz = 0;		break;
  	default:   pwmDz = pwmUn = 0;   break;

      }
      pwmSet(PWM_FREQ, ((int)(pwmDz+pwmUn)*PWM_FREQ)/100 );
    }
    if( send )
    {
      send = 0;
      UART_OutUDec( somaTempo >> POTN2  );
      UART_OutChar(' ');
      UART_OutUDec( rpsB  );
      UART_OutCRLF();
    }

  } // while(1)
} // void main(void)

