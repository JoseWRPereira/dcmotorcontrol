#include "timer.h"
#include "swleds.h"




unsigned char t1flag = 0, t2flag = 0;
unsigned char t1seg = 0;

void IntT1A_Handler_off( void )
{
  TIMER1_ICR_R = TIMER_ICR_TATOCINT; 

  t1flag = 1;
  rps -= fila[indiceFila];
  fila[indiceFila] = contaPulsos;
  rps += contaPulsos;
  contaPulsos = 0;
  indiceFila = (indiceFila+1)%TAMFILA;
  
  if( ++naq >= NAQUISICOES )
  {  
    t1seg = 1; 
    naq = 0;
  }
  SETLED( RED ); 
}

void IntT1A_Handler( void )
{
  TIMER1_ICR_R = TIMER_ICR_TATOCINT; 

  CPLLED( RED ); 
}


unsigned long readT1A( void )
{
  return( TIMER1_TAV_R );
}
void resetT1A( void )
{
  TIMER1_TAV_R = 1;
}

void IntT1B_Handler( void )
{
  TIMER1_ICR_R = TIMER_ICR_TATOCINT; 
  t2flag = 1;
}

void initTimer( unsigned long time )
{
  SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R1;
  // 1: Ensure the timer is disable
  TIMER1_CTL_R &= ((~TIMER_CTL_TAEN) & (~TIMER_CTL_TBEN));
  // 2: Write the GPTMCFG = 0
  TIMER1_CFG_R = TIMER_CFG_32_BIT_TIMER;
  // 3: Cfg TnMR: OneShot/Periodic
  TIMER1_TAMR_R |= TIMER_TAMR_TAMR_PERIOD;
  TIMER1_TBMR_R |= TIMER_TBMR_TBMR_PERIOD;
  // 4: Optionally config.
  TIMER1_SYNC_R |= TIMER_SYNC_SYNCT1_TA;  
  // 5: Load the start value into the INTERVAL LOAD REG
  TIMER1_TAILR_R = time;
  // 6: If interrupt are required, mask interrupt
  TIMER1_IMR_R = TIMER_IMR_TATOIM | TIMER_IMR_TBTOIM;
  // 7: Enable timer and start counting
  TIMER1_CTL_R |= TIMER_CTL_TAEN;
  // 8: Aguarda interrupcao ou polling

  NVIC_PRI5_R	= (NVIC_PRI5_R & 0xFFFF00FF)|0x00004000;
  NVIC_EN0_R	= 0x00200000; // Timer1A
}



unsigned long wt3cont = 0;
unsigned char wt3flag = 0;

void initWideTimer( void )
{
  unsigned char tempo;  
  SYSCTL_RCGCGPIO_R = SYSCTL_RCGCGPIO_R3;// 1) activate clock for Port D
  tempo = SYSCTL_RCGCGPIO_R;
  GPIO_PORTD_LOCK_R = GPIO_LOCK_KEY;   	// 2) unlock GPIO Port D
  GPIO_PORTD_CR_R = 0x04;           	// allow changes to PD2
  GPIO_PORTD_DIR_R &= ~0x04;          	// 3) PD2 In
  GPIO_PORTD_AFSEL_R = 0x04;        	// 4) Enable alt funct on PD2
  GPIO_PORTD_PCTL_R = 0x00000700;   	// 5) PCTL GPIO on PD2 WT3CCP0
  GPIO_PORTD_AMSEL_R = 0x00;        	// 3) disable analog on PD
  GPIO_PORTD_PDR_R = 0x04;          	// enable pull-down on PD3
  GPIO_PORTD_DEN_R = 0x04;		// 7) enable digital I/O on PD3

  SYSCTL_RCGCWTIMER_R |= 0x00000008;	// Enable and provide a clock to WT3


	// 1: Ensure the timer is disable
  WTIMER3_CTL_R &= ~(TIMER_CTL_TBEN | TIMER_CTL_TAEN);
	// 2: Write the GPTMCFG = 0x04
  WTIMER3_CFG_R = TIMER_CFG_16_BIT; 		// 32/64 -> 32 timer cfg
  	// 3:
  WTIMER3_TAMR_R |= TIMER_TAMR_TAMR_CAP; 	// Capture Mode
  WTIMER3_TAMR_R |= TIMER_TAMR_TACMR;		// Edge Time Mode
  WTIMER3_TAMR_R |= TIMER_TAMR_TACDIR;		// The Timer Counts Up
//  WTIMER3_TBMR_R |= TIMER_TBMR_TBMR_CAP; 	// Capture Mode
//  WTIMER3_TBMR_R |= TIMER_TBMR_TBCMR;	// Edge Time Mode
//  WTIMER3_TBMR_R |= TIMER_TBMR_TBCDIR;	// The Timer Counts Up
  	// 4: 
  WTIMER3_CTL_R |= TIMER_CTL_TAEVENT_POS;	// Positive Edge TA
  //WTIMER3_CTL_R |= TIMER_CTL_TBEVENT_POS;	// Positive Edge TB
	// 5:
  WTIMER3_TAPR_R = 2;
  WTIMER3_TBPR_R = 0;
	// 6:
  WTIMER3_TAILR_R = 0;
  WTIMER3_TBILR_R = 0;
	// 7:
  WTIMER3_IMR_R |= TIMER_IMR_CAEIM;
//  WTIMER3_IMR_R |= TIMER_IMR_CBEIM | TIMER_IMR_CAEIM;
	// 8: 
  NVIC_PRI25_R	= (NVIC_PRI25_R & 0xFFFFFF00)|0x00000020;
  NVIC_EN3_R	= 0x00000010; // Wide Timer 3A

  WTIMER3_CTL_R |= TIMER_CTL_TAEN;
//  WTIMER3_CTL_R |= (TIMER_CTL_TBEN | TIMER_CTL_TAEN);


}

void IntWT3A_Handler( void )
{
UART_OutChar('*');
  WTIMER3_ICR_R |= TIMER_ICR_CAECINT;

UART_OutChar('&');
  CLRLED( GREEN ); 
UART_OutChar('0');  
  wt3cont = WTIMER3_TAR_R;
  wt3flag = 1; 
UART_OutUDec(wt3cont);
UART_OutChar('<');
}


