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
	uint8_t j;
	char val[length];
	char val_dez[20];
	unsigned int value;	
	unsigned int value_dez;
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
		sscanf(str,"%hhu-%hhu:%hhu.%hhu.%hhu*%hhu(%s)",&obis_A, &obis_B, &obis_C, &obis_D, &obis_E, &obis_F, val); 

		if(i>0){
			for(j=0;j<length;j++){
				if(val[j]==')'){
					val[j]=0; // remove ')' behind data
				}
	
			}

			//cumulative meter count of import active power (OBIS 1-0:1.8.0*255)
			if( (obis_A==1) && (obis_B==0) && (obis_C==1) && (obis_D==8) ){ 
				uart_puts("Bezug:            ");
				retval=sscanf(val,"%d.%s", &value, val_dez);
				val_dez[4]=0;
				sscanf(val_dez, "%d", &value_dez);
				itoa(value,s,10);
				uart_puts(s);
				uart_putc('.');
				itoa(value_dez,s,10);
				uart_puts(s);
				uart_puts("\r\n");			
			}

			//cumulative meter count of export active power (OBIS 1-0:2.8*255)
			else if( (obis_A==1) && (obis_B==0) && (obis_C==2) && (obis_D==8) ){ 
				uart_puts("Einspeisung:      ");
				sscanf(val,"%d.%s", &value, val_dez);
				val_dez[4]=0;
				sscanf(val_dez, "%d", &value_dez);
				itoa(value,s,10);
				uart_puts(s);
				uart_putc('.');
				itoa(value_dez,s,10);
				uart_puts(s);
				uart_puts("\r\n");
			}

			//current consumption of active power on L2 (OBIS 1-0:41.7.0*255)
			else if( (obis_A==1) && (obis_B==0) && (obis_C==41) && (obis_D==7) ){ 
				uart_puts("Momentanleistung: ");
				retval=sscanf(val,"%d.%d", &value, &value_dez);

				if(retval==1){
					itoa(value,s,10);
					uart_puts(s);
					uart_putc('.');
					itoa(value_dez,s,10);
					uart_puts(s);
					uart_puts("\r\n");
				}
				else if(retval==2){
					val_dez[4]=0;
					sscanf(val_dez, "%d", &value_dez);
					itoa(value,s,10);
					uart_puts(s);
					uart_putc('.');
					itoa(value_dez,s,10);
					uart_puts(s);
					uart_puts("\r\n");
				}
			}

			
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
			uart_puts(val);
			uart_puts("\r\n");
			#endif
		}

	//uart_putc(uart1_getc()); //Send everything rec'd on UART1 to UART0
	} 
	return 0;
}

