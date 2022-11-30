#include <stdlib.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <util/delay.h>

#include "usart_driver.h"
#include "xmem_driver.h"
#include "ADC.h"
#include "positions.h"
#include "OLED.h"
#include "GUI.h"
#include "play.h"




int main(){
	uart_init(9600);
	stdout = &mystdout;
	xmem_init();
	PWM_init();
	OLED_init();
	OLED_clear();
	GUI_menu_init();
	MCP2515_init();
	button_init();
	play_init();
	while (1){
		routine();
	}

}




