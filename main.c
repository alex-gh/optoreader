/*************************************************************************
Test Program reciving ASCII-formatted data from smart meters on UART1, 
parsing of data and transmitting via UART0.

Tested with Hager EHZ and EasyMeter Q3DA1004, each connected to L2.
*************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include "uart.h"


/* define CPU frequency in Mhz here if not defined in Makefile */
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

/* 9600 baud */
#define UART_BAUD_RATE      9600      

void print_value(unsigned int value, unsigned long value_dez); 

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
	
  	
	
	//#define DEBUG
	#define length 100
	uint8_t i;
	unsigned char b;
	char str[length+1];
	char val[length];
	unsigned int value;	
	unsigned long value_dez;
	char s[20];
	uint8_t obis_A;
	uint8_t obis_B;
	uint8_t obis_C;
	uint8_t obis_D;
	uint8_t obis_E;
	uint8_t obis_F;
	uint8_t retval;
	for(;;){
		i=0;
		value=0;
		value_dez=0;
		retval=0;
		val[0]=0;
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
		
		//split line into OBIS numbers and data
		retval=sscanf(str,"%hhu-%hhu:%hhu.%hhu.%hhu*%hhu(%d.%lu%*s)",&obis_A, &obis_B, &obis_C, &obis_D, &obis_E, &obis_F, &value, &value_dez); 
		
		if(i>0){
			//cumulative meter count of import active power (OBIS 1-0:1.8.0*255)
			if( (obis_A==1) && (obis_B==0) && (obis_C==1) && (obis_D==8) ){ 
				uart_puts("Bezugzählerstand:      ");

				if(retval>0) print_value(value, value_dez);
			}
			
			//cumulative meter count of export active power (OBIS 1-0:2.8*255)
			else if( (obis_A==1) && (obis_B==0) && (obis_C==2) && (obis_D==8) ){ 
				uart_puts("Produktionzählerstand: ");

				if(retval>0) print_value(value, value_dez);
			} 

			//current consumption of active power on L1 (OBIS 1-0:21.7.0*255)
			else if( (obis_A==1) && (obis_B==0) && (obis_C==21) && (obis_D==7) ){ 
				uart_puts("Momentanleistung L1:   ");

				if(retval>0) print_value(value, value_dez);
			}

			//current consumption of active power on L2 (OBIS 1-0:41.7.0*255)
			else if( (obis_A==1) && (obis_B==0) && (obis_C==41) && (obis_D==7) ){ 
				uart_puts("Momentanleistung L2:   ");

				if(retval>0) print_value(value, value_dez);
			}

			//current consumption of active power on L3 (OBIS 1-0:61.7.0*255)
			else if( (obis_A==1) && (obis_B==0) && (obis_C==61) && (obis_D==7) ){ 
				uart_puts("Momentanleistung L3:   ");

				if(retval>0) print_value(value, value_dez);
			}
		} 

		//uart_putc(uart1_getc()); //Send everything rec'd on UART1 to UART0
	} 
	return 0;

}

void print_value(unsigned int value, unsigned long value_dez){
	char s[20];
	sprintf(s,"%d",value);
	uart_puts(s);
	uart_putc('.');
	sprintf(s,"%lu",value_dez);
	uart_puts(s);
	uart_puts("\r\n");
}

