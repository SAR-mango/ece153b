/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Lab: 4A
 */


#include "stm32l476xx.h"
#include "LED.h"
#include "SysClock.h"
#include "UART.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>

// Initializes USARTx
// USART2: UART Communication with PC
// USART1: Bluetooth Communication with Phone
void Init_USARTx(int x) {
	if (x == 1) {
		UART1_Init();
		UART1_GPIO_Init();
		USART_Init(USART1);
	}
	else if (x == 2) {
		UART2_Init();
		UART2_GPIO_Init();
		USART_Init(USART2);
	}
	else {}
}

int main(void) {
	System_Clock_Init(); // Switch System Clock = 80 MHz
	LED_Init();
	
	Init_USARTx(2); // comment out for part 2
	// Init_USARTx(1); // comment out for part 1
	
	char rxByte;
	while (1) {
		// prompt user
		printf("Turn LED on? [y/n]\n");
		
		// wait for user input
		while (!scanf("%c", &rxByte)) {}
		
		// turn LED on/off accordingly, and send debug message
		if (tolower(rxByte) == 'y') {
			Green_LED_On();
			printf("Turned LED on.\n");
		}
		else if (tolower(rxByte) == 'n') {
			Green_LED_Off();
			printf("Turned LED off.\n");
		}
		else {
			printf("Unrecognized command.\n");
		}
	}
}
