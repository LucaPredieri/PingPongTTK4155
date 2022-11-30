void solenoid_init(){
		REG_PIOC_PER = PIO_PC18;
		REG_PIOC_OER = PIO_PC18;
		REG_PIOC_CODR=PIO_PC18;
}

void set_solenoid(){
	
	REG_PIOC_CODR=PIO_PC18;
}

void clear_solenoid(){
	REG_PIOC_SODR=PIO_PC18;
}

