#define DDR_SPI DDRB
#define DD_MOSI PB5
#define DD_MISO PB6
#define DD_SCK PB7 
#define DD_SS PB4
#include <avr/io.h>

void SPI_MasterInit(void)
{
	/* Set MOSI and SCK output, all others input */
	DDR_SPI = (1<<DD_MOSI)|(1<<DD_SCK) | (1<<DD_SS) | (0<<DD_MISO);
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR =(1<<SPE)|(1<<MSTR)|(1<<SPR0);
}


uint8_t SPI_MasterTransmit(char cData)
{
	/* Start transmission */
	SPDR = cData;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF))){
	}
	;
return SPDR;
}

