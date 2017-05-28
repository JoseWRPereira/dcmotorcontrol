#include "controlador.h"
#include "swleds.h"

#define RPS_MAX		900 //90.0 rps
#define VALORCONTROLE	10  // Valor de Controle

long Cout;



long controlador_LigaDesliga( long setpoint, long sensor )
{
  if( sensor > setpoint )
    return( 0 );
  else
    return( 100 );
}


float p = 0.4;
float i = 0.0002;
float d = 0.0;
float eT,iT,dT, rT,yT,uT;
long pwmAlvo;

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






float raiz2( float num )
{
  char i;
  float recorre = num;
  for( i=0; i<5; ++i )
    recorre = recorre/2.0 + num/(2.0*recorre);
  return( recorre );
}


float Gc, Gct;

float LPA2v( float u0, float u1 )
{
  float l0, l1;
  l0 = 1.0 - u0;
  l1 = 1.0 - u1;

  Gc  = u0 - l1;
  Gct = (u0 + l1) - 1.0;
}

float LPA2v0( float mi, float lambda )
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
  float aux;

  rT = (float) setpoint / (float) max;
  yT = (float) sensor   / (float) max;

  if( rT > 1.0 )
    rT = 1.0;
  if( yT > 1.0 )
    yT = 1.0;
  LPA2v( rT, yT );
  
  aux = (rT * 100.0);

  Cout = (long)(aux + aux * Gct * 10);
  if( Cout > 99 )
    Cout = 99;
  if( Cout < 0 ) 
    Cout = 0;
  return( Cout );
}

long controlador0( long setpoint, long max, long sensor )
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

