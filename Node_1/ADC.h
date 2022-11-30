// Functions to initialize the PWM pin for clocking the ADC.
#include <stdlib.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <util/delay.h>

void PWM_init(){
	DDRD |= (1 << DDD4 );
	TCCR3A |= (0 << WGM31) | (0 << WGM30) | (1 << COM3A0);
	TCCR3B |= (0 << WGM33) | (1 << WGM32) | (0 << CS32) | (0 << CS31) | (1 << CS30) ;
	OCR3A = 0;
}

void ADC_init_read(){
	uint8_t adc_mode = (1 << SINGLE_CHANNEL_SAMPLE) | (0);
	xmem_write(adc_mode, 0x01, BASE_ADDRESS_ADC);
}