CAN_message score_message;
void play_init(){
	printf("START NODE 1\n\r");

}
void routine(){
	menu_navigation();
	//CAN_send_slider();
	if (is_playing())
		CAN_send_joypos();
	CAN_receive(&score_message);
	check_game(&score_message);
	_delay_ms(1000);
}