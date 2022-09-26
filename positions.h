#include <stdlib.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <util/delay.h>

#define BASE_ADDRESS_ADC 0x1400

struct joy_pos {   // Structure declaration
  uint16_t x_pos;           
  uint16_t y_pos;
  const char* x_currdir;
  const char* y_currdir;         
};

struct slider_pos {   // Structure declaration
  uint8_t left;           
  uint8_t right;       
};

struct joy_pos joystick;
struct slider_pos slider;


struct joy_pos get_joypos(){
	ADC_init_read();
	joystick.x_pos = xmem_read(0x00, BASE_ADDRESS_ADC);
	joystick.y_pos = xmem_read(0x00, BASE_ADDRESS_ADC);	
	xmem_read(0x00, BASE_ADDRESS_ADC);
    xmem_read(0x00, BASE_ADDRESS_ADC);
	_delay_us(5000);

	if (joystick.x_pos>160 && joystick.x_pos<170) 
		{joystick.x_pos = 0;
		joystick.x_currdir="NEUTRAL";
		}
	else if(joystick.x_pos>=0 && joystick.x_pos<160) {
		joystick.x_pos=(joystick.x_pos*100/166)-100;
		joystick.x_currdir="LEFT";
		}	
	else if(joystick.x_pos>=170 && joystick.x_pos<256){
		joystick.x_pos=((joystick.x_pos-170)*100/85);
		joystick.x_currdir="RIGHT";
		}
	else{
		joystick.x_pos=0;
		joystick.x_currdir="---";
		
	}

	if (joystick.y_pos>155 && joystick.y_pos<170) {
		joystick.y_pos = 0;
		joystick.y_currdir="NEUTRAL";
		}
	else if(joystick.y_pos>=0 && joystick.y_pos<=155) {
		joystick.y_pos=(joystick.y_pos*100/166)-100;
		joystick.y_currdir="DOWN";
		}
	else if(joystick.y_pos>=170 && joystick.y_pos<256){
		joystick.y_pos=((joystick.y_pos-170)*100/85);
		joystick.y_currdir="UP";
		}
	else{
		joystick.y_pos=0;
		joystick.y_currdir="---";
	}
	
	return joystick;		
}


struct slider_pos get_sliderpos(){
	ADC_init_read();
	xmem_read(0x00, BASE_ADDRESS_ADC);
    xmem_read(0x00, BASE_ADDRESS_ADC);
	slider.left = xmem_read(0x00, BASE_ADDRESS_ADC)*100/255;
	slider.right = xmem_read(0x00, BASE_ADDRESS_ADC)*100/255;
	_delay_us(5000);
	return slider;			
}

