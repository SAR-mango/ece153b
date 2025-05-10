#include "SPI.h"
#include "SysTimer.h"

void SPI1_GPIO_Init(void) {
	// Set PA4, PB3, PB4, and PB5 to Alternative Functions, and configure their AFR to SPI1
	GPIOA->MODER &= ~GPIO_MODER_MODE4_0;
	GPIOA->MODER |= GPIO_MODER_MODE4_1;
	GPIOB->MODER &= ~GPIO_MODER_MODE3_0;
	GPIOB->MODER |= GPIO_MODER_MODE3_1;
	GPIOB->MODER &= ~GPIO_MODER_MODE4_0;
	GPIOB->MODER |= GPIO_MODER_MODE4_1;
	GPIOB->MODER &= ~GPIO_MODER_MODE5_0;
	GPIOB->MODER |= GPIO_MODER_MODE5_1;
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL4;
	GPIOA->AFR[0] |= (GPIO_AFRL_AFSEL4_0 | GPIO_AFRL_AFSEL4_2);
	GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL3;
	GPIOB->AFR[0] |= (GPIO_AFRL_AFSEL3_0 | GPIO_AFRL_AFSEL3_2);
	GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL4;
	GPIOB->AFR[0] |= (GPIO_AFRL_AFSEL4_0 | GPIO_AFRL_AFSEL4_2);
	GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL5;
	GPIOB->AFR[0] |= (GPIO_AFRL_AFSEL5_0 | GPIO_AFRL_AFSEL5_2);
	// Set GPIO Pins to: Very High Output speed, Output Type Push-Pull, and No Pull-Up/Down
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR4;
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR3;
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR4;
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR5;
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT4;
	GPIOB->OTYPER &= ~GPIO_OTYPER_OT3;
	GPIOB->OTYPER &= ~GPIO_OTYPER_OT4;
	GPIOB->OTYPER &= ~GPIO_OTYPER_OT5;
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD4;
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD3;
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD4;
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD5;
}


void SPI1_Init(void){
	// Enable SPI clock and Reset SPI
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	RCC->APB2RSTR |= RCC_APB2RSTR_SPI1RST;
	RCC->APB2RSTR &= ~RCC_APB2RSTR_SPI1RST;
	// Disable SPI
	SPI1->CR1 &= ~SPI_CR1_SPE;
	// Configure for Full Duplex Communication
	SPI1->CR1 &= ~SPI_CR1_RXONLY;
	// Configure for 2-line Unidirectional Data Mode
	SPI1->CR1 &= ~SPI_CR1_BIDIMODE;
	// Disable Output in Bidirectional Mode
	SPI1->CR1 &= ~SPI_CR1_BIDIOE;
	// Set Frame Format: MSB First, 16-bit, Motorola Mode
	SPI1->CR1 &= ~SPI_CR1_LSBFIRST;
	SPI1->CR2 |= SPI_CR2_DS;
	SPI1->CR2 &= ~SPI_CR2_FRF;
	// Configure Clock. Read DataSheet for required value
	SPI1->CR1 |= SPI_CR1_CPOL;
	SPI1->CR1 |= SPI_CR1_CPHA;
	// Set Baud Rate Prescaler to 16
	SPI1->CR1 &= ~SPI_CR1_BR;
	SPI1->CR1 |= (SPI_CR1_BR_0 | SPI_CR1_BR_1);
	// Disable Hardware CRC Calculation
	SPI1->CR1 &= ~SPI_CR1_CRCEN;
	// Set as Master
	SPI1->CR1 |= SPI_CR1_MSTR;
	// Disable Software Slave Management
	SPI1->CR1 &= ~SPI_CR1_SSM;
	// Enable NSS Pulse Management
	SPI1->CR2 |= SPI_CR2_NSSP;
	// Enable NSS Output
	SPI1->CR2 |= SPI_CR2_SSOE;
	// Set FIFO Reception Threshold to 1/2
	SPI1->CR2 &= ~SPI_CR2_FRXTH;
	// TODO
	SPI1->CR2 |= SPI_CR2_LDMATX;
	SPI1->CR2 |= SPI_CR2_LDMARX;
	// Enable SPI
	SPI1->CR1 |= SPI_CR1_SPE;
}

uint16_t SPI_Transfer_Data(uint16_t write_data){
	// Wait for TXE (Transmit buffer empty)
	while (!((SPI1->SR & SPI_SR_TXE) == SPI_SR_TXE));
	// Write data
	SPI1->DR = write_data;
	// Wait for not busy
	while ((SPI1->SR & SPI_SR_BSY) == SPI_SR_BSY);
	// Wait for RXNE (Receive buffer not empty)
	while (!((SPI1->SR & SPI_SR_RXNE) == SPI_SR_RXNE));
	// Read and return data
	return SPI1->DR;
}
