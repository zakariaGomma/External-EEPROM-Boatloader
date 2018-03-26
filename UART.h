/*
 * UART.H
 *
 * Created: 05/01/2018 03:32:06 PM
 *  Author: Zakaria Gomaa
 */ 


#ifndef UART_H_
#define UART_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <math.h>

						// Variable to set baud rate value for UBRR Register
void UART_INIT(uint16_t BAUD);					// This function is used to initialize the UART
void UART_TRANSMIT (unsigned char data);		// This function is used to send data
unsigned char UART_RECEIVE (void);				// This function is used to receive data
void UART_TRANSMIT_string(unsigned char  *dat);
void Uart_Print( unsigned char word[]);
#define Newline { UART_TRANSMIT('\r'); UART_TRANSMIT('\n');}


#endif /* UART_H_ */