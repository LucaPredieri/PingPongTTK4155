
#define EQ_PWM 630
#define LEFT_PWM 882
#define RIGHT_PWM 378
uint16_t map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void pwm_init(){
	REG_PWM_CLK=PWM_CLK_PREA(0)|PWM_CLK_DIVA(200); //84 Mhz /200
	PMC->PMC_PCER1|=(1<<4); // Peripheral Clock Enable 1
	REG_PIOC_PDR|=PIO_PDR_P19;
	REG_PIOC_ABSR|=	PIO_ABSR_P19; //AB Select Register
	REG_PWM_WPCR=PWM_WPCR_WPKEY(0x50574D); //PWM Write Protect Control Register
	REG_PWM_WPCR|=PWM_WPCR_WPRG0|PWM_WPCR_WPRG2|PWM_WPCR_WPRG3| PWM_WPCR_WPCMD(0);
	REG_PWM_CMR5 = PWM_CMR_CPRE_CLKA; //PWM Channel Mode Register
	REG_PWM_CPRD5=8400; //PWM Channel Period Register
	REG_PWM_ENA=PWM_ENA_CHID5; // PWM Enable Register
	REG_PWM_CDTY5=8400-EQ_PWM; //PWM Channel Duty Cycle Register
}

void pwm_set_dutycycle(uint16_t val){ 
 REG_PWM_CDTY5 = 8400 - map(val, 0, 100, LEFT_PWM, RIGHT_PWM); 
}