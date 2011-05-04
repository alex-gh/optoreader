/*************************************************************************
Test Program sending data recieved on UART0 to UART1 and vice versa.
Using Peter Fleury's UART library

*************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
//#include <avr/signal.h>
#include <avr/pgmspace.h>
#include <stdio.h>
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
	uint8_t flag;
	uint8_t k;
	uint8_t j;
	char t[length+1];

	for(;;){
		i=0;
		flag=0;
		k=0;
		// Read a complete line from UART1
		do{
			b=uart1_getc();    
			if (b!='\n' && (b>0x20)){
				s[i]=b;      
				i++;
			}       
		}
		while( i<length && (b!='\n'));
		s[i]=0; // add terminator to string
		
		if(i>0){
			for(j=0;j<length;j++){
				if(s[j]=='('){
					flag = 1;
					continue;
				}
				if(s[j]==')'){
					flag = 0;
					continue;
				}
				if(flag == 1){
					t[k] = s[j];
					k++;
				}	
	
			}
			t[k]=0; // add terminator to string
			
			uart_puts(t);		
			uart_putc('\n');
			uart_putc('\r');
		}


		// Send recived line to UART0
		/*
		if(i>0){
			if ((s[0] == '1') && (s[2]=='0') && (s[4]=='1') && (s[6]=='8')){ //import active power
				s[25]=0;
				uart_puts(s+14);
				uart_putc('\n');
				uart_putc('\r');
			}
		}   */
		// uart_putc(uart1_getc()); //Send everything rec'd on UART1 to UART0
	} 
	return 0;
}

