/*
 * serial.c
 */ 

#include "serialprint.h"
#include <avr/io.h>


void serial_start(int prescaler) {
	UBRR0H = (unsigned char) (prescaler >> 8);
	UBRR0L = (unsigned char) prescaler;
	
	UCSR0C |= (1 << UCSZ00);
	UCSR0C |= (1 << USBS0);		
	UCSR0C |= (1 << UCSZ01);	
	
	UCSR0B |= (1 << RXEN0);		
	UCSR0B |= (1 << TXEN0);		
}

void serial_push(unsigned char data) {
	while(!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
}

void print(char* pr) {
	while (*pr != 0x00) {
		serial_push(*pr);
		pr++;
	}
}


