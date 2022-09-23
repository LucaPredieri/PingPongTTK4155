struct joy_pos {   // Structure declaration
  uint16_t x_pos;           
  uint16_t y_pos;
  const char* x_currdir;
  const char* y_currdir;         
};

void GUI_main_menu_init(void){
	OLED_clear();
	OLED_goto_pos(1,8);
	OLED_print_string("PING PONG GAME");
	OLED_print_arrow(3,15);	
	OLED_goto_pos(3,32);
	OLED_print_string("Play");
	OLED_goto_pos(5,32);
	OLED_print_string("Quit");	
}

void move_arrow(void){
	
	
	
}