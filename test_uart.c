/*************************************************************************
Test Program sending data recieved on UART0 to UART1 and vice versa.
Using Peter Fleury's UART library

*************************************************************************/
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include <avr/pgmspace.h>

#include "uart.h"


/* define CPU frequency in Mhz here if not defined in Makefile */
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

/* 9600 baud */
#define UART_BAUD_RATE      9600      


int main(void){
    
	/*
	*  Initialize UART library, pass baudrate and AVR cpu clock
	*  with the macro 
	*  UART_BAUD_SELECT() (normal speed mode )
	*  or 
	*  UART_BAUD_SELECT_DOUBLE_SPEED() ( double speed mode)
	*/
	uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU) ); 
	uart1_init(UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU)); 
	
	/*
	* now enable interrupt, since UART library is interrupt controlled
	*/
	sei();
    
    
	#define length 100
	uint8_t i;
	unsigned char b;
	unsigned char s[length+1];
	unsigned char buffer[10];

	

	for(;;){
		i=0;
		do{
			b=uart1_getc();    
			if (b!='\n' && (b>0x20)){
				s[i]=b;      
				i++;
			}       
		}
		while( i<length && (b!='\n'));

		
		

		s[i]=0;
		if(i>0){
			uart_puts(s);
			uart_putc('\n');
			uart_putc('\r');		
		}   
		// uart_putc(uart1_getc());
	} 
	return 0;
}

