#include <stdlib.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "Uart_driver.h"
#include <avr/io.h>
#include <avr/sleep.h>

// this function is called by printf as a stream handler
int usart_putchar_printf(char var, FILE *stream) {
    // translate \n to \r for br@y++ terminal
    if (var == '\n') usart_putchar('\r');
    usart_putchar(var);
    return 0;
}

void usart_putchar(char data) {
    // Wait for empty transmit buffer
    while(!(UCSR0A & (1<<UDRE0))){
			//While the flag is off
		}
		//send message
		UDR0 = data;
}

static FILE mystdout = FDEV_SETUP_STREAM(usart_putchar_printf, NULL, _FDEV_SETUP_WRITE);

void SRAM_test(void)
{
	volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM
    uint16_t ext_ram_size = 0x800;
    uint16_t write_errors = 0;
    uint16_t retrieval_errors = 0;
    printf("Starting SRAM test...\n");
    // rand() stores some internal state, so calling this function in a loop will
    // yield different seeds each time (unless srand() is called before this function)
    uint16_t seed = rand();
    // Write phase: Immediately check that the correct value was stored
    srand(seed);
    for (uint16_t i = 0; i < ext_ram_size; i++) {
        uint8_t some_value = rand();
        ext_ram[i] = some_value;
        uint8_t retreived_value = ext_ram[i];
        if (retreived_value != some_value) {
            printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retreived_value, some_value);
            write_errors++;
        }
    }
    // Retrieval phase: Check that no values were changed during or after the write phase
    srand(seed);
    // reset the PRNG to the state it had before the write phase
    for (uint16_t i = 0; i < ext_ram_size; i++) {
        uint8_t some_value = rand();
        uint8_t retreived_value = ext_ram[i];
        if (retreived_value != some_value) {
            printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retreived_value, some_value);
            retrieval_errors++;
        }
    }
    printf("SRAM test completed with \n%4d errors in write phase and \n%4d errors in retrieval phase\n\n", write_errors, retrieval_errors);
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

/*
int uart_transmit(char * data){
	
	for (int i=0;i<strlen(data);i++){
		while(!(UCSR0A & (1<<UDRE0))){
			//While the flag is off
		}
		//send message
		UDR0 = data[i];
	}
	return 0;
}
*/

int uart_receive(){
	if(!(UCSR0A & (1<<RXC0))){
		return 0;
	}
	return UDR0;
}

int main(){
	uart_init(9600);
	stdout = &mystdout;
	SRAM_test();
	/*while(1){
		printf("ciao\n");
	}/*
	
	
	/* while(1){
		char data [] = "ciao";
		uart_transmit(data);
		int out = uart_receive();
		printf("%c" , out);
	}
	int out= 10;
	printf("%d\n" , out );
	
	while(1){
		PORTB |= (1 << PB0);
		_delay_ms(10);
		PORTB &= ~(1 << PB0);
		_delay_ms(10);
	}*/
}