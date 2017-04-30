#ifndef __CONTROLADOR_H__
#define __CONTROLADOR_H__

#include "tm4c123gh6pm.h"

extern long Cout;

long controlador( long setpoint, long max, long sensor );

float lpa2v( long mi, long lambda );


#endif

