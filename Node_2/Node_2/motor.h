#define DIR  PIO_PD10
#define EN   PIO_PD9
#define SEL  PIO_PD2
#define NOT_RST  PIO_PD1
#define NOT_OE   PIO_PD0
#define ENC_MSK  0x1FE
#define ENC_MIN 0
#define ENC_MAX 8500
#define ENC_MID ENC_MAX/2
#define MAX_MOTOR_SPEED 2000
#define MIN_MOTOR_SPEED 1000

uint8_t game_over=0;

static PID_t PID;

void motor_init(){
	REG_PMC_PCR= PMC_PCR_EN | PMC_PCR_DIV_PERIPH_DIV_MCK | (ID_PIOC << PMC_PCR_PID_Pos);
	REG_PMC_PCER0 |= 1<<ID_PIOC;
	REG_PIOC_PER = PIO_PC1 | PIO_PC2 | PIO_PC3 | PIO_PC4 | PIO_PC5 | PIO_PC6 | PIO_PC7 | PIO_PC8 ;
	REG_PIOC_ODR = PIO_PC1 | PIO_PC2 | PIO_PC3 | PIO_PC4 | PIO_PC5 | PIO_PC6 | PIO_PC7 | PIO_PC8;
	
	REG_PIOD_PER= NOT_OE | NOT_RST | SEL | EN | DIR  ; //PIO Controller PIO Enable Register
	REG_PIOD_OER= NOT_OE | NOT_RST | SEL | EN | DIR  ; //PIO Controller PIO Output Enable Register
	
	REG_PIOD_SODR= NOT_RST | EN; //PIO Controller Set Output Data Register, not encoder reset (P1) ,encoder mj1 enable high(P10)
	REG_PIOD_CODR = NOT_OE | SEL | DIR; //PIO Controller Clear Output Data Register,encoder output enable (P0), encoder select(P2) ,mj1 set dir(P10)
	reset_encoder();
	
	
}

uint16_t read_encoder(){
	REG_PIOD_CODR=REG_PIOD_CODR = NOT_OE | SEL ; //set !OE and SEL low
	delay_us(200); //Wait approx. 20 microseconds for output to settle
	uint8_t high_byte  = (REG_PIOC_PDSR & ENC_MSK)>>1; // Read MJ2 to get high byte
	REG_PIOD_SODR= SEL; //Set SEL high to output low byte	delay_us(200);
	uint8_t low_byte = (REG_PIOC_PDSR & ENC_MSK)>>1; // Read MJ2 to get low byte
	REG_PIOD_SODR = NOT_OE; //Set !OE to high	uint16_t enc_value= high_byte << 8 | low_byte;	if (enc_value & (1 << 15)) {
        return ((uint16_t) (~enc_value) + 1);
    }	return enc_value;
}

uint16_t joystick_map(uint16_t pos,uint8_t dir){
		uint16_t val=pos;
		if (dir){ //right
			val =map(pos, 0, 100, (uint16_t)ENC_MID, ENC_MAX);
		}
		else val =map(pos, 0, 100,(uint16_t)ENC_MID,ENC_MIN); //left
	return val;
	
}


void set_motor_pos(uint16_t des_pos){
	
	uint16_t curr_pos = read_encoder();
	
	des_pos=map(des_pos,0,100,ENC_MIN,ENC_MAX);
	
	uint16_t u=0;
	int err=curr_pos-des_pos;
	
		err=curr_pos-des_pos;

		u=PID_controller(&PID,curr_pos,des_pos);
		
		if(u>MAX_MOTOR_SPEED){
			u=MAX_MOTOR_SPEED;	
		}

		if (err>0){//LEFT
			REG_PIOD_CODR =  DIR;
		}
		else if (err<0){ //RIGHT
			REG_PIOD_SODR =  DIR;
		}
		DAC_write(u);

	
}


void set_motor_pos_joystick(uint16_t des_pos,uint8_t dir){
	
	uint16_t curr_pos = read_encoder();
	des_pos=joystick_map(des_pos,dir);
	
	uint16_t u=0;
	int err=curr_pos-des_pos;
	
		err=curr_pos-des_pos;

		u=PID_controller(&PID,curr_pos,des_pos);
		
		if(u>MAX_MOTOR_SPEED){
			u=MAX_MOTOR_SPEED;	
		}

		if (err>0){//LEFT
			REG_PIOD_CODR =  DIR;
		}
		else if (err<0){ //RIGHT
			REG_PIOD_SODR =  DIR;
		}
		DAC_write(u);
	

	
}

void reset_encoder(){
	REG_PIOD_CODR= NOT_RST;
	delay_us(20);
	REG_PIOD_SODR= NOT_RST;
}






void int_servo_init(){
	REG_PIOC_PER=PIO_PER_P16; ////PIO Controller PIO Enable Register
	PIOC->PIO_ODR=PIO_ODR_P16; //PIO Controller Output Disable Register
	PIOC->PIO_PUDR=PIO_PUDR_P16; //PIO Pull Up Disable Register
	PMC->PMC_PCER0|=(1<<ID_PIOC); // Peripheral Clock Enable 0
	REG_PIOC_AIMER=PIO_AIMER_P16;
	REG_PIOC_IER|=PIO_IER_P16;
	REG_PIOC_ESR=PIO_ESR_P16;
	REG_PIOC_REHLSR=PIO_REHLSR_P16;
	REG_PIOC_SCDR=PIO_SCDR_DIV(100); //debouncing delay
	REG_PIOC_IFER=PIO_IFER_P16;  //enable debouncing delta
	REG_PIOC_DIFSR=PIO_DIFSR_P16; //enable debouncing filter for one specific
	NVIC_EnableIRQ(PIOC_IRQn);		
}

uint8_t is_game_over(){
	return game_over;
}

void finish_game(){
	game_over=0;
}


 void PIOC_Handler(){
	 
	uint32_t status=REG_PIOC_ISR;
	 if(status & PIO_ISR_P16){
			printf("interrupt captured\n\r");
			game_over= 1;
		}
	infrared_interrupt();
 }
 
 
 