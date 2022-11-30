#include "SPI.h"
#include "mcp2515.h"
#define RESET_CMD 0b11000000
#define READ_CMD 0b00000011
#define WRITE_CMD 0b00000010
#define BIT_MOD_CMD 0b00000101
#define READ_S_CMD 0b10100000



void MCP2515_activate_slave(){
	PORTB &= ~(1<< PB4);
}

void MCP2515_deactivate_slave(){
	PORTB |= (1<< PB4);
}

void MCP2515_reset(){
	MCP2515_activate_slave();
	SPI_MasterTransmit(RESET_CMD);
	MCP2515_deactivate_slave();
}

uint8_t MCP2515_read(uint8_t address){
	MCP2515_activate_slave();
	SPI_MasterTransmit(READ_CMD);
	SPI_MasterTransmit(address);
	uint8_t data = SPI_MasterTransmit(0x00);
	MCP2515_deactivate_slave();
	return data;
}
uint8_t MCP2515_read_status(uint8_t address){
	MCP2515_activate_slave();
	SPI_MasterTransmit(READ_S_CMD);
	uint8_t data = SPI_MasterTransmit(0x00);
	MCP2515_deactivate_slave();
	return data;
}

uint8_t MCP2515_write(uint8_t address, uint8_t data){
	MCP2515_activate_slave();
	SPI_MasterTransmit(WRITE_CMD);
	SPI_MasterTransmit(address);
	SPI_MasterTransmit(data);
	MCP2515_deactivate_slave();
}

void MCP2515_request_to_send(uint8_t instruction){
	MCP2515_activate_slave();
	SPI_MasterTransmit(instruction);
	MCP2515_deactivate_slave();
}

uint8_t MCP2515_bit_modify(uint8_t address, uint8_t mask,uint8_t data){
	MCP2515_activate_slave();
	SPI_MasterTransmit(BIT_MOD_CMD);
	SPI_MasterTransmit(address);
	SPI_MasterTransmit(mask);
	SPI_MasterTransmit(data);
	MCP2515_deactivate_slave();
}

uint8_t MCP2515_init ()
{
	uint8_t value ;
	SPI_MasterInit(); // Initialize SPI
	MCP2515_reset(); // Send reset - command

	// More initialization
	
	MCP2515_bit_modify(MCP_CANCTRL,MODE_MASK, MODE_CONFIG);
	// Self - test
	value = MCP2515_read(MCP_CANSTAT);
	if( (value & MODE_MASK) != MODE_CONFIG){
		printf("MCP2515 is not in configuration mode after reset\n");
		return 1;
	}
	
	
	MCP2515_bit_modify(MCP_CNF3, 0b11111111, 0b00000101); //??, ??, Nothing, PS2=6TQ
	MCP2515_bit_modify(MCP_CNF2, 0b11111111, 0b10110001); //PS2, 1 sample point, PS1=7TQ, PropSeg=2TQ
	MCP2515_bit_modify(MCP_CNF1, 0b11111111, 0b00000011); //Length=1TQ, BRP=1
	
	
	
	MCP2515_bit_modify(MCP_CANCTRL,MODE_MASK, MODE_NORMAL);
	MCP2515_bit_modify(MCP_CANINTE, 0b00000011, 0b00000011);
	
	return 0;
}
