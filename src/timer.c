#include "timer.h"
#include "swleds.h"

unsigned char t1flag = 0, t2flag = 0;

void IntT1A_Handler( void )
{
  TIMER1_ICR_R = TIMER_ICR_TATOCINT; 

  t1flag = 1;
  SETLED( RED ); 
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

