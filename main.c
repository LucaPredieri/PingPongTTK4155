#include <stdlib.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <util/delay.h>

#include "Uart_driver.h"
#include "usart_driver.h"
#include "xmem_driver.h"
#include "ADC.h"
#include "positions.h"
#include "OLED.h"
#include "GUI.h"


int main(){
	uart_init(9600);
	stdout = &mystdout;
	xmem_init();
	PWM_init();
	OLED_init();
	OLED_clear();
	//OLED_clear_arrow();
	GUI_menu_init();
	DDRB = 0x00;
	while (1){
		menu_navigation();
		_delay_ms(1000);
	}
}



/*
	while(1){
		OLED_clear();
		_delay_ms(1000);
		OLED_print_arrow(3,64);
		_delay_ms(1000);
	}
}
		//xmem_write(0b10100101, 0xa5, BASE_ADDRESS_OLED); 
/*
joystick = get_joypos();
slider=get_sliderpos();
printf("-----------------------------------------------------------\n");
printf("xjoy= %d%% yjoy= %d%% left= %d%% right = %d%% \n",joystick.x_pos, joystick.y_pos, slider.left, slider.right);
printf("Current joystick's direction: %s, %s\n",joystick.x_currdir,joystick.y_currdir);
*/




