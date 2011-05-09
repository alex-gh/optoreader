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
   
	#define DEBUG
	#define length 100
	uint8_t i;
	unsigned char b;
	char str[length+1];
	uint8_t flag;
	uint8_t k;
	uint8_t j;
	char value[length];
	unsigned int test;	
	unsigned int test_dez;
	char s[20];
	uint8_t retval;
	uint8_t obis_A;
	uint8_t obis_B;
	uint8_t obis_C;
	uint8_t obis_D;
	uint8_t obis_E;
	uint8_t obis_F;

	for(;;){
		i=0;
		flag=0;
		k=0;
		test=0;
		test_dez=0;
	
		value[0]=0;
		str[0]=0;
		s[0]=0;
		// Read a complete line from UART1
		do{
			b=uart1_getc();    
			if (b!='\n' && (b>0x20)){
				str[i]=b;      
				i++;
			}       
		}
		while( i<length && (b!='\n'));
		str[i]=0; // add terminator to string
		
		retval=sscanf(str,"%hhu-%hhu:%hhu.%hhu.%hhu*%hhu(%s)",&obis_A, &obis_B, &obis_C, &obis_D, &obis_E, &obis_F, value);

		if(i>0){
			for(j=0;j<length;j++){
				if(value[j]==')'){
					value[j]=0;
				}
	
			}

			if( (obis_A==1) && (obis_B==0) && (obis_C==1) && (obis_D==8) ){
				uart_puts("Bezug:            ");
				sscanf(value,"%d.%d",&test,&test_dez);
				itoa(test,s,10);
				uart_puts(s);
				uart_putc('.');
				itoa(test_dez,s,10);
				uart_puts(s);
				uart_puts("\r\n");			
			}
			else if( (obis_A==1) && (obis_B==0) && (obis_C==2) && (obis_D==8) ){
				uart_puts("Einspeisung:      ");
				sscanf(value,"%d.%d",&test,&test_dez);
				itoa(test,s,10);
				uart_puts(s);
				uart_putc('.');
				itoa(test_dez,s,10);
				uart_puts(s);
				uart_puts("\r\n");
			}
			else if( (obis_A==1) && (obis_B==0) && (obis_C==41) && (obis_D==7) ){
				uart_puts("Momentanleistung: ");
				sscanf(value,"%d.%d",&test,&test_dez);
				itoa(test,s,10);
				uart_puts(s);
				uart_putc('.');
				itoa(test_dez,s,10);
				uart_puts(s);
				uart_puts("\r\n");
			}

			/*
			retval=sscanf(value,"%d.%d",&test, &test_dez);
			
			//uart_puts(value);
			
			if (value[0]=='1') uart_puts("\r\n");

			itoa(test,s,10);
			uart_putc('$');
			uart_puts(s);
			if (test_dez > 0){
				itoa(test_dez,s,10);
				uart_putc('.');
				uart_puts(s);
			}
			uart_putc('\n');
			uart_putc('\r');
			*/
			
			#ifdef DEBUG
			itoa(obis_A,s,10);
			uart_puts(s);
			uart_putc('-');
			itoa(obis_B,s,10);
			uart_puts(s);
			uart_putc(':');
			itoa(obis_C,s,10);
			uart_puts(s);
			uart_putc('.');
			itoa(obis_D,s,10);
			uart_puts(s);
			uart_putc('.');
			itoa(obis_E,s,10);
			uart_puts(s);
			uart_putc('*');
			itoa(obis_F,s,10);
			uart_puts(s);
			uart_puts("  ");
			uart_puts(value);
			uart_puts("\r\n");
			#endif
		}

		/*
		// Send recived line to UART0
		
		if(i>0){
			if ((s[0] == '1') && (s[2]=='0') && (s[4]=='1') && (s[6]=='8')){ //import active power
				s[25]=0;
				uart_puts(s+14);
				uart_putc('\n');
				uart_putc('\r');
			}
		}   */
	//uart_putc(uart1_getc()); //Send everything rec'd on UART1 to UART0
	} 
	return 0;
}

