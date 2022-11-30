#include "mcp2515.h"
#include "MCP2515_driver.h"

typedef struct{
	unsigned short id;
	unsigned char length;
	char data [8];
}CAN_message;

void set_msg_id(CAN_message * message , unsigned short message_id){
	message->id = message_id;
}


void set_msg_length(CAN_message * message , unsigned char length){
	message->length = length;
}

void CAN_transmit(CAN_message *message, uint8_t buffer){
	uint8_t id_low = (message->id & 7) << 5;
	uint8_t id_high = (message->id) >> 3;
	uint8_t length = message->length;
	uint8_t reg_high,reg_low,reg_dlc,reg_data,instr;

	if(buffer == 0){
		reg_high=TXB0SIDH;
		reg_low=TXB0SIDL;
		reg_dlc=TXB0DLC;
		reg_data=TXB0D0;
		instr=MCP_RTS_TX0;
	}
	else if (buffer == 1){
		reg_high=TXB1SIDH;
		reg_low=TXB1SIDL;
		reg_dlc=TXB1DLC;
		reg_data=TXB1D0;
		instr=MCP_RTS_TX1;		
	}
	else {
		reg_high=TXB2SIDH;
		reg_low=TXB2SIDL;
		reg_dlc=TXB2DLC;
		reg_data=TXB2D0;
		instr=MCP_RTS_TX2;			
	}
	MCP2515_write(reg_high, id_high);
	MCP2515_write(reg_low, id_low);
	MCP2515_write(reg_dlc, length);
	for(int i=0; i<length; i++){
		MCP2515_write(reg_data +i, message->data[i]);
	}
	MCP2515_request_to_send(instr);
}

void CAN_receive(CAN_message* new_message){
	
	uint8_t intf = MCP2515_read(MCP_CANINTF); 
	uint8_t int0 = (intf & MCP_RX0IF);
	uint8_t int1 = (intf & MCP_RX1IF);
	uint8_t reg_high,reg_low,reg_dlc,reg_data;

	if(int1){
		reg_high = MCP_RXB1SIDH;
		reg_low = MCP_RXB1SIDL;
		reg_dlc = MCP_RXB1DLC;
		reg_data = MCP_RXB1D0;

		MCP2515_bit_modify(MCP_CANINTF, MCP_RX1IF, 0x00);

		//int1=0;
	}
	/*else if(int1){
		reg_high = MCP_RXB1SIDH;
		reg_low = MCP_RXB1SIDL;
		reg_dlc = MCP_RXB1DLC;
		reg_data = MCP_RXB1D0;
		MCP2515_bit_modify(MCP_CANINTF, MCP_RX1IF, 0x00);
	}*/

	if(int1){
		uint8_t id_high=MCP2515_read(reg_high) << 3;
		uint8_t id_low=MCP2515_read(reg_low) >> 5;
		unsigned short id = (id_high | id_low);
		set_msg_id(new_message,id);
		unsigned char length=MCP2515_read(reg_dlc) & 0x0F;
		set_msg_length(new_message,length);
		for(uint8_t i=0; i<(uint8_t)(new_message->length); i++){
			new_message->data[i] =(char)MCP2515_read(reg_data+i);
		}
	//printf("ID from receive: %d,id low: %d, id high: %d",id,(id_low<<5),(id_high>>3));
	}

}

void CAN_print(CAN_message *message){
	printf("\nSTART CAN Message print.\n");
	printf("ID: %u \n", (unsigned short)message->id);
	printf("Length: %u \n",(unsigned char) message->length);
	for(uint8_t i=0; i<(uint8_t)(message->length); i++){
		printf("Package [%d]: %d \n", i, message->data[i]);
	}
	printf("END CAN Message print.\n\n");
}
