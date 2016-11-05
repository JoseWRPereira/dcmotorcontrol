#include "pwm.h"
#include "swleds.h"

#define SYSTEM_CLOCK 80000000


void pwmSet( unsigned long freq, unsigned long dutyc )
{
  if( dutyc >= freq )
    dutyc = freq-1;
 	// 6: Cfg the PWM generate for countdown mode with immediate updates
  PWM0_0_CTL_R &= ~PWM_0_CTL_ENABLE;
	// 7: Set the Period
  PWM0_0_COUNT_R = 0;
  PWM0_0_LOAD_R = (SYSTEM_CLOCK / (freq<<3)); // 16 bits
	// 8: Set the Pulse
  PWM0_0_CMPA_R = ((SYSTEM_CLOCK/(freq<<3))*dutyc)/freq;
        // 9: Set the Pulse 
  PWM0_0_CMPB_R = PWM0_0_LOAD_R - PWM0_0_CMPA_R;
  
  PWM0_0_CTL_R |= PWM_0_CTL_ENABLE; 
}

void pwmStart( void )
{
  PWM0_0_CTL_R |= PWM_0_CTL_ENABLE; 
}

void pwmStop( void )
{
  PWM0_0_CTL_R &= ~PWM_0_CTL_ENABLE;
}




void IntPWM0_Handler( void )
{
  if(PWM0_ISC_R & PWM_ISC_INTPWM0 )
 {    
    PWM0_0_ISC_R = PWM_ISC_INTPWM0; 
  }
}

void initPWM( unsigned long freq, unsigned long dutyc )
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
//  SYSCTL_RCC_R &= ~(SYSCTL_RCC_USEPWMDIV | SYSCTL_RCC_PWMDIV_M);
  SYSCTL_RCC_R  = (SYSCTL_RCC_R & ~SYSCTL_RCC_PWMDIV_M) | 
		    (SYSCTL_RCC_USEPWMDIV | SYSCTL_RCC_PWMDIV_8);
  PWM0_CTL_R    |= PWM_CTL_GLOBALSYNC0;
  PWM0_ENABLE_R |= PWM_ENABLE_PWM0EN;
  PWM0_ENUPD_R  |= PWM_ENUPD_ENUPD0_M;

 	// 6: Cfg the PWM generate for countdown mode with immediate updates
  PWM0_0_CTL_R &= ~PWM_0_CTL_ENABLE;
  PWM0_0_GENA_R |= PWM_0_GENA_ACTCMPAD_ZERO;
  PWM0_0_GENA_R |= PWM_0_GENA_ACTLOAD_ONE;
  PWM0_0_GENB_R |= PWM_0_GENB_ACTCMPBD_ZERO;
  PWM0_0_GENB_R |= PWM_0_GENB_ACTLOAD_ONE;

  pwmSet( freq, dutyc ); // PWM_freq = 1kHz DutyCicle=10%

	//11: Enable PWM output
  PWM0_PP_R = (PWM_PP_GCNT_M&0x01);
  PWM0_ENABLE_R = (PWM_ENABLE_PWM0EN | PWM_ENABLE_PWM1EN);

  PWM0_INTEN_R	|= PWM_INTEN_INTPWM0;
  PWM0_RIS_R	|= PWM_RIS_INTPWM0;

  NVIC_PRI2_R	= (NVIC_PRI2_R & 0xFF00FFFF)|0x00800000;
  NVIC_EN0_R	= 0x00000400; // PWM0 Generator 0

}	


