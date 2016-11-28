#ifndef __PARACONTROL_H__
#define __PARACONTROL_H__

#include "tm4c123gh6pm.h"

long controlador( long setpoint, long max, long sensor );

long lpa2v( long l1, long l2 );


#endif

