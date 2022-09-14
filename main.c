#include <stdlib.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "Uart_driver.h"
#include <avr/io.h>
#include <avr/sleep.h>
#include <util/delay.h>

#define BASE_ADDRESS 0x1800;

// this function is called by printf as a stream handler
int usart_putchar_printf(char var, FILE *stream) {
    // translate \n to \r for br@y++ terminal
    if (var == '\n') usart_putchar('\r');
    usart_putchar(var);
    return 0;
}

void xmem_init ( void ){
	MCUCR |= (1 << SRE ); // enable XMEM
	SFIOR |= (1 << XMM2 ); // mask unused bits
}

void xmem_write ( uint8_t data , uint16_t addr ){
	volatile char * ext_mem = ( char *) BASE_ADDRESS ;
	ext_mem [ addr ]= data ;
}
uint8_t xmem_read ( uint16_t addr ){
	volatile char * ext_mem = ( char *) BASE_ADDRESS ;
	uint8_t ret_val = ext_mem [ addr ];
	return ret_val ;
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



int uart_receive(){
	if(!(UCSR0A & (1<<RXC0))){
		return 0;
	}
	return UDR0;
}

int main(){
	
	uart_init(9600);
	stdout = &mystdout;
	xmem_init();
	uint8_t data=1;
	uint16_t addre1=0;
	uint16_t addre2=0;
	uint16_t seed;
	/*while (1) {
		//seed= rand();
		 // Write phase: Immediately check that the correct value was stored
		//srand(seed);
		data=1;
		addre1=0;
		xmem_write(data,addre1);
		_delay_ms(500);
		data=3;
		addre2=0x100;
		xmem_write(data,addre2);
		
		_delay_ms(500);
		
		printf("data: %02X, addr: %04X\n", xmem_read(addre1),addre1);

		printf("data: %02X, addr: %04X\n", xmem_read(addre2),addre2);
		
		
	}*/
	
	
	 SRAM_test();
	/*while(1){
		PORTA |= (1 << PA0);
		_delay_ms(10);
		PORTA &= ~(1 << PA0);
		_delay_ms(10);
		PORTA |= (1 << PA1);
		_delay_ms(10);
		PORTA &= ~(1 << PA1);
		_delay_ms(10);
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
	}

	SRAM_test();
	while(1){
		printf("ciao\n");
	}
	
	
	while(1){
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