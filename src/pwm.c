#include "pwm.h"
#include "swleds.h"


#define SYSTEM_CLOCK 80000000
#define PWM_FREQ        10000
#define DUTY_CYCLE_A	30
#define DUTY_CYCLE_B	(100-DUTY_CYCLE_A)

void IntPWM0Handler( void )
{
  if(PWM0_ISC_R & 0x00000001 )
  {
    PWM0_0_LOAD_R = ( (SYSTEM_CLOCK / (PWM_FREQ<<1)) - 1 );
    PWM0_ISC_R = 0x00000001;
  }

}


void initPWM( void )
{
	// 1: Enable de PWM clock
  SYSCTL_RCGC0_R |= SYSCTL_RCGC0_PWM0;
  SYSCTL_RCGCPWM_R |= SYSCTL_RCGCPWM_R0;
 
	// 2: Enable clk to the appropriate GPIO module (PB6 e PB7)
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;

	// 3: Enable de appropriate pins for their alternate function
  GPIO_PORTB_AFSEL_R |=  0x000000C0; // PB7 e PB6 
  GPIO_PORTB_ODR_R   &= ~0x000000C0;
  GPIO_PORTB_DEN_R   |=  0x000000C0;
  GPIO_PORTB_DIR_R   |=  0x000000C0;
  GPIO_PORTB_DR8R_R  |=  0x000000C0;
  GPIO_PORTB_CR_R    |=  0x000000C0;
  GPIO_PORTB_AMSEL_R &= ~0x000000C0;

	// 4: Configure the PMCn fields
  GPIO_PORTB_PCTL_R = ((GPIO_PORTB_PCTL_R&0x00FFFFFF)|(GPIO_PCTL_PB7_M0PWM1 | GPIO_PCTL_PB6_M0PWM0));

	// 5: Configure the RUN-MODE Clock Configuration
  SYSCTL_RCC_R |= (SYSCTL_RCC_USEPWMDIV | SYSCTL_RCC_PWMDIV_2);
  PWM0_CTL_R    |= PWM_CTL_GLOBALSYNC0;
  PWM0_ENABLE_R |= PWM_ENABLE_PWM0EN;
  PWM0_ENUPD_R  |= PWM_ENUPD_ENUPD0_M;

 	// 6: Cfg the PWM generate for countdown mode with immediate updates
  PWM0_0_CTL_R &= ~PWM_0_CTL_ENABLE;
  PWM0_0_GENA_R |= PWM_0_GENA_ACTCMPAD_ZERO;
  PWM0_0_GENA_R |= PWM_0_GENA_ACTLOAD_ONE;
  PWM0_0_GENB_R |= PWM_0_GENB_ACTCMPBD_ZERO;
  PWM0_0_GENB_R |= PWM_0_GENB_ACTLOAD_ONE;
	// 7: Set the Period
  PWM0_0_COUNT_R = 0;
  PWM0_0_LOAD_R = ( (SYSTEM_CLOCK / (PWM_FREQ<<1)) - 1 );
	// 8: Set the Pulse Width 25%
  PWM0_0_CMPA_R = (((SYSTEM_CLOCK/(PWM_FREQ*2*100))*DUTY_CYCLE_A)-1);
        // 9: Set the Pulse Width 75%
  PWM0_0_CMPB_R = (((SYSTEM_CLOCK/(PWM_FREQ*2*100))*DUTY_CYCLE_B)-1);
	//10: Start de timers in PWM generator 0
  PWM0_0_CTL_R |= PWM_0_CTL_ENABLE;
	//11: Enable PWM output
  PWM0_PP_R = (PWM_PP_GCNT_M&0x01);
  PWM0_ENABLE_R = (PWM_ENABLE_PWM0EN | PWM_ENABLE_PWM1EN);

}	

/*

unsigned long H,L;
void initPWM_off(void)

{
  SYSCTL_RCGC2_R |= 0x00000001; // activate clock for port A
  H = 40000;
  L = 40000;                // 50%
  GPIO_PORTA_AMSEL_R &= ~0x20;      // disable analog functionality on PA5
  GPIO_PORTA_PCTL_R &= ~0x00F00000; // configure PA5 as GPIO
  GPIO_PORTA_DIR_R |= 0x20;     // make PA5 out
  GPIO_PORTA_DR8R_R |= 0x20;    // enable 8 mA drive on PA5
  GPIO_PORTA_AFSEL_R &= ~0x20;  // disable alt funct on PA5
  GPIO_PORTA_DEN_R |= 0x20;     // enable digital I/O on PA5
  GPIO_PORTA_DATA_R &= ~0x20;   // make PA5 low
  NVIC_ST_CTRL_R = 0;           // disable SysTick during setup
  NVIC_ST_RELOAD_R = L-1;       // reload value for 500us
  NVIC_ST_CURRENT_R = 0;        // any write to current clears it
  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x40000000; // priority 2
  NVIC_ST_CTRL_R = 0x00000007;  // enable with core clock and interrupts
}
void SysTick_Handler(void)
{
  if(GPIO_PORTA_DATA_R&0x20)
  {   // toggle PA5
    GPIO_PORTA_DATA_R &= ~0x20; // make PA5 low
    NVIC_ST_RELOAD_R = L-1;     // reload value for low phase
  } 
  else
  {
    GPIO_PORTA_DATA_R |= 0x20;  // make PA5 high
    NVIC_ST_RELOAD_R = H-1;     // reload value for high phase
  }
}

void dutycycle( unsigned char porcento )
{
  if( porcento > 100 ) 
    porcento = 100;
  H = 800 * porcento;
  L = 80000 - H;
  if( !H )
    H = 1;
  if( !L )
    L = 1;
}

*/
