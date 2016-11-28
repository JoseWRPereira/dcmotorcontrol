#include "paracontrol.h"
#include "swleds.h"

#define RPS_MAX		900 //90.0 rps
#define VALORCONTROLE	10  // Valor de Controle


unsigned long Gcr = 100;
unsigned long rpsAnt=0;
long Gct, Gc, D, GcAnt=-100;
long pwm1;

long pwm, pwmAlvo, erro;

long controlador( long setpoint, long max, long sensor )
{
  if( sensor <= 1 )
    pwm = 100;

  pwmAlvo = (setpoint*100) / max ;

  erro = (((setpoint-sensor)*100)/setpoint);
  pwm = pwmAlvo + erro; 
 
  if( pwm > 100 )
    pwm = 100;
  if( pwm < 0 )
    pwm = 0;
  return( pwm );
}








long lpa2v_off2 ( long mi, long lambda )
{
  long aux;
  
  Gct = (mi+lambda)-100;
  Gc  =  mi-lambda;
  if( !mi )
  {
    pwm = 100; 
    GcAnt = -100;
  }
  //pwm = ((100-Gc)>>1) * ((Gc - GcAnt)*71)/200
  //pwm -= (pwm * (((Gc - GcAnt)*200)/200))/100 ;
  //pwm -= pwm * ((100-Gc)>>1);
  GcAnt = Gc;
  return( pwm );
}



long lpa2v_off( long mi, long lambda )
{
  unsigned long aux;
  if( !habilita )
    Gcr = 100;
  if( mi < lambda )
  {
    aux = ((lambda-mi)*100)/lambda;
    if( aux < 20 ) 
      Gcr -= aux;
    else
      Gcr -= ((mi-rpsAnt)*100)/lambda;
  }
  if( lambda < mi )
  {
    aux = ((mi-lambda)*100)/lambda;
    if( aux < 20 )
      Gcr += aux;
    else  
      Gcr += ((mi - rpsAnt)*100)/lambda;
  }
  rpsAnt = mi;

  return( Gcr );
}

