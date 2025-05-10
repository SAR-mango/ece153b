/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Lab: 4B
 */

#include "stm32l476xx.h"
#include "I2C.h"
#include "SysClock.h"
#include "UART.h"
#include <string.h>
#include <stdio.h>

// Initializes USARTx
// USART2: UART Communication with PC
void Init_USARTx(int x) {
	if(x == 1) {}
	else if(x == 2) {
		UART2_Init();
		UART2_GPIO_Init();
		USART_Init(USART2);
	}
	else {}
}

int main(void) {
	System_Clock_Init(); // System Clock = 80 MHz
	
	// Initialize I2C
	I2C_GPIO_Init();
	I2C_Initialization();

	// Initialize UART
	Init_USARTx(2);
	
	uint8_t device_addr = (0x48U << 1);
	uint8_t cmd = 0x00U;
	uint8_t temp_celsius = 0x00U;
	
	while (1) {
		I2C_SendData(I2C1, device_addr, &cmd, 1);
		I2C_ReceiveData(I2C1, device_addr, &temp_celsius, 1);
		printf("Temperature: %d F\n", ((int16_t) (((float) temp_celsius) * 1.8 + 32)));
		// Some delay
		for (int i = 0; i < 5000000; i++); 
	}
}
