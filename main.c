#include <stdlib.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "Uart_driver.h"
#include <avr/io.h>
#include <avr/sleep.h>
#include <util/delay.h>

#define BASE_ADDRESS_OLED 0x1000
#define BASE_ADDRESS_ADC 0x1400
#define BASE_ADDRESS_SRAM 0x1800
#define SINGLE_CHANNEL_SAMPLE 7

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

void xmem_write ( uint8_t data , uint16_t addr, uint16_t base){
	volatile char * ext_mem = ( char *) base ;
	ext_mem [ addr ]= data ;
}
uint8_t xmem_read ( uint16_t addr, uint16_t base ){
	volatile char * ext_mem = ( char *) base;
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

void PWM_init(){
	DDRD |= (1 << DDD4 );
	TCCR3A |= (0 << WGM31) | (0 << WGM30) | (1 << COM3A0);
	TCCR3B |= (0 << WGM33) | (1 << WGM32) | (0 << CS32) | (0 << CS31) | (1 << CS30) ;
	OCR3A = 0;
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
	PWM_init();
	uint8_t data=154;
	uint16_t addre1=0x01;
	uint16_t addre2=0;
	uint16_t seed;
	addre1=0;
	printf("///////////////\n");
	while(1){
		uint8_t adc_mode = (1 << SINGLE_CHANNEL_SAMPLE) | (0);

		xmem_write(adc_mode, 0x01, BASE_ADDRESS_ADC);
		_delay_us(5000);
		uint8_t value1 = xmem_read(0x00, BASE_ADDRESS_ADC);
		uint8_t value2 = xmem_read(0x00, BASE_ADDRESS_ADC);
		printf("value1 = %d value2 = %d \n ", value1, value2);
	}
}