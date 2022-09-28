#include <stdlib.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include "fonts.h"


#define BASE_ADDRESS_OLED_COMMAND 0x1000
#define BASE_ADDRESS_OLED_DATA 0x1200
#define N_ROWS 8
#define N_COLS 128
#define FONTSIZE 8

void OLED_write_command(uint8_t command) //volatile
{
	volatile char* base_addr=BASE_ADDRESS_OLED_COMMAND;
	base_addr[0]=command;
}


void OLED_write_data(uint8_t data) //volatile
{
	volatile char* base_addr=BASE_ADDRESS_OLED_DATA;
	base_addr[0]=data;
}

void OLED_init()
 {
	OLED_write_command(0xae); // display off
	OLED_write_command(0xa1); //segment remap
	OLED_write_command(0Xda); //common pads hardware: alternative
	OLED_write_command(0x12);
	OLED_write_command(0xc8); //common output scan direction:com63~com0
	OLED_write_command(0xa8); //multiplex ration mode:63
	OLED_write_command(0x3f);
	OLED_write_command(0xd5); //display divide ratio/osc. freq. mode
	OLED_write_command(0x80);
	OLED_write_command(0x81); //contrast control
	OLED_write_command(0x50);
	OLED_write_command(0xd9); //set pre-charge period
	OLED_write_command(0x21);
	OLED_write_command(0x20); //Set Memory Addressing Mode
	OLED_write_command(0x02); //THERE WAS WRITTEN 02
	OLED_write_command(0xdb); //VCOM deselect level mode
	OLED_write_command(0x30);
	OLED_write_command(0xad); //master configuration
	OLED_write_command(0x00);
	OLED_write_command(0xa4); //out follows RAM content
	OLED_write_command(0xa7); //set awesome display
	OLED_write_command(0xaf); // display on
	   
	OLED_write_command(0xd3);
	OLED_write_command(0x02);
	
} 


void OLED_goto_line(uint8_t line){
	if(line>=0 && line<8){
		OLED_write_command(0xb0+line);	
	}
}

void OLED_goto_col(uint8_t column){
	if(column>=0 && column<128){	
		OLED_write_command(0x00+(column & 0x0F)); //set the lower column address
		OLED_write_command(0x10+((column & 0xF0) >> 4)); //set the upper column address
	}
}


void OLED_goto_pos(uint8_t line, uint8_t column){
	OLED_goto_line(line);
	OLED_goto_col(column);
}

void OLED_clear(){
	for(uint8_t i=0;i<N_ROWS;i++){
		OLED_goto_pos(i,0);
		for(uint8_t j=0;j<N_COLS;j++){
			OLED_write_data(0x00);
		}		
	}
}

void OLED_clear_arrow(){
	for(uint8_t i=2;i<6;i++){
		OLED_goto_pos(i,0);
		for(uint8_t j=0;j<21;j++){
			OLED_write_data(0x00);
		}
	}
}



void OLED_print_arrow(uint8_t row, uint8_t col){
	
	
	OLED_goto_pos(row,col);
	OLED_write_data(0b00011000);
	OLED_write_data(0b00011000);
	OLED_write_data(0b01111110);
	OLED_write_data(0b00111100);
	OLED_write_data(0b00011000);
	
}

void OLED_print_char(char c){
	for(uint8_t i=0; i<FONTSIZE; i++){
		uint8_t byte = pgm_read_byte(&(font8[c - 32][i]));
		OLED_write_data(byte);
	}
}


void OLED_print_string(const char * str){	 
	 uint8_t i=0;
	 while(str[i]!= '\0'){
		 OLED_print_char(str[i]);
		 i++; 
	 }
	
	
}