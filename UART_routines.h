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

#ifndef _UART_ROUTINES_H
#define _UART_ROUTINES_H

#define CHAR 0
#define INT  1
#define LONG 2

#define TX_NEWLINE {transmitByte(0x0d); transmitByte(0x0a);}

void uart_init(void);
unsigned char receiveByte(void);
void transmitByte(unsigned char);
void transmitString_F(const char*);
void transmitString(unsigned char*);
void transmitHex( unsigned char dataType, unsigned long data );


#endif
