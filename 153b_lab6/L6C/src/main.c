/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Lab: 6C
 */

#include "stm32l476xx.h"
#include "SysClock.h"
#include "SysTimer.h"
#include "LED.h"
#include "CRC.h"
#include "DMA.h"
#include "UART.h"
#include <stdio.h>

/* Expected CRC Value */
static uint32_t uwExpectedCRCValue = 0x5A60861E;	
static volatile uint32_t ComputedCRC;	
static volatile uint32_t ComputationDone = 0;	

void completeCRC(uint32_t crc) {
	ComputedCRC = crc;
	ComputationDone = 1U;
}

int main(void) {
	System_Clock_Init(); 
	SysTick_Init();
	LED_Init();	
	CRC_Init();
	UART2_GPIO_Init();
	UART2_Init();
	USART_Init(USART2);
	DMA_Init();
	uint32_t time_us = 0U;
	
	while (1) {
		LED_Toggle();
		ComputationDone = 0U;
		CRC->CR |= CRC_CR_RESET;
		DMA1_Channel6->CNDTR = BUFFER_SIZE;
		startTimer();
		DMA1_Channel6->CCR |= DMA_CCR_EN;
		while (!ComputationDone);
		time_us = endTimer();
		DMA1_Channel6->CCR &= ~DMA_CCR_EN;
		if (ComputedCRC != uwExpectedCRCValue) {
			printf("CRC failed, received %d, time %d\n", ComputedCRC, time_us);
			LED_Off();
			break;
		}
		printf("Time: %d us\n", time_us);
		delay(1000U);
	}
}


