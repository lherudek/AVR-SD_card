/**
 * @origin	CC Dharmani, Chennai (India) - www.dharmanitech.com
 * @website http://www.dharmanitech.com/2009/01/sd-card-interfacing-with-atmega8-fat32.html 
 * @author  Lukas Herudek
 * @email   lukas.herudek@gmail.com
 * @version v1.0
 * @ide     Atmel Studio 6.2
 * @license GNU GPL v3
 * @brief   SD card library for AVR
 * @verbatim
   ----------------------------------------------------------------------
    Copyright (C) Lukas Herudek, 2018
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.
     
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
	See the GNU General Public License for more details.
	
	<http://www.gnu.org/licenses/>
@endverbatim
 */

#include "UART_routines.h"
#include <avr/io.h>
#include <avr/pgmspace.h>

//**************************************************
//UART0 initialize
//baud rate: 19200  (for controller clock = 8MHz)
//char size: 8 bit
//parity: Disabled
//**************************************************
void uart_init(void)
{
	UBRR0  = (1000000UL/8/9600)-1;
	UCSR0A = (1<<U2X0);//double UART speed
	UCSR0B = (1<<RXCIE0) | (1<<RXEN0) | (1<<TXEN0);//enable RX and TX, enable RX interrupt
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);//8N1
}

//**************************************************
//Function to receive a single byte
//*************************************************
unsigned char receiveByte( void )
{
	while(!(UCSR0A&(1<<RXC0)));//wait for data to be received
	return UDR0;//get and return received data from buffer
}

//***************************************************
//Function to transmit a single byte
//***************************************************
void transmitByte( unsigned char data )
{
	while(!(UCSR0A&(1<<UDRE0)));//wait for empty transmit buffer
	UDR0 = data;//put data into buffer, sends the data
}


//***************************************************
//Function to transmit hex format data
//first argument indicates type: CHAR, INT or LONG
//Second argument is the data to be displayed
//***************************************************
void transmitHex( unsigned char dataType, unsigned long data )
{
unsigned char count, i, temp;
unsigned char dataString[] = "0x        ";

if (dataType == CHAR) count = 2;
if (dataType == INT) count = 4;
if (dataType == LONG) count = 8;

for(i=count; i>0; i--)
{
  temp = data % 16;
  if((temp>=0) && (temp<10)) dataString [i+1] = temp + 0x30;
  else dataString [i+1] = (temp - 10) + 0x41;

  data = data/16;
}

transmitString (dataString);
}

//***************************************************
//Function to transmit a string in Flash
//***************************************************
void transmitString_F(const char* string)
{
  while (pgm_read_byte(&(*string)))
   transmitByte(pgm_read_byte(&(*string++)));
}

//***************************************************
//Function to transmit a string in RAM
//***************************************************
void transmitString(unsigned char* string)
{
  while (*string)
   transmitByte(*string++);
}

//************ END ***** www.dharmanitech.com *******
