#include "controlador.h"
#include "swleds.h"

#define RPS_MAX		900 //90.0 rps
#define VALORCONTROLE	10  // Valor de Controle

long Cout;

unsigned long Gcr = 100;
unsigned long rpsAnt=0;
long D, GcAnt=-100;
long pwm1;


long pwm, pwmAlvo, erro;


float p = 0.4;
float i = 0.0002;
float d = 0.0;
float eT,iT,dT;



float raiz2( float num )
{
  char i;
  float recorre = num;
  for( i=0; i<5; ++i )
    recorre = recorre/2.0 + num/(2.0*recorre);
  return( recorre );
}





float Gc, Gct;

float LPA2v( float mi, float lambda )
{
  if(     mi > 1.0 )     mi = 1.0;
  if(     mi < 0.0 )     mi = 0.0;
  if( lambda > 1.0 ) lambda = 1.0;
  if( lambda < 0.0 ) lambda = 0.0;

  Gc  =  mi-lambda;
  Gct = (mi+lambda)-1.0;
  return( Gct ); 
}


char umaVez = 0;
float rT, uT, yT;
long pwmA = 0, pwmB = 1000, pwmC = 0, pwmD = 0;
long yTant;
char limiar0 = 1, limiar1 = 0;
char estado = 0;
long controlador( long setpoint, long max, long sensor )
{
  rT = (float) setpoint / (float) max;
  yT = (float) sensor   / (float) max;


  switch( estado )
  {
    case 0: 
            pwmA = (long)(rT * 141.42);
    	    pwmB = (long)(rT *  70.71);
            pwmC = 99;
            estado = 1;
            break;
    case 1: 
            if( sensor >= pwmB*10 )
            {
              estado = 2;
            }
	    break;
    case 2: 
            pwmA = (long)(rT * 141.42);
            pwmB = (long)(rT *  70.71);
	    pwmC = pwmA;
            estado = 3;
            break;
    case 3: 
	    if( sensor >= setpoint )
            {
              pwmC = ((pwmA + pwmB)/2);
              estado = 4;
            }
    case 4:
	   break; 

    default:
           estado = 0;
	   break;
  }

  LPA2v( yT/(2.0*rT), pwmC/100.0 );
  uT = pwmC;

  if( uT > 99.0 )
    uT = 99.0;

  yTant = yT;
  return( Cout = (long) uT );
}


































float LPA2v1( float mi, float lambda )
{
  float Gc, Gct;
  if(     mi > 1.0 )     mi = 1.0;
  if(     mi < 0.0 )     mi = 0.0;
  if( lambda > 1.0 ) lambda = 1.0;
  if( lambda < 0.0 ) lambda = 0.0;

  Gc  =  mi-lambda;
  Gct = (mi+lambda)-1.0;

  if( mi > 0.6 )
    return( 0.90 );
  else if( mi > 0.55 )
    return( 0.99 );
  else if( mi > 0.51 )
    return( 0.999 );
  else if( mi < 0.01 )
    return( 1.10 );
  else if( mi < 0.2 )
    return( 1.02 );
  else if( mi < 0.4 )
    return( 1.01 );
  else if( mi < 0.48 )
    return( 1.001 );
  else 
    return( 1.0 ); 

//  if( Gc > 0.5 ) 
//    return( 0.50 );
//  else if( Gc < -0.5 )
//    return( 1.10 );
//  else if( Gct > 0.5 )
//    return( 0.50 );
//  else if( Gct < -0.5 )
//    return( 1.10 );
//  else 
//    return( 1.0 - (Gc/3.0) );

//  else if( mi < 0.5 )
//    return( 1.01 );
//  else if( mi > 0.5 )
//    return( 0.99 );
//  else 
//    return( 1.0 );
}

//char umaVez = 0;

long controlador1( long setpoint, long max, long sensor )
{
  rT = (float) setpoint / (float) max;
  yT = (float) sensor / (float) max;
 
  if( !umaVez )
  {
    umaVez = 1;
    uT = 100.0 - (rT*100.0);
  }
 
  if( uT < 1.0 )
    uT = 99.0;

  uT = (LPA2v(yT/(2*rT), uT/100.0 ) * uT );

  if( uT > 99.0 )
    uT = 99.0;

  return( Cout = (long) uT );
}








long controladorPID( long setpoint, long max, long sensor )
{
  rT = (float)setpoint;
  yT = (float)sensor;
  pwmAlvo = ((setpoint*100)/max);

  dT = (eT-(rT-yT)) * d;
  eT = rT - yT;
  iT += eT * i;
  uT = iT + dT + p*eT;

  Cout = pwmAlvo + uT;

  if( Cout < 0 )
    Cout = 0;
  else if( Cout >= 100 )
    Cout = 99;

  return( Cout );
}





long controlador_LigaDesliga( long setpoint, long sensor )
{
  if( sensor > setpoint )
    return( 0 );
  else
    return( 100 );
}

long controlador_P( long setpoint, long max, long sensor )
{
  rT = setpoint;
  yT = sensor;
  pwmAlvo = ((setpoint*100)/max);

  eT = rT - yT;
  uT = p*eT;

  Cout = pwmAlvo + uT;

  if( Cout < 0 )
    Cout = 0;
  else if( Cout >= 100 )
    Cout = 99;

  return( Cout );
}




long controlador_correto( long setpoint, long max, long sensor )
{
  rT = setpoint;
  yT = sensor;
  pwmAlvo = ((setpoint*100)/max);

  dT = (eT-(rT-yT)) * d;
  eT = rT - yT;
  iT += eT * i;
  uT = iT + dT + p*eT;

  Cout = pwmAlvo + uT;

  if( Cout < 0 )
    Cout = 0;
  else if( Cout >= 100 )
    Cout = 99;

  return( Cout );
}



long controlador_off( long setpoint, long max, long sensor )
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


float LPA2v0( float mi, float lambda )
{
  float Gc, Gct, s;  
  float c1  = -0.50;
  float c2  =  0.00;
  float c3  =  0.33;
  float c4  =  0.67;

  if(     mi > 1.0 )     mi = 1.0;
  if(     mi < 0.0 )     mi = 0.0;
  if( lambda > 1.0 ) lambda = 1.0;
  if( lambda < 0.0 ) lambda = 0.0;

  Gc  =  mi-lambda;
  Gct = (mi+lambda)-1.0;

  if( Gc  < c1 ) 	s = 1.0;
  else if( Gc < c2 )    s = 0.75;
  else if( Gc < c3 )	s = 0.50;
  else if( Gc < c4 )	s = 0.25;
  else 			s = 0.00;	
  
  return( s );
}





