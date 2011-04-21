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
#define F_CPU 4000000UL
#endif

/* 9600 baud */
#define UART_BAUD_RATE      9600      


int main(void){
	unsigned int c;
	unsigned int c1;
	char buffer[7];
	int  num=134;

    
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
    
    
	for(;;){
		/*
		* Get received character from ringbuffer
		* uart_getc() returns in the lower byte the received character and 
		* in the higher byte (bitmask) the last receive error
		* UART_NO_DATA is returned when no data is available.
		*
		*/
		c = uart_getc();
		if ( c & UART_NO_DATA ){
			/* 
			* no data available from UART
			*/
			// uart_puts_P("No Data!\n\r");
		}
		else{
			/*
			* new data available from UART
			* check for Frame or Overrun error
			*/
			if ( c & UART_FRAME_ERROR ){
				/* Framing Error detected, i.e no stop bit detected */
                		uart_puts_P("UART Frame Error: ");
			}
			if ( c & UART_OVERRUN_ERROR ){
				/* 
				* Overrun, a character already present in the UART UDR register was 
				* not read by the interrupt handler before the next character arrived,
				* one or more received characters have been dropped
				*/
				uart_puts_P("UART Overrun Error: ");
			}
			if ( c & UART_BUFFER_OVERFLOW ){
				/* 
				* We are not reading the receive buffer fast enough,
				* one or more received character have been dropped 
				*/
				uart_puts_P("Buffer overflow error: ");
			}
			/* 
			* send received character back
			*/
			uart1_putc( (unsigned char)c );
		}



		c1 = uart1_getc();
		 if ( c1 & UART_NO_DATA ){
			/* 

			* no data available from UART

			*/
			// uart_puts_P("No Data!\n\r");
		}
		else{
			/*

			* new data available from UART

			* check for Frame or Overrun error

			*/
			if ( c1 & UART_FRAME_ERROR ){
				/* Framing Error detected, i.e no stop bit detected */
                		uart1_puts_P("UART Frame Error: ");
			}
			if ( c1 & UART_OVERRUN_ERROR ){
				/* 

				* Overrun, a character already present in the UART UDR register was 

				* not read by the interrupt handler before the next character arrived,

				* one or more received characters have been dropped

				*/
				uart1_puts_P("UART Overrun Error: ");
			}
			if ( c1 & UART_BUFFER_OVERFLOW ){
				/* 

				* We are not reading the receive buffer fast enough,

				* one or more received character have been dropped 

				*/
				uart1_puts_P("Buffer overflow error: ");
			}
			/* 

			* send received character back

			*/
		uart_putc( (unsigned char)c1 );
		}
	}
}
