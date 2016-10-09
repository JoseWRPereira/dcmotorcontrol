#ifndef __BLUETOOTH_H__
#define __BLUETOOTH_H__

#include "io.h"
#include "uart2.h"

#define BT_ENABLE		(*((volatile unsigned long *)(PORTC_BASE|0x20<<2)))
#define BT4  			(*((volatile unsigned long *)(PORTC_BASE|0x10<<2)))
#define BT5  			(*((volatile unsigned long *)(PORTC_BASE|0x20<<2)))
#define BT6  			(*((volatile unsigned long *)(PORTC_BASE|0x40<<2)))
#define BT7  			(*((volatile unsigned long *)(PORTC_BASE|0x80<<2)))


void initBluetooth( void );
void Bluetooth_OutChar( unsigned char data );
void Bluetooth_OutString( char * ptr );
unsigned char Bluetooth_InCharAvailable( void );
unsigned char Bluetooth_InChar( void );
void Bluetooth_OutUDec( unsigned long n );

void Bluetooth_StartATcmd( void );
void Bluetooth_StopATcmd( void );
unsigned char Bluetooth_ATcmd( void );


#endif //__BLUETOOTH_H__

