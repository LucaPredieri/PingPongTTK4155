/*
 * GccApplication1.c
 *
 * Created: 31.08.2022 17:17:18
 * Author : lucapre
 */ 
/*

	ATmega 16 UART echo program
	http://www.electronicwings.com

*/ 
#include <stdint.h>
#include <stdio.h>
#include "Uart_driver.h"
#include <avr/io.h>
#include <avr/sleep.h>
#include <util/delay.h>




#ifndef F_CPU
#define F_CPU 4915200UL
#endif

void uart_init(unsigned int baud){
	int bd = 0.15*((long)F_CPU/((long)(16*baud)) -1);
	//set baud rate
	UBRR0H = (bd>>8);
	UBRR0L = bd;
	//Enable r/t
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
	//set frame format
	UCSR0C = (1<<USBS0) | (1<<URSEL0) | (3<<UCSZ00);
	fdevopen(uart_transmit,uart_receive);
}

int uart_transmit(char data){
	while(!(UCSR0A & (1<<UDRE0))){
		//While the flag is off
	}
	//send message
	UDR0 = data;
	return 0;
}


int uart_receive(){
	if(!(UCSR0A & (1<<RXC0))){
		return 0;
	}
	return UDR0;
}

	
int main(){
	uart_init(9600);
	while(1){
		char data = 'c';
		uart_transmit(data);
		int out = uart_receive();
		printf("%d\n" , out);
	}
	/*int out= 10;
	printf("%d\n" , out );
	
	while(1){
		PORTB |= (1 << PB0);
		_delay_ms(10);
		PORTB &= ~(1 << PB0);
		_delay_ms(10);
	}*/
}