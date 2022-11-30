#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <util/delay.h>

// Functions to handle printf.

int uart_receive(){
	if(!(UCSR0A & (1<<RXC0))){
		return 0;
	}
	return UDR0;
}

int usart_putchar_printf(char var, FILE *stream) {
	// translate \n to \r for br@y++ terminal
	if (var == '\n') usart_putchar('\r');
	usart_putchar(var);
	return 0;
}

static FILE mystdout = FDEV_SETUP_STREAM(usart_putchar_printf, NULL, _FDEV_SETUP_WRITE);

void usart_putchar(char data) {
	// Wait for empty transmit buffer
	while(!(UCSR0A & (1<<UDRE0))){
		//While the flag is off
	}
	//send message
	UDR0 = data;
}

void uart_init(unsigned int baud){
	int bd = 31;
	//set baud rate
	UBRR0H = (bd>>8);
	UBRR0L = bd;
	//Enable r/t
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
	//set frame format
	UCSR0C = (1<<USBS0) | (1<<URSEL0) | (3<<UCSZ00);
	fdevopen(usart_putchar_printf,uart_receive);
}

