/*
 * ECE 153B - Spring 2023
 * Lab: 5A
 */
 
#include "stm32l476xx.h"
#include "SPI.h"
#include "UART.h"
#include "SysClock.h"
#include "SysTimer.h"
#include "accelerometer.h"

#include <stdio.h>



int main(void){
	
	double x = 0.0;
	double y = 0.0;
	double z = 0.0;
	
	// Enable the GPIO Clock for ports A and B
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	
	System_Clock_Init();   // System Clock = 80 MHz
	SysTick_Init();
	UART2_GPIO_Init();
	UART2_Init();
	USART_Init(USART2);
	
	SPI1_GPIO_Init();
	SPI1_Init();
	initAcc();
	
	while (1) {
		readValues(&x, &y, &z);
		printf("%.2f, %.2f, %.2f\n", x, y, z);
		delay(100);
	}
}
