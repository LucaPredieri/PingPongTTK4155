#include <stdlib.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "Uart_driver.h"
#include "usart_driver.h"
#include "xmem_driver.h"
#include "ADC.h"
#include <avr/io.h>
#include <avr/sleep.h>
#include <util/delay.h>

struct joy_pos {   // Structure declaration
  uint8_t x_pos;           
  uint8_t y_pos;       
};

struct slider_pos {   // Structure declaration
  uint8_t left;           
  uint8_t right;       
};

struct joy_pos joystick;
struct slider_pos slider;


struct joy_pos get_joypos(){
	joystick.x_pos = xmem_read(0x00, BASE_ADDRESS_ADC)*100/255;
	joystick.y_pos = xmem_read(0x00, BASE_ADDRESS_ADC)*100/255;	
	xmem_read(0x00, BASE_ADDRESS_ADC);
    xmem_read(0x00, BASE_ADDRESS_ADC);
	return joystick;		
}


struct slider_pos get_sliderpos(){
	xmem_read(0x00, BASE_ADDRESS_ADC);
    xmem_read(0x00, BASE_ADDRESS_ADC);
	slider.left = xmem_read(0x00, BASE_ADDRESS_ADC)*100/255;
	slider.right = xmem_read(0x00, BASE_ADDRESS_ADC)*100/255;
	return slider;			
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
	uint8_t value = 0;
	DDRB = 0x00;

	while(1){
		/*value = PINB;
		uint8_t adc_mode = (1 << SINGLE_CHANNEL_SAMPLE) | (0);

		xmem_write(adc_mode, 0x01, BASE_ADDRESS_ADC);
		_delay_us(5000);
		joystick = get_joypos();
		slider=get_sliderpos();
		printf("xjoy= %d yjoy= %d left= %d right = %d \n",joystick.x_pos, joystick.y_pos, slider.left, slider.right);*/
		SRAM_test();
	} 
}




