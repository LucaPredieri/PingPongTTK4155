#define SCORE_CAN_ID 2
#define SCORE_CAN_DATA_LENGTH 1


uint32_t clk_value=0;
CAN_MESSAGE score_message;


void play_init(){
	score_message.id=SCORE_CAN_ID;
	score_message.data_length=SCORE_CAN_DATA_LENGTH;
}

void routine(){
	uint16_t encVal=read_encoder();
	if (is_game_over()){
		clk_value=read_value_timer();
		printf("Your score is: %d\n\r",clk_value);
		score_message.data[0]=clk_value;
		can_send(&score_message,0);
		finish_game();
	}
}