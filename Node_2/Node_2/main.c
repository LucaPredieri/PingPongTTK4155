/*
 * Node_2.c
 *
 * Created: 14.10.2022 16:51:24
 * Author : lucapre
 */ 

#include <stdio.h>
#include "printf-stdarg.h"
#include "sam.h"
#include "can_controller.h"
#include "can_interrupt.h"
#include "pwm.h"
#include "PID.h"
#include "motor.h"
#include "timer.h"
#include "play.h"

#include "DAC.h"
#include "solenoid.h"


int main(void)
{
    /* Initialize the SAM system */
    SystemInit();
	configure_uart();
	pwm_init();
	can_init_def_tx_rx_mb(get_can_br());
	int_servo_init();
	motor_init();
	DAC_init();
	solenoid_init();
	PID_init(&PID,K_P,K_I,K_D, PERIOD);
	play_init();
	REG_WDT_MR=WDT_MR_WDDIS;

    while (1){
		routine();
	}
	
}
