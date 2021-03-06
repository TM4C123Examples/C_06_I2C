// UART.c
// Runs on LM4F120/TM4C123
// Simple device driver for the UART.
// Daniel Valvano
// June 17, 2013

/* This example accompanies the books
  "Embedded Systems: Introduction to ARM Cortex M Microcontrollers",
  ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2013

"Embedded Systems: Real Time Interfacing to ARM Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2013
 
 Copyright 2013 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

// U0Rx (VCP receive) connected to PA0
// U0Tx (VCP transmit) connected to PA1

#include "UART.h"
#include <stdio.h>
#include "TM4C123.h"  

#define UART_FR_TXFF            0x00000020  // UART Transmit FIFO Full
#define UART_FR_RXFE            0x00000010  // UART Receive FIFO Empty
#define UART_LCRH_WLEN_8        0x00000060  // 8 bit word length
#define UART_LCRH_FEN           0x00000010  // UART Enable FIFOs
#define UART_CTL_UARTEN         0x00000001  // UART Enable
#define SYSCTL_RCGC1_UART0      0x00000001  // UART0 Clock Gating Control
#define SYSCTL_RCGC2_GPIOA      0x00000001  // port A Clock Gating Control

//------------UART_Init------------
// Wait for new serial port input
// Initialize the UART for 115,200 baud rate (assuming 50 MHz UART clock),
// 8 bit word length, no parity bits, one stop bit, FIFOs enabled
// Input: none
// Output: none
void UART_Init(void){
  SYSCTL->RCGC1|= SYSCTL_RCGC1_UART0; // activate UART0
  SYSCTL->RCGC2|= SYSCTL_RCGC2_GPIOA; // activate port A
  UART0->CTL &= ~UART_CTL_UARTEN;      // disable UART
  UART0->IBRD = 27;                    // IBRD = int(50,000,000 / (16 * 115200)) = int(27.12673611)
  UART0->FBRD= 8;                    // FBRD = round(0.12673611 * 64) = 8
                                        // 8 bit word length (no parity bits, one stop bit, FIFOs)
  UART0->LCRH = (UART_LCRH_WLEN_8|UART_LCRH_FEN);
  UART0->CTL|= UART_CTL_UARTEN;       // enable UART
  GPIOA->AFSEL |= 0x03;           // enable alt funct on PA1-0
  GPIOA->DEN |= 0x03;             // enable digital I/O on PA1-0
                                        // configure PA1-0 as UART
  GPIOA->PCTL = (GPIOA->PCTL&0xFFFFFF00)+0x00000011;
  GPIOA->AMSEL &= ~0x03;          // disable analog functionality on PA
}

//------------UART_InChar------------
// Wait for new serial port input
// Input: none
// Output: ASCII code for key typed
 char UART_InChar(void){
  while((UART0->FR&UART_FR_RXFE) != 0);
  return((unsigned char)(UART0->DR&0xFF));
}

int UART_kbhit(void){
	if((UART0->FR&UART_FR_RXFE) != 0){
		return 0;
	}else{
	return 1;
	}
}
//------------UART_OutChar------------
// Output 8-bit to serial port
// Input: letter is an 8-bit ASCII character to be transferred
// Output: none
void UART_OutChar(unsigned char data){
  while((UART0->FR&UART_FR_TXFF) != 0);
  UART0->DR = data;
}

// Print a character to UART0.
void sendchar(char ch){
  UART_OutChar(ch);
}
// input from UART, return data.
char getkey(void){
  return UART_InChar();
}
