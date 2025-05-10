#include "UART.h"

void UART1_Init(void) {
	// enable USART1 clock
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	
	// select system clock as USART1 clock source
	RCC->CCIPR |= RCC_CCIPR_USART1SEL_0;
	RCC->CCIPR &= ~RCC_CCIPR_USART1SEL_1;
}

void UART2_Init(void) {
	// enable USART2 clock
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;
	
	// select system clock as USART2 clock source
	RCC->CCIPR |= RCC_CCIPR_USART2SEL_0;
	RCC->CCIPR &= ~RCC_CCIPR_USART2SEL_1;
}

void UART1_GPIO_Init(void) {
	// set PA9 and PA10 mode to alternative function (10)
	GPIOA->MODER &= ~GPIO_MODER_MODE9_0;
	GPIOA->MODER |= GPIO_MODER_MODE9_1;
	GPIOA->MODER &= ~GPIO_MODER_MODE10_0;
	GPIOA->MODER |= GPIO_MODER_MODE10_1;
	
	// set PA9 and PA10 output type to push-pull (0)
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT9;
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT10;
	
	// set PA9 and PA10 to pull-up (01)
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD9_0;
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD9_1;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD10_0;
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD10_1;
	
	// set PA9 and PA10 speed to very high (11)
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR9;
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR10;
	
	// set PA9 alternative function to USART1_TX (AF7)
	GPIOA->AFR[1] |= (GPIO_AFRH_AFSEL9_0 | GPIO_AFRH_AFSEL9_1 | GPIO_AFRH_AFSEL9_2);
	GPIOA->AFR[1] &= ~GPIO_AFRH_AFSEL9_3;
	
	// set PA10 alternative function to USART1_RX (AF7)
	GPIOA->AFR[1] |= (GPIO_AFRH_AFSEL10_0 | GPIO_AFRH_AFSEL10_1 | GPIO_AFRH_AFSEL10_2);
	GPIOA->AFR[1] &= ~GPIO_AFRH_AFSEL10_3;
}

void UART2_GPIO_Init(void) {
	// set PA2 and PA3 mode to alternative function (10)
	GPIOA->MODER &= ~GPIO_MODER_MODE2_0;
	GPIOA->MODER |= GPIO_MODER_MODE2_1;
	GPIOA->MODER &= ~GPIO_MODER_MODE3_0;
	GPIOA->MODER |= GPIO_MODER_MODE3_1;
	
	// set PA2 and PA3 output type to push-pull (0)
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT2;
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT3;
	
	// set PA2 and PA3 to pull-up (01)
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD2_0;
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD2_1;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD3_0;
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD3_1;
	
	// set PA2 and PA3 speed to very high (11)
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR2;
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR3;
	
	// set PA2 alternative function to USART2_TX (AF7)
	GPIOA->AFR[0] |= (GPIO_AFRL_AFSEL2_0 | GPIO_AFRL_AFSEL2_1 | GPIO_AFRL_AFSEL2_2);
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL2_3;
	
	// set PA3 alternative function to USART2_RX (AF7)
	GPIOA->AFR[0] |= (GPIO_AFRL_AFSEL3_0 | GPIO_AFRL_AFSEL3_1 | GPIO_AFRL_AFSEL3_2);
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL3_3;
}

void USART_Init(USART_TypeDef* USARTx) {
	// disable USART
	USARTx->CR1 &= ~USART_CR1_UE;
		
	// set word length to 8 bits
	USARTx->CR1 &= ~USART_CR1_M1;
	
	// set oversampling mode to oversample by 16
	USARTx->CR1 &= ~USART_CR1_OVER8;
	
	// set number of stop bits to 1
	USARTx->CR2 &= ~USART_CR2_STOP;
	
	// set baud rate to 9600bps
	USARTx->BRR = 8333U;
	
	// enable transmitter and receiver
	USARTx->CR1 |= USART_CR1_TE;
	USARTx->CR1 |= USART_CR1_RE;
	
	// enable USART
	USARTx->CR1 |= USART_CR1_UE;
}

uint8_t USART_Read (USART_TypeDef * USARTx) {
	// SR_RXNE (Read data register not empty) bit is set by hardware
	while (!(USARTx->ISR & USART_ISR_RXNE));  // Wait until RXNE (RX not empty) bit is set
	// USART resets the RXNE flag automatically after reading DR
	return ((uint8_t)(USARTx->RDR & 0xFF));
	// Reading USART_DR automatically clears the RXNE flag 
}

void USART_Write(USART_TypeDef * USARTx, uint8_t *buffer, uint32_t nBytes) {
	// TXE is cleared by a write to the USART_DR register.
	// TXE is set by hardware when the content of the TDR 
	// register has been transferred into the shift register.
	for (int i = 0; i < nBytes; i++) {
		while (!(USARTx->ISR & USART_ISR_TXE));   	// wait until TXE (TX empty) bit is set
		// Writing USART_DR automatically clears the TXE flag 	
		USARTx->TDR = buffer[i] & 0xFF;
		USART_Delay(300); // comment out for part 2
		//USART_Delay(40000); // comment out for part 1
	}
	while (!(USARTx->ISR & USART_ISR_TC));   		  // wait until TC bit is set
	USARTx->ISR &= ~USART_ISR_TC;
}   

void USART_Delay(uint32_t us) {
	uint32_t time = 100*us/7;    
	while(--time);   
}
