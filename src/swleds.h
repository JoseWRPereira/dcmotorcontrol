#ifndef __SWLEDS_H__ 
#define __SWLEDS_H__

#include "io.h"

#define SW1         !(*((volatile unsigned long *)(PORTF_BASE|PORTbit4)))
#define SW2         !(*((volatile unsigned long *)(PORTF_BASE|PORTbit0)))

#define DARK	    0                  // dark     ---    0
#define RED	    0x02               // red      R--    0x02
#define BLUE	    0x04               // blue     --B    0x04
#define GREEN	    0x08               // green    -G-    0x08
#define YELLOW	    (RED|GREEN)        // yellow   RG-    0x0A
#define PINK	    (RED|BLUE)         // pink     R-B    0x06
#define SKYBLUE	    (GREEN|BLUE)       // sky blue -GB    0x0C
#define WHITE	    (RED|GREEN|BLUE)   // white    RGB    0x0E

#define SETLED( n ) (*((volatile unsigned long *)(PORTF_BASE|n<<2))) = n
#define CLRLED( n ) (*((volatile unsigned long *)(PORTF_BASE|n<<2))) = (~n)
#define CPLLED( n ) PORTF ^= n

#define NAQUISICOES	8
#define TAMFILA		NAQUISICOES

extern unsigned long fila[TAMFILA];
extern unsigned int indiceFila;
extern unsigned int rps;
extern unsigned long somaTempo;
extern unsigned char send;

void initSWLEDS( void );

#endif

