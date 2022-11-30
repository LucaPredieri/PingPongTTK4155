/*
 * interrupt.c
 *
 * Author: Gustav O. Often and Eivind H. J�lsgard
 *
 * For use in TTK4155 Embedded and Industrial Computer Systems Design
 * NTNU - Norwegian University of Science and Technology
 *
 */ 

#include "can_interrupt.h"

#include <stdio.h>
#include "sam.h"

#include "printf-stdarg.h"

#include "can_controller.h"

#define DEBUG_INTERRUPT 1
#define SLIDER_CAN_ID 3
#define JOYSTICK_CAN_ID 1
#define PLAY_CAN_ID 5
#define STOP_CAN_ID 6

/**
 * \brief CAN0 Interrupt handler for RX, TX and bus error interrupts
 *
 * \param void
 *
 * \retval 
 */

CAN_MESSAGE message;
uint8_t playing =0;

void CAN0_Handler( void )
{
	//if(DEBUG_INTERRUPT)printf("CAN0 interrupt\n\r");
	char can_sr = CAN0->CAN_SR; 
	
	//RX interrupt
	if(can_sr & (CAN_SR_MB1 | CAN_SR_MB2) )//Only mailbox 1 and 2 specified for receiving
	{
		if(can_sr & CAN_SR_MB1)  //Mailbox 1 event
		{
			can_receive(&message, 1);			
			if (message.id==PLAY_CAN_ID){
				playing=1;
				start_timer();
			}
			if (message.id==STOP_CAN_ID){
				playing=0;
			}
			if (message.id==SLIDER_CAN_ID && playing){
				pwm_set_dutycycle(message.data[1]);
				set_motor_pos(message.data[0]);
				if(message.data[2]){
					set_solenoid();
				}
					
				else {
					clear_solenoid();
				}
			}
		if (message.id==JOYSTICK_CAN_ID && playing){
			pwm_set_dutycycle(message.data[4]);
			set_motor_pos_joystick(message.data[0],message.data[2]);
			if(message.data[3]){
				set_solenoid();
			}
					
			else {
				clear_solenoid();
			}
					
			}

			

		}
		else if(can_sr & CAN_SR_MB2) //Mailbox 2 event
		
		{
			can_receive(&message, 2);
		}
		else
		{
			printf("CAN0 message arrived in non-used mailbox\n\r");
		}

		//(DEBUG_INTERRUPT)printf("message id: %d\n\r", message.id);
		//(DEBUG_INTERRUPT)printf("message data length: %d\n\r", message.data_length);
		for (int i = 0; i < message.data_length; i++)
		{
			//(DEBUG_INTERRUPT)printf("%d ", message.data[i]);
		}
		//(DEBUG_INTERRUPT)printf("\n\r");
	}
	
	if(can_sr & CAN_SR_MB0)
	{
		//if(DEBUG_INTERRUPT) printf("CAN0 MB0 ready to send \n\r");
		
	//Disable interrupt
		CAN0->CAN_IDR = CAN_IER_MB0;

	}

	if(can_sr & CAN_SR_ERRP)
	{
		if(DEBUG_INTERRUPT)printf("CAN0 ERRP error\n\r");

	}
	if(can_sr & CAN_SR_TOVF)
	{
		if(DEBUG_INTERRUPT)printf("CAN0 timer overflow\n\r");

	}
	
	NVIC_ClearPendingIRQ(ID_CAN0);
	//sei();*/
}

void CAN_message_get(CAN_MESSAGE pass_message){
	
	pass_message.id = message.id;
	pass_message.data_length = message.data_length;
	for (int i = 0; i < message.data_length; i++){
		pass_message.data[i] = message.data[i];
		
	}
}

void infrared_interrupt(){
	playing = 0;
}
