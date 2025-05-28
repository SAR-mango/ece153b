/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Lab: 6B
 */

#include "stm32l476xx.h"
#include "SysClock.h"
#include "SysTimer.h"
#include "LED.h"
#include "CRC.h"
#include "UART.h"
#include <stdio.h>

/* Expected CRC Value */
static uint32_t uwExpectedCRCValue = 0x5A60861E;	

int main(void) {	
	// Switch System Clock = 80 MHz
	System_Clock_Init(); 
	SysTick_Init();
	LED_Init();	
	CRC_Init();
	UART2_GPIO_Init();
	UART2_Init();
	USART_Init(USART2);
	uint32_t crc = 0U;
	uint32_t time_us = 0U;
	
	while (1) {
		LED_Toggle();
		CRC->CR |= CRC_CR_RESET;
		startTimer();
		crc = CRC_CalcBlockCRC(DataBuffer, BUFFER_SIZE);
		time_us = endTimer();
		if (crc != uwExpectedCRCValue) {
			LED_Off();
			break;
		}
		printf("Time: %d us\n", time_us);
		delay(1000U);
	}
}


