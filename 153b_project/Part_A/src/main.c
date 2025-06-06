/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Project
 */

#include "stm32l476xx.h"
#include "SysClock.h"
#include "SysTimer.h"
#include "UART.h"
#include "motor.h"
#include <stdio.h>
#include <string.h>

int main(void) {
	char uart_in[10];
	// Switch System Clock = 80 MHz
	System_Clock_Init();
	// Enable GPIO Clock for port A
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	// Enable GPIO Clock for port B
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	// Enable GPIO Clock for port C
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	Motor_Init();
	SysTick_Init();
	UART2_GPIO_Init();
	UART2_Init();
	USART_Init(USART2);
	
	printf("Program started.\n");
	while (1) {
		scanf("%s", uart_in);
		if (!strcmp(uart_in, "cw")) {
			setDirection(DIR_CW);
			printf("Rotating clockwise.\n");
		}
		else if (!strcmp(uart_in, "ccw")) {
			setDirection(DIR_CCW);
			printf("Rotating counterclockwise.\n");
		}
		else if (!strcmp(uart_in, "stop")) {
			setDirection(DIR_STOP);
			printf("Stopping motor.\n");
		}
		else {
			printf("Invalid command; enter \"cw\", \"ccw\", or \"stop\".\n");
		}
	}
}


