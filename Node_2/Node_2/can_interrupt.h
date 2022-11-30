/*
 * can_interrupt.h
 *
 * Author: Gustav O. Often and Eivind H. Jølsgard
 *
 * For use in TTK4155 Embedded and Industrial Computer Systems Design
 * NTNU - Norwegian University of Science and Technology
 *
 */ 

#include "CAN_controller.h"


#ifndef CAN_INTERRUPT_H_
#define CAN_INTERRUPT_H_
void CAN0_Handler       ( void );
void CAN_message_get(CAN_MESSAGE pass_message);






#endif /* CAN_INTERRUPT_H_ */