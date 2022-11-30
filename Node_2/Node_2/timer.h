
//For the score
//sets timer at 1 Hz (Slow Clock 32768kHz and prescaler period 32768), reset enabled
void start_timer(){
	REG_RTT_MR=0x48000; //RTTRST=1, RTPRES=0x8000
}

uint32_t read_value_timer(){
	uint32_t clk_value=	REG_RTT_VR; //value of counts since last reset 
	return clk_value;
}


void delay_us(uint16_t val){
	for (uint16_t i=0;i<val;i++) asm("NOP");
}