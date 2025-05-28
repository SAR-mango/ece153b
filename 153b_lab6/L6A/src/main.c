/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Lab: 6A
 */

#include "stm32l476xx.h"
#include "SysClock.h"
#include "SysTimer.h"
#include "LED.h"
#include "CRC.h"
#include "UART.h"
#include <stdio.h>

/* Expected CRC Value */
#define uwExpectedCRCValue 0x5A60861E

int main(void) {
	// Switch System Clock = 80 MHz
	System_Clock_Init(); 
	SysTick_Init();
	LED_Init();
	UART2_Init();
	UART2_GPIO_Init();
	USART_Init(USART2);
	uint32_t crc = 0U;
	uint32_t time_us = 0U;

	while (1) {
		LED_Toggle();
		crc = INITIAL_CRC_VALUE;
		startTimer();
		for (uint32_t i = 0U; i < BUFFER_SIZE; i++) {
			crc = CrcSoftwareFunc(crc, DataBuffer[i], POLYNOME);
		}
		time_us = endTimer();
		if (crc != uwExpectedCRCValue) {
			LED_Off();
			break;
		}
		printf("Time: %d us\n", time_us);
		delay(1000U);
	}
}
