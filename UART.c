/*
 * UART.C
 *
 * Created: 05/01/2018 03:31:48 PM
 *  Author: Zakaria Gomaa
 */ 

#include "UART.h"

void UART_INIT(uint16_t BAUD)
{
	uint16_t UBRR_VALUE = lrint( (F_CPU / ( 16UL * BAUD ) ) -1);
	UBRRH =(uint8_t) (UBRR_VALUE>>8);		//set baud rate
	UBRRL =(uint8_t) UBRR_VALUE;			//shift the register right by 8 bits
	UCSRB =(1<<RXEN)|(1<<TXEN);				//enable receiver and transmitter
	UCSRC =(1<<URSEL)|(3<<UCSZ0);			//8bit data format & No Parity
}

void UART_TRANSMIT(unsigned char data)
{
	while ( ! (UCSRA & (1<<UDRE) ) );		//wait while register is free
	UDR = data;								// load data in the register
}


unsigned char UART_RECEIVE(void)
{
	while ( ! (UCSRA & (1<<RXC) ) );		//wait while data is being received
	return UDR;								//return 8-bit data
}

void UART_TRANSMIT_string(unsigned char  *dat)
{
	while(*dat)
	{
		UART_TRANSMIT(*dat++);
		
	}
}
void Uart_Print(unsigned char word[]){
	int i=0;
    while(word[i])
    {
	UART_TRANSMIT(word[i]);i++;
	}

}